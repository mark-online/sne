#ifndef SNE_PROXYDATABASE_H
#define SNE_PROXYDATABASE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <sne/database/proxy/DatabaseRequest.h>
#include <sne/database/Database.h>

namespace sne { namespace database { namespace proxy {

/**
 * @struct ProxyDatabase
 *
 * Game Database.
 */
class ProxyDatabase : public Database
{
public:
    virtual bool call(RequestId id, const ModifyRequest& request) = 0;
};

}}} // namespace sne { namespace database { namespace proxy {

#endif // SNE_PROXYDATABASE_H
