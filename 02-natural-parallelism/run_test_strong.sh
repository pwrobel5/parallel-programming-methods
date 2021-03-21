#!/bin/bash

points=$1
outputname=$2

for i in {1..12}
do
    mpiexec -machinefile ./allnodes -np $i ./parallel.x $points $outputname
done
