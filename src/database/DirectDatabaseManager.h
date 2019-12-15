#ifndef SNE_DIRECTDATABASEMANAGER_H
#define SNE_DIRECTDATABASEMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "DatabasePool.h"
#include <sne/database/DatabaseManager.h>
#include <sne/database/Database.h>
#include <sne/base/concurrent/TaskScheduler.h>
#include <sne/base/concurrent/Future.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace database {

class DatabasePool;

/**
 * @struct DirectDatabaseManager
 *
 * DB와 직접 연결하는 Database 관리자
 */
class DirectDatabaseManager :
    public DatabaseManager,
    private sne::base::Runnable
{
    /**
     * @class PingTask
     */
    class PingTask : public sne::database::DatabaseTask
    {
    public:
        virtual void run(sne::database::Database& database) {
            database.ping();
        }
    };

public:
    virtual ~DirectDatabaseManager() {
        sne::base::Future::Ref pingTask = pingTask_.lock();
        if (pingTask.get() != nullptr) {
            pingTask->cancel();
        }
    }

private:
    virtual bool initialize(server::ConfigReader& configReader,
        DatabaseFactory& databaseFactory, msec_t pingInterval) {
        if (! databasePool_.open(configReader, databaseFactory)){
            return false;
        }
        if (! TASK_SCHEDULER) {
            SNE_LOG_WARNING("DirectDatabaseManager - TaskScheduler is NULL");
            return true;
        }
        if (pingInterval > 0) {
            pingTask_ = TASK_SCHEDULER->schedule(*this, pingInterval, pingInterval);
        }
        return true;
    }

    virtual Database* acquire() {
        return databasePool_.acquire();
    }

    virtual void release(Database* db) {
        databasePool_.release(db);
    }

    virtual void enumerate(DatabaseTask& task) {
        databasePool_.enumerate(task);
    }

private:
    // = sne::base::Runnable overriding
    virtual void run() {
        PingTask task;
        databasePool_.enumerateIdleDatabases(task);
    }

private:
    DatabasePool databasePool_;
    sne::base::Future::WeakRef pingTask_;
};

}} // namespace sne { namespace database {

#endif // SNE_DIRECTDATABASEMANAGER_H
