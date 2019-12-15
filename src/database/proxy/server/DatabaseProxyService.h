#ifndef SNE_DATABASEPROXYSERVICE_H
#define SNE_DATABASEPROXYSERVICE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <sne/server/s2s/S2sCertificate.h>
#include <sne/database/proxy/DatabaseRequest.h>
#include <sne/core/utility/Singleton.h>
#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace database {
class DatabaseManager;
}} // namespace sne { namespace database {

namespace sne { namespace database { namespace proxy {

class ProxyScheduler;

/**
 * @class DatabaseProxyService
 *
 * Database 서비스
 */
class SNE_API DatabaseProxyService :
    public boost::noncopyable
{
    SNE_DECLARE_SINGLETON(DatabaseProxyService);

public:
    DatabaseProxyService();
    virtual ~DatabaseProxyService();

    /// 서비스를 초기화한다.
    bool initialize(DatabaseManager& proxyDatabaseManager);

    /// 서비스를 종료한다.
    void finalize();

    /// 서버 인증을 한다
    bool authenticate(const server::S2sCertificate& certificate);

    /// Database 대행 처리를 스케쥴링한다
    void schedule(const ModifyRequest& request);

    uint64_t getQueueCount() const;
    uint64_t getRequestedCount() const;
    uint64_t getProcessedCount() const;

private:
    virtual const server::S2sCertificate& getServerCertificate() const = 0;
    virtual size_t getProxySchedulerThreadCount() const = 0;

private:
    bool initProxyScheduler();

private:
    DatabaseManager* proxyDatabaseManager_;
    std::unique_ptr<ProxyScheduler> scheduler_;
};

}}} // namespace sne { namespace database { namespace proxy {

#define DATABASEPROXY_SERVICE \
    sne::database::proxy::DatabaseProxyService::instance()

#endif // SNE_DATABASEPROXYSERVICE_H
