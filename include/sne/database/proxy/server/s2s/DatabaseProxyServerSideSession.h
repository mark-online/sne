#pragma once

#include <sne/server/session/ServerSideSession.h>
#include <sne/Common.h>

namespace sne { namespace database { namespace proxy {

/**
 * @class DatabaseProxyServerSideSession
 *
 * Database proxy Server-side client session
 */
class SNE_API DatabaseProxyServerSideSession :
    public server::ServerSideSession
{
public:
    DatabaseProxyServerSideSession(
        const server::ServerSideSessionConfig& sessionConfig,
        const server::ServerSpec& serverSpec,
        std::unique_ptr<base::SessionImpl> impl);
    virtual ~DatabaseProxyServerSideSession();

private:
    void authenticateMessageReceived(const base::Message& message);
    void delegateMessageReceived(const base::Message& message);
};

}}} // namespace sne { namespace database { namespace proxy {
