#include "BaseTestPCH.h"
#include <sne/base/memory/ObjectPool.h>

using namespace sne::base;

/**
* @class IntegerAllocator
*/
class IntegerAllocator
{
public:
    int* malloc() {
        return new int(1);
    }

    void free(int* resource) {
        delete resource;
    }
};


static const size_t integerPoolSize = 2;

/**
* @class ObjectPoolTest
*
* ObjectPool Test
*/
class ObjectPoolTest : public testing::Test
{
private:
    virtual void SetUp() override {
        pool_ = new InttegerPool(integerPoolSize, allocator_);
        pool_->initialize();
    }

    virtual void TearDown() override {
        delete pool_;
    }

protected:
    IntegerAllocator allocator_;
    typedef ObjectPool<int, IntegerAllocator> InttegerPool;
    InttegerPool* pool_;
};


TEST_F(ObjectPoolTest, testInitialize)
{
    ASSERT_EQ(0, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(integerPoolSize, pool_->getInactiveResourceCount());
}


TEST_F(ObjectPoolTest, testAcquire)
{
    int* resource = pool_->acquire();
    ASSERT_TRUE(0 != resource);

    ASSERT_EQ(1, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(integerPoolSize - 1, pool_->getInactiveResourceCount());
}


TEST_F(ObjectPoolTest, testRelease)
{
    int* resource = pool_->acquire();

    pool_->release(resource);
    ASSERT_EQ(0, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(integerPoolSize, pool_->getInactiveResourceCount());
}


TEST_F(ObjectPoolTest, testMassAcquireAndRelease)
{
    const size_t acquireCount = 10;
    int* resources[acquireCount];

    for (size_t i = 0; i < acquireCount; ++i) {
        resources[i] = pool_->acquire();
    }

    ASSERT_EQ(acquireCount, pool_->getActiveResourceCount());
    ASSERT_EQ(0, int(pool_->getInactiveResourceCount()));

    for (int i = acquireCount - 1; i >= 0; --i) {
        pool_->release(resources[i]);
    }

    ASSERT_EQ(0, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(10, int(pool_->getInactiveResourceCount()));
}


TEST_F(ObjectPoolTest, testDestroy)
{
    for (int i = 0; i < 10; ++i) {
        (void)pool_->acquire();
    }

    pool_->destroy();

    ASSERT_EQ(0, int(pool_->getActiveResourceCount()));
    ASSERT_EQ(0, int(pool_->getInactiveResourceCount()));
}

