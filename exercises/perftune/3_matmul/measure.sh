#!/bin/bash

measure_line="pfmon -e CPU_CLK_UNHALTED:THREAD_P,INST_RETIRED:ANY_P,FP_COMP_OPS_EXE:SSE_FP,LAST_LEVEL_CACHE_REFERENCES -e MEM_INST_RETIRED:LOADS,MEM_INST_RETIRED:STORES --switch-timeout=10 --eu-c"

if [ "$1" == "all" ]
then
	for d in `ls -F | grep /`
	do
		cd $d
		echo "Measuring in $d --------------------------------"
		$measure_line ./`echo $d | sed -es/\\\///g`
		cd ..
	done
else
	$measure_line $1
fi
