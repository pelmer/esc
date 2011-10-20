#!/bin/bash

ICC="icc"
GCC="gcc"
CC=$ICC

A_OPTNAME=( 'simple' 'o0'  'o1'  'o2'  'o3'  'ipo'  'ipo-o2'   'ipo-o3'   'g'  'fast' )
A_FLAGS=(   '' '-O0' '-O1' '-O2' '-O3' '-ipo' '-ipo -O2' '-ipo -O3' '-g' '-fast' )
ELEMENTS=${#A_OPTNAME[@]}

FILES="matrix.c multiply.c multiply.h"
SOURCES="matrix.c multiply.c"
OBJECTS="matrix.o multiply.o"

# clean
#rm -f *.o core.* *~ $(TARGETS)

echo "Removing all old directories from /tmp"
echo "Moving all directories to /tmp"
for ((i=0; i<$ELEMENTS; i++))
do
        DIR="matmul-${A_OPTNAME[${i}]}"
        rm -rf /tmp/$DIR
	mv "matmul-${A_OPTNAME[${i}]}" /tmp	
done

for ((i=0; i<$ELEMENTS; i++))
do
	echo "*** Building " ${A_OPTNAME[${i}]} 
	DIR="matmul-${A_OPTNAME[${i}]}"
	mkdir $DIR
	cp $FILES $DIR
	cd $DIR
	$CC ${A_FLAGS[${i}]} -S $SOURCES
	set -o xtrace #echo on
	$CC ${A_FLAGS[${i}]} -c $SOURCES
	$CC ${A_FLAGS[${i}]} -o matmul-${A_OPTNAME[${i}]} $OBJECTS
	set +o xtrace #echo off
	for o in $OBJECTS
	do
		objdump -D $o > $o.dump
	done
	objdump -D matmul-${A_OPTNAME[${i}]} > matmul-${A_OPTNAME[${i}]}.dump
	cd ..
done
