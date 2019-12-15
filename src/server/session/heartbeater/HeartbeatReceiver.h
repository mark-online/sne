#pragma once

#include <sne/server/session/ClientId.h>
#include <sne/base/io_context/Timer.h>
#include <sne/core/utility/SystemUtil.h>
#include <chrono>

namespace sne { namespace base {
class Session;
}} // namespace sne { namespace base {

namespace sne { namespace server {

/**
 * @class HeartBeatReceiver
 */
class HeartBeatReceiver :
    private base::TimerCallback
{
public:
    HeartBeatReceiver(base::Session& session,
        msec_t heartbeatTimeout, uint32_t sessionId = 0);
    virtual ~HeartBeatReceiver();

    void setupTimer();
    void cancelTimer();

    void heartbeated() {
        lastHeartbeatedTime_ = std::chrono::steady_clock::now();
    }

private:
    // = base::TimerCallback overriding
    virtual void timedOut(const void* act) override;

private:
    bool isZombie(msec_t elapsed) const {
        return elapsed >= validHeartbeatInterval_;
    }

    msec_t getElapsedHeartbeatTime() const {
        return core::getElapsedTime(lastHeartbeatedTime_);
    }

private:
    const uint32_t sessionId_;
    const msec_t validHeartbeatInterval_;
    base::Session& session_;
    std::unique_ptr<base::Timer> heartbeatTimer_;
    std::chrono::steady_clock::time_point lastHeartbeatedTime_;
};

}} // namespace sne { namespace server {
