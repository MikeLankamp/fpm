#include "common.h"
#include <cstdint>

TEST(manipulation, copysign)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(-13.125), copysign(P(-13.125), P(-7.25)));
    EXPECT_EQ(P(-13.125), copysign(P( 13.125), P(-7.25)));
    EXPECT_EQ(P( 13.125), copysign(P(-13.125), P( 7.25)));
    EXPECT_EQ(P( 13.125), copysign(P( 13.125), P( 7.25)));

    EXPECT_EQ(P(-13), copysign(P(-13), P(-7)));
    EXPECT_EQ(P(-13), copysign(P( 13), P(-7)));
    EXPECT_EQ(P( 13), copysign(P(-13), P( 7)));
    EXPECT_EQ(P( 13), copysign(P( 13), P( 7)));
}
