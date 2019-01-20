#include "common.h"
#include <gtest/gtest.h>
#include <cstdint>

TEST(basic_math, abs)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P(13.125), abs(P(-13.125)));
    EXPECT_EQ(P(13.125), abs(P(13.125)));
    EXPECT_EQ(P(1), abs(P(-1)));
    EXPECT_EQ(P(1), abs(P(1)));
}

TEST(basic_math, fmod)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P( 1.5), fmod(P( 9.5), P( 2)));
    EXPECT_EQ(P(-1.5), fmod(P(-9.5), P( 2)));
    EXPECT_EQ(P( 1.5), fmod(P( 9.5), P(-2)));
    EXPECT_EQ(P(-1.5), fmod(P(-9.5), P(-2)));
}

TEST(basic_math, remainder)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P(-0.5), remainder(P( 9.5), P( 2)));
    EXPECT_EQ(P( 0.5), remainder(P(-9.5), P( 2)));
    EXPECT_EQ(P(-0.5), remainder(P( 9.5), P(-2)));
    EXPECT_EQ(P( 0.5), remainder(P(-9.5), P(-2)));

    EXPECT_EQ(P( 1), remainder(P( 9), P( 2)));
    EXPECT_EQ(P(-1), remainder(P(-9), P( 2)));
    EXPECT_EQ(P( 1), remainder(P( 9), P(-2)));
    EXPECT_EQ(P(-1), remainder(P(-9), P(-2)));

    EXPECT_EQ(P(-1), remainder(P( 11), P( 2)));
    EXPECT_EQ(P( 1), remainder(P(-11), P( 2)));
    EXPECT_EQ(P(-1), remainder(P( 11), P(-2)));
    EXPECT_EQ(P( 1), remainder(P(-11), P(-2)));

    EXPECT_EQ(P(-0.9), remainder(P( 5.1), P( 3)));
    EXPECT_EQ(P( 0.9), remainder(P(-5.1), P( 3)));
    EXPECT_EQ(P(-0.9), remainder(P( 5.1), P(-3)));
    EXPECT_EQ(P( 0.9), remainder(P(-5.1), P(-3)));

    EXPECT_EQ(P(0), remainder(P(0), P(1)));
}
