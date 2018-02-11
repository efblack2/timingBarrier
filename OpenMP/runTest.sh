#!/bin/bash
if [ "$#" -ne 2 ] 
then
  echo "Usage: $0 derivativeLoops  compilerResults"
  exit 1
fi

nloops=5
np=`grep -c ^processor /proc/cpuinfo`

rm -f OpenMp_Result.txt
for i in  `seq 1 $np`; do
    export OMP_NUM_THREADS=$i
    for j in  `seq 1 $nloops`; do
        echo number of threads: $i, run number: $j 
        ./secondDerivative $1 | grep finish >>  OpenMp_Result.txt
    done
done

mkdir -p ../../plots/$(hostname)/$2
cat OpenMp_Result.txt | awk '{}{print $6, $3}{}' | awk '{Prod[$1]++; min[$1]=Prod[$1]==1||min[$1]>$2?$2:min[$1]} END{ for (var in Prod) printf "%s threads: the min is %f\n", var,min[var]}'  | sort -n  > ../../plots/$(hostname)/$2/OpenMP.txt

rm OpenMp_Result.txt

