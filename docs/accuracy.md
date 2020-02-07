# Accuracy

The images below compare the accuracy (versus the real result) of:
* `fpm` (the `Q8.24` and `Q16.16` types)
* [libfixmath](https://github.com/PetteriAimonen/libfixmath) (`fix16`), an alternative C library.

The accuracy of `Q24.8` was not shown since it was wildly inaccurate. It's advised to use at least 16 bits of fraction for reasonable accuracy for trigonometric operations (as in these tests).

Plotted is the relative error (`(result - real) / real`). Closer to 0 is better.

## Trigonometry functions
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-cos.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-sin.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-tan.png)

The results show that `fpm`'s trigonometric functions have better worst-case accuracy than `libfixmath`, although the latter has better overall accuracy except for certain domains where it has very large error.

## Inverse trigonometry functions
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-acos.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-asin.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-atan.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-atan2.png)

The results show that `fpm`'s inverse trigonometric functions have better accuracy then `libfixmath`.

## Power functions
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-sqrt.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-cbrt.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-exp.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-log.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-exp2.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-log2.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-pow.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-log10.png)

The results show that for those power functions that `libfixmath` supports, `fpm` is less accurate. However, the relative error of all power functions is well below 0.1% in the tested cases, and even less some functions.
