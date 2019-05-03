#ifndef FPM_FIXED_H
#define FPM_FIXED_H

#include <cassert>
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace fpm
{

//! Fixed-point number type
//! \tparam BaseType         the base integer type used to store the fixed-point number. This can be a signed or unsigned type.
//! \tparam IntermediateType the integer type used to store intermediate results during calculations.
//! \tparam FractionBits     the number of bits of the BaseType used to store the fraction
template <typename BaseType, typename IntermediateType, unsigned int FractionBits>
class fixed
{
    static_assert(std::is_integral<BaseType>::value, "BaseType must be an integral type");
    static_assert(FractionBits > 0, "FractionBits must be greater than zero");
    static_assert(FractionBits <= sizeof(BaseType) * 8, "BaseType must at least be able to contain entire fraction");
    static_assert(sizeof(IntermediateType) > sizeof(BaseType), "IntermediateType must be larger than BaseType");
    static_assert(std::is_signed<IntermediateType>::value == std::is_signed<BaseType>::value, "IntermediateType must have same signedness as BaseType");

    static constexpr BaseType FRACTION_MULT = BaseType(1) << FractionBits;

    struct raw_construct_tag {};
    constexpr inline fixed(BaseType val, raw_construct_tag) noexcept : m_value(val) {}

public:
    static const fixed E;
    static const fixed PI;
    static const fixed HALF_PI;
    static const fixed TWO_PI;

    constexpr inline fixed() noexcept {}

    // Converts an integral number to the fixed-point type.
    // Like static_cast, this truncates bits that don't fit.
    template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    constexpr inline explicit fixed(T val) noexcept
        : m_value(static_cast<BaseType>(val * FRACTION_MULT))
    {}

    // Converts an floating-point number to the fixed-point type.
    // Like static_cast, this truncates bits that don't fit.
    template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    constexpr inline explicit fixed(T val) noexcept
        : m_value(static_cast<BaseType>(std::round(val * FRACTION_MULT)))
    {}

    // Explicit conversion to a floating-point type
    template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    constexpr inline explicit operator T() const noexcept
    {
        return static_cast<T>(m_value) / FRACTION_MULT;
    }

    // Explicit conversion to an integral type
    template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    constexpr inline explicit operator T() const noexcept
    {
        return static_cast<T>(m_value / FRACTION_MULT);
    }

    // Returns the raw underlying value of this type.
    // Do not use this unless you know what you're doing.
    constexpr inline BaseType raw_value() const noexcept
    {
        return m_value;
    }

    // Constructs a fixed-point number from its raw underlying value.
    // Do not use this unless you know what you're doing.
    static constexpr inline fixed from_raw_value(BaseType value) noexcept
    {
        return fixed(value, raw_construct_tag{});
    }

    //
    // Arithmetic member operators
    //

    constexpr inline fixed operator-() const noexcept
    {
        return fixed::from_raw_value(-m_value);
    }

    inline fixed& operator+=(const fixed& y) noexcept
    {
        m_value += y.m_value;
        return *this;
    }

    template <typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
    inline fixed& operator+=(I y) noexcept
    {
        m_value += y * FRACTION_MULT;
        return *this;
    }

    inline fixed& operator-=(const fixed& y) noexcept
    {
        m_value -= y.m_value;
        return *this;
    }

    template <typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
    inline fixed& operator-=(I y) noexcept
    {
        m_value -= y * FRACTION_MULT;
        return *this;
    }

    inline fixed& operator*=(const fixed& y) noexcept
    {
        // Normal fixed-point multiplication is: x * y / 2**FractionBits.
        // To correctly round the last bit in the result, we need one more bit of information.
        // We do this by multiplying by two before dividing and adding the LSB to the real result.
        auto value = (static_cast<IntermediateType>(m_value) * y.m_value) / (FRACTION_MULT / 2);
        m_value = static_cast<BaseType>((value / 2) + (value % 2));
        return *this;
    }

    template <typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
    inline fixed& operator*=(I y) noexcept
    {
        m_value *= y;
        return *this;
    }

    inline fixed& operator/=(const fixed& y) noexcept
    {
        assert(y.m_value != 0);
        // Normal fixed-point division is: x * 2**FractionBits / y.
        // To correctly round the last bit in the result, we need one more bit of information.
        // We do this by multiplying by two before dividing and adding the LSB to the real result.
        auto value = (static_cast<IntermediateType>(m_value) * FRACTION_MULT * 2) / y.m_value;
        m_value = static_cast<BaseType>((value / 2) + (value % 2));
        return *this;
    }

    template <typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
    inline fixed& operator/=(I y) noexcept
    {
        m_value /= y;
        return *this;
    }

private:
    BaseType m_value;
};

template <typename BaseType, typename IntermediateType, unsigned int FractionBits>
const fixed<BaseType, IntermediateType, FractionBits> fixed<BaseType, IntermediateType, FractionBits>::E(2.7182818284590452353602874713527);

template <typename BaseType, typename IntermediateType, unsigned int FractionBits>
const fixed<BaseType, IntermediateType, FractionBits> fixed<BaseType, IntermediateType, FractionBits>::PI(3.1415926535897932384626433832795);

template <typename BaseType, typename IntermediateType, unsigned int FractionBits>
const fixed<BaseType, IntermediateType, FractionBits> fixed<BaseType, IntermediateType, FractionBits>::HALF_PI(1.5707963267948966192313216916398);

template <typename BaseType, typename IntermediateType, unsigned int FractionBits>
const fixed<BaseType, IntermediateType, FractionBits> fixed<BaseType, IntermediateType, FractionBits>::TWO_PI(6.283185307179586476925286766559);

//
// Convenience typedefs
//

using fixed_16_16 = fixed<std::int32_t, std::int64_t, 16>;
using fixed_24_8 = fixed<std::int32_t, std::int64_t, 8>;
using fixed_8_24 = fixed<std::int32_t, std::int64_t, 24>;

//
// Addition
//

template <typename B, typename I, unsigned int F>
constexpr inline fixed<B, I, F> operator+(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return fixed<B, I, F>(x) += y;
}

template <typename B, typename I, unsigned int F, typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr inline fixed<B, I, F> operator+(const fixed<B, I, F>& x, T y) noexcept
{
    return fixed<B, I, F>(x) += y;
}

template <typename B, typename I, unsigned int F, typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr inline fixed<B, I, F> operator+(T x, const fixed<B, I, F>& y) noexcept
{
    return fixed<B, I, F>(y) += x;
}

//
// Subtraction
//

template <typename B, typename I, unsigned int F>
constexpr inline fixed<B, I, F> operator-(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return fixed<B, I, F>(x) -= y;
}

template <typename B, typename I, unsigned int F, typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr inline fixed<B, I, F> operator-(const fixed<B, I, F>& x, T y) noexcept
{
    return fixed<B, I, F>(x) -= y;
}

template <typename B, typename I, unsigned int F, typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr inline fixed<B, I, F> operator-(T x, const fixed<B, I, F>& y) noexcept
{
    return fixed<B, I, F>(x) -= y;
}

//
// Multiplication
//

template <typename B, typename I, unsigned int F>
constexpr inline fixed<B, I, F> operator*(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return fixed<B, I, F>(x) *= y;
}

template <typename B, typename I, unsigned int F, typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr inline fixed<B, I, F> operator*(const fixed<B, I, F>& x, T y) noexcept
{
    return fixed<B, I, F>(x) *= y;
}

template <typename B, typename I, unsigned int F, typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr inline fixed<B, I, F> operator*(T x, const fixed<B, I, F>& y) noexcept
{
    return fixed<B, I, F>(y) *= x;
}

//
// Division
//

template <typename B, typename I, unsigned int F>
constexpr inline fixed<B, I, F> operator/(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return fixed<B, I, F>(x) /= y;
}

template <typename B, typename I, unsigned int F, typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr inline fixed<B, I, F> operator/(const fixed<B, I, F>& x, T y) noexcept
{
    return fixed<B, I, F>(x) /= y;
}

template <typename B, typename I, unsigned int F, typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
constexpr inline fixed<B, I, F> operator/(T x, const fixed<B, I, F>& y) noexcept
{
    return fixed<B, I, F>(x) /= y;
}

//
// Comparison operators
//

template <typename B, typename I, unsigned int F>
constexpr inline bool operator==(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return x.raw_value() == y.raw_value();
}

template <typename B, typename I, unsigned int F>
constexpr inline bool operator!=(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return x.raw_value() != y.raw_value();
}

template <typename B, typename I, unsigned int F>
constexpr inline bool operator<(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return x.raw_value() < y.raw_value();
}

template <typename B, typename I, unsigned int F>
constexpr inline bool operator>(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return x.raw_value() > y.raw_value();
}

template <typename B, typename I, unsigned int F>
constexpr inline bool operator<=(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return x.raw_value() <= y.raw_value();
}

template <typename B, typename I, unsigned int F>
constexpr inline bool operator>=(const fixed<B, I, F>& x, const fixed<B, I, F>& y) noexcept
{
    return x.raw_value() >= y.raw_value();
}

}

#endif
