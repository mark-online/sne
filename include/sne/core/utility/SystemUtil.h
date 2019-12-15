#pragma once

#include <sne/Common.h>
#include <chrono>
#include <cassert>
#include <limits>

namespace sne { namespace core {

inline msec_t getElapsedTime(const std::chrono::steady_clock::time_point& past)
{
    const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    assert(now >= past);
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - past);
    assert(elapsed.count() <= (std::numeric_limits<msec_t>::max)());
    return static_cast<msec_t>(elapsed.count());
}

inline msec_t getTickCount()
{
#ifdef _WIN32
    return ::GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    return unsigned((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
#endif
}

}} // namespace sne { namespace core {
