#include "common.hpp"
#include <fpm/ios.hpp>

#if __cplusplus >= 202002L
#   include <version>
#   ifdef __cpp_lib_format
#       include <format>
#       include <numbers>

template <typename B, typename I, unsigned int F, bool R>
inline void ExpectFormat(
    const std::string_view format,
    const fpm::fixed<B, I, F, R> valFixed,
    const double valDouble,
    const std::string& expectedValue
)
{
    const auto resFixed = std::vformat(format, std::make_format_args(valFixed));
    const auto resDouble = std::vformat(format, std::make_format_args(valDouble));
    EXPECT_EQ(resFixed, expectedValue);
    EXPECT_EQ(resDouble, expectedValue);
    EXPECT_EQ(resFixed, resDouble);
}

TEST(formatting, basic)
{
    EXPECT_EQ(std::format("{}",    fpm::fixed_16_16{0}), std::string("0"));
    EXPECT_EQ(std::format("{0}",   fpm::fixed_16_16{0}), std::string("0"));
    EXPECT_EQ(std::format("{:+}",  fpm::fixed_16_16{0}), std::string("+0"));
    EXPECT_EQ(std::format("{0:+}", fpm::fixed_16_16{0}), std::string("+0"));

    EXPECT_EQ(std::format("{}",    fpm::fixed_16_16{1}), std::string("1"));
    EXPECT_EQ(std::format("{0}",   fpm::fixed_16_16{1}), std::string("1"));
    EXPECT_EQ(std::format("{:+}",  fpm::fixed_16_16{1}), std::string("+1"));
    EXPECT_EQ(std::format("{0:+}", fpm::fixed_16_16{1}), std::string("+1"));

    EXPECT_EQ(std::format("{}",   fpm::fixed_16_16{42}), std::string("42"));
    EXPECT_EQ(std::format("{:+}", fpm::fixed_16_16{42}), std::string("+42"));

    EXPECT_EQ(std::format("{}",   fpm::fixed_16_16{123}), std::string("123"));
    EXPECT_EQ(std::format("{:+}", fpm::fixed_16_16{123}), std::string("+123"));

    EXPECT_EQ(std::format("{}",   fpm::fixed_16_16{123.25}), std::string("123.25"));
    EXPECT_EQ(std::format("{:+}", fpm::fixed_16_16{123.25}), std::string("+123.25"));

    EXPECT_EQ(std::format("{}",   fpm::fixed_16_16{123.125}), std::string("123.125"));
    EXPECT_EQ(std::format("{:+}", fpm::fixed_16_16{123.125}), std::string("+123.125"));

    EXPECT_EQ(std::format("{}",   fpm::fixed_16_16{123.0625}), std::string("123.062"));
    EXPECT_EQ(std::format("{:+}", fpm::fixed_16_16{123.0625}), std::string("+123.062"));
}

TEST(formatting, width)
{
    EXPECT_EQ(std::format("{:0}", fpm::fixed_16_16{0}), std::string("0"));
    EXPECT_EQ(std::format("{:1}", fpm::fixed_16_16{0}), std::string("0"));
    EXPECT_EQ(std::format("{:2}", fpm::fixed_16_16{0}), std::string(" 0"));
    EXPECT_EQ(std::format("{:3}", fpm::fixed_16_16{0}), std::string("  0"));
    EXPECT_EQ(std::format("{:4}", fpm::fixed_16_16{0}), std::string("   0"));
    EXPECT_EQ(std::format("{:5}", fpm::fixed_16_16{0}), std::string("    0"));
    EXPECT_EQ(std::format("{:6}", fpm::fixed_16_16{0}), std::string("     0"));
}

/*TEST(formatting, width_nested)
{
    for(int precision = 0; precision < 10; precision++)
    {
        const std::string strFixed  = std::format("{:{}}", fpm::fixed_16_16{0}, precision);
        const std::string strDouble = std::format("{:{}}", 0.0, precision);
        EXPECT_EQ(strFixed, strDouble);
    }
}*/

TEST(formatting, fill_and_align)
{
    ExpectFormat("{:6}", fpm::fixed_16_16{  0},   0.0, "     0");
    ExpectFormat("{:6}", fpm::fixed_16_16{  1},   1.0, "     1");
    ExpectFormat("{:6}", fpm::fixed_16_16{ 42},  42.0, "    42");
    ExpectFormat("{:6}", fpm::fixed_16_16{123}, 123.0, "   123");
    ExpectFormat("{:6}", fpm::fixed_16_16{ -1},  -1.0, "    -1");
    ExpectFormat("{:6}", fpm::fixed_16_16{-42}, -42.0, "   -42");

    ExpectFormat("{:*>6}", fpm::fixed_16_16{  0},   0.0, "*****0");
    ExpectFormat("{:*>6}", fpm::fixed_16_16{  1},   1.0, "*****1");
    ExpectFormat("{:*>6}", fpm::fixed_16_16{ 42},  42.0, "****42");
    ExpectFormat("{:*>6}", fpm::fixed_16_16{123}, 123.0, "***123");
    ExpectFormat("{:*>6}", fpm::fixed_16_16{ -1},  -1.0, "****-1");
    ExpectFormat("{:*>6}", fpm::fixed_16_16{-42}, -42.0, "***-42");

    ExpectFormat("{:x>6}", fpm::fixed_16_16{  0},   0.0, "xxxxx0");
    ExpectFormat("{:x>6}", fpm::fixed_16_16{  1},   1.0, "xxxxx1");
    ExpectFormat("{:x>6}", fpm::fixed_16_16{ 42},  42.0, "xxxx42");
    ExpectFormat("{:x>6}", fpm::fixed_16_16{123}, 123.0, "xxx123");
    ExpectFormat("{:x>6}", fpm::fixed_16_16{ -1},  -1.0, "xxxx-1");
    ExpectFormat("{:x>6}", fpm::fixed_16_16{-42}, -42.0, "xxx-42");

    ExpectFormat("{:+>6}", fpm::fixed_16_16{  0},   0.0, "+++++0");
    ExpectFormat("{:+>6}", fpm::fixed_16_16{  1},   1.0, "+++++1");
    ExpectFormat("{:+>6}", fpm::fixed_16_16{ 42},  42.0, "++++42");
    ExpectFormat("{:+>6}", fpm::fixed_16_16{123}, 123.0, "+++123");
    ExpectFormat("{:+>6}", fpm::fixed_16_16{ -1},  -1.0, "++++-1");
    ExpectFormat("{:+>6}", fpm::fixed_16_16{-42}, -42.0, "+++-42");

    ExpectFormat("{:>>6}", fpm::fixed_16_16{  0},   0.0, ">>>>>0");
    ExpectFormat("{:>>6}", fpm::fixed_16_16{  1},   1.0, ">>>>>1");
    ExpectFormat("{:>>6}", fpm::fixed_16_16{ 42},  42.0, ">>>>42");
    ExpectFormat("{:>>6}", fpm::fixed_16_16{123}, 123.0, ">>>123");
    ExpectFormat("{:>>6}", fpm::fixed_16_16{ -1},  -1.0, ">>>>-1");
    ExpectFormat("{:>>6}", fpm::fixed_16_16{-42}, -42.0, ">>>-42");

    ExpectFormat("{:0>6}", fpm::fixed_16_16{  0},   0.0, "000000");
    ExpectFormat("{:0>6}", fpm::fixed_16_16{  1},   1.0, "000001");
    ExpectFormat("{:0>6}", fpm::fixed_16_16{ 42},  42.0, "000042");
    ExpectFormat("{:0>6}", fpm::fixed_16_16{123}, 123.0, "000123");
    ExpectFormat("{:0>6}", fpm::fixed_16_16{ -1},  -1.0, "0000-1");
    ExpectFormat("{:0>6}", fpm::fixed_16_16{-42}, -42.0, "000-42");

    ExpectFormat("{:<6}", fpm::fixed_16_16{  0},   0.0, "0     ");
    ExpectFormat("{:<6}", fpm::fixed_16_16{  1},   1.0, "1     ");
    ExpectFormat("{:<6}", fpm::fixed_16_16{ 42},  42.0, "42    ");
    ExpectFormat("{:<6}", fpm::fixed_16_16{123}, 123.0, "123   ");
    ExpectFormat("{:<6}", fpm::fixed_16_16{ -1},  -1.0, "-1    ");
    ExpectFormat("{:<6}", fpm::fixed_16_16{-42}, -42.0, "-42   ");

    ExpectFormat("{:^6}", fpm::fixed_16_16{  0},   0.0, "  0   ");
    ExpectFormat("{:^6}", fpm::fixed_16_16{  1},   1.0, "  1   ");
    ExpectFormat("{:^6}", fpm::fixed_16_16{ 42},  42.0, "  42  ");
    ExpectFormat("{:^6}", fpm::fixed_16_16{123}, 123.0, " 123  ");
    ExpectFormat("{:^6}", fpm::fixed_16_16{ -1},  -1.0, "  -1  ");
    ExpectFormat("{:^6}", fpm::fixed_16_16{-42}, -42.0, " -42  ");

    ExpectFormat("{:^2}", fpm::fixed_16_16{123}, 123.0, "123");
    ExpectFormat("{:^2}", fpm::fixed_16_16{12345}, 12345.0, "12345");
    ExpectFormat("{:^4}", fpm::fixed_16_16{12345}, 12345.0, "12345");
}

TEST(formatting, sign)
{
    ExpectFormat("{0:},{0:+},{0:-},{0: }", fpm::fixed_16_16{ 1},  1.0, "1,+1,1, 1");
    ExpectFormat("{0:},{0:+},{0:-},{0: }", fpm::fixed_16_16{-1}, -1.0, "-1,-1,-1,-1");
}

TEST(formatting, zero_padding)
{
    ExpectFormat("{:02}", fpm::fixed_16_16{ 1},  1.0, "01");
    ExpectFormat("{:02}", fpm::fixed_16_16{-1}, -1.0, "-1");

    ExpectFormat("{:03}", fpm::fixed_16_16{ 1},  1.0, "001");
    ExpectFormat("{:03}", fpm::fixed_16_16{-1}, -1.0, "-01");

    ExpectFormat("{:04}", fpm::fixed_16_16{ 1},  1.0, "0001");
    ExpectFormat("{:04}", fpm::fixed_16_16{-1}, -1.0, "-001");

    ExpectFormat("{:06}", fpm::fixed_16_16{ 1},  1.0, "000001");
    ExpectFormat("{:06}", fpm::fixed_16_16{-1}, -1.0, "-00001");

    ExpectFormat("{:<02}", fpm::fixed_16_16{ 1},  1.0, "1 ");
    ExpectFormat("{:<02}", fpm::fixed_16_16{-1}, -1.0, "-1");

    ExpectFormat("{:<06}", fpm::fixed_16_16{ 1},  1.0, "1     ");
    ExpectFormat("{:<06}", fpm::fixed_16_16{-1}, -1.0, "-1    ");
}

TEST(formatting, precision)
{
    const double dblValue = 2.015625;
    const auto fpmValue = fpm::fixed_16_16(2.015625);

    ExpectFormat("{:10f}",   fpmValue, dblValue, "  2.015625");
    ExpectFormat("{:.5f}",   fpmValue, dblValue, "2.01562");
    ExpectFormat("{:10.5f}", fpmValue, dblValue, "   2.01562");
    ExpectFormat("{:10.6f}", fpmValue, dblValue, "  2.015625");
    ExpectFormat("{:10.3f}", fpmValue, dblValue, "     2.016");
    ExpectFormat("{:10.2f}", fpmValue, dblValue, "      2.02");
}

TEST(formatting, precision_pi)
{
    const double dblValue = std::numbers::pi_v<double>;
    const auto fpmValue = fpm::fixed_8_24::pi(); // fpm::fixed_16_16 does not have enough precision

    ExpectFormat("{:10f}",   fpmValue, dblValue, "  3.141593");
    ExpectFormat("{:.5f}",   fpmValue, dblValue, "3.14159");

    ExpectFormat("{:10.6f}", fpmValue, dblValue, "  3.141593");
    ExpectFormat("{:10.5f}", fpmValue, dblValue, "   3.14159");
    ExpectFormat("{:10.4f}", fpmValue, dblValue, "    3.1416");
    ExpectFormat("{:10.3f}", fpmValue, dblValue, "     3.142");
    ExpectFormat("{:10.2f}", fpmValue, dblValue, "      3.14");
    ExpectFormat("{:10.1f}", fpmValue, dblValue, "       3.1");
    ExpectFormat("{:10.0f}", fpmValue, dblValue, "         3");
}

/*TEST(formatting, precision_nested)
{
    const double dblValue = 2.015625;
    const auto fpmValue = fpm::fixed_16_16(2.015625);

    for(int precision = 0; precision < 10; precision++)
    {
        const std::string strFixed  = std::format("{:2.{}f}", fpmValue, precision);
        const std::string strDouble = std::format("{:2.{}f}", dblValue, precision);
        EXPECT_EQ(strFixed, strDouble);
    }

    for(int precision = 0; precision < 10; precision++)
    {
        for(int width = 0; width < precision + 2; width++)
        {
            const std::string strFixed  = std::format("{:{}.{}f}", fpmValue, width, precision);
            const std::string strDouble = std::format("{:{}.{}f}", dblValue, width, precision);
            EXPECT_EQ(strFixed, strDouble);
        }

        for(int width = 0; width < precision + 4; width++)
        {
            const std::string strFixed  = std::format("{:{}.{}f}", fpmValue, width, precision);
            const std::string strDouble = std::format("{:{}.{}f}", dblValue, width, precision);
            EXPECT_EQ(strFixed, strDouble);
        }
    }
}*/

TEST(formatting, types)
{
    const double dblValue = 2.015625;
    const auto fpmValue = fpm::fixed_16_16(2.015625);

    // Fixed
    {
        ExpectFormat("{:10.6f}", fpmValue, dblValue, "  2.015625");
        ExpectFormat("{:10.6F}", fpmValue, dblValue, "  2.015625");

        ExpectFormat("{:10.3f}", fpmValue, dblValue, "     2.016");
        ExpectFormat("{:10.3F}", fpmValue, dblValue, "     2.016");
    }
    // Hex
    {
        // Implementation differs from `double`
        // fpm:    " 0x1.02p+1"
        // double: "1.020000P+1"
        //ExpectFormat("{:10.6a}", fpmValue, dblValue, "1.020000p+1");
        //ExpectFormat("{:10.6A}", fpmValue, dblValue, "1.020000P+1");
    }
    // Scientific
    {
        ExpectFormat("{:10.6e}", fpmValue, dblValue, "2.015625e+00");
        ExpectFormat("{:10.6E}", fpmValue, dblValue, "2.015625E+00");

        ExpectFormat("{:10.3e}", fpmValue, dblValue, " 2.016e+00");
        ExpectFormat("{:10.3E}", fpmValue, dblValue, " 2.016E+00");
    }
    // General
    {
        ExpectFormat("{:10.6g}", fpmValue, dblValue, "   2.01562");
        ExpectFormat("{:10.6G}", fpmValue, dblValue, "   2.01562");

        ExpectFormat("{:10.3g}", fpmValue, dblValue, "      2.02");
        ExpectFormat("{:10.3G}", fpmValue, dblValue, "      2.02");
    }
}

#endif
#endif
