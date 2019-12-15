#include "SrpcPCH.h"
#include <sne/srpc/RpcForwarder.h>
#include <sne/srpc/RpcNetwork.h>

namespace sne { namespace srpc {

void RpcForwarder::forward(RpcCommand& command, RpcPacketType packetType,
    const void* rpcHint)
{
    onForwarding(command.getRpcId());

    if (rpcNetwork_ != nullptr) {
        rpcNetwork_->send(command, packetType, rpcHint);
    }
}

}} // namespace sne { namespace srpc {
