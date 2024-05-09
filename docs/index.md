---
layout: default
---
# fpm
A C++ header-only fixed-point math library. "fpm" stands for "fixed-point math".

It is designed to serve as a drop-in replacement for floating-point types and aims to provide as much of the standard library's functionality as possible with exclusively integers. `fpm` requires C++11 or higher.

## Usage
`fpm` defines the `fpm::fixed` class, which is templated on the underlying integer type and the number of bits in the fraction:
```c++
namespace fpm {
    template <typename BaseType, typename IntermediateType, unsigned int FractionBits>
    class fixed;
}
```
**Note:** It's recommended to use a *signed* integer type for `BaseType` (and `IntermediateType`) to emulate floating-point numbers
and to allow the compiler to optimize the computations, since overflow and underflow are undefined
for signed integer types.

To use this class, simply include its header:
```c++
#include <fpm/fixed.hpp>
```
You may wish to typedef a particular choice of underlying type, intermediate type and fraction bitcount, e.g.:
```c++
using position = fpm::fixed<std::int32_t, std::int64_t, 16>;
```
This defines a signed 16.16 fixed-point number with a range of -32768 to 32767.999985... and a resolution of 0.0000153... It uses 64-bit integers as intermediate type during calculations to avoid loss of information.

For your convenience, several popular fixed-point formats have been defined in the `fpm` namespace:
```c++
namespace fpm {
    using fixed_16_16 = fixed<std::int32_t, std::int64_t, 16>;  // Q16.16 format
    using fixed_24_8  = fixed<std::int32_t, std::int64_t, 8>;   // Q24.8 format
    using fixed_8_24  = fixed<std::int32_t, std::int64_t, 24>;  // Q8.24 format
}
```

## Mathematical functions
FPM offers the header `<fpm/math.hpp>` with mathematical functions that operate on its fixed-point types, similar to `<math.hpp>` for floating-point types.
The available functions for fixed-point types include:
* basic functions: `abs`, `fmod`, `remainder`, `copysign`, `remquo`, etc.
* trigonometry functions: `sin`, `cos`, `tan`, `asin`, `acos`, `atan` and `atan2`.
* exponential functions: `exp`, `exp2`, `expm1`, `log`, `log10`, `log2` and `log1p`.
* power functions: `pow`, `sqrt`, `cbrt` and `hypot`.
* classification functions: `fpclassify`, `isnormal`, `isnan`, `isnormal`, etc.

Notes:
* all functions are in the `fpm` namespace.
* certain functions will always return the same value (e.g. `isnan` and `isinf` will always return false).
* be mindful of a function's domain and range: the result of `pow` can quickly overflow with certain inputs. On the other hand, trigonometry functions such as `sin` require more bits in the fraction for accurate results.

## Specialized customization points
The header `<fpm/fixed.hpp>` provides specializations for `fpm::fixed` for the following types:
* `std::hash`
* `std::numeric_limits`

## Conversions
The intent behind `fpm` is to replace floats for purposes of performance or portability. Thus, it guards against accidental usage of floats by requiring explicit conversion:
```c++
fpm::fixed_16_16 a = 0.5;        // Error: implicit construction from float
fpm::fixed_16_16 b { 0.5 };      // OK: explicit construction from float
fpm::fixed_16_16 c = b * 0.5;    // Error: implicit conversion from float
float d = b;                     // Error: implicit conversion to float
float e = static_cast<float>(b); // OK: explicit conversion to float
```

For integers, this still applies to initialization, but arithmetic operations *can* use integers:
```c++
fpm::fixed_16_16 a = 2;        // Error: implicit construction from int
fpm::fixed_16_16 b { 2 };      // OK: explicit construction from int
fpm::fixed_16_16 c = b / 2;    // OK
int d = b;                     // Error: requires explicit conversion
int e = static_cast<int>(b);   // OK: explicit conversion to int
```
You must still guard against underflow and overflow, though.

`fpm::fixed<A, B, C>` can be constructed from an `fpm::fixed<D, E, F>` via explicit construction. This allows for conversion between fixed-point numbers of differing precision and range.
Depending on the respective underlying types and number of fraction bits, this conversion may throw away high bits in the integral or low bits in the fraction.

## Printing and reading fixed-point numbers
The `<fpm/ios.hpp>` header provides streaming operators. Simply stream an expression of type `fpm::fixed` to or from a `std::ostream`.

For instance, the following program prints `"===3.142e+02"`:
```c++
#include <fpm/fixed.hpp>
#include <fpm/ios.hpp>
#include <iostream>
#include <iomanip>

int main() {
    fpm::fixed_16_16 x { 314.1516 };
    std::cout << std::setw(12) << std::setfill('=') << std::setprecision(3) << std::scientific << x << std::endl;
    return 0;
}
```

Reading fixed point numbers works similarly, by streaming `fpm::fixed` types from a `std::istream`.

`fpm`'s implementation of the streaming operators emulates streaming native floats as closely as possible without using floating-point types.

## Common constants
The following static member functions in the `fpm::fixed` class provide common mathematical constants in the fixed type:
* `e()`: _e_, roughly equal to 2.71828183.
* `pi()`: _π_, roughly equal to 3.14159265.
* `half_pi()`: _½π_, roughly equal to 1.57079633.
* `two_pi()`: _2π_, roughly equal to 6.2831853.

## Accuracy and performance
Please refer to the pages for [accuracy](accuracy.md) and [performance](performance.md) results.

## Limitations
Unlike floating-point numbers, `fpm::fixed`:
* can not represent Not-a-Number, infinity or negative zero.
* does not have a notion of subnormal numbers.
* does have a risk of overflow and underflow.

Notably the last point requires careful use of fixed-point numbers: like integers, you must ensure that they do not overflow or underflow.

## Alternatives
* [libfixmath](https://github.com/PetteriAimonen/libfixmath): C99 library, only supports Q16.16 format backed by 32-bit integers.
* [Compositional Numeric Library](https://github.com/johnmcfarlane/cnl): an experimental C++ header-only library worked on as part of WG-21/SG-14. Lacks many mathematical functions.
* [fp](https://github.com/mizvekov/fp): a C++14 header-only library. Does not provide any mathematical functions.