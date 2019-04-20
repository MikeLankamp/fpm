# Performance

The image below shows the approximate performance of:
* `fpm` (the `Q16.16` type)  the alternative 
* [libfixmath](https://github.com/PetteriAimonen/libfixmath) (`fix16`), an alternative C library (see [notes](#notes)).
* native FPU operations (`float` and `double`).

The graph below plots the average number of nanoseconds a single operation takes: lower is better. The data was collected with [Google Benchmark](https://github.com/google/benchmark) on an Intel Core i7-5820K, 3.3GHz.

![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/performance.png)

The results show the following:
* Compared to `libfixmath`, the performance of `fpm` is at least as good, except for `sqrt`, `asin` and `acos` where it is about 50% slower.
* Compared to native single-precision floating-point operations, `fpm` is slower by up to an order of magnitude, except for `add`, `sub` and `atan`, where it is faster.

## Notes

For a fair comparison, `libfixmath` was compiled with `FIXMATH_NO_CACHE`.
It should also have been compiled with `FIXMATH_NO_OVERFLOW` (since `fpm` does not detect overflow), but `libfixmath` failed to compile with that option.
