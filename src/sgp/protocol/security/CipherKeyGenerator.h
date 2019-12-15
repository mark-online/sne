#pragma once

#include <sne/security/cipher/BlockCipher.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace sgp {

/**
* @class CipherKeyGenerator
*
* 암호화 키 생성기
*/
class CipherKeyGenerator : public boost::noncopyable
{
public:
    /**
     * @param useDynamicKey 동적으로 변하는 암호화 키 생성기?
     */
    static std::unique_ptr<CipherKeyGenerator> create(bool useDynamicKey);

public:
    virtual ~CipherKeyGenerator() {}

    virtual void setEncryptCipherKey(security::Key newKey) = 0;
    virtual void setDecryptCipherKey(security::Key newKey) = 0;

    virtual security::Key getEncryptCipherKey() const = 0;
    virtual security::Key getDecryptCipherKey() const = 0;

    virtual void reset() = 0;
};

}} // namespace sne { namespace sgp {
