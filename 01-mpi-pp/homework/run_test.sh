#!/bin/bash

outputname=$1
machinefile=$2

for i in 1 10 20 50 100 250 500 1000 2000 3000 4000 5000 7500 10000 20000 30000 40000 50000
do
    mpiexec -machinefile $machinefile -np 2 ./measure_standard.x $i $outputname
done
