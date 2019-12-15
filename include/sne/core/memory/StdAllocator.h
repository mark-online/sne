#pragma once

#include <sne/Common.h>

#if defined(SNE_USE_BOOST_POOL_ALLOCATOR) || \
    defined(SNE_USE_BOOST_FAST_POOL_ALLOCATOR)

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127)
#endif
#include <boost/pool/pool_alloc.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#else

#include <memory>

#endif

#ifdef SNE_USE_BOOST_POOL_ALLOCATOR
#  define SneNormalAllocator boost::pool_allocator
#else
#  define SneNormalAllocator std::allocator
#endif

#ifdef SNE_USE_BOOST_FAST_POOL_ALLOCATOR
#  define SneNodeAllocator boost::fast_pool_allocator
#else
#  define SneNodeAllocator std::allocator
#endif
