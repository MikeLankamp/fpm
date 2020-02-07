#include "common.hpp"

TEST(arithmethic_int, addition)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(10.5), P(3.5) + 7);
}

TEST(arithmethic_int, subtraction)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(-3.5), P(3.5) - 7);
}

TEST(arithmethic_int, multiplication)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(-24.5), P(3.5) * -7);
}

TEST(arithmethic_int, division)
{
    using P = fpm::fixed_24_8;

    EXPECT_EQ(P(3.5 / 7), P(3.5) / 7);
    EXPECT_EQ(P(-3.5 / 7), P(-3.5) / 7);
    EXPECT_EQ(P(3.5 / -7), P(3.5) / -7);
    EXPECT_EQ(P(-3.5 / -7), P(-3.5) / -7);

#ifndef NDEBUG
    EXPECT_DEATH(P(1) / 0, "");
#endif
}

TEST(arithmethic_int, division_range)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 12>;

    // These calculation will overflow and produce
    // wrong results without the intermediate type.
    EXPECT_EQ(P(32), P(256) / 8);
}