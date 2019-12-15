#include "SecurityPCH.h"
#include <sne/security/cipher/BlockCipherFactory.h>
#include "BlockCipherImpl.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4100 4244 4702)
#endif
#include <cryptopp/aes.h>
#include <cryptopp/tea.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace sne { namespace security {

bool BlockCipherFactory::isValidCipher(const std::string& cipher)
{
    return (cipher == "tea") || (cipher == "aes");
}


std::unique_ptr<BlockCipher> BlockCipherFactory::create(
    const std::string& cipher)
{
    assert(isValidCipher(cipher));

    if (cipher == "tea") {
        return std::make_unique<BlockCipherImpl<
                BlockCipherAlgorithm<CryptoPP::TEAEncryption>,
                BlockCipherAlgorithm<CryptoPP::TEADecryption>>>();
    }

    if (cipher == "aes") {
        return std::make_unique<BlockCipherImpl<
                BlockCipherAlgorithm<CryptoPP::AESEncryption>,
                BlockCipherAlgorithm<CryptoPP::AESDecryption>>>();
    }

    // TODO: throw exception?
    assert(false && "Invalid block cipher algorithm");

    return std::unique_ptr<BlockCipher>();
};

}} // namespace sne { namespace security {
