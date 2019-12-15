#pragma once

#include "ClientId.h"
#include "config/ServerSideSessionConfig.h"
#include "../common/ServerSpec.h"
#include <sne/sgp/protocol/security/PacketSeedExchangerCallback.h>
#include <sne/base/io_context/Timer.h>
#include <sne/base/session/Session.h>
#include <sne/core/utility/SystemUtil.h>

namespace sne { namespace security {
class RsaCipher;
}} // namespace sne { namespace security {

namespace sne { namespace base {
class PacketSeedExchanger;
}} // namespace sne { namespace base {

namespace sne { namespace server {

struct ServerSideSessionConfig;
class HeartBeatReceiver;
class HeartbeatSender;

/**
 * @class ServerSideSession
 *
 * Server side Session
 */
class SNE_API ServerSideSession :
    public base::Session,
    private sgp::PacketSeedExchangerCallback,
    private base::TimerCallback
{
public:
    ServerSideSession(const ServerSideSessionConfig& sessionConfig,
        const ServerSpec& serverSpec,
        std::unique_ptr<base::SessionImpl> impl);
    virtual ~ServerSideSession();

    /// 사용자가 인증(정상적인 요청)을 했을 경우 호출해야 한다.
    void authenticated();

    /// 인증 유효시간을 늘린다
    void extendAuthenticationTimeout();

    /// 로그아웃했을 때 호출해야 한다
    void unauthenticated();

    ClientId getClientId() const {
        return clientId_;
    }

protected:
    // = base::TimerCallback overriding
    virtual void timedOut(const void* act) override;

protected:
    // = base::SessionCallback overriding
    virtual bool onConnected() override;
    virtual void onDisconnected() override;
    virtual void onMessageTransfered(base::MessageType messageType) override;
    virtual bool onMessageArrived(base::MessageType messageType,
        base::MemoryBlock& mblock) override;
    virtual void onThrottling(size_t readBytes, size_t maxBytesPerSecond) override;

    // = sgp::PacketSeedExchangerCallback overriding
    virtual bool generatePublicAndPrivateKey(
        std::string& publicKey, std::string& privateKey) override;
    virtual void encryptSeed(std::string& seed,
        const std::string& otherSidePublicKey) override;
    virtual bool decryptSeed(std::string& seed) override;
    virtual void seedExchanged() override;

private:
    void heartbeatMessageReceived(const base::Message& message);

    void setupAuthenticationTimer();
    void cancelAuthenticationTimer();

    msec_t getConnectionPeriod() const {
        return core::getElapsedTime(connectedTime_);
    }

    void initializeRsaCipher();
    void initHeartbeater();

private:
    const ServerSpec serverSpec_;
    std::unique_ptr<HeartBeatReceiver> heartBeatReceiver_;
    std::unique_ptr<HeartbeatSender> heartbeater_;
    std::unique_ptr<base::Timer> authenticationTimer_;
    ClientId clientId_;

    std::unique_ptr<security::RsaCipher> rsaCipher_;
    std::unique_ptr<sgp::PacketSeedExchanger> packetSeedExchanger_;
    bool isSeedExchanged_;

    std::chrono::steady_clock::time_point connectedTime_; ///< 접속 시간
};

}} // namespace sne { namespace server {
