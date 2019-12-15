#include "ServerPCH.h"
#include <sne/server/s2s/ServerSideBridge.h>
#include <sne/server/s2s/ServerSideBridgeCallback.h>
#include <sne/server/s2s/S2sMessage.h>
#include <sne/base/session/extension/MessagingExtension.h>

namespace sne { namespace server {

ServerSideBridge::ServerSideBridge(
    const ServerSideSessionConfig& sessionConfig,
    const ServerSpec& serverSpec,
    std::unique_ptr<base::SessionImpl> impl) :
    ServerSideSession(sessionConfig, serverSpec, std::move(impl)),
    callback_(nullptr)
{
    getExtension<base::MessagingExtension>()->
        registerMessageCallback<S2sAuthenticateMessage>(
            &ServerSideBridge::authenticateMessageReceived, this);
}


ServerSideBridge::~ServerSideBridge()
{
    if (callback_ != nullptr) {
        callback_->destroyed();
    }
}


void ServerSideBridge::onAuthenticate(bool isAuthenticated)
{
    S2sAuthenticateResultMessage message(isAuthenticated);
    (void)getExtension<base::MessagingExtension>()->sendMessage(message);

    if (isAuthenticated) {
        authenticated();
    }
}


void ServerSideBridge::authenticateMessageReceived(const base::Message& message)
{
    const S2sAuthenticateMessage& theMessage =
        static_cast<const S2sAuthenticateMessage&>(message);

    callback_->authenticate(theMessage.certificate_);
}


// = ServerSideSession overriding

bool ServerSideBridge::onConnected()
{
    if (callback_ != nullptr) {
        callback_->connected();
    }

    if (! ServerSideSession::onConnected()) {
        return false;
    }

    return true;
}


void ServerSideBridge::onDisconnected()
{
    ServerSideSession::onDisconnected();

    if (callback_ != nullptr) {
        callback_->disconnected();
    }
}

}} // namespace sne { namespace server {
