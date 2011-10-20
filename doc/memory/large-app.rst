Memory analysis of a large application
======================================

The goal of this exercise is to do memory profiling with one or more large
applications, where by "large" we mean something for which you will not be
able to read all of the relevant code as you may have done with earlier toy
examples.  To find problems you will need to rely heavily on the profiling
tool to indicate the interesting code sections to examine.

Steps
-----

Memory usage analysis of a full LHC environment, using CMS' DQM reconstruction
and analysis applications.

1. Copy the exercise to your own working area, such as ``~/esc10``::

       cd $HOME && mkdir -p esc10 && cd esc10
       cp -rp /storage/software/tuura/5-large-app .
       cd 5-large-app

2. Source the exercise environment::

       . /storage/software/tuura/environment.sh

3. Examine `run-and-profile <../exercises/memory/run-and-profile>`_, a
   script which does the work of running two DQM-related executables under
   igprof memory profiling.  You do not need to fully understand what the
   script does, but it's helpful to have an idea.

4. Run the script in your own working directory::

       (time . ./run-and-profile) >& output.txt </dev/null &

5. While the program is running, open a few other terminal windows.  Run
   ``top`` in one -- see exercise :doc:`basic` for making it more useful --
   and examine the text files in the ``prof_*`` directories, especially the
   ``*.iglog`` files to see what is going on.  It will take some time, about
   ten minutes, for everything to run, so use the time to get familiar with
   the various state dumps: ``*.txt``.

6. Generate profile analysis for all the profiles with the command::

       for f in prof_mp_*/igprof*.gz; do
         igprof-analyse -d -v -g -r MEM_TOTAL $f >& $f.res.tot
         igprof-analyse -d -v -g -r MEM_LIVE $f >& $f.res.live
         case $f in *.ENDJOB* )
           igprof-analyse -d -v -g -r MEM_LIVE -b \
             $(echo $f | sed 's|\.END.*|.EV.51.gz|') \
             $f >& $f.res.livediff ;;
          esac
        done

   This generates the following profile results:

    - ``*.res.live``: Heap snapshots at various points.  The one at the end of
      the running, ``*.py.gz.res.live``, is all the ~160 MB memory the job
      leaked.

    - ``*.res.livediff``: Heap snapshot delta from event 50 to end.

    - ``*.res.tot``: Total memory allocated in bytes and number of allocations
      at various points.  This is a good measure of the degree of memory
      churn.

7. Try to identify suitable spots for optimisation, any major memory leaks
   (anything involving "event setup" is once per job, so not really fatal),
   and main sources of memory churn.  You can use `CMS LXR
   <http://cmslxr.fnal.gov/lxr/>`_ to look at the code if you think it helps.
   Just type a name of a class or method into the free text search field -- the
   first text box on top left -- to locate the code.

8. Write down the short list (< 10) list of your main observations, and which
   problems or aspects you would address first, and why.

References
----------

* `IgProf <http://igprof.sourceforge.net>`_
