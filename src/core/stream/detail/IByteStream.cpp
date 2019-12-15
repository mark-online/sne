#include "CorePCH.h"
#include <sne/core/stream/detail/IByteStream.h>
#include <sne/core/stream/StreamBuffer.h>
#include <sne/core/stream/Exception.h>
#include <sne/core/utility/Unicode.h>

namespace sne { namespace core {

IByteStream::IByteStream(StreamBuffer& streamBuffer,
    bool shouldUseUtf8ForString) :
    streamBuffer_(streamBuffer),
    shouldUseUtf8ForString_(shouldUseUtf8ForString)
{
}


void IByteStream::read(void* buffer, uint16_t length)
{
    readBytes(buffer, length);
}


void IByteStream::read(std::string& value, size_t maxLength, int sizeBitCount)
{
    assert((sizeBitCount == 8) || (sizeBitCount == 16));

    value.clear();

    const uint32_t size = readStringSize(sizeBitCount);

    if (size > 0) {
        value.resize(size);
        readBytes(&(value[0]), size);

        if (value.size() > maxLength) {
            value.resize(maxLength);
        }
    }
}


void IByteStream::read(std::wstring& value, size_t maxLength, int sizeBitCount)
{
    assert((sizeBitCount == 8) || (sizeBitCount == 16));

    value.clear();

    const uint32_t strLen = readStringSize(sizeBitCount);

    if (strLen > 0) {
        if (shouldUseUtf8ForString_) {
            std::string utf8;
            utf8.resize(strLen);
            readBytes(&(utf8[0]), strLen);

            value = fromUtf8(utf8);
        }
        else {
            value.reserve(strLen);
            for (uint32_t i = 0; i < strLen; ++i) {
                std::wstring::value_type ch;
                readNumeric(ch);
                value.push_back(ch);
            }
        }

        if (value.size() > maxLength) {
            value.resize(maxLength);
        }
    }
}


uint32_t IByteStream::readStringSize(size_t sizeBitCount)
{
    if (sizeBitCount == 8) {
        return readByte();
    }
    
    uint16_t word = 0;
    readNumeric(word);
    return word;
}


void IByteStream::readBytes(void* buffer, size_t readSize)
{
    if (readSize > 0) {
        streamBuffer_.copyTo(static_cast<StreamBuffer::Item*>(buffer),
            readSize);
    }
}


uint8_t IByteStream::readByte()
{
    if (streamBuffer_.empty()) {
        throw StreamingException(__FILE__, __LINE__, "buffer is empty");
    }

    const uint8_t value = streamBuffer_.front();
    streamBuffer_.pop();
    return value;
}


void IByteStream::reset(bool resetBuffer)
{
    if (resetBuffer) {
        streamBuffer_.reset();
    }
}


size_t IByteStream::getTotalSize() const
{
    return streamBuffer_.size();
}

}} // namespace sne { namespace core {
