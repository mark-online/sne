#include "SrpcTestPCH.h"
#include <sne/srpc/detail/RpcId.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class RpcIdTest 
* RRpcId test
*/
class RpcIdTest : public testing::Test
{
};


TEST_F(RpcIdTest, testRpcId)
{
    ASSERT_EQ(RRpcId("abc"), RRpcId("abc"));
    ASSERT_NE(RRpcId("a"), RRpcId("b"));
}
