#include "SgpPCH.h"
#include <sne/sgp/session/extension/RpcingExtension.h>
#include <sne/sgp/protocol/cs/CsMessage.h>
#include <sne/srpc/detail/RpcCommand.h>
#include <sne/srpc/RpcNetwork.h>
#include <sne/srpc/RpcForwarder.h>
#include <sne/base/session/impl/SessionImpl.h>

namespace sne { namespace sgp {

namespace detail {

/**
 * @class SgpRpcNetwork
 */
class SgpRpcNetwork : public srpc::RpcNetwork
{
public:
    SgpRpcNetwork() :
        sessionImpl_(nullptr) {}

    void initialize(base::SessionImpl& sessionImpl) {
        sessionImpl_ = &sessionImpl;
    }

    bool handleMessage(base::MemoryBlock& mblock) {
        core::IStream& istream = sessionImpl_->deserialize(mblock);
        onReceive(istream);
        return true;
    }

public:
    virtual void registerRpcReceiver(srpc::RpcReceiver& receiver) override {
        assert(sessionImpl_ != nullptr);
        srpc::RpcNetwork::registerRpcReceiver(receiver);
    }

    virtual void unregisterRpcReceiver(srpc::RpcReceiver& receiver) override {
        assert(sessionImpl_ != nullptr);
        srpc::RpcNetwork::unregisterRpcReceiver(receiver);
    }

private:
    virtual void send(srpc::RpcCommand& command,
        srpc::RpcPacketType /*packetType*/, const void* /*rpcHint*/) override {
        assert(sessionImpl_ != nullptr);
        base::MemoryBlock* mblock = sessionImpl_->serialize(command);
        if (mblock != nullptr) {
            (void)sessionImpl_->sendMessage(*mblock,
                base::toMessageType(sgp::csmtRpc));
        }
    }

private:
    base::SessionImpl* sessionImpl_;
};


/**
 * @class RpcingExtensionImpl
 */
class RpcingExtensionImpl : public boost::noncopyable
{
public:
    void initialize(base::SessionImpl& sessionImpl) {
        rpcNetwork_.initialize(sessionImpl);
    }

    void registerRpcForwarder(srpc::RpcForwarder& forwarder) {
        forwarder.setRpcNetwork(rpcNetwork_);
    }

    void unregisterRpcForwarder(srpc::RpcForwarder& forwarder) {
        forwarder.resetRpcNetwork();
    }

    void registerRpcReceiver(srpc::RpcReceiver& receiver) {
        rpcNetwork_.registerRpcReceiver(receiver);
    }

    void unregisterRpcReceiver(srpc::RpcReceiver& receiver) {
        rpcNetwork_.unregisterRpcReceiver(receiver);
    }

    bool handleMessage(base::MessageType messageType, base::MemoryBlock& mblock) {
        if (messageType != base::toMessageType(sgp::csmtRpc)) {
            return false;
        }

        return rpcNetwork_.handleMessage(mblock);
    }

private:
    SgpRpcNetwork rpcNetwork_;
};

} // namespace detail

// = RpcingExtension

RpcingExtension::RpcingExtension() :
    impl_(std::make_unique<detail::RpcingExtensionImpl>())
{
}


RpcingExtension::~RpcingExtension()
{
}


void RpcingExtension::registerRpcForwarder(srpc::RpcForwarder& forwarder)
{
    impl_->registerRpcForwarder(forwarder);
}


void RpcingExtension::unregisterRpcForwarder(srpc::RpcForwarder& forwarder)
{
    impl_->unregisterRpcForwarder(forwarder);
}


void RpcingExtension::registerRpcReceiver(srpc::RpcReceiver& receiver)
{
    impl_->registerRpcReceiver(receiver);
}


void RpcingExtension::unregisterRpcReceiver(srpc::RpcReceiver& receiver)
{
    impl_->unregisterRpcReceiver(receiver);
}

// = overriding

void RpcingExtension::attachedTo(base::SessionImpl& sessionImpl)
{
    Parent::attachedTo(sessionImpl);

    impl_->initialize(sessionImpl);
}


bool RpcingExtension::handleMessage(base::MessageType messageType,
    base::MemoryBlock& mblock)
{
    return impl_->handleMessage(messageType, mblock);
}

}} // namespace sne { namespace sgp {
