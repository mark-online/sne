#include "SecurityTestPCH.h"
#include <sne/security/cipher/BlockCipherFactory.h>
#include <sne/security/cipher/BlockCipher.h>

using namespace sne;

/**
 * @class TeaBlockCipherTest
 *
 * TeaBlockCipher xUnit �׽�Ʈ.
 */
class TeaBlockCipherTest : public testing::Test
{
public:
    virtual void SetUp() override {
        cipher_ = security::BlockCipherFactory::create("tea").release();
    }

    virtual void TearDown() override {
        delete cipher_;
    }

protected:
    security::BlockCipher* cipher_;
};

TEST_F(TeaBlockCipherTest, testGetBlockSize)
{
    ASSERT_EQ(8, static_cast<int>(cipher_->getBlockSize())) <<
        "8 bytes align";
}


TEST_F(TeaBlockCipherTest, testEncryptDecrypt)
{
    const std::string plain(100 * cipher_->getBlockSize(), 'T');
    const security::Key cipherKey = 1;

    char* buffer = new char[plain.length()];
    memcpy(buffer, plain.c_str(), plain.length());

    char* begin = &buffer[0];
    char* end = &buffer[plain.length()];

    (void)cipher_->encrypt(begin, end, begin, cipherKey);
    (void)cipher_->decrypt(begin, end, begin, cipherKey);

    std::string decrypted;
    decrypted.assign(begin, (end - begin));
    ASSERT_EQ(plain, decrypted);

    delete[] buffer;
}


TEST_F(TeaBlockCipherTest, testMultipleEncryptDecrypt)
{
    const int count = 147;
    char* buffer = new char[count * cipher_->getBlockSize()];

    for (int i = 0; i < count; ++i) {
        const security::Key cipherKey = i;
        const std::string plain(((i + 1) * cipher_->getBlockSize()), 'M');
        memcpy(buffer, plain.c_str(), plain.length());

        char* begin = &buffer[0];
        char* end = &buffer[plain.length()];

        (void)cipher_->encrypt(begin, end, begin, cipherKey);
        (void)cipher_->decrypt(begin, end, begin, cipherKey);

        std::string decrypted;
        decrypted.assign(begin, (end - begin));
        ASSERT_EQ(plain, decrypted) << "same(#" << i << ")";
    }

    delete[] buffer;
}
