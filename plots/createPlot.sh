#!/bin/bash
if [ "$#" -lt 1 ]
then
  echo "Usage: $0  computer [ylimit]"
  exit 1
fi



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

