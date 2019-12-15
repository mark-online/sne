#pragma once

#include <sne/Common.h>
#include "Future.h"
#include "Runnable.h"
#include <sne/core/utility/Singleton.h>
#include <boost/noncopyable.hpp>

namespace sne { namespace base {

class IoContextTask;

/** @addtogroup Concurrent
* @{
*/

typedef uint64_t ScheduledTaskId;
typedef std::unique_ptr<Runnable> RunnablePtr;

/**
* @class LambdaTask
* 람다식을 실행한다
*/
class LambdaTask : public Runnable
{
public:
    LambdaTask(std::function<void()> action) :
        action_(action) {}

private:
    virtual void run() override {
        action_();
    }

private:
    std::function<void()> action_;
};


/**
 * @class TaskScheduler
 * 일정 기반의 작업 스케쥴러
 */
class SNE_API TaskScheduler : public boost::noncopyable
{
    SNE_DECLARE_SINGLETON(TaskScheduler);

public:
    TaskScheduler(IoContextTask& ioContextTask) :
        ioContextTask_(ioContextTask) {}
    ~TaskScheduler();

public:
    /// task를 바로 실행시킨다.
    void schedule(RunnablePtr task);

    void schedule(Runnable& task);

    void schedule(std::function<void()> action);

    /// delay 시간 만큼 대기 후 한번만 호출되는 Task 스케쥴링
    Future::Ref schedule(RunnablePtr task, msec_t delay) {
        return scheduleTimedTask(task.release(), true, delay);
    }

    /// delay 시간 만큼 대기 후 한번만 호출되는 Task 스케쥴링
    Future::Ref schedule(Runnable& task, msec_t delay) {
        return scheduleTimedTask(&task, false, delay);
    }

    /// delay 시간 만큼 대기 후 interval 시간 간격으로 호출되는 Task 스케쥴링
    Future::Ref schedule(RunnablePtr task, msec_t delay, msec_t interval) {
        return scheduleTimedTask(task.release(), true, delay, interval);
    }

    /// delay 시간 만큼 대기 후 interval 시간 간격으로 호출되는 Task 스케쥴링
    Future::Ref schedule(Runnable& task, msec_t delay, msec_t interval) {
        return scheduleTimedTask(&task, false, delay, interval);
    }

private:
    Future::Ref scheduleTimedTask(Runnable* task, bool shouldDeleteTask,
        msec_t delay, msec_t interval = 0);

private:
    IoContextTask& ioContextTask_;
};

/** @} */ // addtogroup Concurrent

}} // namespace sne { namespace base {

#define TASK_SCHEDULER sne::base::TaskScheduler::instance()
