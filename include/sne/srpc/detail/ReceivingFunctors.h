#pragma once

#include "TypeList.h"
#include <boost/utility.hpp>

namespace sne { namespace core {
class IStream;
}} // namespace sne { namespace core {

namespace sne { namespace srpc {

/** @addtogroup RpcReceiving
* @{
*/

/**
 * @class ReceivingFunctor
 *
 * receiving functor base class
 * - unmarshaling
 * - dispatching
 */
class ReceivingFunctor : public boost::noncopyable
{
public:
    ReceivingFunctor() {}
    virtual ~ReceivingFunctor() {}

    /// istream으로 부터 unmarshaling한다
    virtual void unmarshal(core::IStream& istream) = 0;

    /// functor를 호출한다
    virtual void call(void* objPtr, const void* rpcHint) = 0;
};

// = Unmarshal Functors

template <class T, typename TList>
struct ReceivingFunctorT;


/// 0 parameter
template<class T>
struct ReceivingFunctorT<T, SRPC_TYPELIST_0()> : public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& /*istream*/) override {}

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(rpcHint);
    }

    MemFnPtr memFnPtr_;
};


/// 1 parameter
template <class T, typename P1>
struct ReceivingFunctorT<T, SRPC_TYPELIST_1(P1)> : public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(typename SRPC_PARAM_TYPE(P1) p1,
        const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& istream) override {
        istream >> p1_;
    }

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, rpcHint);
    }

    MemFnPtr memFnPtr_;
    P1 p1_;
};


/// 2 parameter(s)
template <class T, typename P1, typename P2>
struct ReceivingFunctorT<T, SRPC_TYPELIST_2(P1, P2)> : public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& istream) override {
        istream >> p1_ >> p2_;
    }

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, rpcHint);
    }

    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
};


/// 3 parameter(s)
template <class T, typename P1, typename P2, typename P3>
struct ReceivingFunctorT<T, SRPC_TYPELIST_3(P1, P2, P3)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& istream) override {
        istream >> p1_ >> p2_ >> p3_;
    }

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, rpcHint);
    }

    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
};


/// 4 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4>
struct ReceivingFunctorT<T, SRPC_TYPELIST_4(P1, P2, P3, P4)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2,typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& istream) override {
        istream >> p1_ >> p2_ >> p3_ >> p4_;
    }

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, rpcHint);
    }

    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
};


/// 5 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4,
    typename P5>
struct ReceivingFunctorT<T, SRPC_TYPELIST_5(P1, P2, P3, P4, P5)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, typename SRPC_PARAM_TYPE(P5) p5,
        const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& istream) override {
        istream >> p1_ >> p2_ >> p3_ >> p4_ >> p5_;
    }

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, p5_,
            rpcHint);
    }

    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
    P5 p5_;
};


/// 6 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4,
    typename P5, typename P6>
struct ReceivingFunctorT<T, SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, typename SRPC_PARAM_TYPE(P5) p5,
        typename SRPC_PARAM_TYPE(P6) p6, const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& istream) override {
        istream >> p1_ >> p2_ >> p3_ >> p4_ >> p5_ >> p6_;
    }

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, p5_, p6_,
            rpcHint);
    }

    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
    P5 p5_;
    P6 p6_;
};


/// 7 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4,
    typename P5, typename P6, typename P7>
struct ReceivingFunctorT<T, SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, typename SRPC_PARAM_TYPE(P5) p5,
        typename SRPC_PARAM_TYPE(P6) p6, typename SRPC_PARAM_TYPE(P7) p7,
        const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& istream) override {
        istream >> p1_ >> p2_ >> p3_ >> p4_ >> p5_ >> p6_ >> p7_;
    }

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, p5_, p6_,
            p7_, rpcHint);
    }

    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
    P5 p5_;
    P6 p6_;
    P7 p7_;
};

/// 8 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4,
    typename P5, typename P6, typename P7, typename P8>
struct ReceivingFunctorT<T, SRPC_TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, typename SRPC_PARAM_TYPE(P5) p5,
        typename SRPC_PARAM_TYPE(P6) p6, typename SRPC_PARAM_TYPE(P7) p7,
        typename SRPC_PARAM_TYPE(P8) p8,
        const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(core::IStream& istream) override {
        istream >> p1_ >> p2_ >> p3_ >> p4_ >> p5_ >> p6_ >> p7_ >> p8_;
    }

    virtual void call(void* objPtr, const void* rpcHint) override {
        assert(objPtr != nullptr);
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, p5_, p6_,
            p7_, p8_, rpcHint);
    }

    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
    P5 p5_;
    P6 p6_;
    P7 p7_;
    P8 p8_;
};

/** @} */ // addtogroup RpcReceiving

}} // namespace sne { namespace srpc
