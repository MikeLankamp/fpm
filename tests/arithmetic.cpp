#include "common.hpp"

TEST(arithmethic, negation)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(-13.125), -P( 13.125));
    EXPECT_EQ(P( 13.125), -P(-13.125));
}

TEST(arithmethic, addition)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(10.75), P(3.5) + P(7.25));
}

TEST(arithmethic, subtraction)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(-3.75), P(3.5) - P(7.25));
}

TEST(arithmethic, multiplication)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(-25.375), P(3.5) * P(-7.25));
}

TEST(arithmethic, division)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(3.5 / 7.25), P(3.5) / P(7.25));
    EXPECT_EQ(P(-3.5 / 7.25), P(-3.5) / P(7.25));
    EXPECT_EQ(P(3.5 / -7.25), P(3.5) / P(-7.25));
    EXPECT_EQ(P(-3.5 / -7.25), P(-3.5) / P(-7.25));

#ifndef NDEBUG
    EXPECT_DEATH(P(1) / P(0), "");
#endif
}

TEST(arithmethic, division_range)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 12>;

    // These calculation will overflow and produce
    // wrong results without the intermediate type.
    EXPECT_EQ(P(32), P(256) / P(8));
}

TEST(arithmetic, multiplication_rounding)
{
    // Using 1 bit of fractional precision to test rounding
    using Q_round = fpm::fixed<std::int32_t, std::int64_t, 1, true>;
    using Q = fpm::fixed<std::int32_t, std::int64_t, 1, false>;

    EXPECT_EQ(Q_round(1.0), Q_round(1.5) * Q_round(0.5));
    EXPECT_EQ(Q_round(0.5), Q_round(0.5) * Q_round(0.5));
    EXPECT_EQ(Q(0.5), Q(1.5) * Q(0.5));
    EXPECT_EQ(Q(0.0), Q(0.5) * Q(0.5));
}

TEST(arithmetic, division_rounding)
{
    // Using 1 bit of fractional precision to test rounding
    using Q_round = fpm::fixed<std::int32_t, std::int64_t, 1, true>;
    using Q = fpm::fixed<std::int32_t, std::int64_t, 1, false>;

    EXPECT_EQ(Q_round(2.5), Q_round(3.5) / Q_round(1.5));
    EXPECT_EQ(Q_round(0.5), Q_round(1.0) / Q_round(1.5));
    EXPECT_EQ(Q(2.0), Q(3.5) / Q(1.5));
    EXPECT_EQ(Q(0.5), Q(1.0) / Q(1.5));
}
