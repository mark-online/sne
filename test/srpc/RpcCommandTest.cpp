#include "SrpcTestPCH.h"
#include "DummyRpcCommand.h"
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class RpcCommandTest 
*
* RpcCommand test
*/
class RpcCommandTest : public test::BitStreamFixture
{
};


TEST_F(RpcCommandTest, testMarshal)
{
    DummyRpcCommand rpcCommand(100, -100);
    rpcCommand.marshal(*ostream_);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(rpcCommand.getRpcId().get(), id.get());
    int32_t p1;
    istream_->read(p1);
    ASSERT_EQ(100, p1);
    int32_t p2;
    istream_->read(p2);
    ASSERT_EQ(-100, p2);
}
