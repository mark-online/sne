#pragma once

#include "../IStream.h"
#include "../../utility/Endian.h"

namespace sne { namespace core {

class StreamBuffer;

/** @addtogroup Stream
* @{
*/

/**
 * @class IByteStream
 *
 * 바이트 단위의 Input stream.
 */
class IByteStream : public IStream
{
public:
    IByteStream(StreamBuffer& streamBuffer, bool shouldUseUtf8ForString);

private:
    virtual void read(uint64_t& value, int /*bitCount*/) override {
        readNumeric(value);
    }

    virtual void read(uint32_t& value, int /*bitCount*/) override {
        readNumeric(value);
    }

    virtual void read(uint16_t& value, int /*bitCount*/) override {
        readNumeric(value);
    }

    virtual void read(uint8_t& value, int /*bitCount*/) override {
        value = readByte();
    }

    virtual void read(int64_t& value, int /*bitCount*/) override {
        readNumeric(value);
    }

    virtual void read(int32_t& value, int /*bitCount*/) override {
        readNumeric(value);
    }

    virtual void read(int16_t& value, int /*bitCount*/) override {
        readNumeric(value);
    }

    virtual void read(int8_t& value, int /*bitCount*/) override {
        value = readByte();
    }

    virtual void read(bool& value, int /*bitCount*/) override {
        const uint8_t numeric = readByte();
        value = (numeric != 0);
    }

    virtual void read(float32_t& value, int /*bitCount*/) override {
        readNumeric(*static_cast<uint32_t*>(static_cast<void*>(&value)));
    }

    virtual void readEnum(uint64_t& value, int bitCount) override {
        const int bytes = (bitCount + (CHAR_BIT - 1)) / CHAR_BIT;
        readBytes(&value, bytes);
    }

    virtual void read(std::string& value, size_t maxLength,
        int sizeBitCount) override;

    virtual void read(std::wstring& value, size_t maxLength,
        int sizeBitCount) override;

    virtual void read(void* buffer, uint16_t length) override;

    virtual void align() override {}

    virtual void reset(bool resetBuffer) override;

    virtual size_t getTotalBitCount() const override {
        return getTotalSize() * CHAR_BIT;
    }

    virtual size_t getTotalSize() const override;

private:
    template <typename T>
    void readNumeric(T& value, int byteCount = sizeof(T)) {
        assert((2 <= byteCount) && (byteCount <= 8));
        readBytes(&value, byteCount);
        value = toRpcByteOrder(value);
    }
    uint32_t readStringSize(size_t sizeBitCount);
    void readBytes(void* buffer, size_t readSize);
    uint8_t readByte();

private:
    StreamBuffer& streamBuffer_;
    const bool shouldUseUtf8ForString_;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core {
