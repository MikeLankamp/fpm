#include "common.hpp"
#include <utility>

using P = fpm::fixed_16_16;
using Q = fpm::fixed_24_8;

TEST(conversion, construction)
{
    P x;
}

TEST(conversion, copy)
{
    const P x(12);

    // Copy ctor
    P y(x);
    EXPECT_EQ(P(12), y);

    // Copy assignment
    P z = x;
    EXPECT_EQ(P(12), y);
}

TEST(conversion, move)
{
    const P x(12);

    // Move ctor
    P y(std::move(x));
    EXPECT_EQ(P(12), y);

    // Move assignment
    P z = std::move(x);
    EXPECT_EQ(P(12), y);
}

TEST(conversion, floats)
{
    EXPECT_EQ(1.125, static_cast<double>(P{1.125f}));
    EXPECT_EQ(1.125, static_cast<double>(P{1.125}));
}

TEST(conversion, float_rounding)
{
    // Small number of fraction bits to test rounding
    using Q = fpm::fixed<std::int32_t, std::int64_t, 2>;

    EXPECT_EQ(1.25, static_cast<double>(Q{1.125}));
    EXPECT_EQ(1.5, static_cast<double>(Q{1.375}));
    EXPECT_EQ(-1.25, static_cast<double>(Q{-1.125}));
    EXPECT_EQ(-1.5, static_cast<double>(Q{-1.375}));
}

TEST(conversion, float_no_rounding)
{
    // Small number of fraction bits to test no rounding
    using Q = fpm::fixed<std::int32_t, std::int64_t, 2, false>;

    EXPECT_EQ(1.0, static_cast<double>(Q{1.125}));
    EXPECT_EQ(1.25, static_cast<double>(Q{1.375}));
    EXPECT_EQ(1.25, static_cast<double>(Q{1.499}));
    EXPECT_EQ(-1.0, static_cast<double>(Q{-1.125}));
    EXPECT_EQ(-1.0, static_cast<double>(Q{-1.249}));
    EXPECT_EQ(-1.25, static_cast<double>(Q{-1.375}));
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

TEST(conversion, fixed_point_no_rounding)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 16, false>;
    constexpr P epsilon = std::numeric_limits<P>::epsilon();

    EXPECT_EQ(P(-1), P::from_fixed_point<0>(-1));
    EXPECT_EQ(P(1), P::from_fixed_point<0>(1));

    EXPECT_EQ(P(-1.125), P::from_fixed_point<4>(-18));
    EXPECT_EQ(P(1.125), P::from_fixed_point<4>(18));

    // This should NOT round up to 1: there will be a truncation error equal to epsilon
    EXPECT_EQ(P(-1 + epsilon), P::from_fixed_point<20>(-1048575));
    EXPECT_EQ(P(1 - epsilon), P::from_fixed_point<20>(1048575));
}

TEST(conversion, fixed_to_fixed)
{
    EXPECT_EQ(Q(1), Q(P(1)));
    EXPECT_EQ(Q(1), Q(P(1)));

    // Conversion to fewer fraction bits should round
    EXPECT_EQ(Q::from_raw_value(0x13), Q(P::from_raw_value(0x12ff)));
    EXPECT_EQ(Q::from_raw_value(0x12), Q(P::from_raw_value(0x127f)));
    EXPECT_EQ(Q::from_raw_value(-0x13), Q(P::from_raw_value(-0x12ff)));
    EXPECT_EQ(Q::from_raw_value(-0x12), Q(P::from_raw_value(-0x127f)));

    // Conversion to more fraction bits should zero-extend
    EXPECT_EQ(P::from_raw_value(0x1200), P(Q::from_raw_value(0x12)));
    EXPECT_EQ(P::from_raw_value(-0x1200), P(Q::from_raw_value(-0x12)));

    {
        // Assignment requires explicit conversion via construction
        P p(1);
        Q q = Q(p);
        EXPECT_EQ(Q(1), q);
    }

    // Conversion to a smaller base type should truncate the upper bits
    using S1 = fpm::fixed<std::int8_t, std::int16_t, 1>;
    EXPECT_EQ(0x56, S1(P::from_raw_value(0x79AB1000)).raw_value());
    EXPECT_EQ(-0x56, S1(P::from_raw_value(-0x79AB1000)).raw_value());
}
