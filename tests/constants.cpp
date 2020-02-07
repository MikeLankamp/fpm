#include "common.hpp"

using P = fpm::fixed_16_16;

TEST(constants, e)
{
    EXPECT_EQ(P(2.7182818284590452353602874713527), P::e());
}

TEST(constants, pi)
{
    EXPECT_EQ(P(3.1415926535897932384626433832795), P::pi());
}

TEST(constants, half_pi)
{
    EXPECT_EQ(P(1.5707963267948966192313216916398), P::half_pi());
}

TEST(constants, two_pi)
{
    EXPECT_EQ(P(6.283185307179586476925286766559), P::two_pi());
}
