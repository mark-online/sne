#include "DatabasePCH.h"
#include <sne/database/proxy/server/BaseDatabaseProxyServerApp.h>
#include "DatabaseProxyService.h"
#include "mssql/SqlServerProxyDatabase.h"
#include <sne/database/DatabaseManagerFactory.h>
#include <sne/database/DatabaseManager.h>
#include <sne/server/common/ServerSpec.h>
#include <sne/base/utility/Assert.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace database { namespace proxy {

namespace
{

/**
 * @class SneDatabaseProxyService
 */
class SneDatabaseProxyService : public DatabaseProxyService
{
public:
    SneDatabaseProxyService() {}

    bool initialize(const server::ServerSpec& c2sSpec,
        DatabaseManager& proxyDatabaseManager) {
        c2sSpec_ = c2sSpec;

        return DatabaseProxyService::initialize(proxyDatabaseManager);
    }

private:
    virtual const server::S2sCertificate& getServerCertificate() const {
        return c2sSpec_.certificate_;
    }

    virtual size_t getProxySchedulerThreadCount() const {
        return c2sSpec_.workerThreadCount_;
    }

private:
    server::ServerSpec c2sSpec_;
};

} // namespace

// = BaseDatabaseProxyServer

#pragma warning (disable: 4355)

BaseDatabaseProxyServerApp::BaseDatabaseProxyServerApp() : 
    databaseProxyService_(std::make_unique<SneDatabaseProxyService>())
{
}


BaseDatabaseProxyServerApp::~BaseDatabaseProxyServerApp()
{
}


bool BaseDatabaseProxyServerApp::initDatabaseProxyService()
{
    SNE_ASSERT(getServerSpec().isValid());
    SNE_ASSERT(proxyDatabaseManager_.get() != nullptr);

    SneDatabaseProxyService& theDatabaseProxyService =
        static_cast<SneDatabaseProxyService&>(*databaseProxyService_);

    if (! theDatabaseProxyService.initialize(
        getServerSpec(), *proxyDatabaseManager_)) {
        return false;
    }

    return true;
}


void BaseDatabaseProxyServerApp::finishDatabaseProxyService()
{
    databaseProxyService_->finalize();
}


bool BaseDatabaseProxyServerApp::initDatabaseManager()
{
    SqlServerProxyDatabaseFactory databaseFactory;

    proxyDatabaseManager_ = DatabaseManagerFactory::create(
        getServiceConfig().getConfigReader(), databaseFactory);
    if (! proxyDatabaseManager_.get()) {
        return false;
    }

    //DatabaseManager::instance(proxyDatabaseManager_.get());
    return true;
}


bool BaseDatabaseProxyServerApp::initMonitorService()
{
    registerRconQuerySource("queue.count", *this);
    registerRconQuerySource("request.count", *this);
    registerRconQuerySource("process.count", *this);
    return true;
}

// = server::SessionServerApp overriding

bool BaseDatabaseProxyServerApp::onInit()
{
    if (! initDatabaseManager()) {
        SNE_LOG_ERROR("BaseDatabaseProxyServer::initDatabaseManager() FAILED!");
        return false;
    }

    if (! server::SessionServerApp::onInit()) {
        return false;
    }

    if (! initDatabaseProxyService()) {
        SNE_LOG_ERROR("BaseDatabaseProxyServer::initDatabaseProxyService() FAILED!");
        return false;
    }

    return true;
}


bool BaseDatabaseProxyServerApp::onStart()
{
    if (! server::SessionServerApp::onStart()) {
        return false;
    }

    return true;
}


void BaseDatabaseProxyServerApp::onFinish()
{
    server::SessionServerApp::onFinish();

    finishDatabaseProxyService();
}

// = server::rcon::QuerySource overriding

std::string BaseDatabaseProxyServerApp::query(const std::string& name,
    const std::string& detail) const
{
    if ("queue.count" == name) {
        return std::to_string(databaseProxyService_->getQueueCount());
    }

    if ("request.count" == name) {
        return std::to_string(databaseProxyService_->getRequestedCount());
    }

    if ("process.count" == name) {
        return std::to_string(databaseProxyService_->getProcessedCount());
    }

    return server::SessionServerApp::query(name, detail);
}

}}} // namespace sne { namespace database { namespace proxy {
