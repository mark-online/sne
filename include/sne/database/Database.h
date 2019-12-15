#if !defined(SNE_DB_DATABASE_H)
#define SNE_DB_DATABASE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <boost/noncopyable.hpp>

namespace sne { namespace server {
class ConfigReader;
}} // namespace sne { namespace server {

namespace sne { namespace database {

/**
* @class Database
*
* Database root class
*/
class Database : public boost::noncopyable
{
public:
    virtual ~Database() {}

    virtual bool open(server::ConfigReader& configReader) = 0;

    virtual void close() = 0;

    virtual void ping() = 0;
};

}} // namespace sne { namespace database {

#endif // !defined(SNE_DB_DATABASE_H)
