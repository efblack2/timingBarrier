#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0  compilerResults"
  exit 1
fi
nloops=5

rm -f Mpi_Result.txt
for j in  `seq 1 $nloops`; do
    echo run number: $j
    mpiexec -n 1 ./timingBarrier  | grep Barrirer  >>  Mpi_Result.txt
done

mkdir -p ../../plots/$(hostname)/$1
cat Mpi_Result.txt | awk '{} {print $1, $13, $6} {}' | sort -n | head -1  > ../../plots/$(hostname)/$1/Mpi.txt
rm Mpi_Result.txt
