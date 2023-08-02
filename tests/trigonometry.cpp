#include "common.hpp"
#include <fpm/math.hpp>

TEST(trigonometry, sin)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 16>;
    const double PI = std::acos(-1);

    constexpr auto MAX_ERROR_PERC = 0.002;

    for (int angle = -1799; angle <= 1800; ++angle)
    {
        auto flt_angle = angle * PI / 180;
        auto sin_real = std::sin(flt_angle);
        auto sin_fixed = static_cast<double>(sin(P(flt_angle)));
        EXPECT_TRUE(HasMaximumError(sin_fixed, sin_real, MAX_ERROR_PERC));
    }
}

TEST(trigonometry, cos)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 16>;
    const double PI = std::acos(-1);

    constexpr auto MAX_ERROR_PERC = 0.002;

    for (int angle = -1799; angle <= 1800; ++angle)
    {
        auto flt_angle = angle * PI / 180;
        auto cos_real = std::cos(flt_angle);
        auto cos_fixed = static_cast<double>(cos(P(flt_angle)));
        EXPECT_TRUE(HasMaximumError(cos_fixed, cos_real, MAX_ERROR_PERC));
    }

    // Boundary-value analysis
    // x = 2147380704 generates an overflow when calculating cos(x) = sin(x+π/2). Result: -0.9231 instead of 0.9279.
    constexpr int32_t raw_values[] = {INT32_MIN, INT32_MAX, 2147380704 };

    for (auto raw_value : raw_values)
    {
        constexpr auto MAX_ERROR_PERC = 0.0492;  // 4.92% = Maximum relative deviation over the value range of f_16_16
        P angle_fixed = P::from_raw_value(raw_value);
        auto angle_real = static_cast<double>(angle_fixed);
        auto cos_fixed = static_cast<double>(cos(angle_fixed));
        auto cos_real = std::cos(angle_real);
        auto diff = std::abs(cos_fixed - cos_real);
        EXPECT_TRUE(HasMaximumError(cos_fixed, cos_real, MAX_ERROR_PERC)) << ", raw_value=" << raw_value << ", error=" << std::abs(diff/cos_real)*100 << "%";
    }
}

TEST(trigonometry, tan)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 16>;
    const double PI = std::acos(-1);

    constexpr auto MAX_ERROR_PERC = 0.002;

    for (int angle = -1799; angle <= 1800; ++angle)
    {
        // Tangent goes to infinite at 90 and -90 degrees.
        // We can't represent that with fixed-point maths, so don't test for it.
        if ((angle + 90) % 180 != 0)
        {
            auto flt_angle = angle * PI / 180;
            auto tan_real = std::tan(flt_angle);
            auto tan_fixed = static_cast<double>(tan(P(flt_angle)));
            EXPECT_TRUE(HasMaximumError(tan_fixed, tan_real, MAX_ERROR_PERC));
        }
    }

    // Boundary-value analysis
    // x = 2147380704 generates an overflow when calculating tan(x) with cos(x)=sin(x+π/2). cos(x) result: -0.9231 instead of 0.9279.
    constexpr int32_t raw_values[] = {INT32_MIN, INT32_MAX, 2147380704 };
    for (auto raw_value : raw_values)
    {
        constexpr auto MAX_ERROR_PERC = 0.0492;  // 4.92% = Maximum relative deviation over the value range of f_16_16
        P angle_fixed = P::from_raw_value(raw_value);
        auto angle_real = static_cast<double>(angle_fixed);
        auto tan_fixed = static_cast<double>(tan(angle_fixed));
        auto tan_real = std::tan(angle_real);
        auto diff = std::abs(tan_fixed - tan_real);
        EXPECT_TRUE(HasMaximumError(tan_fixed, tan_real, MAX_ERROR_PERC)) << ", raw_value=" << raw_value << ", error=" << std::abs(diff/tan_real)*100 << "%";
    }

#ifndef NDEBUG
    EXPECT_DEATH(tan(P::pi()/2), "");
    EXPECT_DEATH(tan(-P::pi()/2), "");
#endif
}

TEST(trigonometry, atan)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 12>;

    constexpr auto MAX_ERROR_PERC = 0.025;

    for (int x = -1000; x <= 1000; ++x)
    {
        auto value = x / 10.0;
        auto atan_real = std::atan(value);
        auto atan_fixed = static_cast<double>(atan(P(value)));
        EXPECT_TRUE(HasMaximumError(atan_fixed, atan_real, MAX_ERROR_PERC));
    }

    for (int x = -1000; x <= 1000; ++x)
    {
        auto value = x / 1000.0;
        auto atan_real = std::atan(value);
        auto atan_fixed = static_cast<double>(atan(P(value)));
        EXPECT_TRUE(HasMaximumError(atan_fixed, atan_real, MAX_ERROR_PERC));
    }
}

TEST(trigonometry, asin)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 12>;

    constexpr auto MAX_ERROR_PERC = 0.025;

    for (int x = -1000; x <= 1000; ++x)
    {
        auto value = x / 1000.0;
        auto asin_real = std::asin(value);
        auto asin_fixed = static_cast<double>(asin(P(value)));
        EXPECT_TRUE(HasMaximumError(asin_fixed, asin_real, MAX_ERROR_PERC));
    }
}

TEST(trigonometry, acos)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 12>;

    constexpr auto MAX_ERROR_PERC = 0.025;

    for (int x = -1000; x <= 1000; ++x)
    {
        auto value = x / 1000.0;
        auto acos_real = std::acos(value);
        auto acos_fixed = static_cast<double>(acos(P(value)));
        EXPECT_TRUE(HasMaximumError(acos_fixed, acos_real, MAX_ERROR_PERC));
    }
}

TEST(trigonometry, atan2)
{
    using P = fpm::fixed<std::int32_t, std::int64_t, 12>;
    const double PI = std::acos(-1);

    constexpr auto MAX_ERROR_PERC = 0.025;

    for (int angle = -1799; angle <= 1800; ++angle)
    {
        const auto y = std::sin(angle * PI / 1800);
        const auto x = std::cos(angle * PI / 1800);

        auto atan2_real = std::atan2(y, x);
        auto atan2_fixed = static_cast<double>(atan2(P(y), P(x)));
        EXPECT_TRUE(HasMaximumError(atan2_fixed, atan2_real, MAX_ERROR_PERC));
    }

#ifndef NDEBUG
    EXPECT_DEATH(atan2(P(0), P(0)), "");
#endif
}

// Naively, atan2(y, x) does y / x which would overflow for near-zero x with Q16.16.
// Test that we've got protections in place for this.
TEST(trigonometry, atan2_near_zero)
{
    constexpr auto MAX_ERROR_PERC = 0.025;
    using P = fpm::fixed_16_16;

    const auto x = P::from_raw_value(1);
    const auto y = P(100);

    // Positive x
    {
        auto atan2_real = std::atan2(static_cast<double>(y), static_cast<double>(x));
        auto atan2_fixed = static_cast<double>(atan2(y, x));
        EXPECT_TRUE(HasMaximumError(atan2_fixed, atan2_real, MAX_ERROR_PERC));
    }

    // Negative x
    {
        auto atan2_real = std::atan2(static_cast<double>(y), static_cast<double>(-x));
        auto atan2_fixed = static_cast<double>(atan2(y, -x));
        EXPECT_TRUE(HasMaximumError(atan2_fixed, atan2_real, MAX_ERROR_PERC));
    }
}
