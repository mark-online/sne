#if !defined(SNE_DATABASEFACTORY_H)
#define SNE_DATABASEFACTORY_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <boost/noncopyable.hpp>
#include <memory>

namespace sne { namespace database {

class Database;

/**
* @class DatabaseFactory
*/
class DatabaseFactory : public boost::noncopyable
{
public:
    enum ConnectionType {
        ctDirect = 0, ///< DB 직접 연결
        ctProxy ///< DB Proxy 연결
    };
public:
    virtual ~DatabaseFactory() {}

    virtual std::unique_ptr<Database> create(ConnectionType ct) = 0;
};

}} // namespace sne { namespace database {

#endif // !defined(SNE_DATABASEFACTORY_H)
