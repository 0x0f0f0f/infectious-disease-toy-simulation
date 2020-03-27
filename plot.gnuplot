set title ARG2.' in quarantine'
set term png size 1280,720
set output ARG1.".png"

set style line 100 lt 4 lc rgb "grey" lw 4 # linestyle for the grid


set datafile separator ','
set key autotitle columnhead # use the first line as title
set ylabel "Population" # label for the Y axis
set xlabel 'Time' # label for the X axis

plot ARG1.".csv" using 1:2 with lines, '' using 1:3 with lines, '' using 1:4 with lines, '' using 1:5 with lines