#pragma once

#include <sne/core/stream/detail/VectorStreamBuffer.h>
#include <sne/core/stream/Exception.h>

namespace sne { namespace test {

/**
 * @class DummyStreamBuffer
 */
class DummyStreamBuffer : public core::VectorStreamBuffer
{
public:
    DummyStreamBuffer() :
        isPushError_(false) {}

    virtual void push(Item item) override {
        if (isPushError_) {
            throw core::StreamingException(__FILE__, __LINE__, "push error");
        }
        else {
            core::VectorStreamBuffer::push(item);
        }
    }

    virtual void copyFrom(const Item* buffer, size_t bufferSize) override {
        if (isPushError_) {
            throw core::StreamingException(
                __FILE__, __LINE__, "copyFrom error");
        }
        else {
            core::VectorStreamBuffer::copyFrom(buffer, bufferSize);
        }
    }

    void setPushError() {
        isPushError_ = true;
    }
private:
    bool isPushError_;
};

}} // namespace sne { namespace test {
