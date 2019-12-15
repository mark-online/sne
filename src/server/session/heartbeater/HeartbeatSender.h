#pragma once

#include <sne/sgp/session/detail/Heartbeater.h>

namespace sne { namespace server {

/**
 * @class HeartbeatSender
 */
class HeartbeatSender : public sgp::Heartbeater
{
public:
    HeartbeatSender(base::Session& session, msec_t heartbeatInterval) :
        Heartbeater(session, heartbeatInterval) {}

private:
    // = TimerCallback overriding
    virtual void timedOut(const void* /*act*/) override {
        sendHeartbeat();
    }
};

}} // namespace sne { namespace server {
