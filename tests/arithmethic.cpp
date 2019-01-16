#include "common.h"
#include <gtest/gtest.h>
#include <cstdint>

TEST(arithmethic, negation)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P(-13.125), -P( 13.125));
    EXPECT_EQ(P( 13.125), -P(-13.125));
}

TEST(arithmethic, addition)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P(10.75), P(3.5) + P(7.25));
}

TEST(arithmethic, subtraction)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P(-3.75), P(3.5) - P(7.25));
}

TEST(arithmethic, multiplication)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P(-25.375), P(3.5) * P(-7.25));
}

TEST(arithmethic, division)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P( 3.5 /  7.25), P( 3.5) / P( 7.25));
    EXPECT_EQ(P(-3.5 /  7.25), P(-3.5) / P( 7.25));
    EXPECT_EQ(P( 3.5 / -7.25), P( 3.5) / P(-7.25));
    EXPECT_EQ(P(-3.5 / -7.25), P(-3.5) / P(-7.25));
}