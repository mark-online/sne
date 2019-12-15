#pragma once

#include "ReceivingFunctors.h"
#include "RpcEvent.h"
#include "RpcId.h"
#include <sne/core/utility/CppUtils.h>
#include <sne/core/container/Containers.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <map>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <cassert>

namespace sne { namespace srpc {

/** @addtogroup RpcReceiving
* RPC 수신 & 처리
* @{
*/

/**
 * @class RpcEventMap
 * RpcEvent 컨테이너
 */
class RpcEventMap
{
public:
    typedef core::HashMap<RpcId, RpcEvent*> RpcEvents;
    typedef RpcEvents::value_type value_type;
public:
    RpcEventMap(bool deleteRpcEvents) :
        deleteRpcEvents_(deleteRpcEvents) {}

    ~RpcEventMap() {
        if (deleteRpcEvents_) {
            for (auto& entry : rpcEvents_) {
                boost::checked_delete(entry.second);
            }
        }
    }

    void insert(RpcId rpcId, RpcEvent& event) {
        assert(! isExists(rpcId));
        rpcEvents_.emplace(rpcId, &event);
    }

    RpcEvent* getRpcEvent(RpcId rpcId) const {
        return core::search_map(rpcEvents_, rpcId, 0);
    }

    const RpcEvents& getRpcEvents() const {
        return rpcEvents_;
    }

    bool isEmpty() const {
        return rpcEvents_.empty();
    }
private:
    bool isExists(RpcId rpcId) const {
        return rpcEvents_.find(rpcId) != rpcEvents_.end();
    }
private:
    bool deleteRpcEvents_;
    RpcEvents rpcEvents_;
};

/**
 * @class EventRegister
 *
 * 생성자를 통해서 RpcEvent를 등록한다.
 */
template <class T, class RpcClass>
class EventRegister
{
public:
    EventRegister(RpcId rpcId, T& event) {
        RpcClass::getStaticEventMap().insert(rpcId, event);
    }
};

}} // namespace sne { namespace srpc

// = int_ternal Macros

#define SRPC_RPC_EVENT(RpcClass, method) \
    RpcEvent_##RpcClass##_##method

#define SRPC_RPC_EVENT_INSTANCE(RpcClass, method) \
    RpcEvent_##RpcClass##_##method##_instance

#ifdef USE_BOOST_FAST_POOL_ALLOCATOR_FOR_SRPC
// boost::pool을 쓸 경우 singleton pool의 소멸자가 먼저 호출되어
// 메모리를 2번 해제하는 문제가 발생하여 메모리를 해제하지 않게함
#   define DEFINE_GET_SRPC_EVENT_MAP(RpcClass) \
        ::sne::srpc::RpcEventMap& RpcClass :: getStaticEventMap() { \
            static std::unique_ptr<::sne::srpc::RpcEventMap> RpcClass##_staticEventMap = \
                std::make_unique<::sne::srpc::RpcEventMap>(false); \
            return *RpcClass##_staticEventMap; \
        }
#else
#   define DEFINE_GET_SRPC_EVENT_MAP(RpcClass) \
        ::sne::srpc::RpcEventMap& RpcClass :: getStaticEventMap() { \
            static ::sne::srpc::RpcEventMap RpcClass##_staticEventMap(false); \
            return RpcClass##_staticEventMap; \
        }
#endif

/// SRPC 헬퍼를 선언한다.
#define DECLARE_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass) \
    public: \
        static ::sne::srpc::RpcEventMap& getStaticEventMap(); \
    protected: \
        virtual const ::sne::srpc::RpcEventMap& \
            getDefaultEventMap() const override { \
            return RpcClass :: getStaticEventMap(); \
        } \
    private: \
        virtual void dispatch(::sne::srpc::RpcEvent& event, \
            ::sne::core::IStream& istream, const void* rpcHint) override;

/// SRPC 헬퍼를 구현한다.
#define IMPLEMENT_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass) \
    DEFINE_GET_SRPC_EVENT_MAP(RpcClass); \
    void RpcClass::dispatch(::sne::srpc::RpcEvent& event, \
        ::sne::core::IStream& istream, const void* rpcHint) { \
        event.dispatch(this, istream, rpcHint); \
    }

/// RPC 메쏘드를 등록한다
#define REGISTER_SRPC_METHOD(RpcClass, method, TypeList, suffix) \
    namespace { \
        class SRPC_RPC_EVENT(RpcClass, method) : public ::sne::srpc::RpcEvent \
        { \
        public: \
            SRPC_RPC_EVENT(RpcClass, method)() : \
                ::sne::srpc::RpcEvent(RpcClass::SRPC_GET_RPCID(method)()), \
                unmarshalFunctor_(&RpcClass::method##suffix) {} \
        private: \
            virtual std::unique_ptr<RpcEvent> clone() const override { \
                return std::make_unique<SRPC_RPC_EVENT(RpcClass, method)>(); \
            } \
            virtual ::sne::srpc::ReceivingFunctor& getDispatcher() override { \
                return unmarshalFunctor_; \
            } \
        private: \
            ::sne::srpc::ReceivingFunctorT<RpcClass, TypeList > \
                unmarshalFunctor_; \
        }; \
        static SRPC_RPC_EVENT(RpcClass, method) \
            SRPC_RPC_EVENT_INSTANCE(RpcClass, method); \
        ::sne::srpc::EventRegister<SRPC_RPC_EVENT(RpcClass, method), RpcClass> \
            RpcClass##_##method##_EventRegister( \
                RpcClass::SRPC_GET_RPCID(method)(), \
                SRPC_RPC_EVENT_INSTANCE(RpcClass, method)); \
    }

// = RECEIVE_SRPC_METHOD_DETAIL_n

/// 파라미터가 없는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_0(RpcClass, method, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_0(), ); \
    void RpcClass::method(const void* rpcHint)

/// 파라미터가 1개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_1(RpcClass, method, \
        P1, p1, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_1(P1), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, const void* rpcHint)

/// 파라미터가 2개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_2(RpcClass, method, \
        P1, p1, P2, p2, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_2(P1, P2), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        const void* rpcHint)

/// 파라미터가 3개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_3(P1, P2, P3), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, const void* rpcHint)

/// 파라미터가 4개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_4(P1, P2, P3, P4), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, const void* rpcHint)

/// 파라미터가 5개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_5(P1, P2, P3, P4, P5), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        const void* rpcHint)

/// 파라미터가 6개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        SRPC_PARAM_TYPE(P6) p6, const void* rpcHint)

/// 파라미터가 7개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        SRPC_PARAM_TYPE(P6) p6, SRPC_PARAM_TYPE(P7) p7, const void* rpcHint)

/// 파라미터가 8개 있는 RPC Method를 구현한다
#define RECEIVE_SRPC_METHOD_DETAIL_8(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, P8, p8, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        SRPC_PARAM_TYPE(P6) p6, SRPC_PARAM_TYPE(P7) p7, SRPC_PARAM_TYPE(P8) p8, \
        const void* rpcHint)

/** @} */ // addtogroup RpcReceiving
