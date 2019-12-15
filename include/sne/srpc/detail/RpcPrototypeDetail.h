#pragma once

#include "../RpcPacketType.h"

/** @addtogroup RpcPrototype
* RPC 테스트용 매크로 정의
* @{
*/

// = DEFINE_SRPC_METHOD_DETAIL_n

#define DEFINE_SRPC_METHOD_DETAIL_0(RpcClass, method, rpcHint) \
    void RpcClass::method(const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_1(RpcClass, method, P1, p1, rpcHint) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_2(RpcClass, method, P1, p1, P2, p2, \
        rpcHint) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_3(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, rpcHint) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_4(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, rpcHint) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_5(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, rpcHint) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_6(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, rpcHint) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        SRPC_PARAM_TYPE(P6) p6, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_7(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, rpcHint) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        SRPC_PARAM_TYPE(P6) p6, SRPC_PARAM_TYPE(P7) p7, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_8(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, P8, p8, rpcHint) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        SRPC_PARAM_TYPE(P6) p6, SRPC_PARAM_TYPE(P7) p7, SRPC_PARAM_TYPE(P8) p8, \
        const void* rpcHint)

/** @} */ // addtogroup RpcPrototype
