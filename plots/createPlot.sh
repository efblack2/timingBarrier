#!/bin/bash
if [ "$#" -ne 1 ]
then
  echo "Usage: $0  computer"
  exit 1
fi

cd $1/gnu
cat Mpi.txt OpenMP.txt > ../results.txt
cat Mpi_Pin.txt OpenMP_Pin.txt > ../results_Pin.txt

cd ../intel
cat Mpi.txt OpenMP.txt > ../results.txt
cat Mpi_Pin.txt OpenMP_Pin.txt >> ../results_Pin.txt

cd ../pgi
cat Mpi.txt OpenMP.txt > ../results.txt
cat Mpi_Pin.txt OpenMP_Pin.txt >> ../results_Pin.txt

cd ../..


gnuplot -c plot.gnp $1
gnuplot -c plot_Pin.gnp $1

rm `find . -name result*.txt`
