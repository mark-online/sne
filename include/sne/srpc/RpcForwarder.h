#pragma once

#include "detail/RpcForwarderDetail.h"
#include <sne/Common.h>
#include <boost/noncopyable.hpp>
#include <cassert>

namespace sne { namespace srpc {

class RpcNetwork;

/** @addtogroup RpcForwarding
* RPC 송신
* @{
*/

/**
 * @class RpcForwarder
 *
 * rpc forwarder
 * - 클라이언트의 요청을 스트림으로 변환한 후 서버로 전송한다
 */
class SNE_API RpcForwarder : public boost::noncopyable
{
public:
    RpcForwarder(RpcNetwork* rpcNetwork = nullptr) :
        rpcNetwork_(rpcNetwork) {}

    virtual ~RpcForwarder() {}

    /**
     * RPC 메세지를 전송한다.
     * @param command RPC Command 인스턴스.
     * @param packetType 패킷을 어떻게 보낼 것인가? (TCP/IP에선 의미가 없다)
     * @param rpcHint RpcNetwork으로 넘길 힌트 포인터.
     */
    void forward(RpcCommand& command, RpcPacketType packetType,
        const void* rpcHint);

    /// 메세지 전송을 위한 RpcNetwork 인스턴스를 설정한다
    void setRpcNetwork(RpcNetwork& rpcNetwork) {
        rpcNetwork_ = &rpcNetwork;
    }

    void resetRpcNetwork() {
        rpcNetwork_ = nullptr;
    }
private:
    virtual void onForwarding(const RRpcId& /*rpcId*/) {}
private:
    RpcNetwork* rpcNetwork_;
};

}} // namespace sne { namespace srpc

// = Rpc request Macros

// = FORWARD_SRPC_METHOD_n

/// 파라미터가 없는 RPC Method를 정의한다.
#define FORWARD_SRPC_METHOD_0(RpcClass, method) \
    FORWARD_SRPC_METHOD_DETAIL_0(RpcClass, method, ::sne::srpc::ptReliable)

/// 파라미터가 1개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_1(RpcClass, method, T1, p1) \
    FORWARD_SRPC_METHOD_DETAIL_1(RpcClass, method, T1, p1, ::sne::srpc::ptReliable)

/// 파라미터가 2개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_2(RpcClass, method, T1, p1, T2, p2) \
    FORWARD_SRPC_METHOD_DETAIL_2(RpcClass, method, T1, p1, T2, p2, \
        ::sne::srpc::ptReliable)

/// 파라미터가 3개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_3(RpcClass, method, T1, p1, T2, p2, T3, p3) \
    FORWARD_SRPC_METHOD_DETAIL_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        ::sne::srpc::ptReliable)

/// 파라미터가 4개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4) \
    FORWARD_SRPC_METHOD_DETAIL_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, ::sne::srpc::ptReliable)

/// 파라미터가 5개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5) \
    FORWARD_SRPC_METHOD_DETAIL_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, ::sne::srpc::ptReliable)

/// 파라미터가 6개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6) \
    FORWARD_SRPC_METHOD_DETAIL_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, ::sne::srpc::ptReliable)

/// 파라미터가 7개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7) \
    FORWARD_SRPC_METHOD_DETAIL_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
    T4, p4, T5, p5, T6, p6, T7, p7, ::sne::srpc::ptReliable)

/// 파라미터가 8개인 RPC Method를 정의한다
#define FORWARD_SRPC_METHOD_8(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, T8, p8) \
    FORWARD_SRPC_METHOD_DETAIL_8(RpcClass, method, T1, p1, T2, p2, T3, p3, \
    T4, p4, T5, p5, T6, p6, T7, p7, T8, p8, ::sne::srpc::ptReliable)

/** @} */ // addtogroup RpcForwarding
