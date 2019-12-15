#include "DatabasePCH.h"
#include "DatabasePool.h"
#include <sne/database/DatabaseFactory.h>
#include <sne/database/Database.h>
#include <sne/database/DatabaseTask.h>
#include <sne/database/Connection.h>
#include <sne/server/config/ConfigReader.h>
#include <sne/base/utility/Logger.h>
#include <sne/base/utility/Assert.h>
#include <boost/checked_delete.hpp>

namespace sne { namespace database {

DatabasePool::DatabasePool() :
    idleDatabase_(0)
{
}


DatabasePool::~DatabasePool()
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (Item& item : databases_) {
        item.database_->close();
    }
}


bool DatabasePool::open(server::ConfigReader& configReader,
    DatabaseFactory& databaseFactory)
{
    const size_t connectionCount =
        configReader.getNumeric<size_t>("database", "connection-count");
    databases_.reserve(connectionCount);

    for (size_t count = 0; count < connectionCount; ++count) {
        std::unique_ptr<Database> database =
            databaseFactory.create(DatabaseFactory::ctDirect);
        if (! database->open(configReader)) {
            return false;
        }
        databases_.emplace_back(std::move(database));
    }
    idleDatabase_ = connectionCount;
    return true;
}


void DatabasePool::close()
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (Item& item : databases_) {
        item.database_->close();
    }
}


void DatabasePool::enumerate(DatabaseTask& task)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (Item& item : databases_) {
        task.run(*item.database_);
    }
}


void DatabasePool::enumerateIdleDatabases(DatabaseTask& task)
{
    std::unique_lock<std::mutex> lock(mutex_);

    for (Item& item : databases_) {
        if (item.isIdle_) {
            task.run(*item.database_);
        }
    }
}


Database* DatabasePool::acquire()
{
    std::unique_lock<std::mutex> lock(mutex_);

    while (idleDatabase_ <= 0) {
        available_.wait(lock);
    }
    --idleDatabase_;

    for (Item& item : databases_) {
        if (item.isIdle_) {
            item.isIdle_ = false;
            Database* db = item.database_.get();
            SNE_ASSERT(db != nullptr);
            return db;
        }
    }

    SNE_ASSERT(false);
    return nullptr;
}


void DatabasePool::release(Database* db)
{
    SNE_ASSERT(db != nullptr);

    std::unique_lock<std::mutex> lock(mutex_);

    for (Item& item : databases_) {
        if (item.database_.get() == db) {
            SNE_ASSERT(! item.isIdle_);
            item.isIdle_ = true;
            ++idleDatabase_;
            available_.notify_one();
            return;
        }
    }
    SNE_ASSERT(false);
}

}} // namespace sne { namespace database {
