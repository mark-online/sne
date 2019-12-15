#pragma once

#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/Common.h>
#include <chrono>

namespace sne { namespace security {
class BlockCipher;
}} // namespace sne { namespace security {

namespace sne { namespace sgp {

class CipherKeyGenerator;

/**
 * @class SecurePacketCoder
 * 패킷 암호화를 위한 코더.
 * <secure packet-header> :=
 *      <packet-header> + <finger-print> + <checksum> + <body>
 * <check-sum> = 4 bytes
 * <finger-print> = 2 bytes
 * - Decorator Pattern (Concrete Decorator)
 */
class SNE_API SecurePacketCoder : public PacketCoder
{
public:
    /**
     * @param component Decorator Component. 메모리 소유권을 갖는다.
     * @param cipher "aes" or "tea"
     * @param cipherKeyPeriod 암호화 키 만료 시간(밀리초 단위)
     * @param useDynamicKey 동적으로 변하는 암호화 키를 사용할 것인가?
     */
    SecurePacketCoder(std::unique_ptr<PacketCoder> component,
        const std::string& cipher, msec_t cipherKeyPeriod,
        bool useDynamicKey);
    virtual ~SecurePacketCoder();

private:
    virtual void reset() override;

    virtual void generateCipherSeed(Seed& encryptSeed,
        Seed& decryptSeed) const override;

    //virtual Seed generateCipherSeed(uint32_t base) const override;

    virtual void setEncryptSeed(const Seed& seed) override;
    virtual void setDecryptSeed(const Seed& seed) override;

    virtual bool readHeader(AbstractPacketHeader& header,
        const base::MemoryBlock& mblock) const override;

    virtual bool encode(base::MemoryBlock& mblock,
        AbstractPacketHeader& header) override;
    virtual bool decode(base::MemoryBlock& mblock) override;

    virtual void extendCipherKeyTimeLimit() override;

    virtual size_t getHeaderSize() const override {
        return securePacketHeaderSize_;
    }

    virtual size_t getDefaultPacketSize() const override {
        return component_->getDefaultPacketSize();
    }

    virtual size_t getMaxPacketBodySize() const override;

    virtual size_t getDefaultPacketPoolSize() const override {
        return component_->getDefaultPacketPoolSize();
    }

    virtual bool shouldExchangeCipherSeed() const override {
        return cipherKeyPeriod_ > 0;
    }

    virtual bool isCipherKeyExpired() const override;

    virtual bool hasPacketCipher() const override {
        return true;
    }

private:
    /// buffer를 cipher의 블럭 단위에 맞춰 패딩을 추가한다.
    void align(base::MemoryBlock& buffer);

private:
    std::unique_ptr<PacketCoder> component_;
    std::unique_ptr<CipherKeyGenerator> cipherKeyGenerator_;
    std::unique_ptr<security::BlockCipher> cipher_;
    const size_t cipherBlockSize_;
    const msec_t cipherKeyPeriod_;
    const size_t rawPacketHeaderSize_;
    const size_t securePacketHeaderSize_;

    std::chrono::steady_clock::time_point cipherKeyTimeLimit_;
};

}} // namespace sne { namespace sgp {
