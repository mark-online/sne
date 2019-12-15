#include "BaseTestPCH.h"
#include <sne/base/memory/MemoryBlockManager.h>

using namespace sne::base;

/**
 * @class TestMemoryBlockManager
 */
class TestMemoryBlockManager : public MemoryBlockManager
{
public:
    TestMemoryBlockManager(size_t poolSize) :
        MemoryBlockManager(poolSize, 10) {}

    const MemoryBlockPool& getMemoryBlockPool() const {
        return memoryBlockPool_;
    }
};

static const size_t testPoolSize = 2;


/**
* @class MemoryBlockManagerTest
*
* MemoryBlockManager Test
*/
class MemoryBlockManagerTest : public testing::Test
{
private:
    virtual void SetUp() override {
        manager_ = new TestMemoryBlockManager(testPoolSize);
    }

    virtual void TearDown() override {
        delete manager_;
    }

protected:

    TestMemoryBlockManager* manager_;
};


TEST_F(MemoryBlockManagerTest, testInitialize)
{
    ASSERT_LE(testPoolSize,
        manager_->getMemoryBlockPool().getInactiveResourceCount());
}


TEST_F(MemoryBlockManagerTest, testAcquire)
{
    MemoryBlock* mblock = manager_->acquire();
    ASSERT_TRUE(0 != mblock);

    ASSERT_EQ(size_t(1),
        manager_->getMemoryBlockPool().getActiveResourceCount());
}


TEST_F(MemoryBlockManagerTest, testRelease)
{
    manager_->acquire()->release();

    ASSERT_EQ(size_t(0),
        manager_->getMemoryBlockPool().getActiveResourceCount());
}


//TEST_F(MemoryBlockManagerTest, testDuplicate)
//{
//    MemoryBlock* mblock = manager_->acquire();
//    MemoryBlock* duplicated = mblock->duplicate();
//    ASSERT_TRUE(nullptr != duplicated);
//
//    mblock->release();
//    duplicated->release();
//
//    ASSERT_EQ(0,
//        manager_->getMemoryBlockPool().getActiveResourceCount());
//}


//TEST_F(MemoryBlockManagerTest, testClone)
//{
//    MemoryBlock* mblock = manager_->acquire();
//    MemoryBlock* cloned = mblock->clone();
//    ASSERT_TRUE(nullptr != cloned);
//
//    ASSERT_EQ(2,
//        manager_->getMemoryBlockPool().getActiveResourceCount());
//
//    mblock->release();
//    cloned->release();
//
//    ASSERT_EQ(0,
//        manager_->getMemoryBlockPool().getActiveResourceCount());
//}
