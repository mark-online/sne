#include "EchoTestPCH.h"
#include "EchoClient.h"
#include "EchoMessage.h"
#include "Echoer.h"
#include <sne/server/session/config/ClientSideSessionConfig.h>
#include <sne/sgp/session/SgpSessionImplFactory.h>
#include <sne/sgp/protocol/security/SecurePacketCoderFactory.h>
#include <sne/sgp/session/extension/RpcingExtension.h>
#include <sne/base/session/detail/SessionCapacity.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/memory/MemoryBlockManager.h>
#include <boost/core/ignore_unused.hpp>

IMPLEMENT_SRPC_EVENT_DISPATCHER(EchoClientSession);

EchoClientSession::EchoClientSession(bool useRpc,
    uint32_t echoCount, uint32_t blockSize,
    const server::ClientSideSessionConfig& config,
    std::unique_ptr<base::SessionImpl> impl) :
    server::ClientSideSession(config, std::move(impl)),
    useRpc_(useRpc),
    echoCount_(echoCount),
    body_(blockSize, 'X'),
    echoedCount_(0)
{
    if (useRpc_) {
        getExtension<sgp::RpcingExtension>()->registerRpcForwarder(*this);
        getExtension<sgp::RpcingExtension>()->registerRpcReceiver(*this);
    }
    else {
        getExtension<base::MessagingExtension>()->
            registerMessageCallback<EchoMessage>(
                &EchoClientSession::echoMessageReceived, this);
    }
}


void EchoClientSession::echoed()
{
    assert(echoedCount_ < echoCount_);
    if ((++echoedCount_) >= echoCount_) {
        disconnect();
        return;
    }

    sendEcho();
}


void EchoClientSession::echoMessageReceived(const base::Message& /*message*/)
{
    assert(echoedCount_ < echoCount_);
    if ((++echoedCount_) >= echoCount_) {
        disconnect();
    }
    sendEcho();
}


void EchoClientSession::onValidated()
{
    echoedCount_ = 0;
    sendEcho();
}


bool EchoClientSession::onConnected()
{
    return server::ClientSideSession::onConnected();
}


void EchoClientSession::sendEcho()
{
    if (useRpc_) {
        echo(body_);
    }
    else {
        EchoMessage message(body_);
        getExtension<base::MessagingExtension>()->sendMessage(message);
    }
}


FORWARD_SRPC_METHOD_1(EchoClientSession, echo, std::string, data);

RECEIVE_SRPC_METHOD_1(EchoClientSession, onEcho, std::string, data)
{
    boost::ignore_unused(data);
    assert(useRpc_);
    echoed();
}

// = ClientSessionFactory

std::unique_ptr<base::Session> ClientSessionFactory::create() const
{
    // TODO: goto Config.h
    const size_t heartbeatInterval = 0;

    std::unique_ptr<sgp::PacketCoder> packetCoder(
        sgp::SecurePacketCoderFactory(config_.getCipher()).create());

    std::unique_ptr<base::MemoryBlockManager> memoryBlockManager(
        std::make_unique<base::MemoryBlockManager>(
            packetCoder->getDefaultPacketPoolSize(),
            packetCoder->getDefaultPacketSize()));

    server::ClientSideSessionConfig sessionConfig(heartbeatInterval,
        std::move(memoryBlockManager),
        std::move(packetCoder),
        *sessionDestroyer_,
        ioContextTask_, base::SessionCapacity::getUnlimitedCapacity());

    std::unique_ptr<base::SessionImpl> impl(
        sgp::SgpSessionImplFactory::create(sessionConfig));

    return std::make_unique<EchoClientSession>(config_.useRpc(),
        config_.getEchoCount(), config_.getBlockSize(), sessionConfig,
        std::move(impl));
}
