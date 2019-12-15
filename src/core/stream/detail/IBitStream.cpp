#include "CorePCH.h"
#include <sne/core/stream/detail/IBitStream.h>
#include <sne/core/stream/StreamBuffer.h>
#include <sne/core/stream/Exception.h>
#include <sne/core/utility/Unicode.h>

namespace sne { namespace core {

namespace {

template <typename DataType>
void readBitsImpl(DataType& value, int bitCount,
    StreamBuffer& streamBuffer, int& bitReadCount)
{
    if (! isEnoughBits<DataType>(bitCount)) {
        throw StreamingException(__FILE__, __LINE__, "invalid bitCount");
    }

    int shiftCount = CHAR_BIT - bitReadCount;
    int leftBitCount = bitCount - shiftCount;
    const size_t needBytes = static_cast<size_t>(toBytes(leftBitCount) + 1);
    const size_t streamBufferSize = streamBuffer.size();
    if (streamBufferSize < needBytes) {
        throw StreamingException(__FILE__, __LINE__, "buffer is not enough");
    }

    DataType result =
        (static_cast<DataType>(streamBuffer.front()) >> bitReadCount);
    for (; leftBitCount > 0; leftBitCount -= CHAR_BIT) {
        streamBuffer.pop();
        result |= (static_cast<DataType>(streamBuffer.front()) << shiftCount);
        shiftCount += CHAR_BIT;
    }

    bitReadCount = (bitReadCount + bitCount) & (CHAR_BIT - 1);
    if (bitReadCount == 0) {
        streamBuffer.pop();
    }

    const DataType mask =
        (bitCount < Bits<DataType>::size) ? (1 << bitCount) - 1 : ~0;
    value = result & mask;
}

} // namespace

// = IBitStream

IBitStream::IBitStream(StreamBuffer& streamBuffer,
    bool shouldUseUtf8ForString) :
    streamBuffer_(streamBuffer),
    shouldUseUtf8ForString_(shouldUseUtf8ForString),
    bitReadCount_(0)
{
}


void IBitStream::read(void* buffer, uint16_t length)
{
    align();
    streamBuffer_.copyTo(
        static_cast<StreamBuffer::Item*>(buffer), length);
}


void IBitStream::readBits(uint32_t& value, int bitCount)
{
    readBitsImpl(value, bitCount, streamBuffer_, bitReadCount_);
}


void IBitStream::readBits(uint64_t& value, int bitCount)
{
    readBitsImpl(value, bitCount, streamBuffer_, bitReadCount_);
}


void IBitStream::read(std::string& value, size_t maxLength, int sizeBits)
{
    value.clear();

    uint32_t strLen;
    read(strLen, sizeBits);

    if (strLen > 0) {
        value.resize(strLen);
        read(&value[0], static_cast<uint16_t>(strLen));

        if (value.size() > maxLength) {
            value.resize(maxLength);
        }
    }
}


void IBitStream::read(std::wstring& value, size_t maxLength, int sizeBits)
{
    value.clear();

    uint32_t strLen;
    read(strLen, sizeBits);

    if (strLen > 0) {
        if (shouldUseUtf8ForString_) {
            std::string utf8;
            utf8.resize(strLen);
            read(&utf8[0], static_cast<uint16_t>(strLen));

            value = fromUtf8(utf8);
        }
        else {
            value.resize(strLen);
            read(&value[0],
                static_cast<uint16_t>(strLen * sizeof(std::wstring::value_type)));
        }

        if (value.size() > maxLength) {
            value.resize(maxLength);
        }
    }
}


void IBitStream::align()
{
    if (bitReadCount_ > 0) {
        streamBuffer_.pop();
    }
    bitReadCount_ = 0;
}


void IBitStream::reset(bool resetBuffer)
{
    if (resetBuffer) {
        streamBuffer_.reset();
    }
    bitReadCount_ = 0;
}


size_t IBitStream::getTotalBitCount() const
{
    return streamBuffer_.size() * CHAR_BIT;
}

}} // namespace sne { namespace core {
