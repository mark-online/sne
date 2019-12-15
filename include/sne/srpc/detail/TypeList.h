#pragma once

#include <boost/call_traits.hpp>
#include <boost/mpl/vector.hpp>


#define SRPC_TYPELIST_0() \
    boost::mpl::vector<>

#define SRPC_TYPELIST_1(T1) \
    boost::mpl::vector<T1>

#define SRPC_TYPELIST_2(T1, T2) \
    boost::mpl::vector<T1, T2>

#define SRPC_TYPELIST_3(T1, T2, T3) \
    boost::mpl::vector<T1, T2, T3>

#define SRPC_TYPELIST_4(T1, T2, T3, T4) \
    boost::mpl::vector<T1, T2, T3, T4>

#define SRPC_TYPELIST_5(T1, T2, T3, T4, T5) \
    boost::mpl::vector<T1, T2, T3, T4, T5>

#define SRPC_TYPELIST_6(T1, T2, T3, T4, T5, T6) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6>

#define SRPC_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6, T7>

#define SRPC_TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6, T7, T8>

#define SRPC_TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6, T7, T8, T9>

#define SRPC_TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>

// = Type utility

#define SRPC_VALUE_TYPE(T)  boost::call_traits<T>::value_type
#define SRPC_PARAM_TYPE(T)  boost::call_traits<T>::param_type

#define SRPC_VALUE_TYPE_4_FORWARDER(T) boost::call_traits<T>::param_type
