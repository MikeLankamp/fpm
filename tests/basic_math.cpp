#include "common.hpp"
#include <fpm/math.hpp>

TEST(basic_math, abs)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(13.125), abs(P(-13.125)));
    EXPECT_EQ(P(13.125), abs(P(13.125)));
    EXPECT_EQ(P(1), abs(P(-1)));
    EXPECT_EQ(P(1), abs(P(1)));
}

TEST(basic_math, fmod)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P( 1.5), fmod(P( 9.5), P( 2)));
    EXPECT_EQ(P(-1.5), fmod(P(-9.5), P( 2)));
    EXPECT_EQ(P( 1.5), fmod(P( 9.5), P(-2)));
    EXPECT_EQ(P(-1.5), fmod(P(-9.5), P(-2)));
}

TEST(basic_math, remainder)
{
    using P = fpm::fixed_24_8;

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

TEST(basic_math, remquo)
{
    // remquo must return at least 3 bits of quotient
    constexpr int QUO_MIN_SIZE = 1 << 3;

    using P = fpm::fixed_16_16;

    int quo = 999999;
    EXPECT_EQ(P( 1.5), remquo(P( 9.5), P( 2), &quo));
    EXPECT_EQ( 4, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-1.5), remquo(P(-9.5), P( 2), &quo));
    EXPECT_EQ(-4, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P( 1.5), remquo(P( 9.5), P(-2), &quo));
    EXPECT_EQ(-4, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-1.5), remquo(P(-9.5), P(-2), &quo));
    EXPECT_EQ( 4, quo % QUO_MIN_SIZE);

    EXPECT_EQ(P( 1), remquo(P( 9), P( 2), &quo));
    EXPECT_EQ( 4, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-1), remquo(P(-9), P( 2), &quo));
    EXPECT_EQ(-4, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P( 1), remquo(P( 9), P(-2), &quo));
    EXPECT_EQ(-4, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-1), remquo(P(-9), P(-2), &quo));
    EXPECT_EQ( 4, quo % QUO_MIN_SIZE);

    EXPECT_EQ(P( 1), remquo(P( 11), P( 2), &quo));
    EXPECT_EQ( 5, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-1), remquo(P(-11), P( 2), &quo));
    EXPECT_EQ(-5, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P( 1), remquo(P( 11), P(-2), &quo));
    EXPECT_EQ(-5, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-1), remquo(P(-11), P(-2), &quo));
    EXPECT_EQ( 5, quo % QUO_MIN_SIZE);

    EXPECT_EQ(P( 2.1), remquo(P( 5.1), P( 3), &quo));
    EXPECT_EQ( 1, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-2.1), remquo(P(-5.1), P( 3), &quo));
    EXPECT_EQ(-1, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P( 2.1), remquo(P( 5.1), P(-3), &quo));
    EXPECT_EQ(-1, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-2.1), remquo(P(-5.1), P(-3), &quo));
    EXPECT_EQ( 1, quo % QUO_MIN_SIZE);

    EXPECT_EQ(P( 3.375), remquo(P( 97.125), P( 3.75), &quo));
    EXPECT_EQ( 1, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-3.375), remquo(P(-97.125), P( 3.75), &quo));
    EXPECT_EQ(-1, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P( 3.375), remquo(P( 97.125), P(-3.75), &quo));
    EXPECT_EQ(-1, quo % QUO_MIN_SIZE);
    EXPECT_EQ(P(-3.375), remquo(P(-97.125), P(-3.75), &quo));
    EXPECT_EQ( 1, quo % QUO_MIN_SIZE);

    EXPECT_EQ(P(0), remquo(P(0), P(1), &quo));
    EXPECT_EQ(0, quo % QUO_MIN_SIZE);
}
