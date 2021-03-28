#!/bin/bash
#SBATCH --nodes 1
#SBATCH --ntasks 12
#SBATCH --time=06:00:00
#SBATCH --partition=plgrid
#SBATCH --account=plgmpr21zeus
#SBATCH --constraint="intel"

SMALL=5000000
MEDIUM=400000000
BIG=20000000000

output_file=out.dat
if [ $# -ne 0 ]
then
  output_file=$1
fi

module add plgrid/tools/openmpi
mpicc -std=c99 -Wall -o parallel.x parallel.c -lm

echo "Strong scalling" >> $output_file
echo "Small problem" >> $output_file
./run_test_strong.sh $SMALL $output_file
echo "Medium problem" >> $output_file
./run_test_strong.sh $MEDIUM $output_file
echo "Big problem" >> $output_file
./run_test_strong.sh $BIG $output_file
echo "" >> $output_file

echo "Weak scalling" >> $output_file
echo "Small problem" >> $output_file
./run_test_weak.sh $SMALL $output_file
echo "Medium problem" >> $output_file
./run_test_weak.sh $MEDIUM $output_file
echo "Big problem" >> $output_file
./run_test_weak.sh $BIG $output_file
echo "" >> $output_file
