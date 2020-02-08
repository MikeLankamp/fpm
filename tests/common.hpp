#ifndef FPM_TESTS_COMMON_HPP
#define FPM_TESTS_COMMON_HPP

#include <fpm/fixed.hpp>
#include <gtest/gtest.h>
#include <iomanip>
#include <ostream>

namespace fpm
{
template <typename B, typename I, unsigned int F>
void PrintTo(const fpm::fixed<B, I, F>& val, ::std::ostream* os)
{
    auto f = os->flags();
    *os << static_cast<double>(val)
        << " (0x" << std::hex << std::setw(sizeof(B) * 2) << std::setfill('0') << val.raw_value() << ")";
    os->flags(f);
}
}

inline ::testing::AssertionResult HasMaximumError(double value, double reference, double max_error)
{
    auto diff = std::abs(value - reference);
    if (reference < 1e-10 && diff <= max_error)
        return ::testing::AssertionSuccess();
    if (std::abs(diff / reference) <= max_error)
        return ::testing::AssertionSuccess();
    return ::testing::AssertionFailure() << value << " is not within " << (max_error * 100) << "% of " << reference;
}

#endif