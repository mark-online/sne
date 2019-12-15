#pragma once

#include "../session/ServerSideSession.h"
#include <sne/Common.h>

namespace sne { namespace server {

class ServerSideBridgeCallback;

/**
 * @class ServerSideBridge
 * 클라이언트와 통신하기 위한 네트웍 채널
 * - Broker pattern
 */
class SNE_API ServerSideBridge : public ServerSideSession
{
public:
    ServerSideBridge(const ServerSideSessionConfig& sessionConfig,
        const ServerSpec& serverSpec,
        std::unique_ptr<base::SessionImpl> impl);
    virtual ~ServerSideBridge();

    void setServerSideBridgeCallback(ServerSideBridgeCallback* callback) {
        callback_ = callback;
    }

    void onAuthenticate(bool isAuthenticated);

private:
    // = ServerSideSession overriding
    virtual bool onConnected() override;
    virtual void onDisconnected() override;

private:
    void authenticateMessageReceived(const base::Message& message);

private:
    ServerSideBridgeCallback* callback_;
};

}} // namespace sne { namespace server {
