#include "common.hpp"
#include <fpm/math.hpp>

TEST(nearest, round)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P( 2), round(P( 2.3)));
    EXPECT_EQ(P( 3), round(P( 2.5)));
    EXPECT_EQ(P( 3), round(P( 2.7)));
    EXPECT_EQ(P(-2), round(P(-2.3)));
    EXPECT_EQ(P(-3), round(P(-2.5)));
    EXPECT_EQ(P(-3), round(P(-2.7)));
    EXPECT_EQ(P( 0), round(P( 0)));
}

TEST(nearest, ceil)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P( 1), ceil(P( 1)));
    EXPECT_EQ(P(-1), ceil(P(-1)));

    EXPECT_EQ(P( 3), ceil(P( 2.4)));
    EXPECT_EQ(P(-2), ceil(P(-2.4)));
    EXPECT_EQ(P( 0), ceil(P( 0)));
}

TEST(nearest, floor)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P( 1), floor(P( 1)));
    EXPECT_EQ(P(-1), floor(P(-1)));

    EXPECT_EQ(P( 2), floor(P( 2.7)));
    EXPECT_EQ(P(-3), floor(P(-2.7)));
    EXPECT_EQ(P( 0), floor(P( 0)));
}

TEST(nearest, trunc)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P( 1), trunc(P( 1)));
    EXPECT_EQ(P(-1), trunc(P(-1)));

    EXPECT_EQ(P( 2), trunc(P( 2.7)));
    EXPECT_EQ(P(-2), trunc(P(-2.9)));
    EXPECT_EQ(P( 0), trunc(P( 0)));
}

TEST(nearest, nearbyint)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P( 2), nearbyint(P( 2.3)));
    EXPECT_EQ(P( 2), nearbyint(P( 2.5)));
    EXPECT_EQ(P( 4), nearbyint(P( 3.5)));
    EXPECT_EQ(P(-2), nearbyint(P(-2.3)));
    EXPECT_EQ(P(-2), nearbyint(P(-2.5)));
    EXPECT_EQ(P(-4), nearbyint(P(-3.5)));
    EXPECT_EQ(P( 0), nearbyint(P( 0)));
}

TEST(nearest, rint)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P( 2), rint(P( 2.3)));
    EXPECT_EQ(P( 2), rint(P( 2.5)));
    EXPECT_EQ(P( 4), rint(P( 3.5)));
    EXPECT_EQ(P(-2), rint(P(-2.3)));
    EXPECT_EQ(P(-2), rint(P(-2.5)));
    EXPECT_EQ(P(-4), rint(P(-3.5)));
    EXPECT_EQ(P( 0), rint(P( 0)));
}

