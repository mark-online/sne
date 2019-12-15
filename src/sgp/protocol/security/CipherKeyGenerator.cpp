#include "SgpPCH.h"
#include "CipherKeyGenerator.h"
#include <boost/random/linear_congruential.hpp>

namespace sne { namespace sgp {

namespace
{

/**
 * @class StaticCipherKeyGenerator
 * 고정 암호화 키 생성기
 */
class StaticCipherKeyGenerator : public CipherKeyGenerator
{
public:
    StaticCipherKeyGenerator() {
        init();
    }

private:
    virtual void setEncryptCipherKey(security::Key newKey) override {
        encryptSeed_ = newKey;
    }

    virtual void setDecryptCipherKey(security::Key newKey) override {
        decryptSeed_ = newKey;
    }

    virtual security::Key getEncryptCipherKey() const override {
        return encryptSeed_;
    }

    virtual security::Key getDecryptCipherKey() const override {
        return decryptSeed_;
    }


    virtual void reset() override {
        init();
    }

private:
    void init() {
        const security::Key defaultSeed = 0x20100301;
        encryptSeed_ = decryptSeed_ = defaultSeed;
    }

private:
    security::Key encryptSeed_;
    security::Key decryptSeed_;
};


/**
 * @class DynamicCipherKeyGenerator
 * 동적으로 변하는 암호화 키 생성기
 */
class DynamicCipherKeyGenerator : public CipherKeyGenerator
{
public:
    DynamicCipherKeyGenerator() {
        init();
    }

private:
    virtual void setEncryptCipherKey(security::Key newKey) override {
        encryptKey_.seed(static_cast<boost::int32_t>(newKey));
    }

    virtual void setDecryptCipherKey(security::Key newKey) override {
        decryptKey_.seed(static_cast<boost::int32_t>(newKey));
    }

    virtual security::Key getEncryptCipherKey() const override {
        return encryptKey_();
    }

    virtual security::Key getDecryptCipherKey() const override {
        return decryptKey_();
    }

    virtual void reset() override {
        init();
    }

private:
    void init() {
        const boost::int32_t defaultSeed = 0x19720212;
        encryptKey_.seed(defaultSeed);
        decryptKey_.seed(defaultSeed);
    }

private:
    mutable boost::rand48 encryptKey_;
    mutable boost::rand48 decryptKey_;
};

} // namespace

// = CipherKeyGenerator

std::unique_ptr<CipherKeyGenerator> CipherKeyGenerator::create(bool useDynamicKey)
{
    if (useDynamicKey) {
        return std::unique_ptr<CipherKeyGenerator>(std::make_unique<DynamicCipherKeyGenerator>());
    }
    return std::unique_ptr<CipherKeyGenerator>(std::make_unique<StaticCipherKeyGenerator>());
}

}} // namespace sne { namespace sgp {
