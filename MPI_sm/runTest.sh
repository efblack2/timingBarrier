#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0  compilerResults"
  exit 1
fi

nloops=5

rm -f Mpi_sm_Result.txt
for j in  `seq 1 $nloops`; do
    echo run number: $j
    mpiexec -n 1 ./timingBarrier  | grep Barrirer  >>  Mpi_sm_Result.txt
done

mkdir -p ../../plots/$(hostname)/$1
cat Mpi_sm_Result.txt | awk '{}{print $1 , $6, $13}{}' | sort -n  | head -1  > ../../plots/$(hostname)/$1/MPI_sm.txt
rm Mpi_sm_Result.txt

