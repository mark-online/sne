#include "DatabasePCH.h"
#include "DirectDatabaseManager.h"
#include "ProxyDatabaseManager.h"
#include <sne/database/DatabaseManagerFactory.h>
#include <sne/database/DatabaseFactory.h>
#include <sne/server/config/ConfigReader.h>
#include <sne/base/utility/Logger.h>

namespace sne { namespace database {

std::unique_ptr<DatabaseManager> DatabaseManagerFactory::create(
    server::ConfigReader& configReader, DatabaseFactory& databaseFactory)
{
    std::unique_ptr<DatabaseManager> dbManager;

    const std::string  connectionType =
        configReader.getString("database", "connection-type");
    if (connectionType == "direct") {
        dbManager = std::make_unique<DirectDatabaseManager>();
        SNE_LOG_INFO("Using DirectDatabaseManager");
    }
    else if (connectionType == "proxy") {
        dbManager = std::make_unique<ProxyDatabaseManager>();
        SNE_LOG_INFO("Using ProxyDatabaseManager");
    }

    if (dbManager.get() != nullptr) {
        msec_t pingInterval = 0;
        try {
            pingInterval = configReader.getNumeric<msec_t>("database", "ping-interval");
        }
        catch (const server::NotDefinedConfigError&) {}

        if (! dbManager->initialize(configReader, databaseFactory, pingInterval)) {
            dbManager.reset();
        }
    }

    return dbManager;
}

}} // namespace sne { namespace database {
