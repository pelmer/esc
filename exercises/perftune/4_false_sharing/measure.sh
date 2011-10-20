#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 ./program #threads"
	exit
fi

pfmon --eu-c --follow-all --aggr -e MACHINE_CLEARS:MEM_ORDER,MEM_LOAD_RETIRED:OTHER_CORE_L2_HIT_HITM,MEM_LOAD_RETIRED:L3_MISS,MEM_LOAD_RETIRED:L3_UNSHARED_HIT $1 $2
