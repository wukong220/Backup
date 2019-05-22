#!/bin/bash 

for t in {1..20}
do
  if [[ "$t" -lt "10" ]];then
   rename "s/(.*).$t00000(.*)/00$t.init.restart/" init.*.restart 
  else 
   rename "s/(.*).$t00000(.*)/0$t.init.restart/" init.*.restart
  fi
done 
