#pragma once

#include "RpcPacketType.h"
#include "detail/RpcId.h"
#include <sne/core/container/Containers.h>
#include <sne/Common.h>
#include <boost/utility.hpp>
#include <cassert>

namespace sne { namespace core {
class IStream;
}} // namespace sne { namespace core {

namespace sne { namespace srpc {

class RpcReceiver;
class RpcCommand;

/**
 * @class RpcNetwork
 *
 * rpc network
 */
class SNE_API RpcNetwork : public boost::noncopyable
{
    typedef core::Vector<RpcReceiver*> RpcReceivers;
public:
    RpcNetwork();
    virtual ~RpcNetwork() {}

    /**
     * RpcReceiver를 등록한다.
     * - 다중 쓰레드 환경 하에서는 배타적(thread-safe)으로 처리해야 한다.
     */
    virtual void registerRpcReceiver(RpcReceiver& receiver);

    /**
     * RpcReceiver의 등록을 해제한다
     * - 다중 쓰레드 환경 하에서는 배타적(thread-safe)으로 처리해야 한다.
     */
    virtual void unregisterRpcReceiver(RpcReceiver& receiver);

    /**
     * 서버로 메세지를 전송한다.
     * @param command RPC Command 인스턴스.
     * @param packetType 패킷을 어떻게 전송할 것인가?
     * @param rpcHint RPC method를 통해 전달된 힌트 포인터.
     * - RpcCommand::marshal()을 호출하여 스트림으로 변환한 후 전송해야 한다.
     * - 다중 쓰레드 환경 하에서는 배타적(thread-safe)으로 처리해야 한다.
     */
    virtual void send(RpcCommand& command,
        RpcPacketType packetType = ptReliable,
        const void* rpcHint = nullptr) = 0;

    /**
     * 서버로 부터 메세지가 도착하였다
     * @param istream 입력 스트림.
     * @param rpcHint RPC method로 전달될 힌트 포인터.
     * @exception StreamingException 스트리밍 에러가 발생할 경우
     * @exception UnknownRpcMethodException 알 수 없는 요청이 발생할 경우
     * @exception RpcFailedException RPC요청을 처리하는 도중 에러가 발생할 경우
     * - 다중 쓰레드 환경 하에서는 배타적(thread-safe)으로 처리해야 한다
     */
    virtual void onReceive(core::IStream& istream, const void* rpcHint = nullptr);

private:
    bool handleMessage(RpcId rpcId, core::IStream& istream, const void* rpcHint);

private:
    RpcReceivers receivers_;
};

}} // namespace sne { namespace srpc
