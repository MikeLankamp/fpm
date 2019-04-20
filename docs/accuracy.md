# Accuracy

The images below compare the accuracy of FPM (`Q8.24` and `Q16.16`) and the alternative [libfixmath](https://github.com/PetteriAimonen/libfixmath) (`fix16`) against the real result. The accuracy of `Q24.8` was not shown since it was wildly inaccurate. Use at least 12 bits of fraction for reasonable accuracy for trigonometry operations (as in this test).

Plotted is the absolute error (`result - real`). Closer to 0 is better.

## Trignometry functions
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-cos.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-sin.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-tan.png)

## Inverse trignometry functions
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-acos.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-asin.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-atan.png)
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-atan2.png)

## Power functions
![](http://mikelankampgithub.s3-website-eu-west-1.amazonaws.com/fpm/accuracy-sqrt.png)
