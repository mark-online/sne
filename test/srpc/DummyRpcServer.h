#pragma once

#include "DummyRpcInterface.h"
#include <sne/srpc/RpcForwarder.h>
#include <sne/srpc/RpcReceiver.h>
#include <sne/srpc/detail/RpcId.h>
#include <string>

using namespace sne;

/**
 * @class DummyRpcServer
 */
class DummyRpcServer : public DummyRpc,
    private srpc::RpcReceiver,
    private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(DummyRpcServer);
public:
    DummyRpcServer(srpc::RpcNetwork* rpcNetwork = nullptr);

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
    virtual void onReceiving(const srpc::RRpcId& rpcId) override {
        lastRpcId_ = rpcId;
    }
public:
    const srpc::RRpcId& getRpcId() const {
        return rpcId_;
    }
    int32_t getP1() const {
        return p1_;
    }
    int32_t getP2() const {
        return p2_;
    }
    int32_t getP3() const {
        return p3_;
    }
    int32_t getP4() const {
        return p4_;
    }
    int32_t getP5() const {
        return p5_;
    }
    int32_t getP6() const {
        return p6_;
    }
    int32_t getP7() const {
        return p7_;
    }
    int32_t getP8() const {
        return p8_;
    }
    int32_t getP9() const {
        return p9_;
    }
    int32_t getP10() const {
        return p10_;
    }
    intptr_t getHint() const {
        return rpcHint_;
    }

    const srpc::RRpcId& getLastRpcId() const {
        return lastRpcId_;
    }
private:
    srpc::RRpcId rpcId_;
    int32_t p1_;
    int32_t p2_;
    int32_t p3_;
    int32_t p4_;
    int32_t p5_;
    int32_t p6_;
    int32_t p7_;
    int32_t p8_;
    int32_t p9_;
    int32_t p10_;
    intptr_t rpcHint_;

    srpc::RRpcId lastRpcId_;
};


/**
 * @class DummyRpcServer2
 */
class DummyRpcServer2 : public DummyRpc2,
    private srpc::RpcReceiver,
    private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(DummyRpcServer2);
public:
    DummyRpcServer2(srpc::RpcNetwork* rpcNetwork = nullptr);

    OVERRIDE_SRPC_METHOD_1(rpc1, int32_t, p1);
public:
    const srpc::RpcId getRpcId() const {
        return rpcId_;
    }
    int32_t getP1() const {
        return p1_;
    }
private:
    srpc::RpcId rpcId_;
    int32_t p1_;
};
