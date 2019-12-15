#pragma once

#ifndef USE_NO_PROFILER
#  include <sne/base/utility/Logger.h>
#  include <boost/noncopyable.hpp>
#  include <chrono>
#endif // USE_NO_PROFILER

namespace sne { namespace server {

/**
 * @class Profiler
 *
 * 코드 블럭의 실행 시간을 로깅한다.
 */
#ifndef USE_NO_PROFILER

template <int limitTime = 20> // 0.02 sec
class BaseProfiler : public boost::noncopyable
{
public:
    BaseProfiler(const char* entry) :
        entry_(entry),
        startTime_(std::chrono::high_resolution_clock::now()) {}

    ~BaseProfiler() {
        const float limitTimeBySec = (limitTime / 1000.0f);
        const auto stopTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedSecs = (stopTime - startTime_);
        if (elapsedSecs.count() > limitTimeBySec) {
            SNE_LOG_INFO("Profiling(%s) stopped: %.6f secs.",
                entry_, elapsedSecs.count());
        }
    }
private:
    const char* entry_;
    std::chrono::high_resolution_clock::time_point startTime_;
};

typedef BaseProfiler<> Profiler;

#else // USE_NO_PROFILER

class Profiler
{
public:
    Profiler(const char* /*entry*/) {}
    ~Profiler() {}
};

#endif // USE_NO_PROFILER

}} // namespace sne { namespace server {
