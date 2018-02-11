#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0 compilerResults"
  exit 1
fi

nloops=5

rm -f OpenMp_Result.txt
for j in  `seq 1 $nloops`; do
    echo  run number: $j
    ./timingBarrier  | grep Barrirer >>  OpenMp_Result.txt
done

mkdir -p ../../plots/$(hostname)/$1
cat OpenMp_Result.txt | awk '{}{print $1 , $6, $13}{}' | sort -n  | head -1 > ../../plots/$(hostname)/$1/OpenMP.txt
rm OpenMp_Result.txt

