#!/bin/bash

for idr in {1..5}
do
  cd test$idr*
  for iden in 1 2
  do
    cd test$iden*
    for iRad in {1..5}
    do
      cd test$iRad*
      echo $idr $iden $iRad
      cp /comput/gchen/chen03/cross/rotation/backup/Omega_C1.1.cpp .
      numrat=$(head -n4 Rat.lammpstrj | tail -n1) &&
      sed -i '14c const int Num_rat='$numrat';' Omega_C1.1.cpp &&
      ((run=20000000))
      sed -i '16c const int Run='$run';' Omega_C1.1.cpp &&
      g++ Omega_C1.1.cpp -o omega_c1.1 &&
      ./omega_c1.1 &
      #bsub -q gchen -m "compute-00-00 compute-00-02 compute-00-03 compute-00-04 compute-00-05 compute-00-08 compute-00-11 compute-00-12 compute-00-13 compute-00-14 compute-00-17 compute-01-13 compute-01-14 compute-01-15" ./omega_c1.1 
      cd ..
    done
    cd ..
  done
  cd ..
done
