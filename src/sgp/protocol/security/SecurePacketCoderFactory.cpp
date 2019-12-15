#include "SgpPCH.h"
#include "SecurePacketCoder.h"
#include <sne/sgp/protocol/security/SecurePacketCoderFactory.h>
#include <sne/security/cipher/BlockCipherFactory.h>

namespace sne { namespace sgp {

std::unique_ptr<PacketCoder> SecurePacketCoderFactory::create() const
{
    std::unique_ptr<PacketCoder> packetCoder(PacketCoderFactory::create());
    if (!security::BlockCipherFactory::isValidCipher(cipher_)) {
        return packetCoder;
    }

    const bool useDynamicKey = true;
    return std::make_unique<SecurePacketCoder>(
            std::move(packetCoder), cipher_, cipherKeyPeriod_, useDynamicKey);
}

}} // namespace sne { namespace sgp {
