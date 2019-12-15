#include "DatabasePCH.h"
#include "DatabaseProxyService.h"
#include "scheduler/ProxyScheduler.h"
#include <sne/database/DatabaseManager.h>
#include <sne/server/utility/Profiler.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace database { namespace proxy {

SNE_DEFINE_SINGLETON(DatabaseProxyService);

DatabaseProxyService::DatabaseProxyService()
{
    DatabaseProxyService::instance(this);
}


DatabaseProxyService::~DatabaseProxyService()
{
    DatabaseProxyService::instance(0);
}


bool DatabaseProxyService::initialize(DatabaseManager& proxyDatabaseManager)
{
    proxyDatabaseManager_ = &proxyDatabaseManager;

    if (! initProxyScheduler()) {
        SNE_LOG_ERROR("DatabaseProxyService::initProxyScheduler() FAILED");
        return false;
    }

    return true;
}


void DatabaseProxyService::finalize()
{
    if (scheduler_.get() != nullptr) {
        scheduler_->stop();
    }
}


bool DatabaseProxyService::authenticate(const server::S2sCertificate& certificate)
{
    return certificate == getServerCertificate();
}


void DatabaseProxyService::schedule(const ModifyRequest& request)
{
    scheduler_->schedule(request);
}


bool DatabaseProxyService::initProxyScheduler()
{
    scheduler_ = std::make_unique<ProxyScheduler>(*proxyDatabaseManager_);
    return scheduler_->start(getProxySchedulerThreadCount());
}


uint64_t DatabaseProxyService::getQueueCount() const
{
    return scheduler_->getQueueCount();
}


uint64_t DatabaseProxyService::getRequestedCount() const
{
    return scheduler_->getRequestedCount();
}


uint64_t DatabaseProxyService::getProcessedCount() const
{
    return scheduler_->getProcessedCount();
}

}}} // namespace sne { namespace database { namespace proxy {
