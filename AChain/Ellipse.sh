#!/bin/bash

# input: ellipse.in **.restart
# run: lmp_wk
# output: **.in **.restart **.log 
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
	SOURCE="$(readlink "$SOURCE")"
	[[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

dir=$(cd `dirname $0`;pwd)
Phi=0.2 #0.1
R=1.0
D=$(echo "$R * 2.0" | bc)
Kb=0.0
for S in 4.0 #1.0 2.0 3.0 4.0 5.0
do 
	for Fa in 15.0 50.0
	do	
		title="${Phi}Phi_${S}S_${D}D"
		a=$(echo "$S * $D" | bc)
		
		if [ "$1" == "test" ]
		then
			sed "18s/2.0/$S/;19,26s/0.5/$R/;38s/1.0/$Fa/;59s/100000/10000/;132s/.*././;\
			63,134s/0.1P/${Phi}P/;63,134s/2.0S/${S}S/;63,134s/1.0D/${D}D/;186d;\
			127s/init/test/;141s/2.0/$a/;141s/1.0/$D/g" ellipse.in > ${title}.test
			mpirun -np 6 lmp_wk -i ${title}.test -l ${title}.test.log
			#exit 1
		else
			sed "18s/2.0/$S/;19,26s/0.5/$R/;38s/1.0/$Fa/;\
			63,134s/0.1P/${Phi}P/;63,134s/2.0S/${S}S/;63,134s/1.0D/${D}D/;\
			141s/2.0/$a/;141s/1.0/$D/g;" ellipse.in > ${title}.init
            mkdir ${dir}/${title}/
            mkdir ${dir}/${title}/${Kb}Kb_${Fa}Fa/
			mv ${title}.init ${dir}/${title}/${Kb}Kb_${Fa}Fa/
            cp ${title}.restart ${dir}/${title}/${Kb}Kb_${Fa}Fa/
			cd ${dir}/${title}/${Kb}Kb_${Fa}Fa/
			nohup mpirun -np 1 lmp_wk -i ${title}.init -l ${title}.init.log &
			echo "nohup mpirun -np 1 lmp_wk -i ${title}.init -l ${title}.init.log &"
			cd ../../
		fi
	done
done
