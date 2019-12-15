#pragma once

#include <sne/base/session/detail/SessionCapacity.h>
#include <boost/noncopyable.hpp>
#include <ctime>

namespace sne { namespace base {

/** @addtogroup session
* @{
*/

/**
 * @class BandwidthLimit
 */
class BandwidthLimit : public boost::noncopyable
{
public:
    BandwidthLimit(const SessionCapacity& capacity) :
        capacity_(capacity) {
        reset();
    }

    bool check();
    void add(size_t bytes);

    void reset();

    size_t getLeftBytes() const {
        if (badTime_ != 0) {
            return 0;
        }
        return capacity_.maxBytesPerSecond_ - readBytes_;
    }

    size_t getReadBytes() const {
        return readBytes_;
    }

    size_t getMaxBytesPerSecond() const {
        return capacity_.maxBytesPerSecond_;
    }

    uint32_t getSecondsForThrottling() const {
        return capacity_.secondsForThrottling_;
    }

private:
    const SessionCapacity capacity_;

    size_t readBytes_; ///< read bytes
    time_t startedTime_; ///< time we started reading bytes
    time_t badTime_; ///< time we went bad, or 0 if we're not
};

/** @} */ // addtogroup session

}} // namespace sne { namespace base {
