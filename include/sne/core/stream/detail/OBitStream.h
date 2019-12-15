#pragma once

#include "../OStream.h"

namespace sne { namespace core {

class StreamBuffer;

/** @addtogroup Stream
* @{
*/

/**
 * @class OBitStream
 *
 * 비트 단위의 Output stream.
 */
class OBitStream : public OStream
{
public:
    OBitStream(StreamBuffer& streamBuffer, bool shouldUseUtf8ForString);

    /// 테스트용
    int getHoldingBitCount() const {
        return holdingBitCount_;
    }

private:
    virtual void write(uint64_t value, int bitCount) override {
        writeBits(value, bitCount);
    }

    virtual void write(uint32_t value, int bitCount) override {
        writeBits(value, bitCount);
    }

    virtual void write(uint16_t value, int bitCount) override {
        writeBits(uint32_t(value), bitCount);
    }

    virtual void write(uint8_t value, int bitCount) override {
        writeBits(uint32_t(value), bitCount);
    }

    virtual void write(bool value, int bitCount) override {
        writeBits(uint32_t(value), bitCount);
    }

    virtual void write(int64_t value, int bitCount) override {
        writeBits(uint64_t(value), bitCount);
    }

    virtual void write(int32_t value, int bitCount) override {
        writeBits(uint32_t(value), bitCount);
    }

    virtual void write(int16_t value, int bitCount) override {
        writeBits(uint32_t(int32_t(value)), bitCount);
    }

    virtual void write(int8_t value, int bitCount) override {
        writeBits(uint32_t(int32_t(value)), bitCount);
    }

    virtual void write(float32_t value, int /*bitCount*/) override {
        writeBits(*static_cast<uint32_t*>(static_cast<void*>(&value)),
            Bits<float32_t>::size);
    }

    virtual void writeEnum(uint64_t value, int bitCount) override {
        write(value, bitCount);
    }

    virtual void write(const std::string& value, size_t maxLength,
        int sizeBitCount) override;

    virtual void write(const std::wstring& value, size_t maxLength,
        int sizeBitCount) override;

    virtual void write(const void* buffer, uint16_t length) override;

    virtual void align() override;
    virtual void reset(bool resetBuffer) override;

    virtual size_t getTotalBitCount() const override {
        return totalBitCount_;
    }

    virtual size_t getTotalSize() const override;

private:
    /// 비트 스트림을 스트림 버퍼에 기록한다
    void writeBits(uint32_t value, int bitCount);
    void writeBits(uint64_t value, int bitCount);

    void writeByte(uint8_t value) {
        write(value, Bits<uint8_t>::size);
    }

private:
    StreamBuffer& streamBuffer_;
    const bool shouldUseUtf8ForString_;
    /// 저장할 비트 수
    int holdingBitCount_;
    /// 저장된 비트 수
    int totalBitCount_;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core {
