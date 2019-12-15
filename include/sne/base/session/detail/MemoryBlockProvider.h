#pragma once

#include <boost/noncopyable.hpp>

namespace sne { namespace base {

class MemoryBlock;


/** @addtogroup session
* @{
*/

/**
 * @class MemoryBlockProvider
 *
 * MemoryBlockManager를 이용하기 위한 mixin class.
 */
class MemoryBlockProvider : public boost::noncopyable
{
public:
    virtual ~MemoryBlockProvider() {}

    /**
     * 메시지 전송용 블럭을 얻는다.
     * - 획득한 블럭은 release()를 통해 반환해야 한다.
     */
    virtual MemoryBlock& acquireSendBlock() = 0;

    /**
     * 메시지 수신용 블럭을 얻는다.
     * - 획득한 블럭은 release()를 통해 반환해야 한다.
     */
    virtual MemoryBlock& acquireRecvBlock() = 0;
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
