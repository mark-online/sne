#pragma once

#include "../../protocol/cs/CsMessage.h"
#include <sne/base/io_context/IoContext.h>
#include <sne/base/io_context/Timer.h>
#include <sne/base/session/Session.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/utility/SystemUtil.h>

namespace sne { namespace sgp {

/**
 * @class Heartbeater
 */
class Heartbeater : public base::TimerCallback
{
public:
    Heartbeater(base::Session& session, msec_t heartbeatInterval) :
        session_(session),
        heartbeatInterval_(heartbeatInterval) {}
    virtual ~Heartbeater() {} 

    void start() {
        if (! heartbeatTimer_.get()) {
            heartbeatTimer_.reset(
                session_.getIoContext().createTimer(*this).release());
        }

        heartbeatTimer_->cancel();

        if (heartbeatInterval_ > 0) {
            heartbeatTimer_->repeat(heartbeatInterval_);

            SNE_LOG_INFO("Heartbeater::start() - %u msec.",
                heartbeatInterval_);
        }
    }

    void stop() {
        if (heartbeatTimer_.get() != nullptr) {
            heartbeatTimer_->cancel();
        }
    }

    void sent() {
        lastSentTime_ = std::chrono::steady_clock::now();
    }

protected:
    void sendHeartbeat() {
        const msec_t elapsed = core::getElapsedTime(lastSentTime_);
        if (elapsed >= heartbeatInterval_) {
            static sgp::HeartbeatMessage message;
            session_.getExtension<base::MessagingExtension>()->sendMessage(message);
        }
    }

private:
    base::Session& session_;
    const msec_t heartbeatInterval_;
    std::chrono::steady_clock::time_point lastSentTime_;
    std::unique_ptr<base::Timer> heartbeatTimer_;
};

}} // namespace sne { namespace sgp {
