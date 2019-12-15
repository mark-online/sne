#include "DatabasePCH.h"
#include "RequestMethod.h"
#include "../ProxyDatabase.h"
#include <sne/database/DatabaseManager.h>

namespace sne { namespace database { namespace proxy {

bool ModifyRequestMethod::call()
{
    Guard<ProxyDatabase> db(getDatabaseManager());
    return db->call(getRequestId(), request_);
}

}}} // namespace sne { namespace database { namespace proxy {
