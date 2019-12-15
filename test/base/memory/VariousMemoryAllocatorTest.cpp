#include "BaseTestPCH.h"
#include <sne/base/memory/VariousMemoryAllocator.h>
#include <mutex>

using namespace sne::base;

static const size_t poolSize = 2;
static const size_t blockSize = sizeof(int);

/**
* @class VariousMemoryAllocatorTest
*
* VariousMemoryAllocator Test
*/
class VariousMemoryAllocatorTest : public testing::Test
{
protected:
    typedef VariousMemoryAllocator<std::mutex> IntegerAllocator;

private:
    virtual void SetUp() override {
        allocator_ = new IntegerAllocator(poolSize, blockSize);
    }

    virtual void TearDown() override {
        delete allocator_;
    }

protected:
    IntegerAllocator* allocator_;
};


TEST_F(VariousMemoryAllocatorTest, testInitialize)
{
    ASSERT_EQ(poolSize, allocator_->getCachedMemoryCount());
}


TEST_F(VariousMemoryAllocatorTest, testMalloc)
{
    void* memory = allocator_->malloc(blockSize);
    ASSERT_TRUE(0 != memory);

    ASSERT_EQ(poolSize - 1, allocator_->getCachedMemoryCount());
}


TEST_F(VariousMemoryAllocatorTest, testCalloc)
{
    void* memory = allocator_->calloc(blockSize);
    ASSERT_TRUE(0 != memory);

    ASSERT_EQ(poolSize - 1, allocator_->getCachedMemoryCount());
}


TEST_F(VariousMemoryAllocatorTest, testMallocBigger)
{
    void* memory = allocator_->malloc(blockSize * 2);
    ASSERT_TRUE(0 != memory);

    ASSERT_EQ(poolSize, allocator_->getCachedMemoryCount());
}


TEST_F(VariousMemoryAllocatorTest, testMallocSmaller)
{
    void* memory = allocator_->malloc(blockSize / 2);
    ASSERT_TRUE(0 != memory);

    ASSERT_EQ(poolSize - 1, allocator_->getCachedMemoryCount());
}


TEST_F(VariousMemoryAllocatorTest, testFree)
{
    allocator_->free(allocator_->malloc(blockSize));

    ASSERT_EQ(poolSize, allocator_->getCachedMemoryCount());
}


TEST_F(VariousMemoryAllocatorTest, testMultipleVariousMemoryMallocFree)
{
    const int count = 1000;
    void* memories[count];
    for (int i = 0; i < count; ++i) {
        memories[i] = allocator_->malloc((count % blockSize) + 2);
    }
    ASSERT_EQ(0, int(allocator_->getCachedMemoryCount()));

    for (int i = 0; i < count; ++i) {
        allocator_->free(memories[i]);
    }
    ASSERT_EQ(poolSize + count - 2, allocator_->getCachedMemoryCount());
}
