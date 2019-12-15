#pragma once

#include <sne/Common.h>

#ifdef SNE_USE_BOOST_MEMORY_POOL
#   include <boost/pool/singleton_pool.hpp>
#endif
#include <boost/core/ignore_unused.hpp>

namespace sne { namespace core {

/** @addtogroup memory
* @{
*/

#ifdef SNE_USE_BOOST_MEMORY_POOL

/**
 * @class MemoryPoolMixin
 * boost::pool 기반의 메모리 풀 mixin
 * - thread-UNSAFE
 */
template <typename T>
class MemoryPoolMixin
{
public:
    void* operator new(std::size_t size) {
        assert(size == sizeof(T));
        return pool().malloc();
    }

    void operator delete(void* p) {
        pool().free(p);
    }

private:
    static boost::pool<>& pool() {
        static boost::pool<> memoryPool_(sizeof(T));
        return memoryPool_;
    }
};


/**
 * @class ThreadSafeMemoryPoolMixin
 * boost::singleton_pool 기반의 메모리 풀 mixin
 * - thread-SAFE
 */
template <typename T, typename Tag = T>
class ThreadSafeMemoryPoolMixin
{
public:
    void* operator new(std::size_t size) {
        boost::ignore_unused(size);
        assert(size == sizeof(T));
        return boost::singleton_pool<Tag, sizeof(T)>::malloc();
    }

    void operator delete(void* p) {
        boost::singleton_pool<Tag, sizeof(T)>::free(p);
    }
};

#else // SNE_USE_BOOST_MEMORY_POOL

template <typename T>
class MemoryPoolMixin
{
public:
    void* operator new(std::size_t size) {
        assert(size == sizeof(T));
        return ::new T();
    }

    void operator delete(void* p) {
        delete static_cast<uint8_t*>(p);
    }
};


/**
 * @class ThreadSafeMemoryPoolMixin
 * boost::singleton_pool 기반의 메모리 풀 mixin
 * - thread-SAFE
 */
template <typename T, typename Tag = T>
class ThreadSafeMemoryPoolMixin
{
public:
    void* operator new(std::size_t size) {
        assert(size == sizeof(T));
        return ::new uint8_t[size];
    }

    void operator delete(void* p) {
        delete[] static_cast<uint8_t*>(p);
    }
};

#endif // SNE_USE_BOOST_MEMORY_POOL

/** @} */ // addtogroup utility

}} // namespace sne { namespace core {
