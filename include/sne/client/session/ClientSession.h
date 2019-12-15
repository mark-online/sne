#pragma once

#include <sne/sgp/protocol/security/PacketSeedExchangerCallback.h>
#include <sne/base/session/manager/SessionDestroyer.h>
#include <sne/base/session/Session.h>
#include <sne/Common.h>

namespace sne { namespace security {
class RsaCipher;
}} // namespace sne { namespace security {

namespace sne { namespace base {
class MemoryBlockManager;
}} // namespace sne { namespace base {

namespace sne { namespace srpc {
class RpcForwarder;
class RpcReceiver;
}} // namespace sne { namespace srpc {

namespace sne { namespace sgp {
class PacketCoderFactory;
class PacketSeedExchanger;
}} // namespace sne { namespace sgp {

namespace sne { namespace client {

namespace detail {
class ClientHeartbeater;
} // namespace detail

class ClientSessionTick;
class ClientSessionCallback;

/**
 * @class ClientSession
 * SNE Client Session.
 */
class SNE_API ClientSession :
    public base::Session,
    private base::SessionDestroyer,
    private sgp::PacketSeedExchangerCallback
{
public:
    /**
     * ctor
     * @param tick ticker
     * @param packetCoderFactory PacketCoder 팩토리 인스턴스.
     * @param heartbeatInterval 허트비트 전송 간격(밀리초 단위). 0이면 전송하지 않는다.
     * @param callback Session Callback (null-able)
     */
    ClientSession(ClientSessionTick& tick,
        sgp::PacketCoderFactory& packetCoderFactory,
        msec_t heartbeatInterval = 0,
        ClientSessionCallback* callback = nullptr);

    virtual ~ClientSession();

    /**
    * 서버에 연결한다
    * @param ip 서버의 IP 주소
    * @param port 서버의 IP 포트번호
    * @param timeout 접속 제한 시간(밀리초 단위)
    * @param waitUntilValidate 내부적으로 연결이 유효화(키값 교환)될 때까지
    *        대기할 것인가?
    *        false일 경우 ClientSessionCallback::onValidated()가 호출된
    *        이후부터 요청을 전송해야 한다.
    */
    bool connectAndValidate(const std::string& ip, uint16_t port, uint16_t timeout,
        bool waitUntilValidate = true);

    void tick();

public:
    void registerRpcForwarder(srpc::RpcForwarder& forwarder);
    void registerRpcReceiver(srpc::RpcReceiver& receiver);

    void unregisterRpcForwarder(srpc::RpcForwarder& forwarder);
    void unregisterRpcReceiver(srpc::RpcReceiver& receiver);

private:
    void heartbeatMessageReceived(const base::Message& message);

private:
    // = base::Session overriding
    virtual bool onConnected() override;
    virtual void onMessageTransfered(base::MessageType messageType) override;
    virtual void onThrottling(size_t readBytes, size_t maxBytesPerSecond) override;
    virtual void onDisconnected() override;

    // = base::SessionDestroyer overriding
    virtual void release(base::Session* session) override;

    // = sgp::PacketSeedExchangerCallback overriding
    virtual bool generatePublicAndPrivateKey(
        std::string& publicKey, std::string& privateKey) override;
    virtual void encryptSeed(std::string& seed,
        const std::string& otherSidePublicKey) override;
    virtual bool decryptSeed(std::string& seed) override;
    virtual void seedExchanged() override;

private:
    ClientSessionCallback* callback_;
    ClientSessionTick& ticker_;

    std::unique_ptr<detail::ClientHeartbeater> heartbeater_;
    std::unique_ptr<sgp::PacketSeedExchanger> seedExchanger_;
    std::unique_ptr<security::RsaCipher> rsaCipher_;
    bool packetSeedExchanged_;
};

}} // namespace sne { namespace client {
