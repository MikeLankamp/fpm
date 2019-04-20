# Performance

The image below shows the approximate performance of FPM (`Q16.16`) compared to the alternative [libfixmath](https://github.com/PetteriAimonen/libfixmath) (`fix16`) and native FPU operations (`float` and `double`). The performance is plotted in average nanoseconds a single operation takes: lower is better. The data was collected with [Google Benchmark](https://github.com/google/benchmark) on an Intel Core i7-5820K, 3.3GHz.

![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/performance.png)
