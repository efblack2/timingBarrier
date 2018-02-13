#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0  computer"
  exit 1
fi

cd $1/gnu
cat Mpi.txt OpenMP.txt > ../results.txt

cd ../intel
cat Mpi.txt OpenMP.txt >> ../results.txt
cd ../pgi
cat Mpi.txt OpenMP.txt >> ../results.txt

cd ../..


gnuplot -c plot.gnp $1

