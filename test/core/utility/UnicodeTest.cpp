#include "CoreTestPCH.h"
#include <sne/core/utility/Unicode.h>

using namespace sne;
using namespace sne::core;

// Visual Studio에서 한글 문자열이 포함된 코드를 실행하기 위해서
// "UTF-8 with BOM"으로 저장해야 함

/**
* @class UnicodeTest
*
* Unicode Test
*/
class UnicodeTest : public testing::Test
{
private:
    void testEnglish();
    void testKorean();
    void testInvalidUtf8();
};


TEST_F(UnicodeTest, testEnglish)
{
    {
        const std::string expected("You aren't gonna need it!");
        ASSERT_EQ(expected, toUtf8(L"You aren't gonna need it!")) <<
            "UCS to UTF-8";
    }
    {
        const std::wstring expected(L"You aren't gonna need it!");
        ASSERT_TRUE(wcscmp(expected.c_str(),
            fromUtf8("You aren't gonna need it!").c_str()) == 0) <<
            "UTF-8 to UCS";
    }
}


TEST_F(UnicodeTest, testKorean)
{
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4566)
    {
        const std::string expected("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F");
        ASSERT_EQ(expected, toUtf8(L"아햏햏")) << "UCS to UTF-8";
    }
    {
        const std::wstring expected(L"아햏햏");
        ASSERT_EQ(expected, fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F")) <<
            "UTF-8 to UCS";
    }
#pragma warning(pop)
#else
    // GCC의 경우 유니코드 문자열을 바로 입력할 경우 컴파일러가 제대로
    // 파싱하지 못하는 문제가 발생한다.
    {
        const std::string expected("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F");
        ASSERT_EQ(expected, toUtf8(fromUtf8(expected))) <<
            "utf-8 -> ucs -> utf-8";
    }
#endif
}


TEST_F(UnicodeTest, testInvalidUtf8)
{
    const std::wstring str(L"1234567890ABCDEFGHIZKLMNOPQRSTUVWXYZ");

    const std::string utf8(toUtf8(str));
    const size_t strLen = 10;
    std::string invalidUtf8;
    for (std::string::size_type i = 0; i < strLen; ++i) {
        invalidUtf8.push_back(utf8[i]);
    }

    const std::wstring converted(fromUtf8(invalidUtf8));
    ASSERT_TRUE(wcscmp(L"1234567890", converted.c_str()) == 0) <<
        "converting";
}
