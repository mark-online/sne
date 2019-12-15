#pragma once

#include <boost/noncopyable.hpp>
#include <memory>
#include <condition_variable>
#include <mutex>

namespace sne { namespace base {

/** @addtogroup Concurrent
* @{
*/

class Future;


/**
 * @class FutureObserver
*/
class FutureObserver
{
protected:
    virtual ~FutureObserver() {}

public:
    virtual void update(const Future& future) = 0;

    /// 메모리를 해제한다(메모리풀 사용시 필요)
    virtual void deleteFutureObserver() {
        assert(false && "DON'T CALL ME");
    }
};


/**
 * @class Future
 */
class Future : public boost::noncopyable
{
    enum {
        defaultTimeout = 10 * 1000 // 10 secs
    };

public:
    typedef std::shared_ptr<Future> Ref;
    typedef std::weak_ptr<Future> WeakRef;

public:
    Future() :
        observer_(nullptr),
        shouldDeleteObserver_(false),
        isCompleted_(false) {}

    virtual ~Future() {
        if (shouldDeleteObserver_) {
            observer_->deleteFutureObserver();
        }
    }

    /// 완료 observer를 등록한다
    void attach(FutureObserver* observer, bool shouldDeleteObserver = true) {
        assert(! observer_);
        assert(observer != nullptr);
        observer_ = observer;
        shouldDeleteObserver_ = shouldDeleteObserver;

        if (isCompleted_) {
            observer_->update(*this);
        }
    }

public:
    /// 작업이 완료되었을 경우 호출된다
    virtual void completed() {
        isCompleted_ = true;
        if (observer_ != nullptr) {
            observer_->update(*this);
        }
    }

    /// SyncFuture일 경우 작업이 완료될 때까지 대기한다
    virtual bool waitForDone(msec_t msecs = defaultTimeout) = 0;

    /// 작업을 취소한다
    virtual void cancel() {
        assert(false && "not supported");
    }

public:
    /// 반복 task인 경우 한 턴이 지났을 때 호출된다
    void turned() {
        if (observer_ != nullptr) {
            observer_->update(*this);
        }
    }

public:
    bool isDone() const {
        return isCompleted_;
    }

    virtual bool isCancelled() const {
        assert(false && "not supported");
        return false;
    }

private:
    FutureObserver* observer_;
    bool shouldDeleteObserver_;
    bool isCompleted_;
};


/**
 * @class SyncFuture
 */
class SyncFuture : public Future
{
public:
    virtual void completed() override {
        std::unique_lock<std::mutex> lock(mutex_);
        Future::completed();
        cv_.notify_one();
    }

    virtual bool waitForDone(msec_t msecs) override {
        std::unique_lock<std::mutex> lock(mutex_);

        if (isDone()) {
            return true;
        }
        return cv_.wait_for(lock, std::chrono::milliseconds(msecs)) ==
            std::cv_status::no_timeout;
    }

private:
    std::condition_variable cv_;
    std::mutex mutex_;
};


/**
 * @class AsyncFuture
 */
class AsyncFuture : public Future
{
public:
    virtual bool waitForDone(msec_t /*msecs*/) override {
        assert(false && "not supported");
        return false;
    }
};

/** @} */ // addtogroup Concurrent

}} // namespace sne { namespace base {
