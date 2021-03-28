#!/bin/bash

points=$1
outputname=$2

for i in {1..12}
do
    mpirun -n $i ./parallel.x $points >> $outputname
done
