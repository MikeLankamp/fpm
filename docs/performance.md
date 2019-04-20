# Performance

The image below shows the approximate performance of:
* `fpm` (the `Q16.16` type)  the alternative 
* [libfixmath](https://github.com/PetteriAimonen/libfixmath) (`fix16`), an alternative C library
* native FPU operations (`float` and `double`).

The graph below plots the average number of nanoseconds a single operation takes: lower is better. The data was collected with [Google Benchmark](https://github.com/google/benchmark) on an Intel Core i7-5820K, 3.3GHz.

![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/performance.png)

The results show the following:
* arithmetic operations:
  * the performance of `fpm` is comparable to `libfixmath`.
  * addition and subtraction: `fpm` is faster than native FPU operations.
  * multiplication and division: `fpm` is slower than native FPU operations by about half an order of magnitude.
* trigonometric and power functions:
  * `fpm` is slower than `libfixmath` by about half an order of magnitude.
  * `fpm` is slower than native FPU operations by up to an order of magnitude.
