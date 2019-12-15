#pragma once

#include <sne/Common.h>
#include <sne/srpc/RpcInterface.h>
#include <sne/srpc/RpcTypes.h>


using sne::srpc::RInt32;
using sne::srpc::RpcNumericType;

typedef RpcNumericType<int16_t, 15> RInt15;
typedef RpcNumericType<int32_t, 31> RInt31;

/**
 * @class DummyRpc
 */
class DummyRpc
{
public:
    virtual ~DummyRpc() {}

    // = request

    DECLARE_SRPC_METHOD_0(DummyRpc, rpc0);
    DECLARE_SRPC_METHOD_1(DummyRpc, rpc1, int32_t, p1);
    DECLARE_SRPC_METHOD_2(DummyRpc, rpc2, RInt32, p1, RInt32, p2);
    DECLARE_SRPC_METHOD_3(DummyRpc, rpc3, RInt32, p1, RInt32, p2,
        RInt32, p3);
    DECLARE_SRPC_METHOD_4(DummyRpc, rpc4, RInt32, p1, RInt32, p2,
        RInt32, p3, RInt32, p4);
    DECLARE_SRPC_METHOD_5(DummyRpc, rpc5, RInt32, p1, RInt32, p2,
        RInt32, p3, RInt32, p4, RInt32, p5);
    DECLARE_SRPC_METHOD_6(DummyRpc, rpc6, RInt32, p1, RInt32, p2,
        RInt32, p3, RInt32, p4, RInt32, p5, RInt32, p6);
    DECLARE_SRPC_METHOD_7(DummyRpc, rpc7, RInt32, p1, RInt32, p2,
        RInt32, p3, RInt32, p4, RInt32, p5, RInt32, p6, RInt32, p7);
    DECLARE_SRPC_METHOD_2(DummyRpc, rpcBits, RInt15, p1, RInt31, p2);
    DECLARE_SRPC_METHOD_0(DummyRpc, rpcFailed);
};


/**
 * @class DummyRpc2
 */
class DummyRpc2
{
public:
    virtual ~DummyRpc2() {}

    // = request
    DECLARE_SRPC_METHOD_1(DummyRpc2, rpc1, int32_t, p1);
};
