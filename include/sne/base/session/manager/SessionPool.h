#pragma once

#include "../Session.h"
#include "../../memory/ObjectPool.h"
#include <algorithm>

namespace sne { namespace base {

class Session;

/** @addtogroup session
* @{
*/

/**
 * @class SessionPool
 *
 * Session Pool
 */
template <class Allocator, class Mutex>
class SessionPool : public ObjectPool<Session, Allocator, Mutex>
{
    typedef ObjectPool<Session, Allocator, Mutex> Parent;

public:
    SessionPool(size_t poolSize, Allocator& sessionAllocator,
        MemoryPoolCallback* callback = nullptr) :
        Parent(poolSize, sessionAllocator, true, callback) {}

    /// ���� ��� ���� ��� ������ ����Ѵ�.
    void cancel() {
        Parent::forEachActiveResources(std::mem_fun(&Session::disconnect));
    }
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
