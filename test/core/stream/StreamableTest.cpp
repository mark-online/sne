#include "CoreTestPCH.h"
#include <sne/test/StreamFixture.h>
#include <sne/core/stream/Streamable.h>

using namespace sne;
using namespace sne::core;

struct DummyData : public core::Streamable
{
    std::string s_;
    int32_t i32_;

    virtual void serialize(core::Stream& stream) override {
        stream & s_ & i32_;
    }
};

/**
* @class StreamableTest
*
* Streamable Test
*/
class StreamableTest : public test::ByteStreamFixture
{
};


TEST_F(StreamableTest, testOutput)
{
    DummyData inputData;
    inputData.s_ = "abc";
    inputData.i32_ = -1;
    inputData.serialize(*ostream_);

    DummyData outputData;
    outputData.serialize(*istream_);

    ASSERT_EQ(inputData.s_, outputData.s_);
    ASSERT_EQ(inputData.i32_, outputData.i32_);
}
