#include "SrpcTestPCH.h"
#include <sne/srpc/RpcTypes.h>
#include <sne/srpc/detail/ReceivingFunctors.h>
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class ReceivingFunctorTest
*
* marshaling functor test
*/
class ReceivingFunctorTest : public test::BitStreamFixture
{
private:
    virtual void SetUp() override {
        test::BitStreamFixture::SetUp();

        for (int i = 0; i < 10; ++i) {
            RInt32 value(i);
            value.serialize(*ostream_);
        }
    }

public:
    void handler0(const void* /*rpcHint*/) {
        callee_ = "handler0";
    }
    void handler1(const RInt32& p1, const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        callee_ = "handler1";
    }
    void handler2(const RInt32& p1, const RInt32& p2,
        const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        callee_ = "handler2";
    }
    void handler3(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        ASSERT_EQ(2, p3.get());
        callee_ = "handler3";
    }
    void handler4(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        ASSERT_EQ(2, p3.get());
        ASSERT_EQ(3, p4.get());
        callee_ = "handler4";
    }
    void handler5(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        ASSERT_EQ(2, p3.get());
        ASSERT_EQ(3, p4.get());
        ASSERT_EQ(4, p5.get());
        callee_ = "handler5";
    }
    void handler6(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        ASSERT_EQ(2, p3.get());
        ASSERT_EQ(3, p4.get());
        ASSERT_EQ(4, p5.get());
        ASSERT_EQ(5, p6.get());
        callee_ = "handler6";
    }
    void handler7(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const RInt32& p7, const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        ASSERT_EQ(2, p3.get());
        ASSERT_EQ(3, p4.get());
        ASSERT_EQ(4, p5.get());
        ASSERT_EQ(5, p6.get());
        ASSERT_EQ(6, p7.get());
        callee_ = "handler7";
    }
    void handler8(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const RInt32& p7, const RInt32& p8, const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        ASSERT_EQ(2, p3.get());
        ASSERT_EQ(3, p4.get());
        ASSERT_EQ(4, p5.get());
        ASSERT_EQ(5, p6.get());
        ASSERT_EQ(6, p7.get());
        ASSERT_EQ(7, p8.get());
        callee_ = "handler8";
    }
    void handler9(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const RInt32& p7, const RInt32& p8, const RInt32& p9,
        const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        ASSERT_EQ(2, p3.get());
        ASSERT_EQ(3, p4.get());
        ASSERT_EQ(4, p5.get());
        ASSERT_EQ(5, p6.get());
        ASSERT_EQ(6, p7.get());
        ASSERT_EQ(7, p8.get());
        ASSERT_EQ(8, p9.get());
        callee_ = "handler9";
    }
    void handler10(const RInt32& p1, const RInt32& p2, const RInt32& p3,
        const RInt32& p4, const RInt32& p5, const RInt32& p6,
        const RInt32& p7, const RInt32& p8, const RInt32& p9,
        const RInt32& p10, const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1.get());
        ASSERT_EQ(1, p2.get());
        ASSERT_EQ(2, p3.get());
        ASSERT_EQ(3, p4.get());
        ASSERT_EQ(4, p5.get());
        ASSERT_EQ(5, p6.get());
        ASSERT_EQ(6, p7.get());
        ASSERT_EQ(7, p8.get());
        ASSERT_EQ(8, p9.get());
        ASSERT_EQ(9, p10.get());
        callee_ = "handler10";
    }

    void handler1_with_PrimitiveType(int32_t p1, const void* /*rpcHint*/) {
        ASSERT_EQ(0, p1);
        callee_ = "handler1_with_PrimitiveType";
    }

protected:
    std::string callee_;
};


TEST_F(ReceivingFunctorTest, testP0)
{
    ReceivingFunctorT<ReceivingFunctorTest, SRPC_TYPELIST_0()>
        functor(&ReceivingFunctorTest::handler0);
    functor.unmarshal(*istream_);
    ASSERT_EQ(4 * 10, istream_->getTotalSize());

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler0"), callee_);
}


TEST_F(ReceivingFunctorTest, testP1)
{
    ReceivingFunctorT<ReceivingFunctorTest, SRPC_TYPELIST_1(RInt32)>
        functor(&ReceivingFunctorTest::handler1);
    functor.unmarshal(*istream_);
    ASSERT_EQ((4 * 10) - 4, istream_->getTotalSize());
    ASSERT_EQ(0, functor.p1_.get());

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler1"), callee_);
}


TEST_F(ReceivingFunctorTest, testP2)
{
    ReceivingFunctorT<ReceivingFunctorTest, SRPC_TYPELIST_2(RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler2);
    functor.unmarshal(*istream_);
    ASSERT_EQ((4 * 10) - (4 * 2), istream_->getTotalSize());
    ASSERT_EQ(1, functor.p2_.get());

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler2"), callee_);
}


TEST_F(ReceivingFunctorTest, testP3)
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_3(RInt32, RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler3);
    functor.unmarshal(*istream_);
    ASSERT_EQ((4 * 10) - (4 * 3), istream_->getTotalSize());
    ASSERT_EQ(2, functor.p3_.get());

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler3"), callee_);
}


TEST_F(ReceivingFunctorTest, testP4)
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_4(RInt32, RInt32, RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler4);
    functor.unmarshal(*istream_);
    ASSERT_EQ((4 * 10) - (4 * 4), istream_->getTotalSize());
    ASSERT_EQ(3, functor.p4_.get());

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler4"), callee_);
}


TEST_F(ReceivingFunctorTest, testP5)
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_5(RInt32, RInt32, RInt32, RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler5);
    functor.unmarshal(*istream_);
    ASSERT_EQ((4 * 10) - (4 * 5), istream_->getTotalSize());
    ASSERT_EQ(4, functor.p5_.get());

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler5"), callee_);
}


TEST_F(ReceivingFunctorTest, testP6)
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_6(RInt32, RInt32, RInt32, RInt32, RInt32, RInt32)>
        functor(&ReceivingFunctorTest::handler6);
    functor.unmarshal(*istream_);
    ASSERT_EQ((4 * 10) - (4 * 6), istream_->getTotalSize());
    ASSERT_EQ(5, functor.p6_.get());

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler6"), callee_);
}


TEST_F(ReceivingFunctorTest, testP7)
{
    ReceivingFunctorT<ReceivingFunctorTest,
        SRPC_TYPELIST_7(RInt32, RInt32, RInt32, RInt32, RInt32, RInt32,
        RInt32)> functor(&ReceivingFunctorTest::handler7);
    functor.unmarshal(*istream_);
    ASSERT_EQ((4 * 10) - (4 * 7), istream_->getTotalSize());
    ASSERT_EQ(6, functor.p7_.get());

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler7"), callee_);
}


TEST_F(ReceivingFunctorTest, testP1_with_PrimitiveType)
{
    ReceivingFunctorT<ReceivingFunctorTest, SRPC_TYPELIST_1(int32_t)>
        functor(&ReceivingFunctorTest::handler1_with_PrimitiveType);
    functor.unmarshal(*istream_);
    ASSERT_EQ((4 * 10) - 4, istream_->getTotalSize());
    ASSERT_EQ(0, functor.p1_);

    functor.call(this, 0);
    ASSERT_EQ(std::string("handler1_with_PrimitiveType"), callee_);
}
