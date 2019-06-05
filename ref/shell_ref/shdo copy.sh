#!/bin/bash

#for t in {0..9}
for t in 0.07 0.13 0.20 5.33
do
    cd $t
    bsub  -q gchen -R "span[ptile=8]" -n 8 mpich2-mpiexec -np 8 lmp_wk"<"$t.in">"$t.data -l $t.log
    cd ..
done
 
  
