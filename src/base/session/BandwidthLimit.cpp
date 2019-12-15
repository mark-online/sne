#include "BasePCH.h"
#include "BandwidthLimit.h"

namespace sne { namespace base {

bool BandwidthLimit::check()
{
    if (! capacity_.shouldThrottle()) {
        return true;
    }

    if (readBytes_ < capacity_.maxBytesPerSecond_) {
        return true;
    }

    if (badTime_ == 0) {
        return true;
    }

    const time_t now = time(0);
    if ((now - badTime_) >=
        static_cast<time_t>(capacity_.secondsForThrottling_)) {
        reset();
        return true;
    }

    return false;
}


void BandwidthLimit::add(size_t bytes)
{
    if (! capacity_.shouldThrottle()) {
        return;
    }

    readBytes_ += bytes;

    const time_t now = time(0);

    if (startedTime_ == 0) {
        startedTime_ = now;
    }

    if (readBytes_ < capacity_.maxBytesPerSecond_) {
        const time_t checkingint_terval = 1;
        if ((now - startedTime_) >= checkingint_terval) {
            reset();
        }
        return;
    }

    if (badTime_ == 0) {
        badTime_ = now;
    }
}


void BandwidthLimit::reset()
{
    startedTime_ = 0;
    readBytes_ = 0;
    badTime_ = 0;
}

}} // namespace sne { namespace base {
