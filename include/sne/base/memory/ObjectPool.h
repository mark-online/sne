#pragma once

#include "MemoryPool.h"

namespace sne { namespace base {

/** @addtogroup memory
* @{
*/

/**
 * @class ObjectPool
 *
 * Resource pool
 * - Resouce: ObjectPool에서 다룰 데이터 타입.
 * - Allocator: Resource 인스턴스를 할당/해제한다.
 * - Mutex: Mutex 타입
 */
template <typename Resource, class Allocator, class Mutex = boost::null_mutex>
class ObjectPool :
    public MemoryPool<sizeof(Resource), Resource, Allocator, Mutex>
{
public:
    ObjectPool(size_t poolSize, Allocator& allocator, bool growable = true,
        MemoryPoolCallback* callback = nullptr) :
        MemoryPool<sizeof(Resource), Resource, Allocator, Mutex>(
            poolSize, allocator, growable, callback) {}
};

/** @} */ // addtogroup memory

}} // namespace sne { namespace base {
