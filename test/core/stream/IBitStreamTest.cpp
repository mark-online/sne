#include "CoreTestPCH.h"
#include <sne/core/stream/detail/IBitStream.h>
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::core;

/**
* @class IBitStreamTest
*
* input bit-stream Test
*/
class IBitStreamTest : public test::BitStreamFixture
{
};


TEST_F(IBitStreamTest, testEmpty)
{
    ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount()) <<
        "read bit count";
    ASSERT_EQ(0, istream_->getTotalBitCount()) << "empty";
    ASSERT_EQ(0, istream_->getTotalSize()) << "empty";
}


TEST_F(IBitStreamTest, testReadBit)
{
    ostream_->write(uint8_t(1), 1);

    ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount()) <<
        "read bit count";
    ASSERT_EQ(8, istream_->getTotalBitCount()) << "total bit count";
    ASSERT_EQ(1, istream_->getTotalSize()) << "total byte";

    uint8_t bit;
    istream_->read(bit, 1);
    ASSERT_EQ(1, bit) << "bit";
    ASSERT_EQ(1, static_cast<IBitStream&>(*istream_).getReadBitCount()) <<
        "read bit count";
    ASSERT_EQ(8, istream_->getTotalBitCount()) << "total bit count";
    ASSERT_EQ(1, istream_->getTotalSize()) << "total byte";
}


TEST_F(IBitStreamTest, testReadBits)
{
    ostream_->write(uint8_t(0), 1);
    ostream_->write(uint8_t(1), 1);
    ostream_->write(uint8_t(2), 2);
    ostream_->write(uint8_t(3), 3);

    uint32_t bit;
    istream_->read(bit, 1);
    ASSERT_EQ(uint32_t(0), bit);

    istream_->read(bit, 1);
    ASSERT_EQ(uint32_t(1), bit);

    istream_->read(bit, 2);
    ASSERT_EQ(uint32_t(2), bit);

    istream_->read(bit, 3);
    ASSERT_EQ(uint32_t(3), bit);

    ASSERT_EQ(
        1 + 1 + 2 + 3, static_cast<IBitStream&>(*istream_).getReadBitCount()) <<
        "read bit count";
    ASSERT_EQ(8, istream_->getTotalBitCount()) << "total bit count";
    ASSERT_EQ(1, istream_->getTotalSize()) << "total byte";
}


TEST_F(IBitStreamTest, testReadInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int8_t(i - 9));
    }

    ASSERT_EQ(8 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(10, istream_->getTotalSize());

    int8_t value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(i - 9, value);
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadUInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint8_t(i));
    }

    ASSERT_EQ(8 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(10, istream_->getTotalSize());

    uint8_t value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(i, value);
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int16_t(i - 9));
    }

    ASSERT_EQ(16 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(20, istream_->getTotalSize());

    int16_t value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(i - 9, value);
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadUInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint16_t(i));
    }

    ASSERT_EQ(16 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(20, istream_->getTotalSize());

    uint16_t value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(i, value);
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(int32_t(i - 9));
    }

    ASSERT_EQ(32 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(40, istream_->getTotalSize());

    int32_t value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(i - 9, value);
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadUInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint32_t(i));
    }

    ASSERT_EQ(32 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(40, istream_->getTotalSize());

    uint32_t value;
    for (uint32_t i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(i, value);
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadFloat32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(float32_t(i + 0.123));
    }

    ASSERT_EQ(32 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(40, istream_->getTotalSize());

    float32_t value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(float32_t(i + 0.123), value);
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadString)
{
    std::string original("0123456789");
    ostream_->write(original, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(2 + original.size(),
        size_t(ostream_->getTotalSize()));

    std::string value;
    istream_->read(value, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(original, value);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IBitStreamTest, testReadShortString)
{
    std::string original("0123456789");
    ostream_->write(original, UCHAR_MAX, core::Bits<uint8_t>::size);
    std::string value;
    istream_->read(value, USHRT_MAX, core::Bits<uint8_t>::size);
    ASSERT_EQ(original, value);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IBitStreamTest, testReadWString)
{
    std::wstring original(L"0123456789");
    ostream_->write(original, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(2 + (bytesForRpcChar * original.size()),
        size_t(ostream_->getTotalSize()));

    std::wstring value;
    istream_->read(value, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_TRUE(original == value);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IBitStreamTest, testReadWShortString)
{
    std::wstring original(L"0123456789");
    ostream_->write(original, UCHAR_MAX, core::Bits<uint8_t>::size);
    ASSERT_EQ(1 + (bytesForRpcChar * original.size()),
        size_t(ostream_->getTotalSize()));

    std::wstring value;
    istream_->read(value, USHRT_MAX, core::Bits<uint8_t>::size);
    ASSERT_TRUE(original == value);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IBitStreamTest, testUnderflow)
{
    int8_t value = 12;
    ostream_->write(value);

    istream_->read(value);

    ASSERT_THROW(istream_->read(value), core::Exception);
}


TEST_F(IBitStreamTest, testBitsNotEnough)
{
    int8_t value = 12;
    ostream_->write(value);

    istream_->read(value, 7);
    ASSERT_THROW(istream_->read(value, 2), core::Exception);
}


TEST_F(IBitStreamTest, testAlign)
{
    ostream_->write(uint8_t(0), 1);
    ostream_->write(uint8_t(1), 1);

    uint32_t bit;
    istream_->read(bit, 1);
    istream_->read(bit, 1);

    istream_->align();

    ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    ASSERT_EQ(0, istream_->getTotalBitCount());
}


TEST_F(IBitStreamTest, testReset)
{
    ostream_->write(uint8_t(0), 1);
    ostream_->write(uint8_t(1), 1);

    uint32_t bit;
    istream_->read(bit, 1);

    istream_->reset(true);

    ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    ASSERT_EQ(0, istream_->getTotalBitCount());
}


TEST_F(IBitStreamTest, testReadSignedInt)
{
    for (int i = 2; i <= 32; ++i) {
        ostream_->write(int32_t(-(i - 1)), i);
    }

    ASSERT_EQ(ostream_->getTotalSize(), istream_->getTotalSize());

    int32_t value;
    for (int i = 2; i <= 32; ++i) {
        istream_->read(value, i);
        ASSERT_EQ(-(i - 1), value) << i << " bits";
    }
}


TEST_F(IBitStreamTest, testReadZeroLengthString)
{
    std::string original("");
    ostream_->write(original, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(2 + 0, ostream_->getTotalSize()) << "2 + 0 bytes";

    std::string value;
    istream_->read(value, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(original, value);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IBitStreamTest, testReadUInt64)
{
    for (uint64_t i = 0; i < 10; ++i) {
        ostream_->write(i + INT_MAX);
    }

    ASSERT_EQ(64 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(80, istream_->getTotalSize());

    uint64_t value;
    for (uint64_t i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(i + INT_MAX, value) << "#" << i << " value";
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadInt64)
{
    for (int64_t i = 0; i < 10; ++i) {
        ostream_->write(-i);
    }

    ASSERT_EQ(64 * 10, istream_->getTotalBitCount());
    ASSERT_EQ(80, istream_->getTotalSize());

    int64_t value;
    for (int64_t i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(-i, value) << "#" << i << " value";
        ASSERT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
}


TEST_F(IBitStreamTest, testReadBuffer)
{
    char original[] = "0123456789";
    int length = static_cast<int>(strlen(original));

    ostream_->write(original, static_cast<uint16_t>(length));
    ASSERT_EQ(length, ostream_->getTotalSize());

    char actual[10 + 1];
    istream_->read(actual, static_cast<uint16_t>(length));
    actual[length] = '\0';
    ASSERT_EQ(length, static_cast<int>(strlen(actual))) << "buffer size";
    ASSERT_EQ(std::string(original), std::string(actual));
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}
