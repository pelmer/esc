Short introduction to IgProf
============================

`IgProf <http://igprof.sourceforge.net>`_ is both a performance profiler and
memory allocation profiler we will use at this school.  This exercise will
introduce you to the basic mechanics of using igprof.  Later exercises will
go into much more detail, so don't worry about understanding everything at
this point; the main goal here is to learn to use the tools.

Basic profiling environment
---------------------------

To make sure you have technically everything set up right, let's first make
sure you can create a dummy profile for simply running ``ls``.

1. Make sure your environment is correct::

     c++ -v 2>&1 | grep version  # should say 'gcc version 4.8.2 (GCC)'
     igprof -h                   # should print simple help message

2. Create your very first igprof report on ``ls``::

     igprof -pp -z -o ls-test.gz -- ls -laFR /usr/lib >/dev/null
     igprof-analyse --sqlite -d -v -g ls-test.gz |
       sqlite3 ~/public_html/cgi-bin/data/ls-test.sql3

3. View the report at http://131.154.193.30/~<student>/cgi-bin/igprof-navigator.py/ls-test/

Basic performance profile
-------------------------

This exercise will run the program from the :doc:`previous exercise <cputicks>`
using igprof statistical performance profiler.

1. Make sure your environment is correct::

     c++ -v 2>&1 | grep version  # should say 'gcc version 4.8.2 (GCC)'
     igprof -h                   # should print simple help message

2. Run the test program under igprof performance profiler::

     cd esc/exercises/basic
     igprof -pp -z -o cputicks.gz ./cputicks 100000

3. Generate a simple text profile output of run time::

     igprof-analyse -d -v -g cputicks.gz > cputicks.res
     less -SX cputicks.res

   You should see an output like this::

     Counter: PERF_TICKS

     ----------------------------------------------------------------------
     Flat profile (cumulative >= 1%)

     % total      Total  Function
       100.0       0.28  <spontaneous> [1]
       100.0       0.28  _start [2]
       100.0       0.28  __libc_start_main [3]
       100.0       0.28  main [4]
        97.8       0.27  test1(int) [5]
     <... and so on ...>

4. Generate the same profile in web-viewable database, view the profile at
   http://131.154.193.30/~<student>/cgi-bin/igprof-navigator.py/cputicks/ ::

     rm -f ~/public_html/cgi-bin/data/cputicks.sql3
     igprof-analyse --sqlite -d -v -g cputicks.gz |
       sqlite3 ~/public_html/cgi-bin/data/cputicks.sql3
