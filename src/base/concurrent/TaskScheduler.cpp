#include "BasePCH.h"
#include <sne/base/concurrent/TaskScheduler.h>
#include <sne/base/io_context/IoContextTask.h>
#include <sne/base/utility/Logger.h>
#include <sne/core/memory/MemoryPoolMixin.h>
#include <boost/asio/io_context.hpp>

namespace sne { namespace base {

namespace {

/**
 * @class ScheduledTaskFuture
 */
class ScheduledTaskFuture :
    public AsyncFuture,
    public FutureObserver,
    public sne::core::ThreadSafeMemoryPoolMixin<ScheduledTaskFuture>
{
public:
    typedef std::shared_ptr<ScheduledTaskFuture> Ref;

public:
    /// 참고: http://stackoverflow.com/questions/19528375/lambda-capture-shared-ptr-member
    static void schedule(ScheduledTaskFuture::Ref taskFuture, msec_t delay) {
        ScheduledTaskFuture::Ref savedTaskFuture = taskFuture;
        boost::asio::deadline_timer& timer = taskFuture->getTimer();
        timer.expires_from_now(boost::posix_time::milliseconds(delay));
        timer.async_wait([=](const boost::system::error_code& ec) {
            if (!ec) {
                if (savedTaskFuture->isCancelled()) {
                    return;
                }
                if (savedTaskFuture->getInterval() > 0) {
                    savedTaskFuture->turned();

                    schedule(savedTaskFuture, savedTaskFuture->getInterval());
                }
                else {
                    savedTaskFuture->completed();
                }
            }
            else {
                if (ec != boost::asio::error::operation_aborted) {
                    SNE_LOG_DEBUG("scheduled timer error = %s", ec.message().c_str());
                }
            }
        });
    }

public:
    /// @param interval 반복 주기
    ScheduledTaskFuture(boost::asio::io_context& ios,
        Runnable* task, bool shouldDeleteTask, msec_t interval) :
        task_(task),
        shouldDeleteTask_(shouldDeleteTask),
        timer_(ios),
        interval_(interval),
        isCancelled_(false) {
        attach(this, false);
    }

    virtual ~ScheduledTaskFuture() {
        if (shouldDeleteTask_) {
            delete task_;
        }
    }

public:
    boost::asio::deadline_timer& getTimer() {
        return timer_;
    }

    msec_t getInterval() const {
        return interval_;
    }

public:
    virtual void cancel() override {
        timer_.cancel();
        isCancelled_ = true;
    }

    virtual bool isCancelled() const override {
        return isCancelled_;
    }

private:
    // = FutureObserver overriding
    virtual void update(const Future& /*future*/) override {
        task_->run();
    }

private:
    Runnable* task_;
    bool shouldDeleteTask_;
    boost::asio::deadline_timer timer_;
    msec_t interval_;
    bool isCancelled_;
};


/// 참고: http://stackoverflow.com/questions/8236521/how-to-capture-a-unique-ptr-into-a-lambda-expression
std::function<void()> makeTask(RunnablePtr&& task)
{
    return std::bind(
        [](Runnable* t) {
            RunnablePtr savedTask(t);
            savedTask->run();
        },
        task.release());
}

} // namespace

// = TaskScheduler

SNE_DEFINE_SINGLETON(TaskScheduler);

TaskScheduler::~TaskScheduler()
{
}


void TaskScheduler::schedule(RunnablePtr task)
{
    ioContextTask_.getIoContext().post(makeTask(std::move(task)));
}


void TaskScheduler::schedule(Runnable& task)
{
    ioContextTask_.getIoContext().post([&]() {
        task.run();
    });
}


void TaskScheduler::schedule(std::function<void()> action)
{
    ioContextTask_.getIoContext().post(action);
}

Future::Ref TaskScheduler::scheduleTimedTask(
    Runnable* task, bool shouldDeleteTask, msec_t delay, msec_t interval)
{
    auto& ioContext = ioContextTask_.getIoContext();
    ScheduledTaskFuture::Ref taskFuture(
        std::make_shared<ScheduledTaskFuture>(ioContext, task, shouldDeleteTask, interval));
    ScheduledTaskFuture::schedule(taskFuture, delay);
    return taskFuture;
}

}} // namespace sne { namespace base {
