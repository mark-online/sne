#pragma once

#include <sne/sgp/protocol/PacketCoderFactory.h>

namespace sne { namespace sgp {

/**
* @class SecurePacketCoderFactory
*
* Secure PacketCoder Factory
*/
class SNE_API SecurePacketCoderFactory : public PacketCoderFactory
{
public:
    /**
     * @param cipher cipher algorithm.
     *               "none" - no crypt
     *               "tea" - TEA cipher algorithm
     *               "aes" - AES cipher algorithm
     * @param cipherKeyPeriod cipher key valid period (mili-sec. unit).
     */
    SecurePacketCoderFactory(const std::string& cipher,
        msec_t cipherKeyPeriod = 0) :
        cipher_(cipher),
        cipherKeyPeriod_(cipherKeyPeriod) {}

    virtual std::unique_ptr<PacketCoder> create() const override;

private:
    const std::string cipher_;
    const msec_t cipherKeyPeriod_;
};

}} // namespace sne { namespace sgp {
