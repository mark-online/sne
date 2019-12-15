#pragma once

#include <sne/base/protocol/Message.h>
#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace base {
class Session;
}} // namespace sne { namespace base {

namespace sne { namespace sgp {

class PacketSeedExchangerCallback;
class PacketCoder;

/**
 * @class PacketSeedExchanger
 * 클라이언트/서버 간에 패킷 암호화 시드 값을 교환한다
 */
class SNE_API PacketSeedExchanger : public boost::noncopyable
{
public:
    PacketSeedExchanger() :
        callback_(nullptr),
        packetCoder_(nullptr),
        session_(nullptr) {}

    virtual ~PacketSeedExchanger() {}

    /// @internal
    void initialize(PacketSeedExchangerCallback& callback,
        PacketCoder& packetCoder, base::Session& session) {
        callback_ = &callback;
        packetCoder_ = &packetCoder;
        session_ = &session;

        registerMessageCallbacks(*session_);
    }

public:
    virtual void registerMessageCallbacks(base::Session& session) = 0;

    /// 시드값 교환을 위해 공개키 알고리즘을 사용할 경우 공개키를 교환한다
    virtual void exchangePublicKey() = 0;

    /// 서버에서 주기적으로 시드값을 교환한다
    virtual void exchangeNextSeed() = 0;

protected:
    PacketSeedExchangerCallback& getCallback() {
        assert(callback_ != nullptr);
        return *callback_;
    }

    PacketCoder& getPacketCoder() {
        assert(packetCoder_ != nullptr);
        return *packetCoder_;
    }

    base::Session& getSession() {
        assert(session_ != nullptr);
        return *session_;
    }

private:
    PacketSeedExchangerCallback* callback_;
    PacketCoder* packetCoder_;
    base::Session* session_;
};

}} // namespace sne { namespace sgp {
