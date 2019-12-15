#pragma once

#include <sne/base/protocol/Message.h>
#include <sne/core/memory/MemoryPoolMixin.h>

namespace sne { namespace sgp {

/** @addtogroup sgp
* @{
*/

/// C/S 전용 메세지 타입
enum CsMessageType
{
    csmtUnknown = base::mtUnknown,

    // = system messages
    csmtSystemFirst = 0,
    csmtSystem_heartbeat = csmtSystemFirst,
    csmtSystem_exchangeSeed,
    csmtSystem_exchangePublicKey,
    csmtSystem_confirmSeed,
    csmtSystemLast,

    // = user defined messages
    csmtUserDefinedBegin = 21,
    csmtRpc = csmtUserDefinedBegin,
    csmtUserDefinedLast,
};


inline bool isSystemMessage(base::MessageType mt)
{
    assert(base::isValidMessage(mt));
    const auto csmt = static_cast<CsMessageType>(mt);
    return (csmtSystemFirst <= csmt) && (csmt < csmtSystemLast);
}


inline bool isRpcMessage(base::MessageType mt)
{
    assert(base::isValidMessage(mt));
    return base::toMessageType(csmtRpc) == mt;
}


/**
 * @struct HeartbeatMessage
 */
struct HeartbeatMessage :
    base::AbstractMessage<csmtSystem_heartbeat>,
    public core::ThreadSafeMemoryPoolMixin<HeartbeatMessage>
{
private:
    virtual void serialize(core::Stream& /*stream*/) override {}
};


/**
 * @struct ExchangeSeedMessage
 * 패킷 암호화 seed값 변경을 요청한다(서버 -> 클라이언트)
 */
struct ExchangeSeedMessage :
    base::AbstractMessage<csmtSystem_exchangeSeed>,
    public core::ThreadSafeMemoryPoolMixin<ExchangeSeedMessage>
{
    std::string publicKey_; ///< 서버의 공개키
    std::string seedForEncrypt_; ///< 서버의 암호화 시드
    std::string seedForDecrypt_; ///< 서버의 복호화 시드

    bool isEmpty() const {
        return publicKey_.empty() && seedForEncrypt_.empty() &&
            seedForDecrypt_.empty();
    }

private:
    virtual void serialize(core::Stream& stream) override {
        stream & publicKey_ & seedForEncrypt_ & seedForDecrypt_;
    }
};


/**
 * @struct ExchangePublicKeyMessage
 * 공개키 교환을 요청한다(클라이언트 -> 서버)
 */
struct ExchangePublicKeyMessage :
    base::AbstractMessage<csmtSystem_exchangePublicKey>,
    public core::ThreadSafeMemoryPoolMixin<ExchangePublicKeyMessage>
{
    std::string publicKey_; ///< 클라이언트의 공개키

private:
    virtual void serialize(core::Stream& stream) override {
        stream & publicKey_;
    }
};


/**
 * @struct ConfirmSeedMessage
 * seed 변경에 대한 응답을 한다(클라이언트 -> 서버)
 */
struct ConfirmSeedMessage :
    base::AbstractMessage<csmtSystem_confirmSeed>,
    public core::ThreadSafeMemoryPoolMixin<ConfirmSeedMessage>
{
    enum { type = csmtSystem_confirmSeed };

private:
    virtual void serialize(core::Stream& /*stream*/) override {}
};

/** @} */ // addtogroup sgp

}} // namespace sne { namespace sgp {
