#pragma once

// workaround for 'TryEnterCriticalSection' compile error
#ifdef _WIN32
#  ifndef _WIN32_WINNT
#    define _WIN32_WINNT 0x400
#  endif
// Exclude rarely-used stuff from Windows headers
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

// SNE
#include <sne/Common.h>

// GMOCK
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4512)
#endif
#include <gmock/gmock.h>
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
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
