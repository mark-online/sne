#include "SrpcTestPCH.h"
#include "DummyRpcClient.h"
#include "MockRpcNetwork.h"
#include <sne/srpc/detail/ForwardingFunctors.h>
#include <sne/srpc/detail/RpcId.h>
#include <sne/core/stream/detail/SneStreamFactory.h>
#include <sne/core/stream/detail/OBitStream.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class RpcForwarderTest
*
* RpcForwarder test
*/
class RpcForwarderTest  : public testing::Test
{
private:
    virtual void SetUp() override {
        rpcNetwork_ = new MockRpcNetwork;
        request_ = new DummyRpcClient(rpcNetwork_);
        istream_ =
            core::SneStreamFactory(true, true).createIStream(
                rpcNetwork_->getStreamBuffer()).release();
    }

    virtual void TearDown() override {
        delete request_;
        delete rpcNetwork_;
        delete istream_;
    }

protected:
    MockRpcNetwork* rpcNetwork_;
    DummyRpc* request_;
    core::IStream* istream_;
};


TEST_F(RpcForwarderTest, testHint)
{
    request_->rpc0(reinterpret_cast<const void*>(12345));

    ASSERT_EQ(12345, rpcNetwork_->getSentRpcHint());
}


TEST_F(RpcForwarderTest, testParm0)
{
    request_->rpc0(reinterpret_cast<const void*>(12345));

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpc0_0"), id);
    ASSERT_EQ(12345, rpcNetwork_->getSentRpcHint());
}


TEST_F(RpcForwarderTest, testParm1)
{
    request_->rpc1(1);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpc1_1"), id);
    int32_t p1;
    istream_->read(p1);
    ASSERT_EQ(1, p1);
}


TEST_F(RpcForwarderTest, testParm2)
{
    request_->rpc2(1, 2);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpc2_2"), id);
    int32_t p1;
    istream_->read(p1);
    ASSERT_EQ(1, p1);
    int32_t p2;
    istream_->read(p2);
    ASSERT_EQ(2, p2);
}


TEST_F(RpcForwarderTest, testParm3)
{
    request_->rpc3(1, 2, 3);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpc3_3"), id);
    int32_t p1;
    istream_->read(p1);
    ASSERT_EQ(1, p1);
    int32_t p2;
    istream_->read(p2);
    ASSERT_EQ(2, p2);
    int32_t p3;
    istream_->read(p3);
    ASSERT_EQ(3, p3);
}


TEST_F(RpcForwarderTest, testParm4)
{
    request_->rpc4(1, 2, 3, 4);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpc4_4"), id);
    int32_t p1;
    istream_->read(p1);
    ASSERT_EQ(1, p1);
    int32_t p2;
    istream_->read(p2);
    ASSERT_EQ(2, p2);
    int32_t p3;
    istream_->read(p3);
    ASSERT_EQ(3, p3);
    int32_t p4;
    istream_->read(p4);
    ASSERT_EQ(4, p4);
}


TEST_F(RpcForwarderTest, testParm5)
{
    request_->rpc5(1, 2, 3, 4, 5);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpc5_5"), id);
    int32_t p1;
    istream_->read(p1);
    ASSERT_EQ(1, p1);
    int32_t p2;
    istream_->read(p2);
    ASSERT_EQ(2, p2);
    int32_t p3;
    istream_->read(p3);
    ASSERT_EQ(3, p3);
    int32_t p4;
    istream_->read(p4);
    ASSERT_EQ(4, p4);
    int32_t p5;
    istream_->read(p5);
    ASSERT_EQ(5, p5);
}


TEST_F(RpcForwarderTest, testParm6)
{
    request_->rpc6(1, 2, 3, 4, 5, 6);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpc6_6"), id);
    int32_t p1;
    istream_->read(p1);
    ASSERT_EQ(1, p1);
    int32_t p2;
    istream_->read(p2);
    ASSERT_EQ(2, p2);
    int32_t p3;
    istream_->read(p3);
    ASSERT_EQ(3, p3);
    int32_t p4;
    istream_->read(p4);
    ASSERT_EQ(4, p4);
    int32_t p5;
    istream_->read(p5);
    ASSERT_EQ(5, p5);
    int32_t p6;
    istream_->read(p6);
    ASSERT_EQ(6, p6);
}


TEST_F(RpcForwarderTest, testParm7)
{
    request_->rpc7(1, 2, 3, 4, 5, 6, 7);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpc7_7"), id);
    int32_t p1;
    istream_->read(p1);
    ASSERT_EQ(1, p1);
    int32_t p2;
    istream_->read(p2);
    ASSERT_EQ(2, p2);
    int32_t p3;
    istream_->read(p3);
    ASSERT_EQ(3, p3);
    int32_t p4;
    istream_->read(p4);
    ASSERT_EQ(4, p4);
    int32_t p5;
    istream_->read(p5);
    ASSERT_EQ(5, p5);
    int32_t p6;
    istream_->read(p6);
    ASSERT_EQ(6, p6);
    int32_t p7;
    istream_->read(p7);
    ASSERT_EQ(7, p7);
}


TEST_F(RpcForwarderTest, testParmBits)
{
    request_->rpcBits(1, 2);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(RRpcId("DummyRpc_rpcBits_2"), id);
    int32_t p1;
    istream_->read(p1, 15);
    ASSERT_EQ(1, p1);
    int32_t p2;
    istream_->read(p2, 31);
    ASSERT_EQ(2, p2);

    core::OStream& ostream = rpcNetwork_->getOutputStream();
    ASSERT_EQ(0, static_cast<core::OBitStream&>(ostream).getHoldingBitCount());
    ASSERT_EQ(core::toBytes(32 + 15 + 31) * CHAR_BIT, ostream.getTotalBitCount());
}


TEST_F(RpcForwarderTest, testMarshalingError)
{
    ASSERT_FALSE(rpcNetwork_->sendFailed());

    rpcNetwork_->setMarshalingError();
    request_->rpc0();

    ASSERT_TRUE(rpcNetwork_->sendFailed());
}


TEST_F(RpcForwarderTest, testOnForwarding)
{
    request_->rpc6(1, 2, 3, 4, 5, 6);

    DummyRpcClient* client = static_cast<DummyRpcClient*>(request_);

    RRpcId id;
    id.serialize(*istream_);
    ASSERT_EQ(id.get(), client->getLastRpcId().get());

    ASSERT_EQ(std::string("rpc6"),
        std::string(client->getLastRpcId().getMethodName()));
}
