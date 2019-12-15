#include "SrpcTestPCH.h"
#include <sne/srpc/RpcTypes.h>
#include <sne/srpc/RpcContainers.h>
#include <sne/core/utility/Unicode.h>
#include <sne/srpc/detail/RpcId.h>
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class RpcTypesTest
*
* Rpc Types test
* - I(Bit|Byte)StreamTest와 어느 정도 유사하지만 RpcType 테스트를 한 눈에
*   볼 수 있도록 의도적으로 중복 시킴
*/
class RpcTypesTest : public test::BitStreamFixture
{
};


TEST_F(RpcTypesTest, testRInt8)
{
    RInt8 expected = SCHAR_MIN;
    expected.serialize(*ostream_);

    RInt8 actual;
    actual.serialize(*istream_);
    ASSERT_EQ(static_cast<int>(expected), static_cast<int>(actual));
}


TEST_F(RpcTypesTest, testRUInt8)
{
    RUInt8 expected = SCHAR_MAX;
    expected.serialize(*ostream_);

    RUInt8 actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt16)
{
    RInt16 expected = SHRT_MIN;
    expected.serialize(*ostream_);

    RInt16 actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt16)
{
    RUInt16 expected = SHRT_MAX;
    expected.serialize(*ostream_);

    RUInt16 actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt32)
{
    RInt32 expected = INT_MIN;
    expected.serialize(*ostream_);

    RInt32 actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt32)
{
    RUInt32 expected = INT_MAX;
    expected.serialize(*ostream_);

    RUInt32 actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt64)
{
    RInt64 expected = -1;
    expected.serialize(*ostream_);

    RInt64 actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt64)
{
    RUInt64 expected = static_cast<uint64_t>(-1);
    expected.serialize(*ostream_);

    RUInt64 actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRFloat32)
{
    RFloat32 expected = -123.456F;
    expected.serialize(*ostream_);

    RFloat32 actual;
    actual.serialize(*istream_);
    ASSERT_FLOAT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRString)
{
    RString expected = "TANSTAAFL";
    expected.serialize(*ostream_);

    RString actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRShortString)
{
    RShortString expected(std::string(256, 'X'));
    expected.serialize(*ostream_);

    RShortString actual;
    actual.serialize(*istream_);
    ASSERT_EQ(static_cast<int>(expected.size() - 1),
        static_cast<int>(actual.size()));
}


TEST_F(RpcTypesTest, testRBool)
{
    {
        RBool expected = false;
        expected.serialize(*ostream_);

        RBool actual;
        actual.serialize(*istream_);
        ASSERT_EQ(expected, actual);
    }
    {
        RBool expected = true;
        expected.serialize(*ostream_);

        RBool actual;
        actual.serialize(*istream_);
        ASSERT_EQ(expected, actual);
    }
}


TEST_F(RpcTypesTest, testRRpcId)
{
    RRpcId expected("rpc_id");
    expected.serialize(*ostream_);

    RRpcId actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected.get(), actual.get());
}


TEST_F(RpcTypesTest, testRWString)
{
    RWString expected(core::fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    expected.serialize(*ostream_);

    RWString actual;
    actual.serialize(*istream_);
    ASSERT_TRUE(wcscmp(expected.c_str(), actual.c_str()) == 0);
}


TEST_F(RpcTypesTest, testRWShortString)
{
    RWShortString expected(core::fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    expected.serialize(*ostream_);

    RWShortString actual;
    actual.serialize(*istream_);
    ASSERT_TRUE(wcscmp(expected.c_str(), actual.c_str()) == 0);
}


enum EnumType { etA, etB, etC };
typedef RpcNumericType<EnumType, 3, int32_t> REnumType;

TEST_F(RpcTypesTest, testEnum)
{
    EnumType exp(etC);
    REnumType expected(exp);
    expected.serialize(*ostream_);

    REnumType actual;
    actual.serialize(*istream_);
    EnumType act = actual;
    ASSERT_EQ(expected, actual);
    ASSERT_EQ(exp, act);
}


TEST_F(RpcTypesTest, testRUserDefinedString)
{
    typedef RpcStringType<std::string, 10> RUserDefinedString;

    RUserDefinedString expected(std::string(12, 'X'));
    expected.serialize(*ostream_);

    RUserDefinedString actual;
    actual.serialize(*istream_);
    ASSERT_EQ(
        static_cast<int>(expected.size() - 2),
        static_cast<int>(actual.size()));
}


TEST_F(RpcTypesTest, testRStringMaxLength)
{
    const size_t maxLength = 9;
    typedef RpcStringType<std::string, maxLength> RLimitedString;

    RLimitedString expected = "1234567890";
    expected.serialize(*ostream_);

    RLimitedString actual;
    actual.serialize(*istream_);
    ASSERT_EQ(expected.substr(0, maxLength), actual.ref());
}
