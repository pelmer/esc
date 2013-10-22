Memory analysis of a large application
======================================

The goal of this exercise is to do memory profiling with one or more large
applications, where by "large" we mean something for which you will not be
able to read all of the relevant code as you may have done with earlier toy
examples.  To find problems you will need to rely heavily on the profiling
tool to indicate the interesting code sections to examine. 

Ideally you should choose some application that you would use in your
normal work environment. In that case you will also need to 
build `IgProf <igprof.org>`_ for yourself. Otherwise you can choose
some open source (scientific) application that you can build yourself
and which provides some non-trivial test or example job which you
can run.

Steps
-----

1. Choose an application to run.

2. If you are working in the ESC school environment (and will thus use
   the installation of IgProf available to you on the ESC machines), you
   should build the software you have chosen to profile on the ESC machines. 
   If you will be working in some other environment (e.g. your normal working
   environment), you will need to build IgProf for yourself in that
   environment.

3. Run the program, once for performance and once for memory profiles.

4. Try to identify suitable spots for optimisation, any major memory leaks
   and main sources of memory churn.  

5. Write down the short list (< 10) list of your main observations, and which
   problems or aspects you would address first, and why.

References
----------

* `IgProf <http://igprof.org>`_
