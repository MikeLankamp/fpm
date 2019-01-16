# fpm
Modern C++ header-only fixed-point math library.

[![Build Status](https://travis-ci.org/MikeLankamp/fpm.svg?branch=master)](https://travis-ci.org/MikeLankamp/fpm)
[![Build status](https://ci.appveyor.com/api/projects/status/0velpwqk38spu412?svg=true)](https://ci.appveyor.com/project/MikeLankamp/fpm)

## Why use fixed-point math?
There are several reasons why you can not or choose not to use floating-point math, but still want a similar type:
* Your target platform lacks an FPU, does not support floating-point operations or its floating-point operations are
  considerably slower than fixed-point integer operations.
* You require deterministic calculations.

If any of these reasons apply for you, and your problem domain has a clearly outlined range and required resolution,
then fixed-point numbers might be a solution for you.


## Usage
`fpm` defines the `fpm::fixed` class, which is templated on the underlying integer type and the number of bits in the fraction:
```c++
namespace fpm {
    template <typename BaseType, unsigned int FractionBits>
    class fixed;
}
```
**Note:** It's recommended to use a *signed* integer type for `BaseType` to emulate floating-point numbers 
and to allow the compiler to optimize the computations, since overflow and underflow are undefined
for signed integer types.

To use this class, simply include its header:
```c++
#include <fpm/fixed.h>
```
You may wish to typedef a particular choice of underlying type and fraction bitcount, e.g.:
```c++
using position = fpm::fixed<std::int32_t, 16>;
```
This defines a signed 16.16 fixed-point number with a range of -32768 to 65535.999985... and a resolution of 0.0000153...

For your convenience, several popular fixed-point formats have been defined in the `fpm` namespace:
```c++
namespace fpm {
    using fixed_16_16 = fixed<std::int32_t, 16>;  // Q16.16 format
    using fixed_24_8  = fixed<std::int32_t, 8>;   // Q24.8 format
    using fixed_8_24  = fixed<std::int32_t, 24>;  // Q8.24 format
}
```

## Limitations
Unlike floating-point numbers, `fpm::fixed`:
* can not represent NaN, infinity or negative zero.
* do not have a notion of epsilon or subnormal numbers.
* do have a risk of overflow and underflow.

Notably the last point requires careful use of fixed-point numbers:
like integers, you must ensure that they do not overflow or underflow.

## License
See the [LICENSE](LICENSE) file
