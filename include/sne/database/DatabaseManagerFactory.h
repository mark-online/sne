#ifndef SNE_DATABASEMANAGERFACTORY_H
#define SNE_DATABASEMANAGERFACTORY_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <sne/Common.h>
#include <boost/noncopyable.hpp>
#include <memory>

namespace sne { namespace server {
class ConfigReader;
}} // namespace sne { namespace server {

namespace sne { namespace database {

class DatabaseManager;
class DatabaseFactory;

/**
 * @struct DatabaseManagerFactory
 */
struct SNE_API DatabaseManagerFactory : public boost::noncopyable
{
    static std::unique_ptr<DatabaseManager> create(
        server::ConfigReader& configReader,
        DatabaseFactory& databaseFactory);
};

}} // namespace sne { namespace database {

#endif // SNE_DATABASEMANAGERFACTORY_H
