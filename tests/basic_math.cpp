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
