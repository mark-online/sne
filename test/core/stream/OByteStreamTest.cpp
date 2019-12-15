#include "CoreTestPCH.h"
#include <sne/core/stream/detail/SneStreamFactory.h>
#include <sne/core/stream/detail/OByteStream.h>
#include <sne/test/DummyStreamBuffer.h>

using namespace sne;
using namespace sne::core;

/**
* @class OByteStreamTest
*
* output byte-stream Test
*/
class OByteStreamTest : public testing::Test
{
private:
    virtual void SetUp() override {
        ostream_ =
            SneStreamFactory(true, false).createOStream(buffer_).release();
    }

    virtual void TearDown() override {
        delete ostream_;
    }

protected:
    test::DummyStreamBuffer buffer_;
    core::OStream* ostream_;
};


TEST_F(OByteStreamTest, testEmpty)
{
    ASSERT_EQ(0, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteBit)
{
    ostream_->write(uint8_t(1), 1);
    ASSERT_EQ(8, ostream_->getTotalBitCount());
    ASSERT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteBits)
{
    ostream_->write(uint8_t(1), 1);
    ASSERT_EQ(1, ostream_->getTotalSize());

    ostream_->write(uint8_t(0), 1);
    ASSERT_EQ(2, ostream_->getTotalSize());

    ostream_->write(uint8_t(0), 7 + (8 - 2));
    ASSERT_EQ(3, ostream_->getTotalSize());

    ostream_->write(uint8_t(1), 1);
    ASSERT_EQ(4, ostream_->getTotalSize());
    ASSERT_EQ(8 * 4, ostream_->getTotalBitCount());
}


TEST_F(OByteStreamTest, testWriteUnalignedBits)
{
    ostream_->write(uint8_t(1), 9);
    ASSERT_EQ(1 * 8, ostream_->getTotalBitCount());
    ASSERT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int8_t(127));
    }
    ASSERT_EQ(8 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteUInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint8_t(255));
    }
    ASSERT_EQ(8 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(1 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int16_t(-1));
    }
    ASSERT_EQ(16 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(2 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteUInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint16_t(-1));
    }
    ASSERT_EQ(16 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(2 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int32_t(-1));
    }
    ASSERT_EQ(32 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteUInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint32_t(-1));
    }
    ASSERT_EQ(32 * 10 , ostream_->getTotalBitCount());
    ASSERT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteFloat32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(423423.65F);
    }
    ASSERT_EQ(32 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteString)
{
    const std::string s("0123456789");
    ostream_->write(s, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(80 + 16, ostream_->getTotalBitCount());
    ASSERT_EQ(2 + 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteShortString)
{
    const std::string s("0123456789");
    ostream_->write(s, UCHAR_MAX, core::Bits<uint8_t>::size);
    ASSERT_EQ(80 + 8, ostream_->getTotalBitCount());
    ASSERT_EQ(1 + 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testAlign)
{
    ostream_->write(uint8_t(1), 1);
    ostream_->align();
    ASSERT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testReset)
{
    ostream_->write(uint8_t(1), 1);
    ostream_->reset(false);
    ASSERT_EQ(1, ostream_->getTotalSize());

    ostream_->reset(true);
    ASSERT_EQ(0, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testOutOfMemory)
{
    buffer_.setPushError();
    ASSERT_THROW(ostream_->write(uint8_t(1), 1), core::Exception);
}


TEST_F(OByteStreamTest, testWriteZeroLengthString)
{
    const std::string s("");
    ostream_->write(s, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(0 + 16, ostream_->getTotalBitCount());
    ASSERT_EQ(2 + 0, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteUInt64)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint64_t(i + INT_MAX));
    }
    ASSERT_EQ(64 * 10 , ostream_->getTotalBitCount());
    ASSERT_EQ(8 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteInt64)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(-int64_t(i));
    }
    ASSERT_EQ(64 * 10 , ostream_->getTotalBitCount());
    ASSERT_EQ(8 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteBuffer)
{
    char buffer[] = "무궁화꽃이 피었습니다.";
    const uint16_t length = static_cast<uint16_t>(strlen(buffer));
    ostream_->write(buffer, length);

    ASSERT_EQ(length * CHAR_BIT, ostream_->getTotalBitCount());
    ASSERT_EQ(length, ostream_->getTotalSize());
}
