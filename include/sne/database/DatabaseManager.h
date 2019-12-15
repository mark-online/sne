#ifndef SNE_DATABASEMANAGER_H
#define SNE_DATABASEMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "DatabaseTask.h"
#include <sne/core/utility/Singleton.h>

namespace sne { namespace server {
class ConfigReader;
}} // namespace sne { namespace server {

namespace sne { namespace database {

class DatabaseFactory;
class Database;


/**
 * @struct DatabaseManager
 *
 * Database 관리자
 * - Singleton pattern: 사용상의 편의를 위해 어쩔 수 없이(?) 도입함
 */
class SNE_API DatabaseManager : public boost::noncopyable
{
    SNE_DECLARE_SINGLETON(DatabaseManager);
public:
    virtual ~DatabaseManager() {}

    /// @param pingInterval idel 연결에 대한 핑 간격(msec 단위)
    virtual bool initialize(server::ConfigReader& configReader,
        DatabaseFactory& databaseFactory, msec_t pingInterval) = 0;

    /// 휴지 중인 db를 얻는다.
    virtual Database* acquire() = 0;

    /// db를 휴지 상태로 바꾼다.
    virtual void release(Database* db) = 0;

    virtual void enumerate(DatabaseTask& task) = 0;
};


/**
* @class Guard
*
* scoped database guard.
*/
template <class DB>
class Guard : public boost::noncopyable
{
public:
    Guard(DatabaseManager& dbm) :
        dbm_(dbm) {
        database_ = static_cast<DB*>(dbm_.acquire());
    }

    Guard(DatabaseManager* dbm) :
        dbm_(*dbm) {
        assert(dbm != nullptr);
        database_ = static_cast<DB*>(dbm_.acquire());
    }

    ~Guard() {
        dbm_.release(database_);
    }

    DB* operator->() const {
        return database_;
    }

    DB* get() const {
        return database_;
    }

private:
    DatabaseManager& dbm_;
    DB* database_;
};

}} // namespace sne { namespace database {

#define SNE_DATABASE_MANAGER sne::database::DatabaseManager::instance()

#endif // SNE_DATABASEMANAGER_H
