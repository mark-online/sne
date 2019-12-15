#pragma once

#ifdef _WIN32
// workaround for 'TryEnterCriticalSection' compile error
// workaround for boost.log link error
#	undef _WIN32_WINNT
#	define _WIN32_WINNT 0x601 // Windows 7
// Exclude rarely-used stuff from Windows headers
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>

// https://blogs.technet.microsoft.com/wincat/2012/12/05/fast-tcp-loopback-performance-and-low-latency-with-windows-server-2012-tcp-loopback-fast-path/
#   if defined(_WIN32_WINNT) && (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
#       define SNE_USE_TCP_LOOPBACK_FAST_PATH
#   endif
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
#include <boost/smart_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

// CRT
#include <string>
#include <vector>
#include <cassert>
