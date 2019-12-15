#pragma once

#include "ServerSideBridgeCallback.h"
#include "ServerId.h"
#include <sne/Common.h>

namespace sne { namespace base {
class Session;
}} // namespace sne { namespace base {

namespace sne { namespace server {

class ServerSideBridge;

/**
 * @class ServerSideProxy
 * Server-side proxy. 클라이언트 접속에 투명성을 제공하기 위한 proxy.
 * - Broker Pattern
 */
class SNE_API ServerSideProxy :
    private ServerSideBridgeCallback
{
public:
    /// @param bridge 메모리 소유권을 넘기지 말아야 한다
    ServerSideProxy(ServerSideBridge* bridge);
    virtual ~ServerSideProxy();

    ServerId getServerId() const {
        return serverId_;
    }

public:
    /// 테스트 용(직접 호출하지 말 것)
    void setServerId(ServerId serverId) {
        serverId_ = serverId;
    }

public:
    base::Session& getSession();

    bool hasSession() const {
        return bridge_ != nullptr;
    }

private:
    /// 인증 처리를 한다
    virtual bool verifyAuthentication(
        const S2sCertificate& certificate) const = 0;

    /// 클라이언트가 성공적으로 연결되었다
    virtual void bridged() = 0;

    /// 인증된 클라이언트가 연결 해제하였다
    virtual void unbridged() = 0;

private:
    // = ServerSideBridgeCallback overriding
    virtual void connected() override;
    virtual void disconnected() override;
    virtual void destroyed() override;
    virtual void authenticate(const S2sCertificate& certificate) override;

private:
    ServerId serverId_;
    ServerSideBridge* bridge_;
};

}} // namespace sne { namespace server {
