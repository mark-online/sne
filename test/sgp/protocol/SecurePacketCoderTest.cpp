#include "SgpTestPCH.h"
#include <sne/sgp/protocol/security/SecurePacketCoderFactory.h>
#include <sne/sgp/protocol/PacketCoder.h>
#include <sne/sgp/protocol/cs/CsProtocol.h>
#include <sne/base/utility/SystemUtil.h>
#include <sne/core/utility/Endian.h>
#include <gtest/gtest.h>

using namespace sne;

const sgp::PacketCoder::Seed encryptSeed("123456");
const sgp::PacketCoder::Seed decryptSeed("789012");
const std::string packetBody("01234567890123456789012345678");

/**
* @class SecurePacketCoderTest
*
* SecurePacketCoder 테스트
*/
class SecurePacketCoderTest : public testing::Test
{
    // TODO: 패킷 변조 테스트
public:
    virtual void SetUp() override {
        encoder_ = sgp::SecurePacketCoderFactory("aes", 1).create().release();
        encoder_->setEncryptSeed(encryptSeed);
        encoder_->setDecryptSeed(decryptSeed);

        decoder_ = sgp::SecurePacketCoderFactory("aes", 1).create().release();
        decoder_->setEncryptSeed(decryptSeed);
        decoder_->setDecryptSeed(encryptSeed);

        mblock_ = new base::MemoryBlock(2048);
        encoder_->reserveHeader(*mblock_);
        mblock_->copyFrom(packetBody.c_str(), packetBody.length());
    }

    virtual void TearDown() override {
        delete decoder_;
        delete encoder_;
    }

protected:
    sgp::PacketCoder* encoder_;
    sgp::PacketCoder* decoder_;
    base::MemoryBlock* mblock_;
};


TEST_F(SecurePacketCoderTest, testHeaderSize)
{
    ASSERT_EQ(
        static_cast<unsigned int>(
            sgp::CsPacketHeader::getHeaderSize()) +
            sizeof(uint32_t) + sizeof(uint16_t),
        static_cast<unsigned int>(encoder_->getHeaderSize())) << "open";
}


TEST_F(SecurePacketCoderTest, testEncode)
{
    sgp::AbstractPacketHeader header;
    ASSERT_EQ(true, encoder_->encode(*mblock_, header));

    ASSERT_TRUE(
        static_cast<int>(packetBody.length() + encoder_->getHeaderSize()) <=
        static_cast<int>(mblock_->length())) <<
        "mblock size must be equal or greater than";

    const void* rdPtr = mblock_->rd_ptr();
    ASSERT_TRUE(
        static_cast<uint16_t>(packetBody.length()) <=
        core::toRpcByteOrder(*static_cast<const uint16_t*>(rdPtr))) <<
        "packet body size";
}


TEST_F(SecurePacketCoderTest, testReadHeader)
{
    {
        sgp::AbstractPacketHeader header;
        (void)encoder_->encode(*mblock_, header);
    }

    sgp::AbstractPacketHeader header;
    ASSERT_EQ(true, decoder_->readHeader(header, *mblock_));

    ASSERT_TRUE(mblock_->rd_ptr() == mblock_->base());

    ASSERT_TRUE(
        static_cast<uint16_t>(packetBody.length()) <=
        header.bodySize_);
}


TEST_F(SecurePacketCoderTest, testDecode)
{
    {
        sgp::AbstractPacketHeader header;
        (void)encoder_->encode(*mblock_, header);
    }

    sgp::AbstractPacketHeader header;
    (void)decoder_->readHeader(header, *mblock_);

    ASSERT_EQ(true, decoder_->decode(*mblock_));

    const char* beginOfBody = mblock_->rd_ptr() + decoder_->getHeaderSize();
    ASSERT_EQ(packetBody,
        std::string(beginOfBody, packetBody.length()));
}


TEST_F(SecurePacketCoderTest, testMultipleEncodeDecode)
{
    for (int i = 0; i < 1000; ++i) {
        mblock_->reset();
        encoder_->reserveHeader(*mblock_);
        const std::string data((i + 1) % 1000 + 1, 'X');
        mblock_->copyFrom(data.c_str(), data.length());

        sgp::AbstractPacketHeader header;
        (void)encoder_->encode(*mblock_, header);

        sgp::AbstractPacketHeader decodedHeader;
        (void)decoder_->readHeader(decodedHeader, *mblock_);
        ASSERT_EQ(true, decoder_->decode(*mblock_)) <<
            "decode(#" << i << ")";

        const char* beginOfBody =
            mblock_->rd_ptr() + decoder_->getHeaderSize();
        ASSERT_EQ(data,
            std::string(beginOfBody, data.length())) <<
            "same body";
    }
}


TEST_F(SecurePacketCoderTest, testDifferenceCipherSeed)
{
    sgp::AbstractPacketHeader header;
    (void)encoder_->encode(*mblock_, header);

    std::unique_ptr<sgp::PacketCoder> diffDecoder(
        sgp::SecurePacketCoderFactory("aes").create());
    diffDecoder->setDecryptSeed(encryptSeed + "123");
    sgp::AbstractPacketHeader decodedHeader;
    (void)diffDecoder->readHeader(decodedHeader, *mblock_);

    ASSERT_EQ(false, diffDecoder->decode(*mblock_)) <<
        "invalid checksum";
}


TEST_F(SecurePacketCoderTest, testIsCipherKeyExpired)
{
    ASSERT_EQ(true, encoder_->shouldExchangeCipherSeed());
    ASSERT_EQ(true, decoder_->shouldExchangeCipherSeed());

    base::pause(10);

    ASSERT_EQ(true, encoder_->isCipherKeyExpired());
    ASSERT_EQ(true, decoder_->isCipherKeyExpired());
}
