# FPM

## Accuracy

The images below compare the accuracy of FPM (`Q8.24` and `Q16.16`) and the alternative [libfixmath](https://github.com/PetteriAimonen/libfixmath) (`fix16`) against the real result. The accuracy of `Q24.8` was not shown since it was wildly inaccurate. Use at least 12 bits of fraction for reasonable accuracy for trigonometry operations (as in this test).

Plotted is the absolute error (`result - real`). Closer to 0 is better.

(Images TBD)

## Performance

The image below shows the approximate performance of FPM (`Q16.16`) compared to the alternative [libfixmath](https://github.com/PetteriAimonen/libfixmath) (`fix16`) and native FPU operations (`float` and `double`). The performance is plotted in average nanoseconds a single operation takes: lower is better.

The data was collected with [Google Benchmark](https://github.com/google/benchmark) on an Intel Core i7-5820K, 3.3GHz.

(Image TBD)