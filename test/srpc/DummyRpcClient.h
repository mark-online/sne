#pragma once

#include "DummyRpcInterface.h"
#include <sne/srpc/RpcForwarder.h>
#include <sne/srpc/RpcReceiver.h>

using namespace sne;

/**
 * @class DummyRpcClient
 */
class DummyRpcClient : public DummyRpc,
    private srpc::RpcReceiver,
    private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(DummyRpcClient);
public:
    DummyRpcClient(srpc::RpcNetwork* rpcNetwork = nullptr);

    const srpc::RRpcId getLastRpcId() const {
        return lastRpcId_;
    }
private: // encapsulation
    OVERRIDE_SRPC_METHOD_0(rpc0);
    OVERRIDE_SRPC_METHOD_1(rpc1, int32_t, p1);
    OVERRIDE_SRPC_METHOD_2(rpc2, RInt32, p1, RInt32, p2);
    OVERRIDE_SRPC_METHOD_3(rpc3, RInt32, p1, RInt32, p2, RInt32, p3);
    OVERRIDE_SRPC_METHOD_4(rpc4, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4);
    OVERRIDE_SRPC_METHOD_5(rpc5, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5);
    OVERRIDE_SRPC_METHOD_6(rpc6, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5, RInt32, p6);
    OVERRIDE_SRPC_METHOD_7(rpc7, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5, RInt32, p6, RInt32, p7);
    OVERRIDE_SRPC_METHOD_2(rpcBits, RInt15, p1, RInt31, p2);
    OVERRIDE_SRPC_METHOD_0(rpcFailed);
private:
    virtual void onForwarding(const srpc::RRpcId& rpcId) override;
private:
    srpc::RRpcId lastRpcId_;
};
