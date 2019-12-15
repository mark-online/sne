#pragma once

#include <sne/srpc/detail/RpcCommand.h>
#include <sne/srpc/RpcTypes.h>
#include <sne/srpc/detail/ForwardingFunctors.h>

using namespace sne;

/**
 * @class DummyRpcCommand
 */
class DummyRpcCommand : public srpc::RpcCommand
{
public:
    DummyRpcCommand(const srpc::RInt32& p1, const srpc::RInt32& p2) :
        srpc::RpcCommand(DummyRpcCommand::getStaticRpcId(), marshalFunctor_),
        marshalFunctor_(p1, p2) {}

private:
    static const srpc::RRpcId& getStaticRpcId() {
        static srpc::RRpcId rpcId("DummyRpcCommand");
        return rpcId;
    }
private:
    srpc::ForwardingFunctorT<SRPC_TYPELIST_2(srpc::RInt32, srpc::RInt32)>
        marshalFunctor_;
};
