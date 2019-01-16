#include "common.h"
#include <gtest/gtest.h>
#include <cstdint>

TEST(manipulation, copysign)
{
    using P = fpm::fixed<std::int32_t, 8>;

    EXPECT_EQ(P(-13.125), copysign(P(-13.125), P(-7.25)));
    EXPECT_EQ(P(-13.125), copysign(P( 13.125), P(-7.25)));
    EXPECT_EQ(P( 13.125), copysign(P(-13.125), P( 7.25)));
    EXPECT_EQ(P( 13.125), copysign(P( 13.125), P( 7.25)));

    EXPECT_EQ(P(-13), copysign(P(-13), P(-7)));
    EXPECT_EQ(P(-13), copysign(P( 13), P(-7)));
    EXPECT_EQ(P( 13), copysign(P(-13), P( 7)));
    EXPECT_EQ(P( 13), copysign(P( 13), P( 7)));
}
