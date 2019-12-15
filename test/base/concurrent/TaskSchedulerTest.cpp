#include "BaseTestPCH.h"
#include <sne/base/concurrent/TaskScheduler.h>
#include <sne/base/concurrent/Future.h>
#include <sne/base/io_context/IoContextTask.h>
#include <chrono>
#include <thread>

using namespace sne;
using namespace sne::base;

namespace {

int s_callCount = 0;

/**
 * @class DummyTask
 */
class DummyTask : public Runnable
{
private:
    virtual void run() {
        ++s_callCount;
    }
};

} // namespace

/**
* @class ThreadPoolTaskSchedulerTest
*
* MemoryPool Test
*/
class TaskSchedulerTest : public testing::Test
{
    enum {
        threadCount = 2
    };

public:
    TaskSchedulerTest() :
        ioContextTask_("TaskSchedulerTest") {}

private:
    virtual void SetUp() override {
        s_callCount = 0;

        ASSERT_TRUE(ioContextTask_.start(1));

        scheduler_ = new TaskScheduler(ioContextTask_);
    }

    virtual void TearDown() override {
        if (scheduler_ != nullptr) {
            ioContextTask_.stop();
            ioContextTask_.waitForDone();
            delete scheduler_;
        }
    }

protected:
    void pause(msec_t msec) {
        std::this_thread::sleep_for(std::chrono::milliseconds(msec));
    }

protected:
    IoContextTask ioContextTask_;
    TaskScheduler* scheduler_ = nullptr;
};


TEST_F(TaskSchedulerTest, testSchedule)
{
    scheduler_->schedule(std::make_unique<DummyTask>());
    pause(15);

    ASSERT_EQ(1, s_callCount);
}


TEST_F(TaskSchedulerTest, testScheduleOnce)
{
    Future::Ref future = scheduler_->schedule(std::make_unique<DummyTask>(), 1);
    ASSERT_TRUE(future.get() != nullptr);
    pause(15);

    ASSERT_EQ(1, s_callCount);
}


TEST_F(TaskSchedulerTest, testScheduleAtFixedRate)
{
    Future::Ref future = scheduler_->schedule(std::make_unique<DummyTask>(), 1, 1);
    ASSERT_TRUE(future.get() != nullptr);
    pause(15);

    ASSERT_TRUE(1 < s_callCount);
}


TEST_F(TaskSchedulerTest, testCancelScheduledTask)
{
    Future::Ref future = scheduler_->schedule(std::make_unique<DummyTask>(), 10);
    ASSERT_TRUE(future.get() != nullptr);
    ASSERT_EQ(false, future->isCancelled());
    future->cancel();
    pause(10);

    ASSERT_EQ(true, future->isCancelled());
    ASSERT_EQ(0, s_callCount);
}


TEST_F(TaskSchedulerTest, testCancelFixedRateScheduledTask)
{
    Future::Ref future = scheduler_->schedule(std::make_unique<DummyTask>(), 1, 1);
    ASSERT_TRUE(future.get() != nullptr);
    pause(5);
    future->cancel();
    const int callCount = s_callCount;
    pause(10);

    ASSERT_EQ(true, future->isCancelled());
    ASSERT_EQ(callCount, s_callCount);
}
