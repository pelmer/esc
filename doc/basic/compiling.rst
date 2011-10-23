Basic compilation of test programs
==================================

The goal of this exercise is to understand how to compile and run test
programs on the student servers. We have installed 64-bit GCC 4.6.1 and
mixed 32-/64-bit GCC 4.5.1 which you can experiment with.

1. Build 64-bit `cputicks.cc <../exercises/basic/cputicks.cc>`_ with GCC 4.6.1::

     . /storage/software/main/env-gcc461.sh
     type c++
     cd esc/exercises/basic
     c++ -o cputicks cputicks.cc
     file cputicks
     ./cputicks 1

2. Build 64-bit `cputicks.cc <../exercises/basic/cputicks.cc>`_ with GCC 4.5.1::

     . /storage/software/main/env-gcc451.sh
     type c++
     cd esc/exercises/basic
     c++ -o cputicks cputicks.cc
     file cputicks
     ./cputicks 1

3. Build 32-bit `cputicks.cc <../exercises/basic/cputicks.cc>`_ with GCC 4.5.1::

     . /storage/software/main/env-gcc451.sh
     type c++-32
     cd esc/exercises/basic
     c++-32 -o cputicks cputicks.cc
     file cputicks
     ./cputicks 1
