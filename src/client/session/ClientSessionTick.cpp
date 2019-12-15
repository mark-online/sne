#include "ClientPCH.h"
#include <sne/client/session/ClientSessionTick.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace client {

ClientSessionTick::ClientSessionTick()
{
    const size_t singleThread = 1;
    ioContextTask_ = std::make_unique<base::IoContextTask>("tick");
    ioContextTask_->createForTicker();
    ioContext_ = &ioContextTask_->getIoContext();
}


ClientSessionTick::~ClientSessionTick()
{
    ioContextTask_->stop();
}


void ClientSessionTick::tick()
{
    boost::system::error_code error;
    ioContext_->poll(error);
    if (error) {
        SNE_LOG_ERROR("ClientSessionTick::tick() Failed!(%d,%s)",
            error.value(), error.message().c_str());
    }
}

}} // namespace sne { namespace client {
