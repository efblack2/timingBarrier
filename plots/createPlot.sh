#!/bin/bash
if [ "$#" -lt 1 ]
then
  echo "Usage: $0  computer [ylimit]"
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


gnuplot -c plot.gnp $1 $2
gnuplot -c plot_Pin.gnp $1 $2
mv $1.pdf temp.pdf
pdfunite temp.pdf $1_pin.pdf $1.pdf
rm temp.pdf $1_pin.pdf

gnuplot -c plotGnu.gnp $1  $2
gnuplot -c plotGnu_Pin.gnp $1  $2
mv $1_Gnu.pdf temp.pdf
pdfunite temp.pdf $1_GnuPin.pdf $1_Gnu.pdf
rm  temp.pdf $1_GnuPin.pdf


rm `find . -name result*.txt`
