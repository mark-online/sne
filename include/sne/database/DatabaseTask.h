#ifndef SNE_DATABASETASK_H
#define SNE_DATABASETASK_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <sne/Common.h>

namespace sne { namespace database {

class Database;


/**
 * @class DatabaseTask
 */
class DatabaseTask
{
public:
    virtual ~DatabaseTask() {}

    virtual void run(Database& database) = 0;
};

}} // namespace sne { namespace database {

#endif // SNE_DATABASETASK_H
