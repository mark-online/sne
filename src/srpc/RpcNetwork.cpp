#include "SrpcPCH.h"
#include <sne/srpc/RpcNetwork.h>
#include <sne/srpc/RpcReceiver.h>
#include <sne/srpc/Exception.h>
#include <sne/core/stream/IStream.h>
#include <sne/core/utility/CppUtils.h>
#include <algorithm>

namespace sne { namespace srpc {

RpcNetwork::RpcNetwork()
{
    // registerRpcReceiver 호출시 메모리 할당이 일어나는 것을 막기 위해
    const size_t reservedCount = 10;
    receivers_.reserve(reservedCount);
}


void RpcNetwork::registerRpcReceiver(RpcReceiver& receiver)
{
    assert(std::find(receivers_.begin(), receivers_.end(), &receiver) ==
        receivers_.end());
    receivers_.push_back(&receiver);
}


void RpcNetwork::unregisterRpcReceiver(RpcReceiver& receiver)
{
    const RpcReceivers::iterator pos =
        std::find(receivers_.begin(), receivers_.end(), &receiver);
    if (pos != receivers_.end()) {
        receivers_.erase(pos);
    }
}


void RpcNetwork::onReceive(core::IStream& istream, const void* rpcHint)
{
    RpcId rpcId;
    istream.read(rpcId);

    if (handleMessage(rpcId, istream, rpcHint)) {
        return; // succeeded
    }

    const std::string msg = core::formatString("Unknown RPC(Id: %u)", rpcId);
    throw srpc::UnknownRpcMethodException(__FILE__, __LINE__, msg.c_str());
}


bool RpcNetwork::handleMessage(RpcId rpcId, core::IStream& istream,
    const void* rpcHint)
{
    // TODO: 최적화
    for (RpcReceiver* receiver : receivers_) {
        if (receiver->handle(rpcId, istream, rpcHint)) {
            return true;
        }
    }

    return false;
}

}} // namespace sne { namespace srpc {
