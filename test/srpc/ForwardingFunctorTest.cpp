#include "SrpcTestPCH.h"
#include <sne/srpc/detail/ForwardingFunctors.h>
#include <sne/srpc/RpcTypes.h>
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class ForwardingFunctorTest 
*
* marshaling functor test
*/
class ForwardingFunctorTest : public test::BitStreamFixture
{
};


TEST_F(ForwardingFunctorTest, testP0)
{
    ForwardingFunctorT<SRPC_TYPELIST_0()> functor;
    functor.marshal(*ostream_);
    ASSERT_EQ(0, ostream_->getTotalSize()) << "empty";
}


TEST_F(ForwardingFunctorTest, testP1)
{
    const RInt32 expected = 1234;
    ForwardingFunctorT<SRPC_TYPELIST_1(RInt32)> functor(expected);
    functor.marshal(*ostream_);
    ASSERT_EQ(4, ostream_->getTotalSize());

    int32_t value;
    istream_->read(value);

    ASSERT_EQ(expected.get(), value);
}


TEST_F(ForwardingFunctorTest, testP2)
{
    ForwardingFunctorT<SRPC_TYPELIST_2(RInt32, RInt32)> functor(1234, 4321);
    functor.marshal(*ostream_);
    ASSERT_EQ(4 + 4, ostream_->getTotalSize());

    int32_t value1;
    istream_->read(value1);
    ASSERT_EQ(1234, value1);

    int32_t value2;
    istream_->read(value2);
    ASSERT_EQ(4321, value2);
}


TEST_F(ForwardingFunctorTest, testP3)
{
    ForwardingFunctorT<SRPC_TYPELIST_3(RInt32, RInt32, RInt32)> functor(0, 1, 2);
    functor.marshal(*ostream_);
    ASSERT_EQ(4 * 3, ostream_->getTotalSize());

    for (int i = 0; i < 3; ++i) {
        int32_t value;
        istream_->read(value);
        ASSERT_EQ(i, value);
    }
}


TEST_F(ForwardingFunctorTest, testP4)
{
    ForwardingFunctorT<SRPC_TYPELIST_4(RInt32, RInt32, RInt32, RInt32)>
        functor(0, 1, 2, 3);
    functor.marshal(*ostream_);
    ASSERT_EQ(4 * 4, ostream_->getTotalSize());

    for (int i = 0; i < 4; ++i) {
        int32_t value;
        istream_->read(value);
        ASSERT_EQ(i, value);
    }
}


TEST_F(ForwardingFunctorTest, testP5)
{
    ForwardingFunctorT<SRPC_TYPELIST_5(RInt32, RInt32, RInt32, RInt32, RInt32)>
        functor(0, 1, 2, 3, 4);
    functor.marshal(*ostream_);
    ASSERT_EQ(4 * 5, ostream_->getTotalSize());

    for (int i = 0; i < 5; ++i) {
        int32_t value;
        istream_->read(value);
        ASSERT_EQ(i, value);
    }
}


TEST_F(ForwardingFunctorTest, testP6)
{
    ForwardingFunctorT<SRPC_TYPELIST_6(RInt32, RInt32, RInt32, RInt32, RInt32,
        RInt32)> functor(0, 1, 2, 3, 4, 5);
    functor.marshal(*ostream_);
    ASSERT_EQ(4 * 6, ostream_->getTotalSize());

    for (int i = 0; i < 6; ++i) {
        int32_t value;
        istream_->read(value);
        ASSERT_EQ(i, value);
    }
}


TEST_F(ForwardingFunctorTest, testP7)
{
    ForwardingFunctorT<SRPC_TYPELIST_7(RInt32, RInt32, RInt32, RInt32, RInt32,
        RInt32, RInt32)> functor(0, 1, 2, 3, 4, 5, 6);
    functor.marshal(*ostream_);
    ASSERT_EQ(4 * 7, ostream_->getTotalSize());

    for (int i = 0; i < 7; ++i) {
        int32_t value;
        istream_->read(value);
        ASSERT_EQ(i, value);
    }
}


TEST_F(ForwardingFunctorTest, testComplex)
{
    typedef RpcNumericType<uint8_t, 4> RUInt4;

    RString s("0123456789");
    ForwardingFunctorT<SRPC_TYPELIST_4(RUInt4, RInt8, RInt16, RString)>
        functor(0xFF, -1, -1, s);
    functor.marshal(*ostream_);
    ASSERT_EQ(1 + 1 + 2 + (2 + 10), ostream_->getTotalSize());

    RUInt4 value1;
    value1.serialize(*istream_);
    ASSERT_EQ(0x0F, static_cast<int>(value1));

    RInt8 value2;
    value2.serialize(*istream_);
    ASSERT_EQ(-1, static_cast<int>(value2));

    int16_t value3;
    istream_->read(value3);
    ASSERT_EQ(-1, static_cast<int>(value3));

    std::string value4;
    istream_->read(value4, USHRT_MAX, core::Bits<uint16_t>::size);
    ASSERT_EQ(s.ref(), value4);
}


TEST_F(ForwardingFunctorTest, testP1_with_PrimitiveType)
{
    ForwardingFunctorT<SRPC_TYPELIST_1(int32_t)> functor(1234);
    functor.marshal(*ostream_);
    ASSERT_EQ(4, ostream_->getTotalSize());

    int32_t value;
    istream_->read(value);

    ASSERT_EQ(1234, value);
}
