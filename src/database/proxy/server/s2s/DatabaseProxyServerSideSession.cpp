#include "DatabasePCH.h"
#include <sne/database/proxy/server/s2s/DatabaseProxyServerSideSession.h>
#include "../DatabaseProxyService.h"
#include <sne/server/utility/Profiler.h>
#include <sne/base/session/extension/MessagingExtension.h>
#include <sne/base/utility/Logger.h>
#include <sne/database/proxy/DatabaseProxyMessage.h>

namespace sne { namespace database { namespace proxy {

DatabaseProxyServerSideSession::DatabaseProxyServerSideSession(
    const server::ServerSideSessionConfig& sessionConfig,
    const server::ServerSpec& serverSpec,
    std::unique_ptr<base::SessionImpl> impl) :
    server::ServerSideSession(sessionConfig, serverSpec, std::move(impl))
{
    getExtension<base::MessagingExtension>()->
        registerMessageCallback<server::S2sAuthenticateMessage>(
            &DatabaseProxyServerSideSession::authenticateMessageReceived, this);
    getExtension<base::MessagingExtension>()->
        registerMessageCallback<DbpDelegateMessage>(
            &DatabaseProxyServerSideSession::delegateMessageReceived, this);
}


DatabaseProxyServerSideSession::~DatabaseProxyServerSideSession()
{
}


void DatabaseProxyServerSideSession::authenticateMessageReceived(
    const base::Message& message)
{
    const server::S2sAuthenticateMessage& theMessage =
        static_cast<const server::S2sAuthenticateMessage&>(message);

    const bool isAuthenticated =
        DATABASEPROXY_SERVICE->authenticate(theMessage.certificate_);
    if (! isAuthenticated) {
        disconnectGracefully();
        return;
    }

    server::S2sAuthenticateResultMessage reply(isAuthenticated);
    getExtension<base::MessagingExtension>()->sendMessage(reply);

    authenticated();

    const base::InetAddress remoteAddress = getImpl().getRemoteAddress();
    SNE_LOG_INFO("Client(C%u,%s:%u) connected.",
        getClientId(),
        remoteAddress.address_.c_str(), remoteAddress.port_);
}


void DatabaseProxyServerSideSession::delegateMessageReceived(
    const base::Message& message)
{
    const DbpDelegateMessage& theMessage =
        static_cast<const DbpDelegateMessage&>(message);

    DATABASEPROXY_SERVICE->schedule(theMessage.request_);
}

}}} // namespace sne { namespace database { namespace proxy {
