#!/bin/bash

# input:  00*.init.restart 
# output: 00*.init_chain.lammpstrj 
#         00*.lammpstrj 00*u.lammpstrj 00*f.lammpstrj
#         00*.a.restart 00*.b.restart  00*.end.restart
#         00*_msd.txt   00*.log        00*.data

dir=$(cd `dirname $0`;pwd)
Phi=0.2 #0.1
R=1.0
D=$(echo "$R * 2.0" | bc)
Kb=0.0
for S in 4.0 #1.0 2.0 3.0 4.0 5.0
do 
   for Fa in 15.0 50.0
   do
      for t in 001 #002 003 004 005 006 007 008 009 010\
               #011 012 013 014 015 016 017 018 019 020
      do   
         title="${Phi}Phi_${S}S_${D}D"
         a=$(echo "$S * $D" | bc)
         
         if [ "$1" == "test" ]
         then
            sed "19s/2.0/${S}/;20,27s/0.5/${R}/;37s/1.0/${Fa}/;38s/1.0/${Kb}/;\
            72s/2.0/$a/;72s/1.0/$D/g;66,204s/001/$t/g;203d" act.in > ${t}.act.test
            mpirun -np 1 lmp_wk -l $t.log -i $t.act.test
         else
            sed "19s/2.0/${S}/;20,27s/0.5/${R}/;37s/1.0/${Fa}/;38s/1.0/${Kb}/;\
            72s/2.0/$a/;72s/1.0/$D/g;66,204s/001/$t/g" act.in > ${t}.act.in
            mkdir ${dir}/${title}/
            mkdir ${dir}/${title}/${Kb}Kb_${Fa}Fa/
            mv ${t}.act.in ${dir}/${title}/${Kb}Kb_${Fa}Fa/
            cd ${dir}/${title}/${Kb}Kb_${Fa}Fa/
            nohup mpirun -np 1 lmp_wk -l $t.log -i $t.act.in > $t.data &
            echo "nohup mpirun -np 1 lmp_wk -l $t.log -i $t.act.in > $t.data &"
            cd ../../
         fi
       done  
   done
done

:<<BLOCK`
# for t in {0..9}
#for t in 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020
#do
#    cd  ~/wukong/chain
#    bsub  -q gtian -R "span[ptile=8]" -n 8 mpich2-mpiexec -np 8 lmp_wk"<"0.07_$t.in">"0.07_$t.data
#    cd ..
#done 

#for i in 001 002 003 004 005 006 007 008 009 010
#do 
   #  sed "59,194s/001/$i/g" test.in > $i.act.in 

#     bsub -J $i -n 1 mpirun lmp_wk -l $i.log -i $i.act.in > $i.data                                      #NJU
#     echo "bsub -J $i -n 1 mpirun lmp_wk -l $i.log -i $i.act.in > $i.data"
   
     #bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$i.act.in">"$i.data            #gchen
     #echo "bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$i.act.in">"$i.data"

     #bsub  -q gtian -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$i.act.in">"$i.data            #gtian
     #echo "bsub  -q gtian -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$i.act.in">"$i.data"

  #   nohup mpirun -np 1 lmp_wk -l $i.log -i $i.act.in > $i.data &                                        #nohup
 #    echo "nohup mpirun -np 1 lmp_wk -l $i.log -i $i.act.in > $i.data &"
#done 

for t in 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020 
do
     sed "59,194s/001/$t/g" test.in > $t.act.in 

     #bsub -J $t -n 1 mpirun lmp_wk -l $t.log -i $t.act.in > $t.data                                      #NJU
     #echo "bsub -J $t -n 1 mpirun lmp_wk -l $t.log -i $t.act.in > $t.data"
   
     #bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data            #gchen
     #echo "bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data"

     #bsub  -q gtian -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data            #gtian
     #echo "bsub  -q gtian -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data"

     nohup mpirun -np 1 lmp_wk -l $t.log -i $t.act.in > $t.data &                                        #nohup
     echo "nohup mpirun -np 1 lmp_wk -l $t.log -i $t.act.in > $t.data &"
done
`BLOCK
