Basic counters
==============

*The goal of this exercise is to get you acquainted with the pfmon tool and
the pfmon_deluxe script.  You should as well be able to understand the basic
reported metrics.*

The example used in this exercise is FullCMS, a benchmark from the Geant 4
framework.  You first need to run ``build.sh``, and then run it by typing::

  ./run.sh

Tweak the script if needed.  Re-run it with Pfmon Deluxe - it might take
several minutes.  **Compare some of the reported counters to the course
materials (Table 1).**

If you look at the output of ``pfmon –l``, you will notice that some counters
count instructions which are "retired" and some count instructions which are
"executed".  Do you know the difference between the two?

What percentage of all SIMD micro-ops is computational?
