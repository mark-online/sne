#pragma once

#include <sne/core/container/Containers.h>
#include <sne/core/utility/CppUtils.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace base {


/**
 * @class MemoryBlock
 * 가변 길이의 동적 할당된 메모리 블럭
 * - ACE_Message_Block 참고
 * - VectorStreamBuffer를 개조함
 */
class MemoryBlock : public boost::noncopyable
{
    enum { defaultInitialSize = 1024 };

    typedef core::Vector<char> Buffer;
    typedef Buffer::size_type Pointer;

public:
    class Releaser
    {
    public:
        virtual ~Releaser() {}

        virtual void release(MemoryBlock* mb) = 0;
    };

public:
    explicit MemoryBlock(size_t initialSize = defaultInitialSize) :
        releaser_(nullptr),
        rd_ptr_(0),
        wr_ptr_(0) {
        size(initialSize);
    }

    void acquire(Releaser& releaser) {
        reset();
        releaser_ = &releaser;
    }

    void release() {
        assert(releaser_ != nullptr);
        releaser_->release(this);
    }

public:
    /// neededSize만큼 공간을 확보한다
    void reserve(size_t neededSize) {
        if (space() < neededSize) {
            size(capacity() + (neededSize - space()));
        }
        assert(space() >= neededSize);
    }

    void reset() {
        rd_ptr_ = wr_ptr_ = 0;
    }

    void size(size_t n) {
        buffer_.resize(n);
    }

    void crunch() {
        const size_t boundary = (capacity() * 3) / 10;
        if (rd_ptr_ >= boundary) {
            if (rd_ptr_ > wr_ptr_) {
                assert(false);
                return;
            }
            const size_t len = length();
            if (len > 0) {
                memmove(base(), rd_ptr(), len);
            }
            rd_ptr_ = 0;
            wr_ptr_ = len;
        }
    }

    void copyFrom(const char* buf, size_t n) {
        reserve(n);
        std::copy(buf, buf + n, wr_ptr());
        wr_ptr_ += n;
    }

    void copyTo(void* buf, size_t n) {
        char* source = &buffer_[rd_ptr_];
        std::copy(source, source + n, reinterpret_cast<char*>(buf));
        rd_ptr_ += n;
    }

public:
    char* base() {
        return &buffer_[0];
    }

    const char* base() const {
        return &buffer_[0];
    }

    char* wr_ptr() {
        return &buffer_[wr_ptr_];
    }

    void wr_ptr(size_t n) {
        wr_ptr_ += n;
    }

    char* rd_ptr() {
        return &buffer_[rd_ptr_];
    }

    void rd_ptr(const char* newPtr) {
        rd_ptr_ = newPtr - base();
    }

    void rd_ptr(size_t n) {
        rd_ptr_ += n;
    }

    const char* rd_ptr() const {
        return &buffer_[rd_ptr_];
    }

public:
    size_t length() const {
        return wr_ptr_ - rd_ptr_;
    }

    size_t capacity() const {
        return buffer_.size();
    }

    size_t space() const {
        return capacity() - wr_ptr_;
    }

private:
    Releaser* releaser_;

    Buffer buffer_;
    Pointer rd_ptr_;
    Pointer wr_ptr_;
};


/**
* @class MemoryBlockGuard
*
* MemoryBlock Guard Class
*/
class MemoryBlockGuard : public boost::noncopyable
{
public:
    explicit MemoryBlockGuard(MemoryBlock* mblk = 0) :
        mblock_(mblk) {}

    MemoryBlockGuard(MemoryBlock& mblk) :
        mblock_(&mblk) {}

    ~MemoryBlockGuard() {
        if (mblock_ != nullptr) {
            mblock_->release();
        }
    }

    void reset(MemoryBlock* mblk = nullptr) {
        mblock_ = mblk;
    }

    MemoryBlock* release() {
        MemoryBlock* released = mblock_;
        mblock_ = nullptr;
        return released;
    }

    MemoryBlock* operator->() const {
        return mblock_;
    }

    MemoryBlock& operator*() const {
        assert(mblock_ != nullptr);
        return *mblock_;
    }

    MemoryBlock* get() const {
        return mblock_;
    }
private:
    MemoryBlock* mblock_;
};

}} // namespace sne { namespace base {
