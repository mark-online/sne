#include "SrpcTestPCH.h"
#include "DummyRpcServer.h"
#include "MockRpcNetwork.h"
#include <sne/srpc/detail/ForwardingFunctors.h>
#include <sne/srpc/detail/RpcId.h>
#include <sne/srpc/Exception.h>
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class MultipleRpcReceiverTest
*
* RpcReceiver test
*/
class MultipleRpcReceiverTest : public test::BitStreamFixture
{
private:
    virtual void SetUp() override {
        test::BitStreamFixture::SetUp();

        rpcNetwork_ = new MockRpcNetwork;
        response_ = new DummyRpcServer(rpcNetwork_);
        response2_ = new DummyRpcServer2(rpcNetwork_);
    }

    virtual void TearDown() override {
        delete rpcNetwork_;
        delete response_;
        delete response2_;

        test::BitStreamFixture::TearDown();
    }

protected:
    MockRpcNetwork* rpcNetwork_;
    DummyRpcServer* response_;
    DummyRpcServer2* response2_;
};


TEST_F(MultipleRpcReceiverTest, testDummyRpcParm1)
{
    RRpcId rpcId("DummyRpc_rpc1_1");
    rpcId.serialize(*ostream_);
    RInt32(-1).serialize(*ostream_);
    ostream_->align();
    rpcNetwork_->onReceive(*istream_);
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(-1, response_->getP1());
}


TEST_F(MultipleRpcReceiverTest, testDummyRpc2Parm1)
{
    RRpcId rpcId("DummyRpc2_rpc1_1");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    ostream_->align();
    rpcNetwork_->onReceive(*istream_);
    ASSERT_EQ(rpcId.get(), response2_->getRpcId());
    ASSERT_EQ(1, response2_->getP1());
}


TEST_F(MultipleRpcReceiverTest, testUnknownRpcMethod)
{
    RRpcId rpcId("DummyRpc_rpc1_unknown");
    rpcId.serialize(*ostream_);
    RInt32(-1).serialize(*ostream_);
    ostream_->align();

    ASSERT_THROW(
        rpcNetwork_->onReceive(*istream_), UnknownRpcMethodException);
}
