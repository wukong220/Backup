#!/bin/bash
((idr=3))
for dr in 0.002 0.005 #0.001 0.003 0.01
do
  ((idr=$idr+1))
  mkdir test"$idr"_Dr$dr
  cd test$idr*
  ((iden=0))
  for den in 0.05 0.10
  do
    ((iden=$iden+1))
    mkdir test"$iden"_Den$den
    cd test$iden*
      ((iRad=0))
      for Rad in 4.0 8.0 12.0 16.0 20.0
      do
        ((iRad=$iRad+1))
        mkdir test"$iRad"_Rad$Rad
        cd test$iRad*
        cp /comput/gchen/chen03/cross/rotation/backup/Model4.3_Cross.cpp .
        sed -i '12c const int N_arm=4;' Model4.3_Cross.cpp
        sed -i '13c const double R_arm='$Rad';' Model4.3_Cross.cpp
        sed -i '15c const double Den_active='$den';' Model4.3_Cross.cpp
        g++ Model4.3_Cross.cpp -o model4.3_Cross
        ./model4.3_Cross&
        cp /comput/gchen/chen03/cross/rotation/backup/rigid.data .
        cp /comput/gchen/chen03/cross/rotation/backup/sc.in .
        ((n=$RANDOM))
        sed -i '23c variable            l equal '$n'' sc.in
        ((n=$RANDOM))
        sed -i '24c variable            a equal '$n'' sc.in
        ((n=$RANDOM))
        sed -i '25c variable            r equal '$n'' sc.in
        tau=$(echo "1/$dr"|bc)
        ((dump=1000))
        ((run=20000000))
        sed -i '17c neigh_modify        delay 0  exclude group Na  Na' sc.in
        sed -i '28c fix                 41 Na       langevin  0.0  0.0  0.1  $a  zero yes' sc.in
        sed -i '30c fix                 12 Na       nve/rot  '$dr'  $r' sc.in
        sed -i '37c dump                35 all custom '$dump' xy.lammpstrj id type x y' sc.in
        sed -i '38c dump                36 Na custom '$dump' Act.lammpstrj id x y vx vy mux muy' sc.in
        sed -i '39c dump                37 Nr custom '$dump' Rat.lammpstrj id x y vx vy' sc.in
        sed -i '42c run                 '$run'' sc.in
        cd ..
      done
    cd ..
  done
  cd ..
done
