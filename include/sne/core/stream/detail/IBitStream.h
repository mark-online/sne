#pragma once

#include "../IStream.h"

namespace sne { namespace core {

class StreamBuffer;

/** @addtogroup Stream
* @{
*/

/**
 * @class IBitStream
 *
 * 비트 단위의 Input stream.
 */
class IBitStream : public IStream
{
public:
    IBitStream(StreamBuffer& streamBuffer, bool shouldUseUtf8ForString);

    /// 테스트용
    int getReadBitCount() const {
        return bitReadCount_;
    }

public:
    virtual void read(uint64_t& value, int bitCount) override {
        readBits(value, bitCount);
    }

    virtual void read(uint32_t& value, int bitCount) override {
        readNumeric(value, bitCount);
    }

    virtual void read(uint16_t& value, int bitCount) override {
        readNumeric(value, bitCount);
    }

    virtual void read(uint8_t& value, int bitCount) override {
        readNumeric(value, bitCount);
    }

    virtual void read(int64_t& value, int bitCount) override {
        readBits(*static_cast<uint64_t*>(static_cast<void*>(&value)), bitCount);
        if (bitCount < Bits<int64_t>::size) {
            const uint64_t mask = (1 << bitCount) - 1;
            const uint64_t sign = (value & mask) >> (bitCount - 1);
            if (sign != 0) {
                value |= ~mask;
            }
        }
    }

    virtual void read(int32_t& value, int bitCount) override {
        readSignedNumeric(value, bitCount);
    }

    virtual void read(int16_t& value, int bitCount) override {
        readSignedNumeric(value, bitCount);
    }

    virtual void read(int8_t& value, int bitCount) override {
        readSignedNumeric(value, bitCount);
    }

    virtual void read(bool& value, int bitCount) override {
        uint32_t numeric;
        readNumeric(numeric, bitCount);
        value = (numeric != 0) ? true : false;
    }

    virtual void read(float32_t& value, int bitCount) override {
        readNumeric(*static_cast<uint32_t*>(static_cast<void*>(&value)),
            bitCount);
    }

    virtual void readEnum(uint64_t& value, int bitCount) override {
        readBits(value, bitCount);
    }

    virtual void read(std::string& value, size_t maxLength,
        int sizeBitCount) override;

    virtual void read(std::wstring& value, size_t maxLength,
        int sizeBitCount) override;

    virtual void read(void* buffer, uint16_t length) override;

    virtual void align() override;

    virtual void reset(bool resetBuffer) override;

    virtual size_t getTotalBitCount() const override;

    virtual size_t getTotalSize() const override {
        return toBytes(getTotalBitCount());
    }

private:
    template <typename T>
    void readNumeric(T& value, int bitCount) {
        uint32_t numeric;
        readBits(numeric, bitCount);
        value = static_cast<T>(numeric);
    }
    template <typename T>
    void readSignedNumeric(T& value, int bitCount) {
        uint32_t numeric;
        readBits(numeric, bitCount);
        if (bitCount < Bits<T>::size) {
            const uint32_t mask = (1 << bitCount) - 1;
            const uint32_t sign = (numeric & mask) >> (bitCount - 1);
            if (sign != 0) {
                numeric |= ~mask;
            }
        }
        value = static_cast<T>(static_cast<int32_t>(numeric));
    }
    void readBits(uint32_t& value, int bitCount);
    void readBits(uint64_t& value, int bitCount);
    uint8_t readByte() {
        uint8_t ch;
        read(ch, Bits<uint8_t>::size);
        return ch;
    }

private:
    StreamBuffer& streamBuffer_;
    const bool shouldUseUtf8ForString_;

    /// 읽혀진 데이터의 비트 수(<= 8)
    int bitReadCount_;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core {
