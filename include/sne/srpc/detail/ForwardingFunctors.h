#pragma once

#include "TypeList.h"
#include "../RpcTypes.h"
#include <boost/utility.hpp>

namespace sne { namespace core {
class OStream;
}} // namespace sne { namespace core {

namespace sne { namespace srpc {

/** @addtogroup RpcForwarding
* @{
*/

/**
 * @struct ForwardingFunctor
 *
 * forwarding functor base struct
 * - marshaling
 */
class ForwardingFunctor : public boost::noncopyable
{
public:
    ForwardingFunctor() {}
    virtual ~ForwardingFunctor() {}

    virtual void marshal(core::OStream& ostream) = 0;
};

// = Marshal Functors

template <typename TList>
struct ForwardingFunctorT;


/// 0 parameter
template<>
struct ForwardingFunctorT<SRPC_TYPELIST_0()> : public ForwardingFunctor
{
    virtual void marshal(core::OStream& /*ostream*/) override {}
};


/// 1 parameter
template <typename P1>
struct ForwardingFunctorT<SRPC_TYPELIST_1(P1)> : public ForwardingFunctor
{
    ForwardingFunctorT(typename SRPC_PARAM_TYPE(P1) p1) :
        p1_(p1) {}

    virtual void marshal(core::OStream& ostream) override {
        ostream << p1_;
    }

    typename SRPC_VALUE_TYPE_4_FORWARDER(P1) p1_;
};


/// 2 parameters
template <typename P1, typename P2>
struct ForwardingFunctorT<SRPC_TYPELIST_2(P1, P2)> : public ForwardingFunctor
{
    ForwardingFunctorT(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2) :
        p1_(p1), p2_(p2) {}

    virtual void marshal(core::OStream& ostream) override {
        ostream << p1_ << p2_;
    }

    typename SRPC_VALUE_TYPE_4_FORWARDER(P1) p1_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P2) p2_;
};


/// 3 parameters
template <typename P1, typename P2, typename P3>
struct ForwardingFunctorT<SRPC_TYPELIST_3(P1, P2, P3)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3) :
        p1_(p1), p2_(p2), p3_(p3) {}

    virtual void marshal(core::OStream& ostream) override {
        ostream << p1_ << p2_ << p3_;
    }

    typename SRPC_VALUE_TYPE_4_FORWARDER(P1) p1_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P2) p2_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P3) p3_;
};


/// 4 parameters
template <typename P1, typename P2, typename P3, typename P4>
struct ForwardingFunctorT<SRPC_TYPELIST_4(P1, P2, P3, P4)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4) {}

    virtual void marshal(core::OStream& ostream) override {
        ostream << p1_ << p2_ << p3_ << p4_;
    }

    typename SRPC_VALUE_TYPE_4_FORWARDER(P1) p1_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P2) p2_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P3) p3_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P4) p4_;
};


/// 5 parameters
template <typename P1, typename P2, typename P3, typename P4, typename P5>
struct ForwardingFunctorT<SRPC_TYPELIST_5(P1, P2, P3, P4, P5)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, typename SRPC_PARAM_TYPE(P5) p5) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5) {}

    virtual void marshal(core::OStream& ostream) override {
        ostream << p1_ << p2_ << p3_ << p4_ << p5_;
    }

    typename SRPC_VALUE_TYPE_4_FORWARDER(P1) p1_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P2) p2_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P3) p3_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P4) p4_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P5) p5_;
};


/// 6 parameters
template <typename P1, typename P2, typename P3, typename P4, typename P5,
    typename P6>
struct ForwardingFunctorT<SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, typename SRPC_PARAM_TYPE(P5) p5,
        typename SRPC_PARAM_TYPE(P6) p6) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5), p6_(p6) {}

    virtual void marshal(core::OStream& ostream) override {
        ostream << p1_ << p2_ << p3_ << p4_ << p5_ << p6_;
    }

    typename SRPC_VALUE_TYPE_4_FORWARDER(P1) p1_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P2) p2_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P3) p3_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P4) p4_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P5) p5_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P6) p6_;
};


/// 7 parameters
template <typename P1, typename P2, typename P3, typename P4, typename P5,
    typename P6, typename P7>
struct ForwardingFunctorT<SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, typename SRPC_PARAM_TYPE(P5) p5,
        typename SRPC_PARAM_TYPE(P6) p6, typename SRPC_PARAM_TYPE(P7) p7) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5), p6_(p6), p7_(p7) {}

    virtual void marshal(core::OStream& ostream) override {
        ostream << p1_ << p2_ << p3_ << p4_ << p5_ << p6_ << p7_;
    }

    typename SRPC_VALUE_TYPE_4_FORWARDER(P1) p1_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P2) p2_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P3) p3_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P4) p4_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P5) p5_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P6) p6_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P7) p7_;
};

/// 8 parameters
template <typename P1, typename P2, typename P3, typename P4, typename P5,
    typename P6, typename P7, typename P8>
struct ForwardingFunctorT<SRPC_TYPELIST_8(P1, P2, P3, P4, P5, P6, P7, P8)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(typename SRPC_PARAM_TYPE(P1) p1,
        typename SRPC_PARAM_TYPE(P2) p2, typename SRPC_PARAM_TYPE(P3) p3,
        typename SRPC_PARAM_TYPE(P4) p4, typename SRPC_PARAM_TYPE(P5) p5,
        typename SRPC_PARAM_TYPE(P6) p6, typename SRPC_PARAM_TYPE(P7) p7,
        typename SRPC_PARAM_TYPE(P8) p8) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5), p6_(p6), p7_(p7), p8_(p8) {}

    virtual void marshal(core::OStream& ostream) override {
        ostream << p1_ << p2_ << p3_ << p4_ << p5_ << p6_ << p7_ << p8_;
    }

    typename SRPC_VALUE_TYPE_4_FORWARDER(P1) p1_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P2) p2_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P3) p3_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P4) p4_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P5) p5_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P6) p6_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P7) p7_;
    typename SRPC_VALUE_TYPE_4_FORWARDER(P8) p8_;
};

/** @} */ // addtogroup RpcForwarding

}} // namespace sne { namespace srpc
