#include "common.hpp"

template <typename T>
class customizations : public ::testing::Test
{
};

using FixedTypes = ::testing::Types<fpm::fixed_16_16, fpm::fixed_24_8, fpm::fixed_8_24>;

TYPED_TEST_SUITE(customizations, FixedTypes);

TYPED_TEST(customizations, swap)
{
    using std::swap;

    using P = TypeParam;
    P x{1}, y{2};
    swap(x, y);
    EXPECT_EQ(P{2}, x);
    EXPECT_EQ(P{1}, y);
}

TYPED_TEST(customizations, hash)
{
    using P = TypeParam;

    const std::hash<P> hash{};
    for (int y = -50; y < 50; ++y)
    {
        EXPECT_EQ(hash(P{y}/10), hash(P{y}/10));
        for (int x = -50; x < 50; ++x)
        {
            if (x != y)
            {
                EXPECT_NE(hash(P{x}/10), hash(P{y}/10));
            }
        }
    }
}

template <typename T>
struct Limits {};

template <>
struct Limits<fpm::fixed_16_16>
{
    static constexpr bool is_signed() noexcept { return true; }
    static constexpr int digits() noexcept { return 31; }
    static constexpr int max_digits10() noexcept { return 5+5; }
    static constexpr int min_exponent() noexcept { return -15; }
    static constexpr int max_exponent() noexcept { return  15; }
    static constexpr int min_exponent10() noexcept { return -4; }
    static constexpr int max_exponent10() noexcept { return 4; }
    static constexpr fpm::fixed_16_16 min() noexcept { return fpm::fixed_16_16::from_raw_value(-2147483647 - 1); }
    static constexpr fpm::fixed_16_16 max() noexcept { return fpm::fixed_16_16::from_raw_value( 2147483647); }
};

template <>
struct Limits<fpm::fixed_24_8>
{
    static constexpr bool is_signed() noexcept { return true; }
    static constexpr int digits() noexcept { return 31; }
    static constexpr int max_digits10() noexcept { return 7+3; }
    static constexpr int min_exponent() noexcept { return -7; }
    static constexpr int max_exponent() noexcept { return 23; }
    static constexpr int min_exponent10() noexcept { return -2; }
    static constexpr int max_exponent10() noexcept { return 6; }
    static constexpr fpm::fixed_24_8 min() noexcept { return fpm::fixed_24_8::from_raw_value(-2147483647 - 1); }
    static constexpr fpm::fixed_24_8 max() noexcept { return fpm::fixed_24_8::from_raw_value( 2147483647); }
};

template <>
struct Limits<fpm::fixed_8_24>
{
    static constexpr bool is_signed() noexcept { return true; }
    static constexpr int digits() noexcept { return 31; }
    static constexpr int max_digits10() noexcept { return 3+8; }
    static constexpr int min_exponent() noexcept { return -23; }
    static constexpr int max_exponent() noexcept { return  7; }
    static constexpr int min_exponent10() noexcept { return -7; }
    static constexpr int max_exponent10() noexcept { return  2; }
    static constexpr fpm::fixed_8_24 min() noexcept { return fpm::fixed_8_24::from_raw_value(-2147483647 - 1); }
    static constexpr fpm::fixed_8_24 max() noexcept { return fpm::fixed_8_24::from_raw_value( 2147483647); }
};

TYPED_TEST(customizations, numeric_limits)
{
    using L = std::numeric_limits<TypeParam>;
    using TL = Limits<TypeParam>;

    EXPECT_EQ(L::is_specialized, true);
    EXPECT_EQ(L::is_signed, TL::is_signed());
    EXPECT_EQ(L::is_integer, false);
    EXPECT_EQ(L::is_exact, true);
    EXPECT_EQ(L::has_infinity, false);
    EXPECT_EQ(L::has_quiet_NaN, false);
    EXPECT_EQ(L::has_signaling_NaN, false);
    EXPECT_EQ(L::has_denorm, std::denorm_absent);
    EXPECT_EQ(L::has_denorm_loss, false);
    EXPECT_EQ(L::round_style, std::round_to_nearest);
    EXPECT_EQ(L::is_iec_559, false);
    EXPECT_EQ(L::is_bounded, true);
    EXPECT_EQ(L::is_modulo, false);
    EXPECT_EQ(L::digits, TL::digits());
    EXPECT_EQ(L::digits10, 1);
    EXPECT_EQ(L::max_digits10, TL::max_digits10());
    EXPECT_EQ(L::radix, 2);
    EXPECT_EQ(L::min_exponent, TL::min_exponent());
    EXPECT_EQ(L::min_exponent10, TL::min_exponent10());
    EXPECT_EQ(L::max_exponent, TL::max_exponent());
    EXPECT_EQ(L::max_exponent10, TL::max_exponent10());
    EXPECT_EQ(L::traps, true);
    EXPECT_EQ(L::tinyness_before, false);

    EXPECT_EQ(L::min(), TL::min());
    EXPECT_EQ(L::lowest(), TL::min());
    EXPECT_EQ(L::max(), TL::max());
    EXPECT_EQ(L::epsilon(), TypeParam::from_raw_value(1));
    EXPECT_EQ(L::round_error(), TypeParam(0.5));
    EXPECT_EQ(L::denorm_min(), TL::min());
}

// Verify that a a type with a single integral bit works correctly
TEST(customizations, numeric_limits_edge)
{
    using Q15 = fpm::fixed<std::int16_t, std::int32_t, 15>;
    EXPECT_TRUE(HasMaximumError(static_cast<double>(std::numeric_limits<Q15>::max()), 0.999, 0.01));
    EXPECT_EQ(-1.0, static_cast<double>(std::numeric_limits<Q15>::lowest()));

    using Q31 = fpm::fixed<std::int32_t, std::int64_t, 31>;
    EXPECT_TRUE(HasMaximumError(static_cast<double>(std::numeric_limits<Q31>::max()), 0.999, 0.01));
    EXPECT_EQ(-1.0, static_cast<double>(std::numeric_limits<Q31>::lowest()));
}