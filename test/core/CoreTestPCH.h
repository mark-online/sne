#pragma once

#include <sne/Common.h>

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4512)
#endif
#include <gtest/gtest.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

// Boost
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4819)
#endif
#include <boost/mpl/vector.hpp>
#include <boost/type_traits.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

// CRT
#include <string>
#include <cassert>
