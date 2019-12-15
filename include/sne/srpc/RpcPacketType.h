#pragma once

#include "RpcTypes.h"

namespace sne { namespace srpc {

/// 패킷을 어떻게 전송할 것인가? (TCP/IP에선 의미가 없다)
enum RpcPacketType
{
    ptUnknown = -1,
    ptReliable, ///< 순서 보장. 전송 보장
    ptUnreliable ///< 순서, 전송 보장하지 않음
};

/// 신뢰성 있는 패킷인가?
inline bool isReliable(RpcPacketType packetType)
{
    return ptReliable == packetType;
}


/// 비신뢰성 있는 패킷인가?
inline bool isUnreliable(RpcPacketType packetType)
{
    return ptUnreliable == packetType;
}


/**
 * RpcType for RpcPacketType
 */
typedef RpcNumericType<RpcPacketType, 1, uint8_t> RRpcPacketType;

}} // namespace sne { namespace srpc
