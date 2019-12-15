#pragma once

// workaround for 'TryEnterCriticalSection' compile error
// workaround for boost.log link error
#ifdef _WIN32
#	undef _WIN32_WINNT
#	define _WIN32_WINNT 0x601 // Windows 7
// Exclude rarely-used stuff from Windows headers
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

// SNE
#include <sne/Common.h>

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
