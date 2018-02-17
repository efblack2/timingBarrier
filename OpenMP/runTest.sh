#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0 compilerResults"
  exit 1
fi
nloops=5
np=`grep -c ^processor /proc/cpuinfo`
#np=4

rm -f OpenMp_Result.txt
for i in  `seq 1 $np`; do
    export OMP_NUM_THREADS=$i
    for j in  `seq 1 $nloops`; do
        echo  run number: $j
        ./timingBarrier | grep Barrirer >>  OpenMp_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$1

     
cat OpenMp_Result.txt |  awk '{}{print $25, $1, $6, $13 }{}' | sort  -k1,1n -k2,2n   | awk 'BEGIN{ prev=-1} { if ($1 != prev) { print $0; prev=$1}  } END{}' > ../../plots/$(hostname)/$1/OpenMP.txt

rm OpenMp_Result.txt

