#pragma once

#include <sne/srpc/detail/RpcEvent.h>
#include <sne/srpc/detail/ReceivingFunctors.h>
#include <sne/srpc/RpcTypes.h>

/**
 * @class DummyRpcEvent
 */
class DummyRpcEvent : public sne::srpc::RpcEvent
{
public:
    DummyRpcEvent() :
        sne::srpc::RpcEvent(sne::srpc::RRpcId()),
        unmarshalFunctor_(&DummyRpcEvent::handler2),
        p1_(0),
        p2_(0) {}

    int32_t getP1() const {
        return p1_;
    }
    int32_t getP2() const {
        return p2_;
    }
private:
    virtual std::unique_ptr<sne::srpc::RpcEvent> clone() const override {
        return std::unique_ptr<sne::srpc::RpcEvent>();
    }
    virtual sne::srpc::ReceivingFunctor& getDispatcher() override {
        return unmarshalFunctor_;
    }
private:
    void handler2(const sne::srpc::RInt32& p1,
        const sne::srpc::RInt32& p2, const void* /*rpcHint*/) {
        p1_ = p1;
        p2_ = p2;
    }
private:
    sne::srpc::ReceivingFunctorT<DummyRpcEvent,
        SRPC_TYPELIST_2(sne::srpc::RInt32, sne::srpc::RInt32)> unmarshalFunctor_;
    sne::srpc::RInt32 p1_;
    sne::srpc::RInt32 p2_;
};
