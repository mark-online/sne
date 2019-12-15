#include "CoreTestPCH.h"
#include <sne/core/stream/detail/VectorStreamBuffer.h>

using namespace sne;
using namespace sne::core;

const size_t space_ = 10;

/**
* @class VectorStreamBufferTest
*
* std::vector cored stream buffer Test
*/
class VectorStreamBufferTest : public testing::Test
{
private:
    virtual void SetUp() override {
        buffer_ = new VectorStreamBuffer(space_);
    }

    virtual void TearDown() override {
        delete buffer_;
    }

protected:
    StreamBuffer* buffer_;
};


TEST_F(VectorStreamBufferTest, testEmpty)
{
    ASSERT_TRUE(buffer_->empty()) << "empty";
    ASSERT_EQ(0, static_cast<int>(buffer_->size())) << "0 size";
}


TEST_F(VectorStreamBufferTest, testPush)
{
    buffer_->push(1);
    ASSERT_EQ(1, static_cast<int>(buffer_->size())) << "1 size";
    buffer_->push(2);
    ASSERT_EQ(2, static_cast<int>(buffer_->size())) << "2 size";
}


TEST_F(VectorStreamBufferTest, testRead)
{
    buffer_->push(1);
    ASSERT_EQ(1, buffer_->back()) << "back(size 1)";
    buffer_->push(2);
    ASSERT_EQ(2, buffer_->back()) << "back(size 2)";
    ASSERT_EQ(1, buffer_->front()) << "front";
}


TEST_F(VectorStreamBufferTest, testPop)
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->pop();
    ASSERT_EQ(2, buffer_->front()) << "front";
    buffer_->pop();
    ASSERT_EQ(true, buffer_->empty()) << "empty";

    buffer_->push(3);
    ASSERT_EQ(1, static_cast<int>(buffer_->size())) << "1 size";
}


TEST_F(VectorStreamBufferTest, testReset)
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->reset();
    ASSERT_TRUE(buffer_->empty());
    ASSERT_EQ(0, static_cast<int>(buffer_->size()));

    ASSERT_TRUE(static_cast<VectorStreamBuffer*>(buffer_)->isValid()) <<
        "must be valid";
}


TEST_F(VectorStreamBufferTest, testSpace)
{
    ASSERT_EQ(space_, buffer_->space());
    buffer_->push(1);
    buffer_->push(2);
    ASSERT_EQ(space_ - 2, buffer_->space());

    buffer_->pop();
    buffer_->pop();
    ASSERT_EQ(space_, buffer_->space());
}


TEST_F(VectorStreamBufferTest, testExtendedSpace)
{
    ASSERT_EQ(space_, buffer_->space());

    for (size_t i = 0; i < (space_ * 2); ++i) {
        buffer_->push(1);
    }
    ASSERT_EQ(0, static_cast<int>(buffer_->space()));

    buffer_->reset();
    ASSERT_EQ(space_ * 2, buffer_->space());
}


TEST_F(VectorStreamBufferTest, testCopyFrom)
{
    const int32_t value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const StreamBuffer::Item*>(valuePtr),
        sizeof(value));
    ASSERT_EQ(sizeof(value), buffer_->size());
}


TEST_F(VectorStreamBufferTest, testCopyTo)
{
    const int32_t value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const StreamBuffer::Item*>(valuePtr),
        sizeof(value));

    int32_t to = 0;
    void* toPtr = &to;
    buffer_->copyTo(static_cast<StreamBuffer::Item*>(toPtr),
        sizeof(to));
    ASSERT_TRUE(buffer_->empty());
    ASSERT_EQ(0, static_cast<int>(buffer_->size()));
    ASSERT_EQ(value, to);
}
