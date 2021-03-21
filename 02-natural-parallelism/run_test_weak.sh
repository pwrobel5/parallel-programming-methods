#!/bin/bash

points=$1
outputname=$2
actual_points=0

for i in {1..12}
do
    actual_points=`expr $actual_points + $points`
    mpiexec -machinefile ./allnodes -np $i ./parallel.x $actual_points $outputname
done
