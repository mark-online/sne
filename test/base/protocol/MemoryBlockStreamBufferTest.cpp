#include "BaseTestPCH.h"
#include <sne/base/protocol/MemoryBlockStreamBuffer.h>

using namespace sne;
using namespace sne::base;

static const size_t initialSpace = 10;

/**
* @class MemoryBlockStreamBufferTest
*
* MemoryBlock based stream buffer Test
*/
class MemoryBlockStreamBufferTest : public testing::Test
{
private:
    virtual void SetUp() override {
        mblock_ = new MemoryBlock(initialSpace);
        buffer_ = new MemoryBlockStreamBuffer(mblock_);
    }

    virtual void TearDown() override {
        delete buffer_;
        delete mblock_;
    }

protected:
    MemoryBlock* mblock_;
    MemoryBlockStreamBuffer* buffer_;
};


TEST_F(MemoryBlockStreamBufferTest, testEmpty)
{
    ASSERT_TRUE(buffer_->empty());
    ASSERT_EQ(0, int(buffer_->size()));
}


TEST_F(MemoryBlockStreamBufferTest, testPush)
{
    buffer_->push(1);
    ASSERT_EQ(1, int(buffer_->size()));
    buffer_->push(2);
    ASSERT_EQ(2, int(buffer_->size()));

    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MemoryBlockStreamBufferTest, testRead)
{
    buffer_->push(1);
    ASSERT_EQ(1, buffer_->back());
    buffer_->push(2);
    ASSERT_EQ(2, buffer_->back());
    ASSERT_EQ(1, buffer_->front());

    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MemoryBlockStreamBufferTest, testPop)
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->pop();
    ASSERT_EQ(2, buffer_->front());
    buffer_->pop();
    ASSERT_EQ(true, buffer_->empty());

    buffer_->push(3);
    ASSERT_EQ(1, int(buffer_->size()));

    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MemoryBlockStreamBufferTest, testReset)
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->reset();
    ASSERT_TRUE(buffer_->empty());
    ASSERT_EQ(0, int(buffer_->size()));

    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MemoryBlockStreamBufferTest, testSpace)
{
    ASSERT_EQ(initialSpace, buffer_->space());
    buffer_->push(1);
    buffer_->push(2);
    ASSERT_EQ(initialSpace - 2, buffer_->space());

    buffer_->pop();
    buffer_->pop();
    ASSERT_EQ(initialSpace, buffer_->space());

    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MemoryBlockStreamBufferTest, testExtendedSpace)
{
    ASSERT_EQ(initialSpace, buffer_->space()) << "empty";

    for (size_t i = 0; i < (initialSpace * 2); ++i) {
        buffer_->push(1);
    }
    ASSERT_EQ(0, int(buffer_->space()));

    buffer_->reset();
    ASSERT_EQ(initialSpace * 2, buffer_->space()) << "fully empty";

    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MemoryBlockStreamBufferTest, testCopyFrom)
{
    const int32_t value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const core::StreamBuffer::Item*>(valuePtr),
        sizeof(value));
    ASSERT_EQ(sizeof(value), buffer_->size());

    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MemoryBlockStreamBufferTest, testCopyTo)
{
    const int32_t value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const core::StreamBuffer::Item*>(valuePtr),
        sizeof(value));

    int32_t to = 0;
    void* toPtr = &to;
    buffer_->copyTo(static_cast<core::StreamBuffer::Item*>(toPtr),
        sizeof(to));
    ASSERT_TRUE(buffer_->empty());
    ASSERT_EQ(0, int(buffer_->size()));
    ASSERT_EQ(value, to);

    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MemoryBlockStreamBufferTest, testCrunch)
{
    const int elements = 20;
    for (int i = 0; i < elements; ++i) {
        buffer_->push(char(i));
    }
    ASSERT_NE(mblock_->space(), mblock_->capacity());

    for (int i = 0; i < elements; ++i) {
        buffer_->pop();
    }
    ASSERT_EQ(mblock_->space(), mblock_->capacity());
}


//TEST_F(MemoryBlockStreamBufferTest, testResetWithNewBlock)
//{
//    buffer_->push(1);
//    ASSERT_EQ(1, int(buffer_->size()));
//
//    AceMemoryBlockGuard second(new MemoryBlock(initialSpace));
//    buffer_->reset(*second);
//    ASSERT_EQ(0, int(buffer_->size()));
//
//    buffer_->push(2);
//    ASSERT_EQ(1, int(buffer_->size()));
//
//    ASSERT_TRUE(static_cast<MemoryBlockStreamBuffer*>(buffer_)->isValid());
//}
