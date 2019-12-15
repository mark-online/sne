#pragma once

#include "../StreamBuffer.h"
#include "../Exception.h"
#include <sne/core/container/Containers.h>
#include <sne/core/utility/CppUtils.h>
#include <cassert>

namespace sne { namespace core {

/** @addtogroup Stream
* @{
*/

/**
 * @class VectorStreamBuffer
 *
 * std::vector 기반의 StreamBuffer
 * - 효율을 위하여 read/write pointer를 사용함
 */
class VectorStreamBuffer : public StreamBuffer
{
    typedef Vector<Item> Buffer;
    typedef Buffer::size_type Index;

    enum { defaultStreamCapacity = 512 };
public:
    VectorStreamBuffer(
        size_t defaultCapacity = defaultStreamCapacity) :
        readIndex_(0),
        writeIndex_(0) {
        buffer_.reserve(defaultCapacity);
    }

    virtual void push(Item item) override {
        reserve(sizeof(item));
        buffer_.insert(buffer_.end(), item);
        ++writeIndex_;
    }

    virtual void pop() override {
        if (! empty()) {
            ++readIndex_;
            if (empty()) {
                reset();
            }
        }
    }

    virtual void copyFrom(const Item* buffer, size_t bufferSize) override {
        reserve(bufferSize);
        buffer_.insert(buffer_.end(), buffer, buffer + bufferSize);
        writeIndex_ += bufferSize;
    }

    virtual void copyTo(Item* buffer, size_t bufferSize) override {
        if (size() < bufferSize) {
            throw StreamingException(__FILE__, __LINE__,
                "buffer is corrupted");
        }

        const auto source = &buffer_[readIndex_];
        std::copy(source, source + bufferSize, buffer);
        readIndex_ += bufferSize;

        if (empty()) {
            reset();
        }
    }

    virtual void reset() override {
        readIndex_ = writeIndex_ = 0;
        buffer_.resize(0);
        // CAUTION: buffer_.clear()를 호출하면 안됨. 내부적으로 메모리를
        // 해제하여 capacity()가 0으로 설정되어 버림.
    }

    virtual Item front() const override {
        assert(! empty());
        return buffer_[readIndex_];
    }

    virtual Item& front() override {
        assert(! empty());
        return buffer_[readIndex_];
    }

    virtual Item back() const override {
        assert(! empty());
        return buffer_[writeIndex_ - 1];
    }

    virtual Item& back() override {
        assert(! empty());
        return buffer_[writeIndex_ - 1];
    }

    virtual size_t size() const override {
        return writeIndex_ - readIndex_;
    }

    virtual bool empty() const override {
        return writeIndex_ == readIndex_;
    }

    virtual size_t space() const override {
        return buffer_.capacity() - writeIndex_;
    }
public:
    bool isValid() const {
        return size() == buffer_.size();
    }
private:
    void reserve(size_t neededSize) {
        if (space() < neededSize) {
            buffer_.reserve(buffer_.capacity() * 2);
            if (space() < neededSize) {
                throw StreamingException(__FILE__, __LINE__, "out of memory");
            }
        }
    }
private:
    Index readIndex_;
    Index writeIndex_;
    Buffer buffer_;
};

/** @} */ // addtogroup Stream

}} // namespace sne { namespace core {
