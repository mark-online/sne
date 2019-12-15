#pragma once

#ifdef _WIN32

#include "../Connection.h"
#include "AdoDatabase.h"

namespace sne { namespace database {

/**
* @class AdoConnection
*
* ADO Connection
*/
class /*SNE_API*/ AdoConnection : public Connection
{
public:
    virtual bool open(const std::string & connectionString,
        uint8_t connectionTimeout) {
        return database_.open(connectionString, connectionTimeout);
    }

    virtual void close() {
        database_.close();
    }

    virtual bool isOpened() {
        return database_.isOpened();
    }

    virtual std::string  getLastError() const {
        return database_.getLastError();
    }

    virtual unsigned int getLastResult() const {
        return static_cast<unsigned int>(database_.getLastResult());
    }

public:
    AdoDatabase& getDatabase() {
        return database_;
    }

private:
    AdoDatabase database_;
};

}} // namespace sne { namespace database {

#endif // #ifdef _WIN32
