#ifndef SNE_PROXYDATABASEMANAGER_H
#define SNE_PROXYDATABASEMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <sne/database/DatabaseManager.h>
#include <sne/database/DatabaseFactory.h>
#include <sne/database/Database.h>
#include <sne/base/utility/Assert.h>

namespace sne { namespace database {

class DatabaseFactory;

/**
 * @struct ProxyDatabaseManager.
 *
 * DB Proxy를 통한 Database 관리자
 */
class ProxyDatabaseManager : public DatabaseManager
{
private:
    virtual bool initialize(server::ConfigReader& configReader,
        DatabaseFactory& databaseFactory, msec_t /*pingInterval*/) {
        database_ = databaseFactory.create(DatabaseFactory::ctProxy);
        return database_->open(configReader);
    }

    virtual Database* acquire() {
        return database_.get();
    }

    virtual void release(Database* db) {
        SNE_ASSERT(db == database_.get());
    }

    virtual void enumerate(DatabaseTask& task) {
        task.run(*database_);
    }

private:
    std::unique_ptr<Database> database_;
};

}} // namespace sne { namespace database {

#endif // SNE_PROXYDATABASEMANAGER_H
