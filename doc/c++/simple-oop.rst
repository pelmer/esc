Simple OOP benchmarks and optimization levels
=============================================

The goal of this exercise is to understand the relative cost of various C++
operations and the effect of different compiler optimization levels.

Steps
-----

1. Set up your environment::

     . /storage/software/env-gcc472.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code, making sure you understand everything that is being done:
    * `benchmark-lib.h <../exercises/c++/benchmark-lib.h>`_
    * `benchmark-main.cc <../exercises/c++/benchmark-main.cc>`_

4. Compile with different optimisation levels -O0 / -O1 / -O2 / -O3::

     for opt in O{0,1,2,3}; do
       c++ -o benchmark-$opt -$opt -g -W -Wall benchmark-main.cc
     done

5. Do you obtain the same results shown in the presentation when running
   these programs?  Interpret any discrepancies you see.
