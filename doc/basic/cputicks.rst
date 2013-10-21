Basic profiling using CPU ticks
===============================

The goal of this exercise is to introduce you to using CPU tick counters for
very rudimentary and basic performance profiling.  Our test program includes
manually inserted ``rdtsc`` instructions in the program flow to measure CPU
cycles elapsed between two points.  This is an extremely crude but sometimes
practically useful method of measuring elapsed time, at least for one-shot
evaluations involving small regions of code.

1. Build and run `cputicks.cc <../exercises/basic/cputicks.cc>`_::

     cd esc/exercises/basic
     c++ -O -o cputicks cputicks.cc
     ./cputicks

2. Run the program a few times, possibly also with arguments::

     ./cputicks 100
     ./cputicks 1000

3. Why does ``test1()`` report much more time spent than ``test2()``?

4. Why do the times reported by ``test1()`` fluctuate so much?
