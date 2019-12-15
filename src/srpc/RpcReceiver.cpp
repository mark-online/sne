#include "SrpcPCH.h"
#include <sne/srpc/RpcReceiver.h>
#include <sne/srpc/RpcNetwork.h>
#include <sne/core/stream/IStream.h>

namespace sne { namespace srpc {

RpcReceiver::RpcReceiver(RpcNetwork* rpcNetwork) :
    rpcNetwork_(nullptr),
    eventMap_(true)
{
    if (rpcNetwork != nullptr) {
        setRpcNetwork(*rpcNetwork);
    }
}


RpcReceiver::~RpcReceiver()
{
}


bool RpcReceiver::handle(RpcId rpcId, core::IStream& istream,
    const void* rpcHint)
{
    if (shouldCloneRpcEvents()) {
        cloneRpcEvents();
    }

    RpcEvent* rpcEvent = lookupEvent(rpcId);
    if (! rpcEvent)
        return false;

    return handleEvent(*rpcEvent, istream, rpcHint);
}


void RpcReceiver::setRpcNetwork(RpcNetwork& rpcNetwork)
{
    rpcNetwork_ = &rpcNetwork;
    assert(rpcNetwork_ != nullptr);
    rpcNetwork_->registerRpcReceiver(*this);
}


void RpcReceiver::resetRpcNetwork()
{
    if (rpcNetwork_ != nullptr) {
        rpcNetwork_->unregisterRpcReceiver(*this);
        rpcNetwork_ = nullptr;
    }
}


bool RpcReceiver::handleEvent(RpcEvent& rpcEvent, core::IStream& istream,
    const void* rpcHint)
{
    onReceiving(rpcEvent.getRpcId());

    // CAUTION: 다중상속으로 인해 잘못된 this 포인터를 이용하여 멤버 함수가
    // 호출되는 것을 방지하기 위해 우회(indirection)함
    // - 참고: http://www.codeproject.com/cpp/FastDelegate.asp
    dispatch(rpcEvent, istream, rpcHint);

    istream.align();

    return true;
}


void RpcReceiver::cloneRpcEvents()
{
    const RpcEventMap::RpcEvents& defaultEvents =
        getDefaultEventMap().getRpcEvents();
    for (const auto& value : defaultEvents) {
        const RpcId rpcId = value.first;
        const RpcEvent* event = value.second;
        insertRpcEvent(rpcId, *(event->clone().release()));
    }
}

}} // namespace sne { namespace srpc {
