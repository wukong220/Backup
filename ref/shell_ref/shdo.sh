#!/bin/bash

for t in {0..9}
# for t in 5 21 3 sdsa d sa
do
    cd test$t
    bsub -q gchen lmp2017 "<"sg.in">" out11.$t.data
    cd ..
done
 
  
