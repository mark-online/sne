#ifndef SNE_SQLSERVERPROXYDATABASE_H
#define SNE_SQLSERVERPROXYDATABASE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../ProxyDatabase.h"
#include <sne/database/DatabaseFactory.h>
#include <sne/database/ado/AdoConnection.h>
#include <sne/base/utility/Assert.h>

namespace sne { namespace database { namespace proxy {

/**
 * @class SqlServerProxyDatabase
 *
 * MS SQL Server에 특화된 ProxyDatabase 구현 클래스.
 */
class SqlServerProxyDatabase : public ProxyDatabase
{
private:
    virtual bool open(server::ConfigReader& configReader);
    virtual void close();
    virtual void ping() {
        getAdoDatabase().ping();
    }

    virtual bool call(RequestId id, const ModifyRequest& request);

private:
    AdoDatabase& getAdoDatabase() {
        return connection_.getDatabase();
    }

private:
    AdoConnection connection_;
};


/**
* @class SqlServerProxyDatabaseFactory
*
*/
class SqlServerProxyDatabaseFactory : public DatabaseFactory
{
protected:
    virtual std::unique_ptr<Database> create(ConnectionType ct) {
        SNE_ASSERT(ct == DatabaseFactory::ctDirect);
        return std::make_unique<SqlServerProxyDatabase>();
    }
};

}}} // namespace sne { namespace database { namespace proxy {

#endif // SNE_SQLSERVERPROXYDATABASE_H
