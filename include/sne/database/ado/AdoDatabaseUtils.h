#pragma once

#ifdef _WIN32

#include "AdoDatabase.h"
#include <sne/base/utility/Logger.h>

namespace sne { namespace database {

inline void logDatabaseError(AdoDatabase& database, const char* what)
{
    assert(what != nullptr);
    SNE_LOG_ERROR("ADO Error(%s,%s).",
        what, database.getLastError());
}


inline void logDatabaseError(AdoDatabase& database, const AdoException& e)
{
    logDatabaseError(database, e.what());
}

}} // namespace sne { namespace database {

#endif // #ifdef _WIN32
