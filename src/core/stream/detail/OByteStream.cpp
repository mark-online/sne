#include "CorePCH.h"
#include <sne/core/stream/detail/OByteStream.h>
#include <sne/core/stream/StreamBuffer.h>
#include <sne/core/stream/Exception.h>
#include <sne/core/utility/Unicode.h>

namespace sne { namespace core {

OByteStream::OByteStream(StreamBuffer& streamBuffer,
    bool shouldUseUtf8ForString) :
    streamBuffer_(streamBuffer),
    shouldUseUtf8ForString_(shouldUseUtf8ForString)
{
}


void OByteStream::write(const std::string& value, size_t maxLength,
    int sizeBitCount)
{
    const uint16_t strLen =
        static_cast<uint16_t>((std::min)(value.size(), maxLength));
    writeStringLength(strLen, sizeBitCount);
    writeBytes(value.data(), strLen);
}


void OByteStream::write(const std::wstring& value, size_t maxLength,
    int sizeBitCount)
{
    if (shouldUseUtf8ForString_) {
        std::wstring realValue(value);
        if (realValue.size() > maxLength) {
            realValue.resize(maxLength);
        }

        const std::string utf8(toUtf8(realValue));
        const uint16_t strLen = static_cast<uint16_t>(utf8.size());
        writeStringLength(strLen, sizeBitCount);
        writeBytes(utf8.data(), utf8.size());
    }
    else {
        const uint16_t strLen =
            static_cast<uint16_t>((std::min)(value.size(), maxLength));
        writeStringLength(strLen, sizeBitCount);

        for (uint32_t i = 0; i < strLen; ++i) {
            writeNumeric(value[i]);
        }
    }
}


void OByteStream::write(const void* buffer, uint16_t length)
{
    writeBytes(buffer, length);
}


void OByteStream::writeStringLength(uint16_t strLen, int sizeBitCount)
{
    assert((sizeBitCount == 8) || (sizeBitCount == 16));
    if (sizeBitCount <= 8) {
        writeNumeric(static_cast<uint8_t>(strLen));
    }
    else {
        writeNumeric(strLen);
    }
}


void OByteStream::writeBytes(const void* value, size_t byteCount)
{
    streamBuffer_.copyFrom(static_cast<const StreamBuffer::Item*>(value),
        byteCount);
}


void OByteStream::writeByte(uint8_t value)
{
    streamBuffer_.push(value);
}


void OByteStream::reset(bool resetBuffer)
{
    if (resetBuffer) {
        streamBuffer_.reset();
    }
}


size_t OByteStream::getTotalSize() const
{
    return streamBuffer_.size();
}

}} // namespace sne { namespace core {
