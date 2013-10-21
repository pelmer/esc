Basic compilation of test programs
==================================

The goal of this exercise is to understand how to compile and run test
programs on the student servers. We have installed 64-bit GCC 4.8.2 
which you can experiment with.

1. Build 64-bit `cputicks.cc <../exercises/basic/cputicks.cc>`_ with GCC 4.8.2::

     type c++
     cd esc/exercises/basic
     c++ -o cputicks cputicks.cc
     file cputicks
     ./cputicks 1

