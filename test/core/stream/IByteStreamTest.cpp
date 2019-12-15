#include "CoreTestPCH.h"
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::core;

/**
* @class IByteStreamTest
*
* input bit-stream Test
*/
class IByteStreamTest : public test::ByteStreamFixture
{
};


TEST_F(IByteStreamTest, testEmpty)
{
    ASSERT_EQ(0, istream_->getTotalBitCount()) << "empty";
    ASSERT_EQ(0, istream_->getTotalSize()) << "empty";
}


TEST_F(IByteStreamTest, testReadBit)
{
    ostream_->write(uint8_t(1), 1);

    ASSERT_EQ(8, istream_->getTotalBitCount());
    ASSERT_EQ(1, istream_->getTotalSize());

    uint8_t bit;
    istream_->read(bit, 1);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadBits)
{
    ostream_->write(uint8_t(0), 1);
    ostream_->write(uint8_t(1), 1);
    ostream_->write(uint8_t(2), 2);
    ostream_->write(uint8_t(3), 3);
    ASSERT_EQ(4, ostream_->getTotalSize());

    uint8_t bit;
    istream_->read(bit, 1);
    ASSERT_EQ(3, istream_->getTotalSize());
    ASSERT_EQ(0, bit);

    istream_->read(bit, 1);
    ASSERT_EQ(1, bit);

    istream_->read(bit, 2);
    ASSERT_EQ(2, bit);

    istream_->read(bit, 3);
    ASSERT_EQ(3, bit);

    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadInt8)
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
    }
}


TEST_F(IByteStreamTest, testReadUInt8)
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
    }
}


TEST_F(IByteStreamTest, testReadInt16)
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
    }
}


TEST_F(IByteStreamTest, testReadUInt16)
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
    }
}



TEST_F(IByteStreamTest, testReadInt32)
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
    }
}


TEST_F(IByteStreamTest, testReadUInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint32_t(i));
    }

    ASSERT_EQ(4 * 10 * 8, istream_->getTotalBitCount());
    ASSERT_EQ(40, istream_->getTotalSize());

    uint32_t value;
    for (uint32_t i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(i, value);
    }
}


TEST_F(IByteStreamTest, testReadFloat32)
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
    }
}


TEST_F(IByteStreamTest, testReadString)
{
    std::string original("0123456789");

    ostream_->write(original, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(2 + 10, ostream_->getTotalSize());

    std::string value;
    istream_->read(value, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(original, value);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadShortString)
{
    std::string original("0123456789");
    ostream_->write(original, UCHAR_MAX, core::Bits<uint8_t>::size);
    ASSERT_EQ(1 + original.size(), size_t(ostream_->getTotalSize()));

    std::string value;
    istream_->read(value, USHRT_MAX, core::Bits<uint8_t>::size);
    ASSERT_EQ(original, value);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadWString)
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


TEST_F(IByteStreamTest, testReadWShortString)
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


TEST_F(IByteStreamTest, testUnderflow)
{
    int8_t value = 12;
    ostream_->write(value);

    istream_->read(value);

    ASSERT_THROW(istream_->read(value), core::Exception);
}


TEST_F(IByteStreamTest, testAlign)
{
    ostream_->write(uint8_t(0), 1);
    ostream_->write(uint8_t(1), 1);

    uint8_t bit;
    istream_->read(bit, 1);
    istream_->read(bit, 1);

    istream_->align();

    ASSERT_EQ(0, istream_->getTotalBitCount());
}


TEST_F(IByteStreamTest, testReadSignedInt)
{
    for (int i = 2; i <= 2; ++i) {
        ostream_->write(int32_t(-(i - 1)), i);
    }

    ASSERT_EQ((2 - 2 + 1) * 4, ostream_->getTotalSize());

    int32_t value;
    for (int i = 2; i <= 2; ++i) {
        istream_->read(value, i);
        ASSERT_EQ(-(i - 1), value) << i << " bits";
    }

    ASSERT_EQ(0, istream_->getTotalBitCount());
}


TEST_F(IByteStreamTest, testReadZeroLengthString)
{
    std::string original("");
    ostream_->write(original, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(2 + 0, ostream_->getTotalSize());

    std::string value;
    istream_->read(value, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(original, value);
    ASSERT_EQ(0, istream_->getTotalBitCount());
    ASSERT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadUInt64)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(uint64_t(i) + INT_MAX);
    }

    ASSERT_EQ(8 * 10 * 8, istream_->getTotalBitCount());
    ASSERT_EQ(8 * 10, istream_->getTotalSize());

    uint64_t value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(static_cast<uint64_t>(i) + INT_MAX, value) << "#" << i << value;
    }
}


TEST_F(IByteStreamTest, testReadInt64)
{
    for (int64_t i = 0; i < 10; ++i) {
        ostream_->write(-i);
    }

    ASSERT_EQ(8 * 10 * 8, istream_->getTotalBitCount());
    ASSERT_EQ(8 * 10, istream_->getTotalSize());

    int64_t value;
    for (int64_t i = 0; i < 10; ++i) {
        istream_->read(value);
        ASSERT_EQ(-i, value) << "#" << i << "value";
    }
}


TEST_F(IByteStreamTest, testReadBuffer)
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
