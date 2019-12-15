#include "SrpcTestPCH.h"
#include "DummyRpcEvent.h"
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class RpcEventTest 
*
* RpcEvent test
*/
class RpcEventTest : public test::BitStreamFixture
{
};


TEST_F(RpcEventTest, testCalling)
{
    RInt32(100).serialize(*ostream_);
    RInt32(-100).serialize(*ostream_);

    DummyRpcEvent event;
    event.dispatch(&event, *istream_);

    ASSERT_EQ(100, event.getP1());
    ASSERT_EQ(-100, event.getP2());
}
