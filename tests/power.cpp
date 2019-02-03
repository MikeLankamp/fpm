#include "common.h"
#include <cstdint>

TEST(power, sqrt)
{
    // For several circles, verify that the error of fixed-point sine is close to std::sin.
    using P = fpm::fixed<std::int32_t, std::int64_t, 12>;

    // Maximum relative error (percentage) we allow
    constexpr auto MAX_ERROR_PERC = 0.001;

    // Small numbers
    for (double value = 0; value <= 100; value += 0.01)
    {
        auto sqrt_real = std::sqrt(value);
        auto sqrt_fixed = static_cast<double>(sqrt(P(value)));
        EXPECT_TRUE(HasMaximumError(sqrt_fixed, sqrt_real, MAX_ERROR_PERC));
    }

    // Larger numbers, step by PI/10 to get an irregular pattern
    for (double value = 0; value <= 10000; value += 0.3141593)
    {
        auto sqrt_real = std::sqrt(value);
        auto sqrt_fixed = static_cast<double>(sqrt(P(value)));
        EXPECT_TRUE(HasMaximumError(sqrt_fixed, sqrt_real, MAX_ERROR_PERC));
    }

#ifndef NDEBUG
    EXPECT_DEATH(sqrt(P(-1)), "");
#endif
}
