#pragma once

#include "ClientSideBridgeCallback.h"
#include "ServerInfo.h"
#include "../session/config/ClientSideSessionConfig.h"
#include <sne/Common.h>

namespace sne { namespace base {
class Session;
}} // namespace sne { namespace base {

namespace sne { namespace server {

class ClientSideBridge;

/**
* @class ClientSideProxy
* Client-side proxy. 서버 접속에 투명성을 제공하기 위한 proxy.
* - Broker Pattern
*/
class SNE_API ClientSideProxy :
    public ClientSideBridgeCallback
{
public:
    ClientSideProxy(const ServerInfo& serverInfo,
        const ClientSideSessionConfig& config);

    virtual ~ClientSideProxy();

    void setConnectionTimeout(msec_t timeout) {
        connectionTimeout_ = timeout;
        if (connectionTimeout_ <= 0) {
            connectionTimeout_ = 2 * 1000;
        }
    }

public:
    /// 대상 Server에 접속하고 협상이 완료될 때까지 대기한다
    bool connect();

    void disconnect();

    void sendMessage(base::Message& message);

public:
    base::Session* getSession();

public:
    /// 서비스가 활성화(준비)되었는가?
    virtual bool isActivated() const {
        return isConnected_;
    }

protected:
    /// 서버에 인증되었다
    virtual void onAuthenticated() {}

    /// 서버로 부터 접속이 끊겼다
    virtual void onDisconnected() {}

protected:
    const ServerInfo& getServerInfo() const {
        return serverInfo_;
    }

protected:
    // = ClientSideBridgeCallback overriding
    virtual void disconnected() override;
    virtual void onAuthenticate(bool authenticated) override;

private:
    /// timeout(ms) 동안 협상이 완료되었는지 검사한다
    bool wait(msec_t timeout);

private:
    const ServerInfo serverInfo_;
    const ClientSideSessionConfig sessionConfig_;
    msec_t connectionTimeout_;
    bool isConnected_;
    std::unique_ptr<ClientSideBridge> bridge_;
};

}} // namespace sne { namespace server {
