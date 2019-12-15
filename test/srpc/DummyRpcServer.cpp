#include "SrpcTestPCH.h"
#include "DummyRpcServer.h"
#include <sne/srpc/Exception.h>
#include <boost/core/ignore_unused.hpp>

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4311)
#endif

using namespace sne;

// = DummyRpcServer

IMPLEMENT_SRPC_EVENT_DISPATCHER(DummyRpcServer)

DummyRpcServer::DummyRpcServer(srpc::RpcNetwork* rpcNetwork) :
    srpc::RpcReceiver(rpcNetwork),
    srpc::RpcForwarder(rpcNetwork)
{
}


RECEIVE_SRPC_METHOD_WITH_HINT_0(DummyRpcServer, rpc0)
{
    rpcId_ = getRpcId_rpc0();
    rpcHint_ = reinterpret_cast<intptr_t>(rpcHint);
}


RECEIVE_SRPC_METHOD_WITH_HINT_1(DummyRpcServer, rpc1, int32_t, p1)
{
    rpcId_ = getRpcId_rpc1();
    p1_ = p1;
    rpcHint_ = reinterpret_cast<intptr_t>(rpcHint);
}


RECEIVE_SRPC_METHOD_WITH_HINT_2(DummyRpcServer, rpc2,
    RInt32, p1, RInt32, p2)
{
    rpcId_ = getRpcId_rpc2();
    p1_ = p1;
    p2_ = p2;
    rpcHint_ = reinterpret_cast<intptr_t>(rpcHint);
}


RECEIVE_SRPC_METHOD_WITH_HINT_3(DummyRpcServer, rpc3,
    RInt32, p1, RInt32, p2, RInt32, p3)
{
    rpcId_ = getRpcId_rpc3();
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;
    rpcHint_ = reinterpret_cast<intptr_t>(rpcHint);
}


RECEIVE_SRPC_METHOD_WITH_HINT_4(DummyRpcServer, rpc4,
    RInt32, p1, RInt32, p2, RInt32, p3, RInt32, p4)
{
    rpcId_ = getRpcId_rpc4();
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;
    p4_ = p4;
    rpcHint_ = reinterpret_cast<intptr_t>(rpcHint);
}


RECEIVE_SRPC_METHOD_WITH_HINT_5(DummyRpcServer, rpc5,
    RInt32, p1, RInt32, p2, RInt32, p3, RInt32, p4, RInt32, p5)
{
    rpcId_ = getRpcId_rpc5();
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;
    p4_ = p4;
    p5_ = p5;
    rpcHint_ = reinterpret_cast<intptr_t>(rpcHint);
}


RECEIVE_SRPC_METHOD_WITH_HINT_6(DummyRpcServer, rpc6,
    RInt32, p1, RInt32, p2, RInt32, p3, RInt32, p4, RInt32, p5, RInt32, p6)
{
    rpcId_ = getRpcId_rpc6();
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;
    p4_ = p4;
    p5_ = p5;
    p6_ = p6;
    rpcHint_ = reinterpret_cast<intptr_t>(rpcHint);
}


RECEIVE_SRPC_METHOD_WITH_HINT_7(DummyRpcServer, rpc7,
    RInt32, p1, RInt32, p2, RInt32, p3, RInt32, p4, RInt32, p5, RInt32, p6,
    RInt32, p7)
{
    rpcId_ = getRpcId_rpc7();
    p1_ = p1;
    p2_ = p2;
    p3_ = p3;
    p4_ = p4;
    p5_ = p5;
    p6_ = p6;
    p7_ = p7;
    rpcHint_ = reinterpret_cast<intptr_t>(rpcHint);
}


RECEIVE_SRPC_METHOD_2(DummyRpcServer, rpcBits,
    RInt15, p1, RInt31, p2)
{
    rpcId_ = getRpcId_rpcBits();
    p1_ = p1;
    p2_ = p2;
}


RECEIVE_SRPC_METHOD_0(DummyRpcServer, rpcFailed)
{
    rpcId_ = getRpcId_rpcFailed();
    throw srpc::RpcFailedException(__FILE__, __LINE__, "rpc failed");
}

// = DummyRpcServer2

IMPLEMENT_SRPC_EVENT_DISPATCHER(DummyRpcServer2)

DummyRpcServer2::DummyRpcServer2(srpc::RpcNetwork* rpcNetwork) :
    srpc::RpcReceiver(rpcNetwork),
    srpc::RpcForwarder(rpcNetwork)
{
}


RECEIVE_SRPC_METHOD_WITH_HINT_1(DummyRpcServer2, rpc1, int32_t, p1)
{
    boost::ignore_unused(rpcHint);
    rpcId_ = getRpcId_rpc1();
    p1_ = p1;
}

#ifdef _MSC_VER
#  pragma warning (pop)
#endif
