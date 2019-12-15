#pragma once

#include "detail/RpcReceiverDetail.h"
#include <sne/Common.h>
#include <cassert>

namespace sne { namespace core {
class IStream;
}} // namespace sne { namespace core {

namespace sne { namespace srpc {

class RpcNetwork;

/** @addtogroup RpcReceiving
* RPC 수신 & 처리
* @{
*/

/**
 * @class RpcReceiver
 *
 * rpc receiver
 * - 서버로 부터의 응답을 받아 처리한다
 */
class SNE_API RpcReceiver : public boost::noncopyable
{
public:
    RpcReceiver(RpcNetwork* rpcNetwork = nullptr);
    virtual ~RpcReceiver();

    /// RPC 요청을 파싱하여 RPC method를 실행한다.
    bool handle(RpcId rpcId, core::IStream& istream, const void* rpcHint = nullptr);

    void setRpcNetwork(RpcNetwork& rpcNetwork);
    void resetRpcNetwork();
protected:
    virtual const RpcEventMap& getDefaultEventMap() const = 0;
private:
    virtual void onReceiving(const RRpcId& /*rpcId*/) {}

    virtual void dispatch(RpcEvent& rpcEvent, core::IStream& istream,
        const void* rpcHint) = 0;
private:
    void cloneRpcEvents();
    void insertRpcEvent(RpcId rpcId, RpcEvent& event) {
        eventMap_.insert(rpcId, event);
    }
    RpcEvent* lookupEvent(RpcId rpcId) const {
        return eventMap_.getRpcEvent(rpcId);
    }
    bool handleEvent(RpcEvent& rpcEvent, core::IStream& istream,
        const void* rpcHint);
    bool shouldCloneRpcEvents() const {
        return eventMap_.isEmpty();
    }
private:
    RpcNetwork* rpcNetwork_;
    RpcEventMap eventMap_;
};

}} // namespace sne { namespace srpc

// = Macros

/// SRPC 헬퍼를 선언한다.
#define DECLARE_SRPC_EVENT_DISPATCHER(RpcClass) \
    DECLARE_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass)

/// SRPC 헬퍼를 구현한다.
#define IMPLEMENT_SRPC_EVENT_DISPATCHER(RpcClass) \
    IMPLEMENT_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass)

// = RECEIVE_SRPC_METHOD_n

/// 힌트가 없고 파라미터가 없는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_0(RpcClass, method) \
    RECEIVE_SRPC_METHOD_DETAIL_0(RpcClass, method, )

/// 힌트가 없고 파라미터가 1개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_1(RpcClass, method, T1, p1) \
    RECEIVE_SRPC_METHOD_DETAIL_1(RpcClass, method, T1, p1, )

/// 힌트가 없고 파라미터가 2개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_2(RpcClass, method, T1, p1, T2, p2) \
    RECEIVE_SRPC_METHOD_DETAIL_2(RpcClass, method, T1, p1, T2, p2, )

/// 힌트가 없고 파라미터가 3개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3) \
    RECEIVE_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, )

/// 힌트가 없고 파라미터가 4개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4) \
    RECEIVE_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, )

/// 힌트가 없고 파라미터가 5개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_5(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    RECEIVE_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, )

/// 힌트가 없고 파라미터가 6개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6) \
    RECEIVE_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, )

/// 힌트가 없고 파라미터가 7개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_7(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    RECEIVE_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, )

/// 힌트가 없고 파라미터가 8개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_8(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, T8, p8) \
    RECEIVE_SRPC_METHOD_DETAIL_8(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, T8, p8, )

// = RECEIVE_SRPC_METHOD_WITH_HINT_n

/// 힌트가 있고 파라미터가 없는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_WITH_HINT_0(RpcClass, method) \
    RECEIVE_SRPC_METHOD_DETAIL_0(RpcClass, method, rpcHint)

/// 힌트가 있고 파라미터가 1개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_WITH_HINT_1(RpcClass, method, T1, p1) \
    RECEIVE_SRPC_METHOD_DETAIL_1(RpcClass, method, \
        T1, p1, rpcHint)

/// 힌트가 있고 파라미터가 2개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_WITH_HINT_2(RpcClass, method, \
        T1, p1, T2, p2) \
    RECEIVE_SRPC_METHOD_DETAIL_2(RpcClass, method, \
        T1, p1, T2, p2, rpcHint)

/// 힌트가 있고 파라미터가 3개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_WITH_HINT_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3) \
    RECEIVE_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, rpcHint)

/// 힌트가 있고 파라미터가 4개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_WITH_HINT_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4) \
    RECEIVE_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, rpcHint)

/// 힌트가 있고 파라미터가 5개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_WITH_HINT_5(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    RECEIVE_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, rpcHint)

/// 힌트가 있고 파라미터가 6개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_WITH_HINT_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6) \
    RECEIVE_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, rpcHint)

/// 힌트가 있고 파라미터가 7개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_WITH_HINT_7(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    RECEIVE_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, rpcHint)

/** @} */ // addtogroup RpcReceiving
