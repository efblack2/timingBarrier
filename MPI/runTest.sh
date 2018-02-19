#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0  compilerResults"
  exit 1
fi
nloops=5
np=`grep -c ^processor /proc/cpuinfo`
#np=8

rm -f Mpi_Result.txt
for i in  `seq 1 $np`; do
    for j in  `seq 1 $nloops`; do
        echo Number of processors: $i,  run number: $j
        mpiexec -n $i ./timingBarrier  | grep Barrirer  >>  Mpi_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$1

cat Mpi_Result.txt | awk '{}{print $25, $1, $6, $13 }{}' |  sort  -k1,1n -k2,2n  | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' > ../../plots/$(hostname)/$1/Mpi.txt

rm Mpi_Result.txt

