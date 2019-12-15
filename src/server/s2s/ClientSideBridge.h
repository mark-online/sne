#pragma once

#include <sne/server/session/ClientSideSession.h>
#include <sne/server/s2s/S2sCertificate.h>
#include <sne/base/session/manager/SessionDestroyer.h>

namespace sne { namespace server {

class ClientSideBridgeCallback;

namespace detail {
class ClientSideBridgeConnector;
} // namespace detail


/**
 * @class ClientSideBridge
 * 서버와 통신하기 위한 네트웍 채널
 * - Broker pattern
 */
class ClientSideBridge : public ClientSideSession
{
public:
    ClientSideBridge(ClientSideBridgeCallback& callback,
        const ClientSideSessionConfig& config,
        std::unique_ptr<base::SessionImpl> impl);
    virtual ~ClientSideBridge();

    /// 연결을 시도한다
    bool bridge(const std::string& serverName,
        const std::string& address, uint16_t port,
        const S2sCertificate& certificate);

    /// 연결 시도를 취소한다
    void cancelConnection();

private:
    // = overring
    virtual void onValidated() override {}
    virtual void onDisconnected() override;

    // = sgp::PacketSeedExchangerCallback overriding
    virtual void seedExchanged() override;

private:
    void authenticateResultMessageReceived(const base::Message& message);

private:
    std::unique_ptr<detail::ClientSideBridgeConnector> connector_;
    ClientSideBridgeCallback* callback_;
    S2sCertificate certificate_;
    bool isShuttingDown_;
};

}} // namespace sne { namespace server {
