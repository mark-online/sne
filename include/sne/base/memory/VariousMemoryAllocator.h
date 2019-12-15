#pragma once

#include <sne/core/container/Containers.h>
#include <algorithm>
#include <functional>

namespace sne { namespace base {

/** @addtogroup memory
* @{
*/

/**
 * @class VariousMemoryAllocator
 *
 * 다양한 크기의 메모리 할당자
 * - Mutex: Mutex 타입(예, std::null_mutex, std::mutex, ...)
 * - 원하는 크기의 메모리가 풀에 없는 경우 새로 할당한다.
 * - vector를 사용해서 메모리 복사 비용이 높아지지만, multiset이나 multimap을
 *   사용하는 것보다 훨씬 효율적이다.
 * - Cache Hit Ratio에 따라 성능이 달라진다.
 * - Effective STL Item 23 참고.
 */
template <class Mutex>
class VariousMemoryAllocator
{
private:
    /// memory block
    struct Block
    {
        enum { headerSize = sizeof(size_t) };

        size_t chunkSize_;
        void* chunk_;

        Block() :
            chunkSize_(0),
            chunk_(nullptr) {}

        Block(size_t chunkSize) :
            chunkSize_(chunkSize) {
            chunk_ = new uint8_t[chunkSize + headerSize];
            *static_cast<size_t*>(chunk_) = chunkSize;
        }

        Block(size_t chunkSize, void* block) :
            chunkSize_(chunkSize),
            chunk_(block) {}

        bool operator < (const Block& rhs) const {
            return chunkSize_ < rhs.chunkSize_;
        }

        void free() {
            delete[] static_cast<uint8_t*>(chunk_);
            chunk_ = nullptr;
        }
    };

    typedef core::Vector<Block> Blocks;

public:
    /**
     * ctor
     * @param poolSize Pool 크기.
     * @param chunkSize 할당할 메모리 크기
     */
    VariousMemoryAllocator(size_t poolSize, size_t chunkSize) {
        preallocate(poolSize, chunkSize);
    }

    virtual ~VariousMemoryAllocator() {
        destroy();
    }
public:
    void* malloc(size_t nbytes) {
        void* chunk = nullptr;

        {
            std::lock_guard<Mutex> monitor(lock_);

            const typename Blocks::iterator pos = find(nbytes);
            if (pos != cachedBlocks_.end()) {
                Block& block = *pos;
                assert(block.chunkSize_ >= nbytes);
                chunk = block.chunk_;
                cachedBlocks_.erase(pos); // 비용이 가장 높다!!!
            }
            else {
                chunk = Block(nbytes).chunk_;
            }
        }

        if (! chunk) {
            assert(false);
            return nullptr;
        }
        return static_cast<char*>(chunk) + Block::headerSize;
    }

    void* calloc(size_t nbytes, char initial_value = '\0') {
        void* chunk = malloc(nbytes);
        if (chunk != nullptr) {
            memset(chunk, initial_value, nbytes);
        }
        return chunk;
    }

    void* calloc(size_t n_elem, size_t elem_size,
        char initial_value = '\0') {
        return VariousMemoryAllocator::calloc(n_elem * elem_size,
            initial_value);
    }

    void free(void* ptr) {
        if (ptr != nullptr) {
            void* chunk = static_cast<char*>(ptr) - Block::headerSize;

            std::lock_guard<Mutex> monitor(lock_);

            insert(Block(*static_cast<size_t*>(chunk), chunk));
        }
    }

public:
    size_t getCachedMemoryCount() const {
        std::lock_guard<Mutex> monitor(lock_);

        return cachedBlocks_.size();
    }

private:
    void preallocate(size_t poolSize, size_t chunkSize) {
        assert(cachedBlocks_.empty());
        cachedBlocks_.reserve(cachedBlocks_.size() + poolSize);
        for (size_t i = 0; i < poolSize; ++i) {
            cachedBlocks_.emplace_back(chunkSize);
        }
    }

    void insert(const Block& block) {
        const typename Blocks::iterator pos = find(block.chunkSize_);
        if (pos != cachedBlocks_.end()) {
            assert(! (*pos < block));
            cachedBlocks_.insert(pos, block);
        }
        else {
            cachedBlocks_.push_back(block);
        }
    }

    typename Blocks::iterator find(size_t chunkSize) {
        return std::lower_bound(cachedBlocks_.begin(), cachedBlocks_.end(),
            Block(chunkSize, 0));
    }

    void destroy() {
        std::for_each(cachedBlocks_.begin(), cachedBlocks_.end(),
            std::mem_fun_ref(&Block::free));
        cachedBlocks_.clear();
    }

private:
    Blocks cachedBlocks_;

    mutable Mutex lock_;
};

/** @} */ // addtogroup memory

}} // namespace sne { namespace base {
