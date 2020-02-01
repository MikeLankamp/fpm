#!/usr/bin/gnuplot
set terminal png size 860,350 font "Arial,8"
set datafile separator ","
set datafile missing "-"

set style data histogram
set style fill solid border linecolor black
set style histogram clustered gap 2
set boxwidth 1

set key left top noenhanced

set xtics scale 0
set grid ytics
set ylabel "avg. time (ns)"

DATA_FILE="performance.csv"
set output "performance.png"

plot for [COL=2:6] DATA_FILE using COL:xtic(1) title columnheader