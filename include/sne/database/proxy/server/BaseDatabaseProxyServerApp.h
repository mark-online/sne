#pragma once

#include <sne/server/app/SessionServerApp.h>
#include <sne/Common.h>

namespace sne { namespace database {
class DatabaseManager;
}} // namespace sne { namespace database {

namespace sne { namespace database { namespace proxy {

class DatabaseProxyService;

/**
 * @class BaseDatabaseProxyServer
 *
 * DatabaseClientSideProxy Server
 */
class SNE_API BaseDatabaseProxyServerApp :
    public server::SessionServerApp
{
public:
    BaseDatabaseProxyServerApp();
    virtual ~BaseDatabaseProxyServerApp();

protected:
    // = server::SessionServerApp overriding
    virtual bool onInit();
    virtual bool onStart();
    virtual void onFinish();

private:
    // = server::rcon::QuerySource overriding
    virtual std::string query(const std::string& name,
        const std::string& detail) const override;

private:
    bool initDatabaseProxyService();
    void finishDatabaseProxyService();

    bool initDatabaseManager();
    bool initMonitorService();

private:
    std::unique_ptr<DatabaseManager> proxyDatabaseManager_;
    std::unique_ptr<DatabaseProxyService> databaseProxyService_;
};

}}} // namespace sne { namespace database { namespace proxy {
