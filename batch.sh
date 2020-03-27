#!/bin/bash

set -e

# do a bunch of simulations and plot them

pushd examples

for i in `seq 10 100 800`; do
    (../main -o $i.csv -p 1100 -f 60 -q $i -w 1920 -h 1080;
    gnuplot -c ../plot.gnuplot $i $i) &
done


popd