#pragma once

#include "RpcCommand.h"
#include "ForwardingFunctors.h"
#include "../RpcPrototype.h"
#include "../RpcPacketType.h"

/** @addtogroup RpcForwarding
* RPC 송신
* @{
*/

// = Rpc request Macros

// = FORWARD_SRPC_METHOD_DETAIL_n

/// 파라미터가 없는 RPC Method를 정의한다.
#define FORWARD_SRPC_METHOD_DETAIL_0(RpcClass, method, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_0(RpcClass, method) \
    { \
        ::sne::srpc::ForwardingFunctorT<SRPC_TYPELIST_0() > marshaler; \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 1개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_DETAIL_1(RpcClass, method, \
        P1, p1, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_1(RpcClass, method, P1, p1) \
    { \
        ::sne::srpc::ForwardingFunctorT<SRPC_TYPELIST_1(P1) > marshaler(p1); \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 2개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_DETAIL_2(RpcClass, method, \
        P1, p1, P2, p2, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_2(RpcClass, method, P1, p1, P2, p2) \
    { \
        ::sne::srpc::ForwardingFunctorT<SRPC_TYPELIST_2(P1, P2) > marshaler(p1, p2); \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 3개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_3(RpcClass, method, P1, p1, P2, p2, \
        P3, p3) \
    { \
        ::sne::srpc::ForwardingFunctorT<SRPC_TYPELIST_3(P1, P2, P3) > \
            marshaler(p1, p2, p3); \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 4개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_4(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4) \
    { \
        ::sne::srpc::ForwardingFunctorT<SRPC_TYPELIST_4(P1, P2, P3, P4) > \
            marshaler(p1, p2, p3, p4); \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 5개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_5(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5) \
    { \
        ::sne::srpc::ForwardingFunctorT<SRPC_TYPELIST_5(P1, P2, P3, P4, P5) > \
            marshaler(p1, p2, p3, p4, p5); \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 6개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_6(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6) \
    { \
        ::sne::srpc::ForwardingFunctorT<SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6) > \
            marshaler(p1, p2, p3, p4, p5,  p6); \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 7개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_7(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7) \
    { \
        ::sne::srpc::ForwardingFunctorT< \
            SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7) > \
                marshaler(p1, p2, p3, p4, p5, p6, p7); \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 8개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_DETAIL_8(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, P8, p8, packetType) \
    DEFINE_SNE_SRPC_METHOD_WITH_HINT_8(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, P8, p8) \
    { \
        ::sne::srpc::ForwardingFunctorT< \
            SRPC_TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8) > \
                marshaler(p1, p2, p3, p4, p5, p6, p7, p8); \
        ::sne::srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/** @} */ // addtogroup RpcForwarding
