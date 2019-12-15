#pragma once

#include <sne/Common.h>
#include "detail/RpcInterfaceDetail.h"
#include "detail/TypeList.h"

/** @addtogroup RpcInterface
* RPC 인터페이스를 선언/정의한다
* @{
*/

// = srpc method declaration macros

// = DECLARE_SRPC_METHOD_n

/// 파라미터가 없는 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_0(rpcInterface, method) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 0) \
    VIRTUAL_SRPC_METHOD_0(method) = 0

/// 파라미터가 1개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_1(rpcInterface, method, T1, p1) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 1) \
    VIRTUAL_SRPC_METHOD_1(method, T1, p1) = 0

/// 파라미터가 2개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_2(rpcInterface, method, T1, p1, T2, p2) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 2) \
    VIRTUAL_SRPC_METHOD_2(method, T1, p1, T2, p2) = 0

/// 파라미터가 3개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_3(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 3) \
    VIRTUAL_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3) = 0

/// 파라미터가 4개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_4(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 4) \
    VIRTUAL_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) = 0

/// 파라미터가 5개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_5(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 5) \
    VIRTUAL_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) = 0

/// 파라미터가 6개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_6(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 6) \
    VIRTUAL_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6) = 0

/// 파라미터가 7개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_7(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 7) \
    VIRTUAL_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7) = 0

/// 파라미터가 8개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_8(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, T8, p8) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 8) \
    VIRTUAL_SRPC_METHOD_8(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7, T8, p8) = 0

// = OVERRIDE_SRPC_METHOD_n

/// 파라미터가 없는 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_0(method) \
    VIRTUAL_SRPC_METHOD_0(method) override

/// 파라미터가 1개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_1(method, T1, p1) \
    VIRTUAL_SRPC_METHOD_1(method, T1, p1) override

/// 파라미터가 2개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_2(method, T1, p1, T2, p2) \
    VIRTUAL_SRPC_METHOD_2(method, T1, p1, T2, p2) override

/// 파라미터가 3개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3) \
    VIRTUAL_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3) override

/// 파라미터가 4개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) \
    VIRTUAL_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) override

/// 파라미터가 5개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    VIRTUAL_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) override

/// 파라미터가 6개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6) \
    VIRTUAL_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6) override

/// 파라미터가 7개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7) \
    VIRTUAL_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7) override

/// 파라미터가 8개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_8(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7, T8, p8) \
    VIRTUAL_SRPC_METHOD_8(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7, T8, p8) override

// = VIRTUAL_SRPC_METHOD_n

/// 파라미터가 없는 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_0(method) \
    virtual void method(const void* rpcHint = nullptr)

/// 파라미터가 1개인 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_1(method, T1, p1) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, \
        const void* rpcHint = nullptr)

/// 파라미터가 2개인 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_2(method, T1, p1, T2, p2) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        const void* rpcHint = nullptr)

/// 파라미터가 3개인 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, const void* rpcHint = nullptr)

/// 파라미터가 4개인 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, \
        const void* rpcHint = nullptr)

/// 파라미터가 5개인 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, SRPC_PARAM_TYPE(T5) p5, \
        const void* rpcHint = nullptr)

/// 파라미터가 6개인 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, SRPC_PARAM_TYPE(T5) p5, \
        SRPC_PARAM_TYPE(T6) p6, const void* rpcHint = nullptr)

/// 파라미터가 7개인 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, SRPC_PARAM_TYPE(T5) p5, \
        SRPC_PARAM_TYPE(T6) p6, SRPC_PARAM_TYPE(T7) p7, \
        const void* rpcHint = nullptr)

/// 파라미터가 8개인 RPC 메쏘드를 선언한다.
#define VIRTUAL_SRPC_METHOD_8(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7, T8, p8) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, SRPC_PARAM_TYPE(T5) p5, \
        SRPC_PARAM_TYPE(T6) p6, SRPC_PARAM_TYPE(T7) p7, SRPC_PARAM_TYPE(T8) p8, \
        const void* rpcHint = nullptr)

/** @} */ // addtogroup RpcInterface
