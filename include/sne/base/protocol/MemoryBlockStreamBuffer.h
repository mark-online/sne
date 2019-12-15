#pragma once

#include <sne/base/memory/MemoryBlock.h>
#include <sne/core/stream/StreamBuffer.h>
#include <sne/core/stream/Exception.h>
#include <sne/core/utility/CppUtils.h>
#include <cassert>

namespace sne { namespace base {

/** @addtogroup protocol
* @{
*/

/**
 * @class MemoryBlockStreamBuffer
 *
 * MemoryBlock 기반의 StreamBuffer
 * - MemoryBlock을 소유하지 않는다(메모리를 해제하지 않는다)
 */
class MemoryBlockStreamBuffer : public core::StreamBuffer
{
public:
    MemoryBlockStreamBuffer(MemoryBlock* block = nullptr) :
        block_(block) {}

    /// 내부 버퍼를 newBlock으로 대체한다.
    void reset(MemoryBlock& newBlock) {
        block_ = &newBlock;
    }

    MemoryBlock& getBlock() {
        return *block_;
    }

public:
    virtual void push(Item item) override {
        copyFrom(&item, 1);
    }

    virtual void pop() override {
        if (block_->length() != 0) {
            block_->rd_ptr(sizeof(Item));
            if (block_->length() == 0) {
                reset();
            }
            else {
                block_->crunch();
            }
        }
    }

    virtual void copyFrom(const Item* block, size_t blockSize) override {
        assert(block != nullptr);
        block_->copyFrom(reinterpret_cast<const char*>(block),
            sizeof(Item) * blockSize);
    }

    virtual void copyTo(Item* block, size_t blockSize) override {
        assert(block != nullptr);
        if (block_->length() < blockSize) {
            throw core::StreamingException(__FILE__, __LINE__,
                "block is corrupted");
        }

        block_->copyTo(block, blockSize);

        if (block_->length() == 0) {
            reset();
        }
        else {
            block_->crunch();
        }
    }

    virtual void reset() override {
        block_->reset();
    }

    virtual Item front() const override {
        return const_cast<MemoryBlockStreamBuffer*>(this)->front();
    }

    virtual Item& front() override {
        assert(! empty());
        return *reinterpret_cast<Item*>(block_->rd_ptr());
    }

    virtual Item back() const override {
        return const_cast<MemoryBlockStreamBuffer*>(this)->back();
    }

    virtual Item& back() override {
        assert(! empty());
        return *reinterpret_cast<Item*>(block_->wr_ptr() - sizeof(Item));
    }

    virtual size_t size() const override {
        return block_->length();
    }

    virtual bool empty() const override {
        return block_->length() == 0;
    }

    virtual size_t space() const override {
        return block_->space();
    }

public:
    bool isValid() const {
        //return size() == block_->total_length();
        return true;
    }

private:
    MemoryBlock* block_;
};

/** @} */ // addtogroup protocol

}} // namespace sne { namespace base {
