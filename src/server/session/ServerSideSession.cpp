#include "ServerPCH.h"
#include "heartbeater/HeartbeatReceiver.h"
#include "heartbeater/HeartbeatSender.h"
#include <sne/server/session/ServerSideSession.h>
#include <sne/server/session/config/ServerSideSessionConfig.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>
#include <sne/security/cipher/RsaCipher.h>
#include <atomic>

namespace sne { namespace server {

namespace
{

ClientId getNextClientId()
{
    static std::atomic<uint32_t> globalClientId(
        static_cast<uint32_t>(ClientId::invalid));

    return static_cast<ClientId>(++globalClientId);
}

} // namespace

// = ServerSideSession

ServerSideSession::ServerSideSession(
    const ServerSideSessionConfig& sessionConfig,
    const ServerSpec& serverSpec,
    std::unique_ptr<base::SessionImpl> impl) :
    serverSpec_(serverSpec),
    authenticationTimer_(impl->getIoContext().createTimer(*this)),
    clientId_(ClientId::invalid),
    packetSeedExchanger_(sessionConfig.packetSeedExchanger_),
    isSeedExchanged_(false)
{
    base::Session::setImplementation(std::move(impl));

    SNE_ASSERT(sessionConfig.isValid());

    packetSeedExchanger_->initialize(*this, *sessionConfig.packetCoder_, *this);

    getExtension<base::MessagingExtension>()->
        registerMessageCallback<sgp::HeartbeatMessage>(
            &ServerSideSession::heartbeatMessageReceived, this);
}


ServerSideSession::~ServerSideSession()
{
    SNE_ASSERT(! authenticationTimer_->isRunning());
}


void ServerSideSession::authenticated()
{
    cancelAuthenticationTimer();
    heartBeatReceiver_->setupTimer();
    heartbeater_->start();
}


void ServerSideSession::extendAuthenticationTimeout()
{
    cancelAuthenticationTimer();
    setupAuthenticationTimer();
}


void ServerSideSession::unauthenticated()
{
    clientId_ = ClientId::invalid;
}


void ServerSideSession::setupAuthenticationTimer()
{
    const msec_t authenticationTimeout =
        serverSpec_.authenticationTimeout_;

    if (authenticationTimeout > 0) {
        authenticationTimer_->once(authenticationTimeout);
    }
}


void ServerSideSession::cancelAuthenticationTimer()
{
    authenticationTimer_->cancel();
}


void ServerSideSession::initializeRsaCipher()
{
    using security::RsaCipher;

    if (! rsaCipher_.get()) {
        rsaCipher_ = std::make_unique<RsaCipher>(RsaCipher::defaultPrivateKeyLength,
            RsaCipher::generateSeed(RsaCipher::defaultPrivateKeySeedLength));
    }
}


void ServerSideSession::initHeartbeater()
{
    if (! heartBeatReceiver_) {
        heartBeatReceiver_ = std::make_unique<HeartBeatReceiver>(*this,
            serverSpec_.heartbeatTimeout_, static_cast<uint32_t>(clientId_));
    }

    if (! heartbeater_.get()) {
        heartbeater_ = std::make_unique<HeartbeatSender>(*this,
            (serverSpec_.heartbeatTimeout_ / 3));
    }
}


void ServerSideSession::heartbeatMessageReceived(const base::Message& /*message*/)
{
}

// = base::SessionCallback overriding

bool ServerSideSession::onConnected()
{
    initializeRsaCipher();

    isSeedExchanged_ = false;
    packetSeedExchanger_->exchangePublicKey();

    initHeartbeater();

    clientId_ = getNextClientId();

    setupAuthenticationTimer();

    connectedTime_ = std::chrono::steady_clock::now();

    const base::InetAddress remoteAddress = getImpl().getRemoteAddress();
    SNE_LOG_INFO("Client(C%u) is connected from (%s:%d).",
        clientId_,
        remoteAddress.address_.c_str(), remoteAddress.port_);

    return true;
}


void ServerSideSession::onMessageTransfered(base::MessageType /*messageType*/)
{
    if (isConnected()) {
        heartbeater_->sent();
    }
}


bool ServerSideSession::onMessageArrived(base::MessageType messageType,
    base::MemoryBlock& mblock)
{
    heartBeatReceiver_->heartbeated();

    if (base::Session::onMessageArrived(messageType, mblock)) {
        return true;
    }

    assert(! sgp::isSystemMessage(messageType));
    if (! isSeedExchanged_) {
        SNE_LOG_INFO("Client(C%u) is disconnecting by server,"
            "because general packet(%d,%d) is not allowed before seed exchanged.",
            clientId_, messageType, mblock.length());
        return false;
    }

    packetSeedExchanger_->exchangeNextSeed();
    return false;
}


void ServerSideSession::onThrottling(size_t readBytes, size_t maxBytesPerSecond)
{
    SNE_LOG_INFO("Client(C%u) is THROTTLED(%d > %d), delaying read.",
        clientId_, readBytes, maxBytesPerSecond);
}


void ServerSideSession::onDisconnected()
{
    cancelAuthenticationTimer();
    if (heartBeatReceiver_) {
        heartBeatReceiver_->cancelTimer();
    }
    if (heartbeater_) {
        heartbeater_->stop();
    }

    SNE_LOG_INFO("Client(C%u) is disconnected(period:%ums).",
        clientId_, getConnectionPeriod());
}

// = base::TimerCallback overriding

void ServerSideSession::timedOut(const void* /*act*/)
{
    cancelAuthenticationTimer();
    SNE_LOG_INFO("Client(C%u) is disconnecting by server,"
        "because authentication timeout(period:%ums).",
        clientId_, getConnectionPeriod());
    disconnectGracefully();
}

// = sgp::PacketSeedExchangerCallback overriding

bool ServerSideSession::generatePublicAndPrivateKey(
    std::string& publicKey, std::string& privateKey)
{
    publicKey = rsaCipher_->getPublicKey();
    privateKey = rsaCipher_->getPrivateKey();
    return true;
}


void ServerSideSession::encryptSeed(std::string& seed,
    const std::string& otherSidePublicKey)
{
    if (! rsaCipher_->encrypt(seed, otherSidePublicKey)) {
        SNE_LOG_ERROR("Client(C%u) - ServerSideSession::encryptSeed(%s) FAILED",
            clientId_, rsaCipher_->getLastErrorMessage().c_str());
    }
}


bool ServerSideSession::decryptSeed(std::string& /*seed*/)
{
    assert(false && "DON'T CALL ME!");
    return false;
}

void ServerSideSession::seedExchanged()
{
    isSeedExchanged_ = true;
}

}} // namespace sne { namespace server {
