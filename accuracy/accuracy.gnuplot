#!/usr/bin/gnuplot
set terminal png size 430,300 font "Arial,8"
set datafile separator ","
set key noenhanced
set key autotitle columnhead
set colors classic

SERIES=ARG1
DATA_FILE=SERIES.".csv"

if (ARG2 eq "trig") {
    # Trig functions range from -pi to pi
    set xtics pi
    set format x '%.0Pπ'
    set xrange [-pi:pi]
}

err(x,real) = (real != 0) ? (x - real) : x

set output "accuracy-".SERIES.".png"
set title 'Δ '.SERIES

plot DATA_FILE using 1:(err($6,$2)) with linespoints, \
     DATA_FILE using 1:(err($5,$2)) with linespoints, \
     DATA_FILE using 1:(err($4,$2)) with linespoints, \
     DATA_FILE using 1:(err($7,$2)) with linespoints
