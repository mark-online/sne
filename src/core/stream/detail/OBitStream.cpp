#include "CorePCH.h"
#include <sne/core/stream/detail/OBitStream.h>
#include <sne/core/stream/StreamBuffer.h>
#include <sne/core/stream/Exception.h>
#include <sne/core/utility/Unicode.h>

namespace sne { namespace core {

namespace {

template <typename DataType>
inline DataType resetUnusedBits(DataType value, int bitCount)
{
    if (bitCount < Bits<DataType>::size) {
        return value & ((static_cast<DataType>(1) << bitCount) - 1);
    }
    return value;
}


template <typename DataType>
void writeBitsImpl(DataType value, int bitCount,
    StreamBuffer& streamBuffer, int& holdingBitCount, int& totalBitCount)
{
    assert(isEnoughBits<DataType>(bitCount));

    if (! isEnoughBits<DataType>(bitCount)) {
        throw StreamingException(__FILE__, __LINE__, "invalid bitCount");
    }

    DataType theValue = resetUnusedBits(value, bitCount);
    if (holdingBitCount > 0) {
        if (streamBuffer.empty()) {
            throw StreamingException(__FILE__, __LINE__, "buffer is empty");
        }
        streamBuffer.back() |= uint8_t(theValue << holdingBitCount);
    }
    else {
        streamBuffer.push(uint8_t(theValue));
    }

    const int bitShiftCount = CHAR_BIT - holdingBitCount;
    if (bitCount > bitShiftCount) {
        int leftBitCount = bitCount - bitShiftCount;
        theValue >>= bitShiftCount;
        do {
            streamBuffer.push(uint8_t(theValue));
            theValue >>= CHAR_BIT;
            leftBitCount -= CHAR_BIT;
        }
        while (leftBitCount > 0);
    }
    holdingBitCount = (holdingBitCount + bitCount) & (CHAR_BIT - 1);

    totalBitCount += bitCount;
}

} // namespace

// = OBitStream

OBitStream::OBitStream(StreamBuffer& streamBuffer,
    bool shouldUseUtf8ForString) :
    streamBuffer_(streamBuffer),
    shouldUseUtf8ForString_(shouldUseUtf8ForString),
    holdingBitCount_(0),
    totalBitCount_(0)
{
}


void OBitStream::writeBits(uint32_t value, int bitCount)
{
    writeBitsImpl(value, bitCount, streamBuffer_, holdingBitCount_,
        totalBitCount_);
}


void OBitStream::writeBits(uint64_t value, int bitCount)
{
    writeBitsImpl(value, bitCount, streamBuffer_, holdingBitCount_,
        totalBitCount_);
}


void OBitStream::align()
{
    if (holdingBitCount_ > 0) {
        assert(getTotalSize() > 0);
        writeBits(uint32_t(0), CHAR_BIT - holdingBitCount_);
        assert(0 == holdingBitCount_);
    }
}


void OBitStream::reset(bool resetBuffer)
{
    if (resetBuffer) {
        streamBuffer_.reset();
    }
    totalBitCount_ = 0;
}


void OBitStream::write(const std::string& value, size_t maxLength, int sizeBitCount)
{
    const uint32_t strLen =
        static_cast<uint32_t>((std::min)(value.size(), maxLength));
    write(strLen, sizeBitCount);

    write(&value[0], static_cast<uint16_t>(strLen));
}


void OBitStream::write(const std::wstring& value, size_t maxLength,
    int sizeBitCount)
{
    if (shouldUseUtf8ForString_) {
        std::wstring realValue = value;
        if (value.size() > maxLength) {
            realValue.resize(maxLength);
        }

        const std::string utf8(toUtf8(realValue));
        const uint32_t strLen =
            static_cast<uint32_t>((std::min)(utf8.size(), maxLength));
        write(strLen, sizeBitCount);

        write(&utf8[0], static_cast<uint16_t>(strLen));
    }
    else {
        const uint32_t strLen =
            static_cast<uint32_t>((std::min)(value.size(), maxLength));
        write(strLen, sizeBitCount);

        write(&value[0],
            static_cast<uint16_t>(strLen * sizeof(std::wstring::value_type)));
    }
}


void OBitStream::write(const void* buffer, uint16_t length)
{
    align();
    streamBuffer_.copyFrom(
        static_cast<const StreamBuffer::Item*>(buffer), length);
    totalBitCount_ += (length * CHAR_BIT);
}


size_t OBitStream::getTotalSize() const
{
    return streamBuffer_.size();
}

}} // namespace sne { namespace core {
