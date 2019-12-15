#pragma once

#include <sne/Common.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace sgp {

/**
 * @class PacketSeedExchangerCallback
 * 패킷 시드 교환 콜백
 */
class PacketSeedExchangerCallback : public boost::noncopyable
{
public:
    virtual ~PacketSeedExchangerCallback() {}

    /// 공개키 및 개인키를 생성한다
    /// - 클라이언트/서버 공용
    /// @return 키값이 유효한가?
    virtual bool generatePublicAndPrivateKey(
        std::string& publicKey, std::string& privateKey) = 0;

    /// 상대방의 공개키를 이용하여 시드값을 암호화한다
    /// - 서버 전용
    virtual void encryptSeed(std::string& seed,
        const std::string& otherSidePublicKey) = 0;

    /// 암호화된 시드값을 복호화한다
    /// - 클라이언트 전용
    /// @return 성공 여부
    virtual bool decryptSeed(std::string& seed) = 0;

    /// 시드 값이 교환되었다.
    virtual void seedExchanged() = 0;
};

}} // namespace sne { namespace sgp {
