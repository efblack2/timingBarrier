#!/bin/bash


mkdir -p  MPI/buildGnu
#mkdir -p  MPI_fence/buildGnu
#mkdir -p  MPI_sync/buildGnu
mkdir -p  OpenMP/buildGnu

mkdir -p  MPI/buildIntel
#mkdir -p  MPI_fence/buildIntel
#mkdir -p  MPI_sync/buildIntel
mkdir -p  OpenMP/buildIntel

mkdir -p  MPI/buildPgi
#mkdir -p  MPI_fence/buildPgi
#mkdir -p  MPI_sync/buildPgi
mkdir -p  OpenMP/buildPgi



cd OpenMP/buildGnu
cmake .. ; make clean; make
cd ../../MPI/buildGnu
cmake .. ; make clean; make
#cd ../../MPI_fence/buildGnu
#cmake .. ; make clean; make
#cd ../../MPI_sync/buildGnu
#cmake .. ; make clean; make
cd ../../

export CC=icc
export CXX=icpc
source setIcc intel64
source setImpi


cd OpenMP/buildIntel
cmake .. ; make clean; make
cd ../../MPI/buildIntel
cmake .. ; make clean; make
#cd ../../MPI_fence/buildIntel
#cmake .. ; make clean; make
#cd ../../MPI_sync/buildIntel
#cmake .. ; make clean; make
cd ../../

export CC=pgcc
export CXX=pgc++
source setPgi 18.3
source setPgiMpi 18.3

cd OpenMP/buildPgi
cmake .. ; make clean; make
cd ../../MPI/buildPgi
cmake .. ; make clean; make
#cd ../../MPI_fence/buildPgi
#cmake .. ; make clean; make
#cd ../../MPI_sync/buildPgi
#cmake .. ; make clean; make
cd ../../

