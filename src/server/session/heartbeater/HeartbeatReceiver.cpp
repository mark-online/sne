#include "ServerPCH.h"
#include "HeartbeatReceiver.h"
#include <sne/base/io_context/IoContext.h>
#include <sne/base/session/Session.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace server {

HeartBeatReceiver::HeartBeatReceiver(base::Session& session,
    msec_t heartbeatTimeout, uint32_t sessionId) :
    sessionId_(sessionId),
    validHeartbeatInterval_(heartbeatTimeout),
    session_(session)
{
    heartbeatTimer_.reset(session.getIoContext().createTimer(*this).release());
}


HeartBeatReceiver::~HeartBeatReceiver()
{
}


void HeartBeatReceiver::setupTimer()
{
    if (validHeartbeatInterval_ <= 0) {
        return;
    }

    heartbeated();

    SNE_ASSERT(! heartbeatTimer_->isRunning());
    heartbeatTimer_->repeat(validHeartbeatInterval_);
    if (! heartbeatTimer_->isRunning()) {
        SNE_LOG_ERROR("HeartbeatReceiver::setupTimer(%u) FAILED",
            validHeartbeatInterval_);
    }
}


void HeartBeatReceiver::cancelTimer()
{
    heartbeatTimer_->cancel();
}


void HeartBeatReceiver::timedOut(const void* /*act*/)
{
    const msec_t elapsed = getElapsedHeartbeatTime();
    if (isZombie(elapsed)) {
        cancelTimer();
        SNE_LOG_INFO("Client(C%u) is disconnecting by server,"
            "because heartbeat timeout(zombie, %u>%u msec).",
            sessionId_, elapsed, validHeartbeatInterval_);
        session_.disconnect();
    }
}

}} // namespace sne { namespace server {
