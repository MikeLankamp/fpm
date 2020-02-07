#include "common.hpp"

using P = fpm::fixed_16_16;

TEST(conversion, construction)
{
    P x;
}

TEST(conversion, floats)
{
    EXPECT_EQ(1.125, static_cast<double>(P{1.125f}));
    EXPECT_EQ(1.125, static_cast<double>(P{1.125}));
}

TEST(conversion, ints)
{
    EXPECT_EQ(-125, static_cast<int>(P{-125}));
    EXPECT_EQ(-125l, static_cast<long>(P{-125l}));
    EXPECT_EQ(-125ll, static_cast<long long>(P{-125ll}));

    EXPECT_EQ(125u, static_cast<unsigned int>(P{125u}));
    EXPECT_EQ(125lu, static_cast<unsigned long>(P{125lu}));
    EXPECT_EQ(125llu, static_cast<unsigned long long>(P{125llu}));
}

TEST(conversion, fixed_point)
{
    EXPECT_EQ(P(-1), P::from_fixed_point<0>(-1));
    EXPECT_EQ(P(1), P::from_fixed_point<0>(1));

    EXPECT_EQ(P(-1.125), P::from_fixed_point<4>(-18));
    EXPECT_EQ(P(1.125), P::from_fixed_point<4>(18));

    // This should round up to 1
    EXPECT_EQ(P(-1), P::from_fixed_point<20>(-1048575));
    EXPECT_EQ(P(1), P::from_fixed_point<20>(1048575));
}
