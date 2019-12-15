#include "BaseTestPCH.h"
#include <sne/base/memory/MemoryPool.h>

using namespace sne::base;

static const size_t memorySize = 128;
static const size_t memoryPoolSize = 2;

/**
* @class MemoryPoolTest
*
* MemoryPool Test
*/
class MemoryPoolTest : public testing::Test
{
    typedef MemoryPool<memorySize> TestingPool;
private:
    virtual void SetUp() override {
        pool_ = new TestingPool(memoryPoolSize, allocator_);
        pool_->initialize();
    }

    virtual void TearDown() override {
        delete pool_;
    }

protected:

    MemoryAllocator<memorySize> allocator_;
    TestingPool* pool_;
};


TEST_F(MemoryPoolTest, testInitialize)
{
    ASSERT_EQ(0, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(memoryPoolSize, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testAcquire)
{
    void* resource = pool_->acquire();
    ASSERT_TRUE(0 != resource);

    ASSERT_EQ(1, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(memoryPoolSize - 1, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testRelease)
{
    void* resource = pool_->acquire();

    pool_->release(resource);
    ASSERT_EQ(0, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(memoryPoolSize, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testMassAcquireAndRelease)
{
    const int acquireCount = 10;
    void* resources[acquireCount];

    for (int i = 0; i < acquireCount; ++i) {
        resources[i] = pool_->acquire();
    }

    ASSERT_EQ(acquireCount, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(0, int(pool_->getInactiveResourceCount()));

    for (int i = acquireCount - 1; i >= 0; --i) {
        pool_->release(resources[i]);
    }

    ASSERT_EQ(0, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(10, int(pool_->getInactiveResourceCount()));
}


TEST_F(MemoryPoolTest, testDestroy)
{
    for (int i = 0; i < 10; ++i) {
        (void)pool_->acquire();
    }

    pool_->destroy();

    ASSERT_EQ(0, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(0, int(pool_->getInactiveResourceCount()));
}

