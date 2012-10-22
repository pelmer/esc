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

1. Set up your environment::

     . /storage/software/env-gcc462.sh

2. Go to the exercise directory::

     cd esc/exercises/large-app

3. Prepare a configuration file::

     cmsDriver.py recocms -s RAW2DIGI,RECO,DQM -n 100 \
       --filein file:/storage/software/main/2680E635-9AF7-E011-8AB0-00248C0BE016.root \
       --eventcontent RECOSIM  --conditions auto:mc --no_exec

4. Add ``IgProfService`` to the configuration::

     f=recocms_RAW2DIGI_RECO_DQM.py
     for p in pp mp; do
       (cat $f
        echo 'process.SimpleMemoryCheck=cms.Service("SimpleMemoryCheck",'
        echo ' ignoreTotal=cms.untracked.int32(1),'
        echo ' oncePerEventMode=cms.untracked.bool(True))'
        echo 'process.IgProfService = cms.Service("IgProfService",'
        echo ' reportFirstEvent            = cms.untracked.int32(1),'
        echo ' reportEventInterval         = cms.untracked.int32(50),'
        echo ' reportToFileAtPostEndRun    = cms.untracked.string("|gzip -9c>'prof_${p}/igprof.${p}_$f.ENDRUN.%R.%I.gz'"),'
        echo ' reportToFileAtPostEndJob    = cms.untracked.string("|gzip -9c>'prof_${p}/igprof.${p}_$f.ENDJOB.%R.%I.gz'"),'
        echo ' reportToFileAtPreEvent 	   = cms.untracked.string("|gzip -9c>'prof_${p}/igprof.${p}_$f.EV.%I.gz'"))'
       ) > ${p}_$f
     done

5. Run the program, once for performance and memory profiles::

     for p in pp mp; do
       mkdir -p prof_${p}
       (time igprof -z -o prof_${p}/igprof.gz -d -t cmsRun -$p \
        cmsRun ${p}_recocms_RAW2DIGI_RECO_DQM.py) >& prof_${p}/log.txt </dev/null &
     done

6. While the program is running, open a few other terminal windows.  Run
   ``top`` in one -- see exercise :doc:`process` for making it more useful --
   and examine the outputs in ``prof_*`` directories and following programs
   to see what is going on.  It will take some time to run.

   ::

     numactl --hardware   # NUMA statistics
     cat /proc/meminfo    # Overall system memory use
     ps -lfC cmsRun       # Process statistics
     ps wwwuf -C cmsRun   # Process statistics

     # Memory maps for the process
     for p in pp mp; do
       for pid in $(pgrep -u $USER -f cmsRun); do
         for f in /proc/$pid/{,s,numa_}maps; do
           cat $f > prof_${p}/$(echo $f | tr / - | sed s/^-//).txt
         done
       done
     done

7. Generate profile analysis for all the profiles with the command::

     for f in prof_pp/igprof*.gz; do
       igprof-analyse -d -v -g $f >& $f.res
       case $f in *.ENDJOB* )
         igprof-analyse -d -v -g -b \
           $(echo $f | sed 's|\.END.*|.EV.51.gz|') \
           $f >& $f.res.diff ;;
       esac
     done

     for f in prof_mp/igprof*.gz; do
       igprof-analyse -d -v -g -r MEM_TOTAL $f >& $f.res.tot
       igprof-analyse -d -v -g -r MEM_LIVE $f >& $f.res.live
       case $f in *.ENDJOB* )
         igprof-analyse -d -v -g -r MEM_LIVE -b \
           $(echo $f | sed 's|\.END.*|.EV.51.gz|') \
           $f >& $f.res.livediff ;;
       esac
     done

   This generates the following performance profiles:

    - ``*.res``: Performance snapshots at various points.

    - ``*.res.diff``: Delta from event 50 to end.

   And the following memory profiles:

    - ``*.res.live``: Heap snapshots at various points.  The one at the end of
      the running, ``*.py.gz.res.live``, is all the ~160 MB memory the job
      leaked.

    - ``*.res.livediff``: Heap snapshot delta from event 50 to end.

    - ``*.res.tot``: Total memory allocated in bytes and number of allocations
      at various points.  This is a good measure of the degree of memory
      churn.

8. Try to identify suitable spots for optimisation, any major memory leaks
   (anything involving "event setup" is once per job, so not really fatal),
   and main sources of memory churn.  You can use `CMS LXR
   <http://cmslxr.fnal.gov/lxr/>`_ to look at the code if you think it helps.
   Just type a name of a class or method into the free text search field -- the
   first text box on top left -- to locate the code.

9. Write down the short list (< 10) list of your main observations, and which
   problems or aspects you would address first, and why.

References
----------

* `IgProf <http://igprof.sourceforge.net>`_
