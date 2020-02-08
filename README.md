# fpm
A C++ header-only fixed-point math library. "fpm" stands for "fixed-point math".

It is designed to serve as a drop-in replacement for floating-point types and aims to provide as much of the standard library's functionality as possible with exclusively integers. `fpm` requires C++11 or higher.

[![Build Status](https://travis-ci.org/MikeLankamp/fpm.svg?branch=master)](https://travis-ci.org/MikeLankamp/fpm)
[![Build status](https://ci.appveyor.com/api/projects/status/0velpwqk38spu412?svg=true)](https://ci.appveyor.com/project/MikeLankamp/fpm)

`fpm` is designed to guard against accidental conversion to and from floats and supports many of the standard C++ maths functions, including trigonometry, power and logarithmic functions, with performance and accuracy generally comparable to alternative libraries.

## Why use fixed-point math?
There are several reasons why you can not or choose not to use floating-point math, but still want a similar type:
* Your target platform lacks an FPU, does not support floating-point operations or its floating-point operations are
  considerably slower than fixed-point integer operations.
* You require deterministic calculations.

If any of these reasons apply for you, and your problem domain has a clearly outlined range and required resolution,
then fixed-point numbers might be a solution for you.

## Quick Start
To use `fpm`, include its header `<fpm/fixed.hpp>` and use the `fpm::fixed_16_16`, `fpm::fixed_24_8` or `fpm::fixed_8_24`
types as if they were native floating-pointer types:
```c++
#include <fpm/fixed.hpp>  // For fpm::fixed_16_16
#include <fpm/math.hpp>   // For fpm::cos
#include <fpm/ios.hpp>    // For fpm::operator<<
#include <iostream>       // For std::cin, std::cout

int main() {
    std::cout << "Please input a number: ";
    fpm::fixed_16_16 x;
    std::cin >> x;
    std::cout << "The cosine of " << x << " radians is: " << cos(x) << std::endl;
    return 0;
}
```

To use the fixed-point equivalents of the `<math.h>` functions such as `sqrt`, `sin` and `log`, include the header `<fpm/math.hpp>`.
To stream fixed-point values to or from streams, include the header `<fpm/ios.hpp>`.

## Documentation
Please refer to the [documentation](docs/index.md) for detailed information how to use `fpm`, or skip straight to the [performance](docs/performance.md) or [accuracy](docs/accuracy.md) results.

## Contributions
This library is a work-in-progress. We welcome any contributions that improve the functional coverage or the performance or accuracy of the mathematical functions.

## License
See the [LICENSE](LICENSE) file
