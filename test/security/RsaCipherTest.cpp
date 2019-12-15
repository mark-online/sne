#include "SecurityTestPCH.h"
#include <sne/security/cipher/RsaCipher.h>

using namespace sne;

/**
 * @class RsaCipherTest
 *
 * RSA cipher 테스트.
 */
class RsaCipherTest : public testing::Test
{
public:
    virtual void SetUp() override {
        rsaSeed_ =
            security::RsaCipher::generateSeed(
                security::RsaCipher::defaultPrivateKeySeedLength);
        cipher_ =
            new security::RsaCipher(
                security::RsaCipher::defaultPrivateKeyLength, rsaSeed_);
        outputBuffer_ = reinterpret_cast<const char*>(cipher_->getOutputBuffer());
    }

    virtual void TearDown() override {
        delete cipher_;
    }

protected:
    security::RsaCipher::Seed rsaSeed_;
    security::RsaCipher* cipher_;
    const char* outputBuffer_;
};

const std::string message("A gi da ri go gi da ri");

TEST_F(RsaCipherTest, testEncryptAndDecrypt)
{
    const size_t encryptedLen =
        cipher_->encrypt(message.c_str(), message.length());
    const std::string crypted(outputBuffer_, encryptedLen);
    ASSERT_TRUE(0 < encryptedLen) << cipher_->getLastErrorMessage();

    const size_t decryptedLen =
        cipher_->decrypt(crypted.c_str(), crypted.length());
    const std::string decrypted(outputBuffer_, decryptedLen);
    ASSERT_TRUE(0 < decryptedLen) << cipher_->getLastErrorMessage();

    ASSERT_EQ(message, decrypted);

    security::RsaCipher rsa2(cipher_->getPublicKey(),
        cipher_->getPrivateKey(), rsaSeed_);

    const size_t decryptedLen2 =
        cipher_->decrypt(crypted.c_str(), crypted.length());
    const std::string decrypted2(outputBuffer_, decryptedLen2);
    ASSERT_TRUE(0 < decryptedLen2) << cipher_->getLastErrorMessage();

    ASSERT_EQ(message, decrypted2);
}


TEST_F(RsaCipherTest, testSignAndVerify)
{
    const size_t sigLen = cipher_->sign(message.c_str(), message.length());
    const std::string signature(outputBuffer_, sigLen);

    ASSERT_EQ(true,
        cipher_->verify(message.c_str(), message.length(),
            signature.c_str(), signature.length())) <<
            cipher_->getLastErrorMessage();
}
