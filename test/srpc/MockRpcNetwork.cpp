#include "SrpcTestPCH.h"
#include "MockRpcNetwork.h"
#include <sne/core/stream/detail/SneStreamFactory.h>

using namespace sne::core;

MockRpcNetwork::MockRpcNetwork() :
    ostream_(
        SneStreamFactory(true, true).createOStream(buffer_).release()),
    sendFailed_(false)
{
}


MockRpcNetwork::~MockRpcNetwork()
{
}


void MockRpcNetwork::send(srpc::RpcCommand& command,
    srpc::RpcPacketType /*packetType*/, const void* rpcHint)
{
    try {
        command.marshal(*ostream_);
        sentRpcHint_ = static_cast<int>(reinterpret_cast<size_t>(rpcHint));
        //sendFailed_ = false;
    }
    catch (const core::Exception&) {
        sendFailed_ = true;
    }
}
