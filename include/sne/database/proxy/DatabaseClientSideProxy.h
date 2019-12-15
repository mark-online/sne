#pragma once

#include <sne/server/s2s/ClientSideProxy.h>
#include <sne/Common.h>

namespace sne { namespace database { namespace proxy {

struct ModifyRequest;

/**
 * @class DatabaseClientSideProxy
 *
 * DatabaseClientSideProxy
 */
class SNE_API DatabaseClientSideProxy : public server::ClientSideProxy
{
public:
    DatabaseClientSideProxy(const server::ServerInfo& serverInfo,
        const server::ClientSideSessionConfig& config) :
        server::ClientSideProxy(serverInfo, config) {}

    void call(const ModifyRequest& request);
};

}}} // namespace sne { namespace database { namespace proxy {
