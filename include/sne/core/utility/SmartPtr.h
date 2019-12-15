#pragma once

#ifdef SNE_USE_SMARTPTR

#include <sne/Common.h>
#include <boost/swap.hpp>
#include <boost/noncopyable.hpp>
#include <cassert>

// reference count object 기반의 smart pointer

namespace sne { namespace core {

/**
* @class SharedObject
*
* reference count 기반의 베이스 클래스.
* - 공유 가능한 객체는 이 클래스로부터 상속받고 소멸자를 보호(protected)하여
*   객체 소멸을 막아야 한다.
*/
class SharedObject : public boost::noncopyable
{
    friend class SmartPtrTraits;
    friend class ThreadSafeSmartPtrTraits;

protected:
    SharedObject() :
        referenceCount_(0) {}
    virtual ~SharedObject() {
        assert((referenceCount_ == 0) &&
            "Shared object must be allocated dynamically.");
    }

public:
    long getReferenceCount() const {
        return referenceCount_;
    }

private:
    mutable uint32_t referenceCount_;
};


/**
 * 디폴트 SmartPtr traits.
 */
class SmartPtrTraits
{
public:
    static void addReference(SharedObject* rawPtr) {
        if (rawPtr != nullptr) {
            rawPtr->referenceCount_++;
        }
    }

    static void removeReference(SharedObject* rawPtr) {
        if ((rawPtr != nullptr) && (--rawPtr->referenceCount_ == 0)) {
            delete rawPtr;
        }
    }

    static void changeReference(SharedObject* rawPtr1, SharedObject* rawPtr2) {
        addReference(rawPtr1);
        removeReference(rawPtr2);
    }
};


/**
 * 멀티쓰레드 기반의 SmartPtr traits.
 */
class SNE_API ThreadSafeSmartPtrTraits
{
public:
    static void addReference(SharedObject* rawPtr);
    static void removeReference(SharedObject* rawPtr);
    static void changeReference(SharedObject* rawPtr1, SharedObject* rawPtr2) {
        ThreadSafeSmartPtrTraits::addReference(rawPtr1);
        ThreadSafeSmartPtrTraits::removeReference(rawPtr2);
    }
};


/**
 * @class SmartPtr
 *
 * 공유 객체의 smart pointer
 */
template <class Type, class Traits = SmartPtrTraits>
class SmartPtr
{
    typedef SmartPtr<Type, Traits> ThisSmartPtr;

public:
    SmartPtr(Type* ptr = nullptr) {
        rawPtr_ = ptr;
        Traits::addReference(rawPtr_);
    }

    SmartPtr(const ThisSmartPtr& r) {
        rawPtr_ = r.rawPtr_;
        Traits::addReference(rawPtr_);
    }

    ~SmartPtr() {
        Traits::removeReference(rawPtr_);
    }

public:
    ThisSmartPtr& operator=(const Type* ptr) {
        Traits::changeReference(ptr, rawPtr_);
        rawPtr_ = const_cast<Type*>(ptr);
        return *this;
    }

    ThisSmartPtr& operator=(const ThisSmartPtr& r) {
        return operator=(r.rawPtr_);
    }

public:
    Type& operator*() const {
        assert(rawPtr_);
        return *rawPtr_;
    }

    Type* operator->() const {
        assert(rawPtr_);
        return rawPtr_;
    }

    bool operator!() const {
        return ! rawPtr_;
    }

    bool operator<(const ThisSmartPtr& rhs) const {
        return rawPtr_ < rhs.rawPtr_;
    }

    Type* get() const {
        return rawPtr_;
    }

public:
    void reset() {
        Traits::removeReference(rawPtr_);
        rawPtr_ = nullptr;
    }

    bool isNull() const {
        return rawPtr_ == nullptr;
    }

    void swap(ThisSmartPtr& r) {
        boost::swap(rawPtr_, r.rawPtr_);
    }

public:
    friend bool operator==(const ThisSmartPtr& r, const Type* ptr) {
        return r.rawPtr_ == ptr;
    }

    friend bool operator!=(const ThisSmartPtr& r, const Type* ptr) {
        return ! operator==(r, ptr);
    }

    friend bool operator==(const Type* ptr, const ThisSmartPtr& r) {
        return r.rawPtr_ == ptr;
    }

    friend bool operator!=(const Type* ptr, const ThisSmartPtr& r) {
        return ! operator==(ptr, r);
    }

    friend bool operator==(const ThisSmartPtr& r1, const ThisSmartPtr& r2) {
        return r1.rawPtr_ == r2.rawPtr_;
    }

    friend bool operator!=(const ThisSmartPtr& r1, const ThisSmartPtr& r2) {
        return ! operator==(r1, r2);
    }

private:
    Type* rawPtr_;
};

}} // namespace sne { namespace core {

#endif // SNE_USE_SMARTPTR
