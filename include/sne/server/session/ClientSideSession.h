#pragma once

#include <sne/sgp/protocol/security/PacketSeedExchangerCallback.h>
#include <sne/base/session/Session.h>
#include <sne/Common.h>

namespace sne { namespace security {
class RsaCipher;
}} // namespace sne { namespace security {

namespace sne { namespace sgp {
class PacketSeedExchanger;
}} // namespace sne { namespace sgp {

namespace sne { namespace server {

struct ClientSideSessionConfig;
class HeartbeatSender;
class HeartBeatReceiver;

/**
 * @class ClientSideSession
 *
 * Client Side Session
 */
class SNE_API ClientSideSession :
    public base::Session,
    private sgp::PacketSeedExchangerCallback
{
public:
    ClientSideSession(const ClientSideSessionConfig& config,
        std::unique_ptr<base::SessionImpl> impl);
    virtual ~ClientSideSession();

protected:
    virtual void onValidated() = 0;

protected:
    // = base::SessionCallback overriding
    virtual bool onConnected() override;
    virtual void onDisconnected() override;
    virtual void onMessageTransfered(base::MessageType messageType) override;
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

private:
    std::unique_ptr<HeartbeatSender> heartbeater_;
    std::unique_ptr<HeartBeatReceiver> heartBeatReceiver_;
    std::unique_ptr<security::RsaCipher> rsaCipher_;
    std::unique_ptr<sgp::PacketSeedExchanger> packetSeedExchanger_;
};

}} // namespace sne { namespace server {
