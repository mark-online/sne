#if !defined(SNE_DATABASEPOOL_H)
#define SNE_DATABASEPOOL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <sne/core/container/Containers.h>
#include <boost/noncopyable.hpp>
#include <mutex>

namespace sne { namespace server {
class ConfigReader;
}} // namespace sne { namespace server {

namespace sne { namespace database {

class Database;
class DatabaseFactory;
class DatabaseTask;

/**
* @class DatabasePool
*
* Direct Database connection Pooling
*/
class DatabasePool : public boost::noncopyable
{
private:
    /**
     * @struct Item
     */
    struct Item
    {
        std::unique_ptr<Database> database_;
        bool isIdle_;

        Item(std::unique_ptr<Database> database) :
            database_(std::move(database)),
            isIdle_(true) {}
    };

    typedef core::Vector<Item> Databases;

public:
    DatabasePool();
    ~DatabasePool();

    bool open(server::ConfigReader& configReader,
        DatabaseFactory& databaseFactory);
    void close();

    Database* acquire();
    void release(Database* db);

    void enumerate(DatabaseTask& task);
    void enumerateIdleDatabases(DatabaseTask& task);

private:
    Databases databases_;
    size_t idleDatabase_;
    std::mutex mutex_;
    std::condition_variable available_;
};

}} // namespace sne { namespace database {

#endif // !defined(SNE_DATABASEPOOL_H)
