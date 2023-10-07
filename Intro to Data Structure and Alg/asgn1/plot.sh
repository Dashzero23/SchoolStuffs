#!/bin/bash

make clean && make monte_carlo # Rebuild the monte_carlo executable .
./monte_carlo -n 1000 > data.dat # Place the data points into a file .
awk '{print $3 " " $4 " " $5}' data.dat > monte_carlo.dat
# This is the here - document that is sent to gnuplot .
gnuplot << END
set terminal pdf
set size square
set key autotitle columnhead
unset key
set output "figure2.pdf"
set title "Monte Carlo"
set xrange [0:1]
set yrange [0:1]
set palette defined (0 "#FF0000", 1 "#0000FF")
unset colorbox
plot "monte_carlo.dat" with dots linecolor palette, [x=0:1] sqrt(1-x**2) with lines linecolor rgb "#000000"
END
./monte_carlo -n 20000 -r "$RANDOM" > tempdata1.dat
./monte_carlo -n 20000 -r "$RANDOM" > tempdata2.dat
./monte_carlo -n 20000 -r "$RANDOM" > tempdata3.dat
./monte_carlo -n 20000 -r "$RANDOM" > tempdata4.dat
awk '{PI = 3.141592654} NR!=1 {print $1 " " PI - $2}' tempdata1.dat > data1.dat
awk '{PI = 3.141592654} NR!=1 {print $1 " " PI - $2}' tempdata2.dat > data2.dat
awk '{PI = 3.141592654} NR!=1 {print $1 " " PI - $2}' tempdata3.dat > data3.dat
awk '{PI = 3.141592654} NR!=1 {print $1 " " PI - $2}' tempdata4.dat > data4.dat
gnuplot << END
set terminal pdf
set output "figure3.pdf"
set title "Monte Carlo Error Estimation"
set logscale x 4
set xrange [1:20000]
set ylabel "Error"
set yrange [-1:1]
plot "data1.dat" title "" with lines linecolor rgb "#FF0000", "data2.dat" title "" with lines linecolor rgb "#ADD8E6", "data3.dat" title "" with lines linecolor rgb "#0000FF", "data4.dat" title "" with lines linecolor rgb "#FFF000"
END
