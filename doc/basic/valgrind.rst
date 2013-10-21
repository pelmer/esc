Short introduction to Valgrind
==============================

`Valgrind <http://http://valgrind.org/>`_ is an instrumentation framework for
dynamic analysis tools. At this school we use the tools for checking memory
management bugs, but it includes many other tools such as for finding threading
bugs and analysing cache access patterns.  This exercise will introduce you to
the basic mechanics of using valgrind; later exercises will go into more detail.

1. Set up your environment::

     c++ -v 2>&1 | grep version  # should say 'gcc version 4.8.2 (GCC)'
     valgrind --version          # should say 'valgrind-3.8.1'

2. Build `memerror.cc <../exercises/basic/memerror.cc>`_::

     cd esc/exercises/basic
     c++ -g -o memerror memerror.cc

3. Run the test program under valgrind::

     valgrind ./memerror

4. Let's ask compiler to tell us the same thing::

     c++ -g -W -Wall -Werror -ansi -pedantic -o memerror memerror.cc
