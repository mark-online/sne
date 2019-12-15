#include "SecurityPCH.h"
#include <sne/security/hash/AuthlogicSha512Hash.h>
#include <cryptopp/emsa2.h>

using CryptoPP::byte;

namespace sne { namespace security {

namespace {

std::string hexencode(const byte* buffer, size_t length)
{
    static const char hex[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    std::string output;
    output.resize(length * 2);
    for (size_t i = 0; i < length; ++i) {
        const uint8_t byte = buffer[i];
        output[i + i]     = hex[byte >> 4];
        output[i + i + 1] = hex[byte & 0x0f];
    }
    return output;
}


std::string hexdigest(const std::string& digest)
{
    byte output[CryptoPP::SHA512::DIGESTSIZE];
    CryptoPP::SHA512 hash;
    hash.CalculateDigest(output,
        reinterpret_cast<const byte*>(digest.c_str()), digest.length());

    return hexencode(output, CryptoPP::SHA512::DIGESTSIZE);
}

} // namespace {

// = AuthlogicSha512Hash

std::string AuthlogicSha512Hash::digest(const std::string& password, const std::string passwordSalt,
    size_t stretches)
{
    std::string digest = password + passwordSalt;
    for (size_t i = 0; i < stretches; ++i) {
        digest = hexdigest(digest);
    }
    return digest;
}

}} // namespace sne { namespace security {
