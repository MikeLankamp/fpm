#include "common.hpp"
#include <fpm/ios.hpp>

#if __cplusplus >= 202002L
#   include <version>
#   ifdef __cpp_lib_format
#       include <format>
#       include <numbers>

template <typename B, typename I, unsigned int F, bool R>
inline void ExpectFormat(
    const std::wstring_view format,
    const fpm::fixed<B, I, F, R> valFixed,
    const double valDouble,
    const std::wstring& expectedValue
)
{
    const auto resFixed = std::vformat(format, std::make_wformat_args(valFixed));
    const auto resDouble = std::vformat(format, std::make_wformat_args(valDouble));
    EXPECT_EQ(resFixed, expectedValue);
    EXPECT_EQ(resDouble, expectedValue);
    EXPECT_EQ(resFixed, resDouble);
}

TEST(formatting_wchar, basic)
{
    EXPECT_EQ(std::format(L"{}",    fpm::fixed_16_16{0}), std::wstring(L"0"));
    EXPECT_EQ(std::format(L"{0}",   fpm::fixed_16_16{0}), std::wstring(L"0"));
    EXPECT_EQ(std::format(L"{:+}",  fpm::fixed_16_16{0}), std::wstring(L"+0"));
    EXPECT_EQ(std::format(L"{0:+}", fpm::fixed_16_16{0}), std::wstring(L"+0"));

    EXPECT_EQ(std::format(L"{}",    fpm::fixed_16_16{1}), std::wstring(L"1"));
    EXPECT_EQ(std::format(L"{0}",   fpm::fixed_16_16{1}), std::wstring(L"1"));
    EXPECT_EQ(std::format(L"{:+}",  fpm::fixed_16_16{1}), std::wstring(L"+1"));
    EXPECT_EQ(std::format(L"{0:+}", fpm::fixed_16_16{1}), std::wstring(L"+1"));

    EXPECT_EQ(std::format(L"{}",   fpm::fixed_16_16{42}), std::wstring(L"42"));
    EXPECT_EQ(std::format(L"{:+}", fpm::fixed_16_16{42}), std::wstring(L"+42"));

    EXPECT_EQ(std::format(L"{}",   fpm::fixed_16_16{123}), std::wstring(L"123"));
    EXPECT_EQ(std::format(L"{:+}", fpm::fixed_16_16{123}), std::wstring(L"+123"));

    EXPECT_EQ(std::format(L"{}",   fpm::fixed_16_16{123.25}), std::wstring(L"123.25"));
    EXPECT_EQ(std::format(L"{:+}", fpm::fixed_16_16{123.25}), std::wstring(L"+123.25"));

    EXPECT_EQ(std::format(L"{}",   fpm::fixed_16_16{123.125}), std::wstring(L"123.125"));
    EXPECT_EQ(std::format(L"{:+}", fpm::fixed_16_16{123.125}), std::wstring(L"+123.125"));

    EXPECT_EQ(std::format(L"{}",   fpm::fixed_16_16{123.0625}), std::wstring(L"123.062"));
    EXPECT_EQ(std::format(L"{:+}", fpm::fixed_16_16{123.0625}), std::wstring(L"+123.062"));
}

TEST(formatting_wchar, width)
{
    EXPECT_EQ(std::format(L"{:0}", fpm::fixed_16_16{0}), std::wstring(L"0"));
    EXPECT_EQ(std::format(L"{:1}", fpm::fixed_16_16{0}), std::wstring(L"0"));
    EXPECT_EQ(std::format(L"{:2}", fpm::fixed_16_16{0}), std::wstring(L" 0"));
    EXPECT_EQ(std::format(L"{:3}", fpm::fixed_16_16{0}), std::wstring(L"  0"));
    EXPECT_EQ(std::format(L"{:4}", fpm::fixed_16_16{0}), std::wstring(L"   0"));
    EXPECT_EQ(std::format(L"{:5}", fpm::fixed_16_16{0}), std::wstring(L"    0"));
    EXPECT_EQ(std::format(L"{:6}", fpm::fixed_16_16{0}), std::wstring(L"     0"));
}

/*TEST(formatting_wchar, width_nested)
{
    for(int precision = 0; precision < 10; precision++)
    {
        const std::wstring strFixed  = std::format(L"{:{}}", fpm::fixed_16_16{0}, precision);
        const std::wstring strDouble = std::format(L"{:{}}", 0.0, precision);
        EXPECT_EQ(strFixed, strDouble);
    }
}*/

TEST(formatting_wchar, fill_and_align)
{
    ExpectFormat(L"{:6}", fpm::fixed_16_16{  0},   0.0, L"     0");
    ExpectFormat(L"{:6}", fpm::fixed_16_16{  1},   1.0, L"     1");
    ExpectFormat(L"{:6}", fpm::fixed_16_16{ 42},  42.0, L"    42");
    ExpectFormat(L"{:6}", fpm::fixed_16_16{123}, 123.0, L"   123");
    ExpectFormat(L"{:6}", fpm::fixed_16_16{ -1},  -1.0, L"    -1");
    ExpectFormat(L"{:6}", fpm::fixed_16_16{-42}, -42.0, L"   -42");

    ExpectFormat(L"{:*>6}", fpm::fixed_16_16{  0},   0.0, L"*****0");
    ExpectFormat(L"{:*>6}", fpm::fixed_16_16{  1},   1.0, L"*****1");
    ExpectFormat(L"{:*>6}", fpm::fixed_16_16{ 42},  42.0, L"****42");
    ExpectFormat(L"{:*>6}", fpm::fixed_16_16{123}, 123.0, L"***123");
    ExpectFormat(L"{:*>6}", fpm::fixed_16_16{ -1},  -1.0, L"****-1");
    ExpectFormat(L"{:*>6}", fpm::fixed_16_16{-42}, -42.0, L"***-42");

    ExpectFormat(L"{:x>6}", fpm::fixed_16_16{  0},   0.0, L"xxxxx0");
    ExpectFormat(L"{:x>6}", fpm::fixed_16_16{  1},   1.0, L"xxxxx1");
    ExpectFormat(L"{:x>6}", fpm::fixed_16_16{ 42},  42.0, L"xxxx42");
    ExpectFormat(L"{:x>6}", fpm::fixed_16_16{123}, 123.0, L"xxx123");
    ExpectFormat(L"{:x>6}", fpm::fixed_16_16{ -1},  -1.0, L"xxxx-1");
    ExpectFormat(L"{:x>6}", fpm::fixed_16_16{-42}, -42.0, L"xxx-42");

    ExpectFormat(L"{:+>6}", fpm::fixed_16_16{  0},   0.0, L"+++++0");
    ExpectFormat(L"{:+>6}", fpm::fixed_16_16{  1},   1.0, L"+++++1");
    ExpectFormat(L"{:+>6}", fpm::fixed_16_16{ 42},  42.0, L"++++42");
    ExpectFormat(L"{:+>6}", fpm::fixed_16_16{123}, 123.0, L"+++123");
    ExpectFormat(L"{:+>6}", fpm::fixed_16_16{ -1},  -1.0, L"++++-1");
    ExpectFormat(L"{:+>6}", fpm::fixed_16_16{-42}, -42.0, L"+++-42");

    ExpectFormat(L"{:>>6}", fpm::fixed_16_16{  0},   0.0, L">>>>>0");
    ExpectFormat(L"{:>>6}", fpm::fixed_16_16{  1},   1.0, L">>>>>1");
    ExpectFormat(L"{:>>6}", fpm::fixed_16_16{ 42},  42.0, L">>>>42");
    ExpectFormat(L"{:>>6}", fpm::fixed_16_16{123}, 123.0, L">>>123");
    ExpectFormat(L"{:>>6}", fpm::fixed_16_16{ -1},  -1.0, L">>>>-1");
    ExpectFormat(L"{:>>6}", fpm::fixed_16_16{-42}, -42.0, L">>>-42");

    ExpectFormat(L"{:0>6}", fpm::fixed_16_16{  0},   0.0, L"000000");
    ExpectFormat(L"{:0>6}", fpm::fixed_16_16{  1},   1.0, L"000001");
    ExpectFormat(L"{:0>6}", fpm::fixed_16_16{ 42},  42.0, L"000042");
    ExpectFormat(L"{:0>6}", fpm::fixed_16_16{123}, 123.0, L"000123");
    ExpectFormat(L"{:0>6}", fpm::fixed_16_16{ -1},  -1.0, L"0000-1");
    ExpectFormat(L"{:0>6}", fpm::fixed_16_16{-42}, -42.0, L"000-42");

    ExpectFormat(L"{:<6}", fpm::fixed_16_16{  0},   0.0, L"0     ");
    ExpectFormat(L"{:<6}", fpm::fixed_16_16{  1},   1.0, L"1     ");
    ExpectFormat(L"{:<6}", fpm::fixed_16_16{ 42},  42.0, L"42    ");
    ExpectFormat(L"{:<6}", fpm::fixed_16_16{123}, 123.0, L"123   ");
    ExpectFormat(L"{:<6}", fpm::fixed_16_16{ -1},  -1.0, L"-1    ");
    ExpectFormat(L"{:<6}", fpm::fixed_16_16{-42}, -42.0, L"-42   ");

    ExpectFormat(L"{:^6}", fpm::fixed_16_16{  0},   0.0, L"  0   ");
    ExpectFormat(L"{:^6}", fpm::fixed_16_16{  1},   1.0, L"  1   ");
    ExpectFormat(L"{:^6}", fpm::fixed_16_16{ 42},  42.0, L"  42  ");
    ExpectFormat(L"{:^6}", fpm::fixed_16_16{123}, 123.0, L" 123  ");
    ExpectFormat(L"{:^6}", fpm::fixed_16_16{ -1},  -1.0, L"  -1  ");
    ExpectFormat(L"{:^6}", fpm::fixed_16_16{-42}, -42.0, L" -42  ");

    ExpectFormat(L"{:^2}", fpm::fixed_16_16{123}, 123.0, L"123");
    ExpectFormat(L"{:^2}", fpm::fixed_16_16{12345}, 12345.0, L"12345");
    ExpectFormat(L"{:^4}", fpm::fixed_16_16{12345}, 12345.0, L"12345");
}

TEST(formatting_wchar, sign)
{
    ExpectFormat(L"{0:},{0:+},{0:-},{0: }", fpm::fixed_16_16{ 1},  1.0, L"1,+1,1, 1");
    ExpectFormat(L"{0:},{0:+},{0:-},{0: }", fpm::fixed_16_16{-1}, -1.0, L"-1,-1,-1,-1");
}

TEST(formatting_wchar, zero_padding)
{
    ExpectFormat(L"{:02}", fpm::fixed_16_16{ 1},  1.0, L"01");
    ExpectFormat(L"{:02}", fpm::fixed_16_16{-1}, -1.0, L"-1");

    ExpectFormat(L"{:03}", fpm::fixed_16_16{ 1},  1.0, L"001");
    ExpectFormat(L"{:03}", fpm::fixed_16_16{-1}, -1.0, L"-01");

    ExpectFormat(L"{:04}", fpm::fixed_16_16{ 1},  1.0, L"0001");
    ExpectFormat(L"{:04}", fpm::fixed_16_16{-1}, -1.0, L"-001");

    ExpectFormat(L"{:06}", fpm::fixed_16_16{ 1},  1.0, L"000001");
    ExpectFormat(L"{:06}", fpm::fixed_16_16{-1}, -1.0, L"-00001");

    ExpectFormat(L"{:<02}", fpm::fixed_16_16{ 1},  1.0, L"1 ");
    ExpectFormat(L"{:<02}", fpm::fixed_16_16{-1}, -1.0, L"-1");

    ExpectFormat(L"{:<06}", fpm::fixed_16_16{ 1},  1.0, L"1     ");
    ExpectFormat(L"{:<06}", fpm::fixed_16_16{-1}, -1.0, L"-1    ");
}

TEST(formatting_wchar, precision)
{
    const double dblValue = 2.015625;
    const auto fpmValue = fpm::fixed_16_16(2.015625);

    ExpectFormat(L"{:10f}",   fpmValue, dblValue, L"  2.015625");
    ExpectFormat(L"{:.5f}",   fpmValue, dblValue, L"2.01562");
    ExpectFormat(L"{:10.5f}", fpmValue, dblValue, L"   2.01562");
    ExpectFormat(L"{:10.6f}", fpmValue, dblValue, L"  2.015625");
    ExpectFormat(L"{:10.3f}", fpmValue, dblValue, L"     2.016");
    ExpectFormat(L"{:10.2f}", fpmValue, dblValue, L"      2.02");
}
TEST(formatting_wchar, precision_pi)
{
    const double dblValue = std::numbers::pi_v<double>;
    const auto fpmValue = fpm::fixed_8_24::pi(); // fpm::fixed_16_16 does not have enough precision

    ExpectFormat(L"{:10f}",   fpmValue, dblValue, L"  3.141593");
    ExpectFormat(L"{:.5f}",   fpmValue, dblValue, L"3.14159");

    ExpectFormat(L"{:10.6f}", fpmValue, dblValue, L"  3.141593");
    ExpectFormat(L"{:10.5f}", fpmValue, dblValue, L"   3.14159");
    ExpectFormat(L"{:10.4f}", fpmValue, dblValue, L"    3.1416");
    ExpectFormat(L"{:10.3f}", fpmValue, dblValue, L"     3.142");
    ExpectFormat(L"{:10.2f}", fpmValue, dblValue, L"      3.14");
    ExpectFormat(L"{:10.1f}", fpmValue, dblValue, L"       3.1");
    ExpectFormat(L"{:10.0f}", fpmValue, dblValue, L"         3");
}

/*TEST(formatting_wchar, precision_nested)
{
    const double dblValue = 2.015625;
    const auto fpmValue = fpm::fixed_16_16(2.015625);

    for(int precision = 0; precision < 10; precision++)
    {
        const std::wstring strFixed  = std::format(L"{:2.{}f}", fpmValue, precision);
        const std::wstring strDouble = std::format(L"{:2.{}f}", dblValue, precision);
        EXPECT_EQ(strFixed, strDouble);
    }

    for(int precision = 0; precision < 10; precision++)
    {
        for(int width = 0; width < precision + 2; width++)
        {
            const std::wstring strFixed  = std::format(L"{:{}.{}f}", fpmValue, width, precision);
            const std::wstring strDouble = std::format(L"{:{}.{}f}", dblValue, width, precision);
            EXPECT_EQ(strFixed, strDouble);
        }

        for(int width = 0; width < precision + 4; width++)
        {
            const std::string strFixed  = std::format(L"{:{}.{}f}", fpmValue, width, precision);
            const std::string strDouble = std::format(L"{:{}.{}f}", dblValue, width, precision);
            EXPECT_EQ(strFixed, strDouble);
        }
    }
}*/

TEST(formatting_wchar, types)
{
    const double dblValue = 2.015625;
    const auto fpmValue = fpm::fixed_16_16(2.015625);

    // Fixed
    {
        ExpectFormat(L"{:10.6f}", fpmValue, dblValue, L"  2.015625");
        ExpectFormat(L"{:10.6F}", fpmValue, dblValue, L"  2.015625");

        ExpectFormat(L"{:10.3f}", fpmValue, dblValue, L"     2.016");
        ExpectFormat(L"{:10.3F}", fpmValue, dblValue, L"     2.016");
    }
    // Hex
    {
        // Implementation differs from `double`
        // fpm:    " 0x1.02p+1"
        // double: "1.020000P+1"
        //ExpectFormat(L"{:10.6a}", fpmValue, dblValue, L"1.020000p+1");
        //ExpectFormat(L"{:10.6A}", fpmValue, dblValue, L"1.020000P+1");
    }
    // Scientific
    {
        ExpectFormat(L"{:10.6e}", fpmValue, dblValue, L"2.015625e+00");
        ExpectFormat(L"{:10.6E}", fpmValue, dblValue, L"2.015625E+00");

        ExpectFormat(L"{:10.3e}", fpmValue, dblValue, L" 2.016e+00");
        ExpectFormat(L"{:10.3E}", fpmValue, dblValue, L" 2.016E+00");
    }
    // General
    {
        ExpectFormat(L"{:10.6g}", fpmValue, dblValue, L"   2.01562");
        ExpectFormat(L"{:10.6G}", fpmValue, dblValue, L"   2.01562");

        ExpectFormat(L"{:10.3g}", fpmValue, dblValue, L"      2.02");
        ExpectFormat(L"{:10.3G}", fpmValue, dblValue, L"      2.02");
    }
}

#endif
#endif
