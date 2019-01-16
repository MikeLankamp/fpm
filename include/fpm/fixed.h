#ifndef FPM_FIXED_H
#define FPM_FIXED_H

#include <cassert>
#include <cmath>
#include <cstdint>
#include <type_traits>

namespace fpm
{

    //! Fixed-point number type
//! \tparam BaseType     the base integer type used to store the fixed-point number. This can be a signed or unsigned type.
//! \tparam FractionBits the number of bits of the BaseType used to store the fraction
template <typename BaseType, unsigned int FractionBits>
class fixed
{
    static_assert(std::is_integral<BaseType>::value, "BaseType must be an integral type");
    static_assert(FractionBits > 0, "FractionBits must be greater than zero");
    static_assert(FractionBits <= sizeof(BaseType) * 8, "BaseType must at least be able to contain entire fraction");

    static constexpr BaseType FRACTION_MULT = BaseType(1) << FractionBits;
public:
    fixed() = default;

    // Converts an integral number to the fixed-point type.
    // Like static_cast, this truncates bits that don't fit.
    template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    explicit fixed(T val)
        : m_value(static_cast<BaseType>(val * FRACTION_MULT))
    {}

    // Converts an floating-point number to the fixed-point type.
    // Like static_cast, this truncates bits that don't fit.
    template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    explicit fixed(T val)
        : m_value(static_cast<BaseType>(std::round(val * FRACTION_MULT)))
    {}

    // Explicit conversion to a floating-point type
    template <typename T, typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr>
    explicit operator T() const
    {
        return static_cast<T>(m_value) / FRACTION_MULT;
    }

    // Explicit conversion to an integral type
    template <typename T, typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
    explicit operator T() const
    {
        return static_cast<T>(m_value / FRACTION_MULT);
    }

    // Returns the raw underlying value of this type.
    // Do not use this unless you know what you're doing.
    BaseType raw_value() const
    {
        return m_value;
    }

    //
    // Arithmetic member operators
    //

    fixed operator-() const
    {
        fixed f;
        f.m_value = -m_value;
        return f;
    }

    fixed& operator+=(const fixed& y)
    {
        m_value += y.m_value;
        return *this;
    }

    fixed& operator-=(const fixed& y)
    {
        m_value -= y.m_value;
        return *this;
    }

    fixed& operator*=(const fixed& y)
    {
        // Normal fixed-point multiplication is: x * y / 2**FractionBits.
        // To correctly round the last bit in the result, we need one more bit of information.
        // We do this by multiplying by two before dividing and adding the LSB to the real result.
        auto value = (m_value * y.m_value) / (FRACTION_MULT / 2);
        m_value = (value / 2) + (value % 2);
        return *this;
    }

    fixed& operator/=(const fixed& y)
    {
        assert(y.m_value != 0);
        // Normal fixed-point division is: x * 2**FractionBits / y.
        // To correctly round the last bit in the result, we need one more bit of information.
        // We do this by multiplying by two before dividing and adding the LSB to the real result.
        auto value = (m_value * FRACTION_MULT * 2) / y.m_value;
        m_value = (value / 2) + (value % 2);
        return *this;
    }

    fixed& operator%=(const fixed& y)
    {
        assert(y.m_value != 0);
        m_value = m_value % y.m_value;
        return *this;
    }

private:
    BaseType m_value;
};

//
// Convenience typedefs
//

using fixed_16_16 = fixed<std::int32_t, 16>;
using fixed_24_8 = fixed<std::int32_t, 24>;
using fixed_8_24 = fixed<std::int32_t, 8>;

//
// Addition
//

template <typename B, unsigned int F>
fixed<B, F> operator+(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return fixed<B, F>(x) += y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator+(const fixed<B, F>& x, I y)
{
    return fixed<B, F>(x) += y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator+(I x, const fixed<B, F>& y)
{
    return fixed<B, F>(y) += x;
}

//
// Subtraction
//

template <typename B, unsigned int F>
fixed<B, F> operator-(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return fixed<B, F>(x) -= y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator-(const fixed<B, F>& x, I y)
{
    return fixed<B, F>(x) -= y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator-(I x, const fixed<B, F>& y)
{
    return fixed<B, F>(x) -= y;
}

//
// Multiplication
//

template <typename B, unsigned int F>
fixed<B, F> operator*(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return fixed<B, F>(x) *= y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator*(const fixed<B, F>& x, I y)
{
    return fixed<B, F>(x) *= y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator*(I x, const fixed<B, F>& y)
{
    return fixed<B, F>(y) *= x;
}

//
// Division
//

template <typename B, unsigned int F>
fixed<B, F> operator/(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return fixed<B, F>(x) /= y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator/(const fixed<B, F>& x, I y)
{
    return fixed<B, F>(x) /= y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator/(I x, const fixed<B, F>& y)
{
    return fixed<B, F>(x) /= y;
}

//
// Modulo
//

template <typename B, unsigned int F>
fixed<B, F> operator%(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return fixed<B, F>(x) %= y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator%(const fixed<B, F>& x, I y)
{
    return fixed<B, F>(x) %= y;
}

template <typename B, unsigned int F, typename I, typename std::enable_if<std::is_integral<I>::value>::type* = nullptr>
fixed<B, F> operator%(I x, const fixed<B, F>& y)
{
    return fixed<B, F>(x) %= y;
}

//
// Comparison operators
//

template <typename B, unsigned int F>
bool operator==(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return x.raw_value() == y.raw_value();
}

template <typename B, unsigned int F>
bool operator!=(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return x.raw_value() != y.raw_value();
}

template <typename B, unsigned int F>
bool operator<(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return x.raw_value() < y.raw_value();
}

template <typename B, unsigned int F>
bool operator>(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return x.raw_value() > y.raw_value();
}

template <typename B, unsigned int F>
bool operator<=(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return x.raw_value() <= y.raw_value();
}

template <typename B, unsigned int F>
bool operator>=(const fixed<B, F>& x, const fixed<B, F>& y)
{
    return x.raw_value() >= y.raw_value();
}

//
// Mathematical functions
//
template <typename B, unsigned int F>
fixed<B, F> abs(fixed<B, F> x)
{
    return (x >= fixed<B, F>{0}) ? x : -x;
}

template <typename B, unsigned int F, typename C, unsigned int G>
fixed<B, F> copysign(fixed<B, F> x, fixed<C, G> y)
{
    x = abs(x);
    return (y >= fixed<C, G>{0}) ? x : -x;
}

template <typename B, unsigned int F>
fixed<B, F> fmod(fixed<B, F> x, fixed<B, F> y)
{
    return x % y;
}

}

#endif
