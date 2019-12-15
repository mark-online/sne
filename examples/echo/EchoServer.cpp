#include "EchoTestPCH.h"
#include "EchoServer.h"
#include "EchoMessage.h"
#include "Echoer.h"
#include <sne/server/session/config/ServerSideSessionConfig.h>
#include <sne/sgp/protocol/security/SecurePacketCoderFactory.h>
#include <sne/sgp/session/SgpSessionImplFactory.h>
#include <sne/sgp/session/extension/RpcingExtension.h>
#include <sne/base/session/detail/SessionCapacity.h>
#include <sne/base/session/extension/MessagingExtension.h>

IMPLEMENT_SRPC_EVENT_DISPATCHER(EchoServerSession);

EchoServerSession::EchoServerSession(bool useRpc,
    const server::ServerSideSessionConfig& sessionConfig,
    const server::ServerSpec& serverSpec,
    std::unique_ptr<base::SessionImpl> impl) :
    server::ServerSideSession(sessionConfig, serverSpec, std::move(impl)),
    useRpc_(useRpc)
{
    if (useRpc_) {
        getExtension<sgp::RpcingExtension>()->registerRpcForwarder(*this);
        getExtension<sgp::RpcingExtension>()->registerRpcReceiver(*this);
    }
    else {
        getExtension<base::MessagingExtension>()->
            registerMessageCallback<EchoMessage>(
                &EchoServerSession::echoMessageReceived, this);
    }
}


void EchoServerSession::echoMessageReceived(const base::Message& message)
{
    //const EchoMessage& theMessage =
    //    static_cast<const EchoMessage&>(message);

    (void)getExtension<base::MessagingExtension>()->
        sendMessage(const_cast<base::Message&>(message));
}


bool EchoServerSession::onConnected()
{
    if (! server::ServerSideSession::onConnected()) {
        return false;
    }

    authenticated();
    return true;
}


RECEIVE_SRPC_METHOD_1(EchoServerSession, echo, std::string, data)
{
    assert(useRpc_);
    onEcho(data);
}


FORWARD_SRPC_METHOD_1(EchoServerSession, onEcho, std::string, data);

// = ServerSessionFactory

std::unique_ptr<base::Session> ServerSessionFactory::create() const
{
    // TODO: goto Config.h
    const size_t heartbeatInterval = 0;

    std::unique_ptr<sgp::PacketCoder> packetCoder(
        sgp::SecurePacketCoderFactory(config_.getCipher()).create());

    std::unique_ptr<base::MemoryBlockManager> memoryBlockManager(
        std::make_unique<base::MemoryBlockManager>(
            packetCoder->getDefaultPacketPoolSize(),
            packetCoder->getDefaultPacketSize()));

    server::ServerSideSessionConfig sessionConfig(std::move(memoryBlockManager),
        std::move(packetCoder), *sessionDestroyer_,
        ioContextTask_, base::SessionCapacity::getUnlimitedCapacity());

    server::ServerSpec serverSpec;
    serverSpec.heartbeatTimeout_ = heartbeatInterval * 3;
    //serverSpec.authenticationTimeout_ = ;
    serverSpec.listeningAddresses_.emplace_back("dummy");
    serverSpec.listeningPort_ = static_cast<uint16_t>(-1);
    serverSpec.workerThreadCount_ = 1;
    serverSpec.sessionPoolSize_ = 1;
    serverSpec.packetCipher_ = "dummy";
    assert(serverSpec.isValid());

    std::unique_ptr<base::SessionImpl> impl(
        sgp::SgpSessionImplFactory::create(sessionConfig));

    return std::unique_ptr<base::Session>(std::make_unique<EchoServerSession>(
        config_.useRpc(), sessionConfig, serverSpec, std::move(impl)));
}
