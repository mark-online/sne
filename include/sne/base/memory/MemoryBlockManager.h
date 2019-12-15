#pragma once

#include "MemoryBlock.h"
#include "ObjectPool.h"
#include <boost/checked_delete.hpp>
#include <mutex>
#include <cassert>

namespace sne { namespace base {

/*
 * @class MemoryBlockAllocator
 */
class MemoryBlockAllocator
{
public:
    static MemoryBlockAllocator& instance(size_t initialBlockSize) {
        static MemoryBlockAllocator allocator;
        allocator.setInitialBlockSize(initialBlockSize);
        return allocator;
    }

public:
    MemoryBlock* malloc() {
        assert(initialBlockSize_ > 0);
        return new MemoryBlock(initialBlockSize_);
    }
    void free(void* resource) {
        boost::checked_delete(static_cast<MemoryBlock*>(resource));
    }

    void setInitialBlockSize(size_t initialBlockSize) {
        initialBlockSize_ = initialBlockSize;
    }

private:
    size_t initialBlockSize_ = 0;
};


/**
 * @class MemoryBlockManager
 *
 * MemoryBlock Manager
 * - pool & cache
 */
class MemoryBlockManager :
    private MemoryBlock::Releaser,
    public boost::noncopyable
{
protected:
    typedef ObjectPool<MemoryBlock, MemoryBlockAllocator, std::mutex> MemoryBlockPool;

public:
    /**
     * ctor
     * @param poolSize 기본 Pool 크기.
     */
    MemoryBlockManager(size_t poolSize, size_t initialBlockSize) :
        memoryBlockPool_(poolSize, MemoryBlockAllocator::instance(initialBlockSize)) {
        memoryBlockPool_.initialize();
    }

    /// MemoryBlock을 요청한다.
    MemoryBlock* acquire() {
        MemoryBlock* mb = memoryBlockPool_.acquire();
        if (mb != nullptr) {
            mb->acquire(*this);
        }
        return mb;
    }

private: // = MemoryBlock::Releaser overriding
    virtual void release(MemoryBlock* mb) override {
        memoryBlockPool_.release(mb);
    }

protected: // for Test
    MemoryBlockPool memoryBlockPool_;
};

}} // namespace sne { namespace base {
