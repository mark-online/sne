#pragma once

#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/sgp/protocol/cs/CsProtocol.h>
#include <limits>

namespace sne { namespace sgp {

/** @addtogroup sgp
* @{
*/

/**
 * @class CsPacketCoder
 * C/S 전용 PacketCoder
 */
class CsPacketCoder : public PacketCoder
{
    enum {
        defaultPacketSize = 2 * 1024,
        defaultPacketPoolSize = 10
    };
public:
    virtual ~CsPacketCoder() {}

    virtual void reset() override {}

    virtual void setEncryptSeed(const Seed& /*seed*/) override {}

    virtual void setDecryptSeed(const Seed& /*seed*/) override {}

    virtual void generateCipherSeed(Seed& encryptSeed,
        Seed& decryptSeed) const override {
        encryptSeed.clear();
        decryptSeed.clear();
    }

    virtual void extendCipherKeyTimeLimit() override {}

    virtual size_t getHeaderSize() const override {
        return CsPacketHeader::getHeaderSize();
    }

    virtual size_t getDefaultPacketSize() const override {
        return defaultPacketSize;
    }

    virtual size_t getMaxPacketBodySize() const override {
        return (std::numeric_limits<BodySize>::max)() -
            CsPacketHeader::getHeaderSize();
    }

    virtual size_t getDefaultPacketPoolSize() const override {
        return defaultPacketPoolSize;
    }

    virtual bool shouldExchangeCipherSeed() const override {
        return false;
    }

    virtual bool isCipherKeyExpired() const override {
        return false;
    }
};

/** @} */ // addtogroup sgp

}} // namespace sne { namespace sgp {
