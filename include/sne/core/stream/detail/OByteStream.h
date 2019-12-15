#pragma once

#include "../OStream.h"
#include "../../utility/Endian.h"

namespace sne { namespace core {

class StreamBuffer;

/** @addtogroup Stream
* @{
*/

/**
 * @class OByteStream
 *
 * 바이트 단위의 Output stream.
 */
class OByteStream : public OStream
{
public:
    OByteStream(StreamBuffer& streamBuffer, bool shouldUseUtf8ForString);

private:
    virtual void write(uint64_t value, int /*bitCount*/) override {
        writeNumeric(value);
    }

    virtual void write(uint32_t value, int /*bitCount*/) override {
        writeNumeric(value);
    }

    virtual void write(uint16_t value, int /*bitCount*/) override {
        writeNumeric(value);
    }

    virtual void write(uint8_t value, int /*bitCount*/) override {
        writeByte(value);
    }

    virtual void write(bool value, int /*bitCount*/) override {
        writeByte(uint8_t(value));
    }

    virtual void write(int64_t value, int /*bitCount*/) override {
        writeNumeric(value);
    }

    virtual void write(int32_t value, int /*bitCount*/) override {
        writeNumeric(value);
    }

    virtual void write(int16_t value, int /*bitCount*/) override {
        writeNumeric(value);
    }

    virtual void write(int8_t value, int /*bitCount*/) override {
        writeByte(value);
    }

    virtual void write(float32_t value, int /*bitCount*/) override {
        writeNumeric(*static_cast<uint32_t*>(static_cast<void*>(&value)));
    }

    virtual void writeEnum(uint64_t value, int bitCount) override {
        const int bytes = (bitCount + (CHAR_BIT - 1)) / CHAR_BIT;
        writeBytes(&value, bytes);
    }

    virtual void write(const std::string& value, size_t maxLength,
        int sizeBitCount) override;

    virtual void write(const std::wstring& value, size_t maxLength,
        int sizeBitCount) override;

    virtual void write(const void* buffer, uint16_t length) override;

    virtual void align() override {}
    virtual void reset(bool resetBuffer) override;

    virtual size_t getTotalBitCount() const override {
        return getTotalSize() * CHAR_BIT;
    }

    virtual size_t getTotalSize() const override;

private:
    template <typename T>
    void writeNumeric(T value) {
        value = toRpcByteOrder(value);
        writeBytes(&value, sizeof(value));
    }
    void writeStringLength(uint16_t strLen, int sizeBitCount);
    void writeBytes(const void* value, size_t byteCount);
    void writeByte(uint8_t value);

private:
    StreamBuffer& streamBuffer_;
    const bool shouldUseUtf8ForString_;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core {
