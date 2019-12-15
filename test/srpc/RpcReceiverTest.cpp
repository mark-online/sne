#include "SrpcTestPCH.h"
#include "DummyRpcServer.h"
#include "MockRpcNetwork.h"
#include <sne/srpc/detail/ForwardingFunctors.h>
#include <sne/srpc/detail/RpcId.h>
#include <sne/srpc/Exception.h>
#include <sne/core/stream/detail/IBitStream.h>
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class RpcReceiverTest
*
* RpcReceiver test
*/
class RpcReceiverTest : public test::BitStreamFixture
{
private:
    virtual void SetUp() override {
        test::BitStreamFixture::SetUp();

        rpcNetwork_ = new MockRpcNetwork;
        response_ = new DummyRpcServer(rpcNetwork_);
    }

    virtual void TearDown() override {
        delete rpcNetwork_;
        delete response_;
    }

protected:
    MockRpcNetwork* rpcNetwork_;
    DummyRpcServer* response_;
};


TEST_F(RpcReceiverTest, testParm0)
{
    RRpcId rpcId("DummyRpc_rpc0_0");
    rpcId.serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(0, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm1)
{
    RRpcId rpcId("DummyRpc_rpc1_1");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(1));
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(1, response_->getP1());
    ASSERT_EQ(1, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm2)
{
    RRpcId rpcId("DummyRpc_rpc2_2");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(2));
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(1, response_->getP1());
    ASSERT_EQ(2, response_->getP2());
    ASSERT_EQ(2, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm3)
{
    RRpcId rpcId("DummyRpc_rpc3_3");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(3));
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(1, response_->getP1());
    ASSERT_EQ(2, response_->getP2());
    ASSERT_EQ(3, response_->getP3());
    ASSERT_EQ(3, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm4)
{
    RRpcId rpcId("DummyRpc_rpc4_4");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);
    RInt32(4).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(4));
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(1, response_->getP1());
    ASSERT_EQ(2, response_->getP2());
    ASSERT_EQ(3, response_->getP3());
    ASSERT_EQ(4, response_->getP4());
    ASSERT_EQ(4, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm5)
{
    RRpcId rpcId("DummyRpc_rpc5_5");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);
    RInt32(4).serialize(*ostream_);
    RInt32(5).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(5));
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(1, response_->getP1());
    ASSERT_EQ(2, response_->getP2());
    ASSERT_EQ(3, response_->getP3());
    ASSERT_EQ(4, response_->getP4());
    ASSERT_EQ(5, response_->getP5());
    ASSERT_EQ(5, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm6)
{
    RRpcId rpcId("DummyRpc_rpc6_6");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);
    RInt32(4).serialize(*ostream_);
    RInt32(5).serialize(*ostream_);
    RInt32(6).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(6));
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(1, response_->getP1());
    ASSERT_EQ(2, response_->getP2());
    ASSERT_EQ(3, response_->getP3());
    ASSERT_EQ(4, response_->getP4());
    ASSERT_EQ(5, response_->getP5());
    ASSERT_EQ(6, response_->getP6());
    ASSERT_EQ(6, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm7)
{
    RRpcId rpcId("DummyRpc_rpc7_7");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);
    RInt32(4).serialize(*ostream_);
    RInt32(5).serialize(*ostream_);
    RInt32(6).serialize(*ostream_);
    RInt32(7).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(7));
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(1, response_->getP1());
    ASSERT_EQ(2, response_->getP2());
    ASSERT_EQ(3, response_->getP3());
    ASSERT_EQ(4, response_->getP4());
    ASSERT_EQ(5, response_->getP5());
    ASSERT_EQ(6, response_->getP6());
    ASSERT_EQ(7, response_->getP7());
    ASSERT_EQ(7, response_->getHint());
}


TEST_F(RpcReceiverTest, testParmBits)
{
    RRpcId rpcId("DummyRpc_rpcBits_2");
    rpcId.serialize(*ostream_);
    RInt15(1).serialize(*ostream_);
    RInt31(2).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_);
    ASSERT_EQ(rpcId.get(), response_->getRpcId().get());
    ASSERT_EQ(1, response_->getP1());
    ASSERT_EQ(2, response_->getP2());
}


TEST_F(RpcReceiverTest, testAlign)
{
    RRpcId rpcId("DummyRpc_rpcBits_2");

    for (int i = 0; i < 3; ++i) {
        rpcId.serialize(*ostream_);
        RInt15(1).serialize(*ostream_);
        RInt31(-1).serialize(*ostream_);
        ostream_->align();
    }

    for (int i = 0; i < 3; ++i) {
        rpcNetwork_->onReceive(*istream_);
        ASSERT_EQ(rpcId.get(), response_->getRpcId().get()) <<
            "#" << i << " - rpcId";
        ASSERT_EQ(1, response_->getP1()) << "#" << i << " - p1";
        ASSERT_EQ(-1, response_->getP2()) << "#" << i << " - p2";;
        ASSERT_EQ(0, static_cast<core::IBitStream&>(*istream_).getReadBitCount()) <<
            "#" << i << " - read bit count";
    }
    ASSERT_EQ(0, istream_->getTotalBitCount());
}


TEST_F(RpcReceiverTest, testUnmarshalingError)
{
    ASSERT_THROW(rpcNetwork_->onReceive(*istream_), core::StreamingException);
}


TEST_F(RpcReceiverTest, testUnknownRpcMethod)
{
    RRpcId rpcId("DummyRpc_rpc0_unknown");
    rpcId.serialize(*ostream_);

    ASSERT_THROW(rpcNetwork_->onReceive(*istream_), UnknownRpcMethodException);
}


TEST_F(RpcReceiverTest, testRpcFailed)
{
    RRpcId rpcId("DummyRpc_rpcFailed_0");
    rpcId.serialize(*ostream_);

    ASSERT_THROW(rpcNetwork_->onReceive(*istream_), RpcFailedException);
}


TEST_F(RpcReceiverTest, testRpcPrototype)
{
    RRpcId rpcId("DummyRpc_rpc0_0");
    rpcId.serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    ASSERT_EQ(std::string("rpc0"),
        std::string(response_->getRpcId().getMethodName()));
}


TEST_F(RpcReceiverTest, testOnReceiving)
{
    RRpcId rpcId("DummyRpc_rpc0_0");
    rpcId.serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    ASSERT_EQ(std::string("rpc0"),
        std::string(response_->getLastRpcId().getMethodName()));
}
