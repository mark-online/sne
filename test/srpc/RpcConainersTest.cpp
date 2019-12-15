#include "SrpcTestPCH.h"
#include <sne/srpc/RpcContainers.h>
#include <sne/test/StreamFixture.h>

using namespace sne;
using namespace sne::srpc;

/**
* @class RpcConainersTest
*
* Rpc containers test
*/
class RpcConainersTest : public test::BitStreamFixture
{
};


TEST_F(RpcConainersTest, testRVector)
{
    RVector<int32_t> expected;
    expected.push_back(1);
    expected.push_back(3);
    expected.push_back(5);
    expected.serialize(*ostream_);

    RVector<int32_t> actual;
    actual.serialize(*istream_);

    ASSERT_EQ(expected.size(), actual.size());

    ASSERT_EQ(expected.back(), actual.back());
}


TEST_F(RpcConainersTest, testRList)
{
    RList<int32_t> expected;
    expected.push_back(1);
    expected.push_back(3);
    expected.push_back(5);
    expected.serialize(*ostream_);

    RList<int32_t> actual;
    actual.serialize(*istream_);

    ASSERT_EQ(expected.size(), actual.size());

    ASSERT_EQ(expected.back(), actual.back());
}


TEST_F(RpcConainersTest, testRSet)
{
    RSet<RShortString> expected;
    expected.emplace("1234");
    expected.emplace("abcd");
    expected.emplace("4567");
    expected.serialize(*ostream_);

    RSet<RShortString> actual;
    actual.serialize(*istream_);

    ASSERT_EQ(expected.size(), actual.size());

    ASSERT_EQ(*expected.begin(), *actual.begin());
}


TEST_F(RpcConainersTest, testRHashSet)
{
    RHashSet<RString> expected;
    expected.emplace("1234");
    expected.emplace("abcd");
    expected.emplace("4567");
    expected.serialize(*ostream_);

    RHashSet<RString> actual;
    actual.serialize(*istream_);

    ASSERT_EQ(expected.size(), actual.size());

    // FYI: boost 특정 버전 이상부터 삽입 순서가 달라짐
    std::set<RString> expected2(expected.begin(), expected.end());
    std::set<RString> actual2(actual.begin(), actual.end());
    ASSERT_EQ(*expected2.begin(), *actual2.begin());
}


TEST_F(RpcConainersTest, testRMap)
{
    typedef RMap<int32_t, RShortString> MapType;
    MapType expected;
    expected.emplace(3, "1234");
    expected.emplace(1, "1234");
    expected.emplace(2, "1234");
    expected.serialize(*ostream_);

    MapType actual;
    actual.serialize(*istream_);

    ASSERT_EQ(expected.size(), actual.size());

    ASSERT_EQ((*expected.begin()).second, (*actual.begin()).second);
}


TEST_F(RpcConainersTest, testRHashMap)
{
    typedef RHashMap<int32_t, RShortString> MapType;
    MapType expected;
    expected.emplace(3, "1234");
    expected.emplace(1, "1234");
    expected.emplace(2, "1234");
    expected.serialize(*ostream_);

    MapType actual;
    actual.serialize(*istream_);

    ASSERT_EQ(expected.size(), actual.size());

    ASSERT_EQ((*expected.begin()).second, (*actual.begin()).second);
}
