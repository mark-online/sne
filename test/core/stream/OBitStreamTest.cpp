#include "CoreTestPCH.h"
#include <sne/core/stream/detail/SneStreamFactory.h>
#include <sne/core/stream/detail/OBitStream.h>
#include <sne/test/DummyStreamBuffer.h>

using namespace sne;
using namespace sne::core;

/**
* @class OBitStreamTest
*
* output bit-stream Test
*/
class OBitStreamTest : public testing::Test
{
private:
    virtual void SetUp() override {
        ostream_ =
            SneStreamFactory(true, true).createOStream(buffer_).release();
    }

    virtual void TearDown() override {
        delete ostream_;
    }

protected:
    test::DummyStreamBuffer buffer_;
    core::OStream* ostream_;
};


TEST_F(OBitStreamTest, testEmpty)
{
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(0, ostream_->getTotalBitCount());
    ASSERT_EQ(0, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteBit)
{
    ostream_->write(uint8_t(1), 1);
    ASSERT_EQ(1, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(1, ostream_->getTotalBitCount());
    ASSERT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteBits)
{
    ostream_->write(uint8_t(1), 1);
    ASSERT_EQ(1, ostream_->getTotalBitCount()) << "1 bit";
    ASSERT_EQ(1, ostream_->getTotalSize()) << "1 byte";

    ostream_->write(uint8_t(0), 1);
    ASSERT_EQ(2, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(2, ostream_->getTotalBitCount());
    ASSERT_EQ(1, ostream_->getTotalSize());

    ostream_->write(uint8_t(0), 7 + (8 - 2));
    ASSERT_EQ(7, static_cast<OBitStream*>(ostream_)->getHoldingBitCount()) <<
        "7 + 8 bits";
    ASSERT_EQ(7 + 8, ostream_->getTotalBitCount());
    ASSERT_EQ(2, ostream_->getTotalSize());

    ostream_->write(uint8_t(1), 1);
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(16, ostream_->getTotalBitCount());
    ASSERT_EQ(2, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteUnalignedBits)
{
    ostream_->write(uint8_t(1), 9);
    ASSERT_EQ(1, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(9, ostream_->getTotalBitCount());
    ASSERT_EQ(2, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int8_t(127));
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(8 * 10, ostream_->getTotalBitCount()) << "8 * 10 bits";
    ASSERT_EQ(1 * 10, ostream_->getTotalSize()) << "10 bytes";
}


TEST_F(OBitStreamTest, testWriteUInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint8_t(255));
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(8 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(1 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int16_t(-1));
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(16 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(2 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteUInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint16_t(-1));
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(16 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(2 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int32_t(-1));
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(32 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteUInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint32_t(-1));
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(32 * 10 , ostream_->getTotalBitCount());
    ASSERT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteFloat32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(423423.65F);
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(32 * 10, ostream_->getTotalBitCount());
    ASSERT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteString)
{
    const std::string s("0123456789");
    ostream_->write(s, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(80 + 16, ostream_->getTotalBitCount());
    ASSERT_EQ(2 + 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteShortString)
{
    const std::string s(256, 'X');
    ostream_->write(s, UCHAR_MAX, core::Bits<uint8_t>::size);
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ((255 * 8) + 8, ostream_->getTotalBitCount());
    ASSERT_EQ(1 + 255, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testAlign)
{
    ostream_->write(uint8_t(1), 1);
    ostream_->align();
    ASSERT_EQ(8, ostream_->getTotalBitCount());
    ASSERT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testReset)
{
    ostream_->write(uint8_t(1), 1);
    ostream_->reset(false);
    ASSERT_EQ(0, ostream_->getTotalBitCount());
    ASSERT_EQ(1, ostream_->getTotalSize());

    ostream_->reset(true);
    ASSERT_EQ(0, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testOutOfMemory)
{
    buffer_.setPushError();

    ASSERT_THROW(ostream_->write(uint8_t(1), 1), core::Exception);
}


TEST_F(OBitStreamTest, testWriteZeroLengthString)
{
    const std::string s("");
    ostream_->write(s, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(0 + 16, ostream_->getTotalBitCount());
    ASSERT_EQ(2 + 0, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteUInt64)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint64_t(i + INT_MAX));
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(64 * 10 , ostream_->getTotalBitCount());
    ASSERT_EQ(8 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteInt64)
{
    for (int64_t i = 0; i < 10; ++i) {
        ostream_->write(-i);
    }
    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(64 * 10 , ostream_->getTotalBitCount());
    ASSERT_EQ(8 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteBuffer)
{
    char buffer[] = "독도는 우리땅이라고!";
    const uint16_t length = static_cast<uint16_t>(strlen(buffer));
    ostream_->write(buffer, length);

    ASSERT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    ASSERT_EQ(length * CHAR_BIT, ostream_->getTotalBitCount());
    ASSERT_EQ(length, ostream_->getTotalSize());
}
