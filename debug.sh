#!/bin/bash

while true ; do
    ./main &
    inotifywait -e modify main.c
    pkill main
    make
done