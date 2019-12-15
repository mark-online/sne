#include "ServerPCH.h"
#include "heartbeater/HeartbeatSender.h"
#include "heartbeater/HeartbeatReceiver.h"
#include <sne/server/session/ClientSideSession.h>
#include <sne/server/session/config/ClientSideSessionConfig.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/Assert.h>
#include <sne/security/cipher/RsaCipher.h>
#include <boost/core/ignore_unused.hpp>

namespace sne { namespace server {

using security::RsaCipher;

ClientSideSession::ClientSideSession(const ClientSideSessionConfig& config,
    std::unique_ptr<base::SessionImpl> impl) :
    rsaCipher_(std::make_unique<RsaCipher>(
        RsaCipher::defaultPrivateKeyLength,
        RsaCipher::generateSeed(RsaCipher::defaultPrivateKeySeedLength))),
    packetSeedExchanger_(config.packetSeedExchanger_)
{
    SNE_ASSERT(config.isValid());

    base::Session::setImplementation(std::move(impl));

    heartbeater_ = std::make_unique<HeartbeatSender>(*this, config.heartbeatInterval_);
    heartBeatReceiver_ = std::make_unique<HeartBeatReceiver>(*this, (config.heartbeatInterval_ * 3));

    packetSeedExchanger_->initialize(*this, *config.packetCoder_, *this);

    getExtension<base::MessagingExtension>()->
        registerMessageCallback<sgp::HeartbeatMessage>(
            &ClientSideSession::heartbeatMessageReceived, this);
}


ClientSideSession::~ClientSideSession()
{
}


void ClientSideSession::heartbeatMessageReceived(const base::Message& /*message*/)
{
}

// = base::SessionCallback overriding

bool ClientSideSession::onConnected()
{
    packetSeedExchanger_->exchangePublicKey();

    heartBeatReceiver_->setupTimer();

    return true;
}


void ClientSideSession::onDisconnected()
{
    heartbeater_->stop();
    heartBeatReceiver_->cancelTimer();
}


void ClientSideSession::onMessageTransfered(base::MessageType /*messageType*/)
{
    if (isConnected()) {
        heartbeater_->sent();
    }
}


void ClientSideSession::onThrottling(size_t readBytes, size_t maxBytesPerSecond)
{
    boost::ignore_unused(readBytes);
    boost::ignore_unused(maxBytesPerSecond);
}

// = sgp::PacketSeedExchangerCallback overriding

bool ClientSideSession::generatePublicAndPrivateKey(
    std::string& publicKey, std::string& privateKey)
{
    publicKey = rsaCipher_->getPublicKey();
    privateKey = rsaCipher_->getPrivateKey();
    return true;
}


void ClientSideSession::encryptSeed(std::string& /*seed*/,
    const std::string& /*otherSidePublicKey*/)
{
    assert(false && "DON'T CALL ME!");
}


bool ClientSideSession::decryptSeed(std::string& seed)
{
    if (! rsaCipher_->decrypt(seed)) {
        SNE_LOG_ERROR("Failed RsaCipher::decrypt(%s)",
            rsaCipher_->getLastErrorMessage().c_str());
        return false;
    }
    return true;
}


void ClientSideSession::seedExchanged()
{
    heartbeater_->start();

    onValidated();
}

}} // namespace sne { namespace server {
