#include "ClientPCH.h"
#include <sne/client/session/ClientSession.h>
#include <sne/client/session/ClientSessionTick.h>
#include <sne/client/session/ClientSessionCallback.h>
#include <sne/sgp/session/extension/RpcingExtension.h>
#include <sne/sgp/session/detail/Heartbeater.h>
#include <sne/sgp/session/SgpSessionImplFactory.h>
#include <sne/sgp/session/SgpSessionConfig.h>
#include <sne/sgp/protocol/security/PacketSeedExchangerFactory.h>
#include <sne/sgp/protocol/security/PacketSeedExchanger.h>
#include <sne/sgp/protocol/PacketCoderFactory.h>
#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/base/session/Session.h>
#include <sne/base/memory/MemoryBlockManager.h>
#include <sne/base/utility/Logger.h>
#include <sne/security/cipher/RsaCipher.h>
#include <boost/core/ignore_unused.hpp>

namespace sne { namespace client {

namespace detail {

/**
 * @class ClientHeartbeater
 */
class ClientHeartbeater : public sgp::Heartbeater
{
public:
    ClientHeartbeater(base::Session& session, msec_t heartbeatInterval) :
        Heartbeater(session, heartbeatInterval) {}

private:
    // = TimerCallback overriding
    virtual void timedOut(const void* /*act*/) override {
        sendHeartbeat();
    }
};

} // namespace detail

// = ClientSession

ClientSession::ClientSession(ClientSessionTick& tick,
    sgp::PacketCoderFactory& packetCoderFactory,
    msec_t heartbeatInterval,
    ClientSessionCallback* callback) :
    callback_(callback),
    ticker_(tick),
    packetSeedExchanged_(false)
{
    std::unique_ptr<sgp::PacketCoder> packetCoder(packetCoderFactory.create());

    std::unique_ptr<base::MemoryBlockManager> memoryBlockManager(
        std::make_unique<base::MemoryBlockManager>(
            packetCoder->getDefaultPacketPoolSize(),
            packetCoder->getDefaultPacketSize()));

    const sgp::SgpSessionConfig sessionConfig(std::move(memoryBlockManager),
        std::move(packetCoder),
        std::unique_ptr<sgp::PacketSeedExchanger>(),
        *this, tick.getIoContextTask(),
        base::SessionCapacity::getUnlimitedCapacity());
    base::Session::setImplementation(
        sgp::SgpSessionImplFactory::create(sessionConfig));

    heartbeater_ = std::make_unique<detail::ClientHeartbeater>(*this, heartbeatInterval);

    using security::RsaCipher;
    rsaCipher_ = std::make_unique<RsaCipher>(
        RsaCipher::defaultPrivateKeyLength,
        RsaCipher::generateSeed(RsaCipher::defaultPrivateKeySeedLength));

    seedExchanger_ = sgp::PacketSeedExchangerFactory::createForClient();
    seedExchanger_->initialize(*this, *sessionConfig.packetCoder_, *this);

    getExtension<base::MessagingExtension>()->
        registerMessageCallback<sgp::HeartbeatMessage>(
            &ClientSession::heartbeatMessageReceived, this);
}


ClientSession::~ClientSession()
{
}


bool ClientSession::connectAndValidate(const std::string& ip, uint16_t port,
    uint16_t timeout, bool waitUntilValidate)
{
    const auto deadline = std::chrono::steady_clock::now() +
        std::chrono::milliseconds(timeout);

    if (! connect(ip, port, timeout)) {
        return false;
    }

    if (! waitUntilValidate) {
        return true;
    }

    for (;;) {
        const auto now = std::chrono::steady_clock::now();
        if (now >= deadline) {
            break;
        }

        ticker_.tick();

        if (packetSeedExchanged_) {
            return true;
        }

        base::pause(1);
    }

    // timeout
    disconnect();
    return false;
}


void ClientSession::tick()
{
    ticker_.tick();
}


void ClientSession::registerRpcForwarder(srpc::RpcForwarder& forwarder)
{
    getExtension<sgp::RpcingExtension>()->registerRpcForwarder(forwarder);
}


void ClientSession::registerRpcReceiver(srpc::RpcReceiver& receiver)
{
    getExtension<sgp::RpcingExtension>()->registerRpcReceiver(receiver);
}


void ClientSession::unregisterRpcForwarder(srpc::RpcForwarder& forwarder)
{
    getExtension<sgp::RpcingExtension>()->unregisterRpcForwarder(forwarder);
}


void ClientSession::unregisterRpcReceiver(srpc::RpcReceiver& receiver)
{
    getExtension<sgp::RpcingExtension>()->unregisterRpcReceiver(receiver);
}


void ClientSession::heartbeatMessageReceived(const base::Message& /*message*/)
{
}

// = base::Session overriding

bool ClientSession::onConnected()
{
    packetSeedExchanged_ = false;
    seedExchanger_->exchangePublicKey();
    heartbeater_->stop();
    return true;
}


void ClientSession::onMessageTransfered(base::MessageType /*messageType*/)
{
    heartbeater_->sent();
}


void ClientSession::onThrottling(size_t readBytes, size_t maxBytesPerSecond)
{
    boost::ignore_unused(readBytes);
    boost::ignore_unused(maxBytesPerSecond);
}


void ClientSession::onDisconnected()
{
    heartbeater_->stop();

    if (callback_ != nullptr) {
        callback_->onDisconnected();
    }

    packetSeedExchanged_ = false;
}

// = base::SessionDestroyer overriding

void ClientSession::release(base::Session* /*session*/)
{
}

// = sgp::PacketSeedExchangerCallback overriding

bool ClientSession::generatePublicAndPrivateKey(
    std::string& publicKey, std::string& privateKey)
{
    publicKey = rsaCipher_->getPublicKey();
    privateKey = rsaCipher_->getPrivateKey();
    return true;
}


void ClientSession::encryptSeed(std::string& /*seed*/,
    const std::string& /*otherSidePublicKey*/)
{
    assert(false && "NOP");
}


bool ClientSession::decryptSeed(std::string& seed)
{
    if (! rsaCipher_->decrypt(seed)) {
        SNE_LOG_ERROR("Failed RsaCipher::decrypt(%s)",
            rsaCipher_->getLastErrorMessage().c_str());
        return false;
    }
    return true;
}


void ClientSession::seedExchanged()
{
    if (! packetSeedExchanged_) {
        packetSeedExchanged_ = true;
        callback_->onValidated();
        heartbeater_->start();
    }
}

}} // namespace sne { namespace client {
