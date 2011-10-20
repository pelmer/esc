I/O Efficiency Excercises
=========================

Gerhard Brandt <gbrandt@cern.ch>

ESC10, 21-27 November 2010, Bertinoro

References
----------

* http://root.cern.ch/
* http://root.cern.ch/root/html/TFile.html
* http://root.cern.ch/root/html/TTree.html
* http://root.cern.ch/root/html/TTreeCache.html
* http://root.cern.ch/root/html/TTreePerfStats.html
* http://kcachegrind.sourceforge.net

Preparation
-----------

We will repeat a cycle of excercises to explore impact of different changes on I/O Efficiency when reading a file from disk.

* Copy the exercise macros and files from::

    /storage/software/brandt

* The workhorse macro for all the excercises is called::

    taodr1.C

* Open this macro in an editor and keep it open, you will make changes directly in the macro to try out the different options

* The workcycle for each excercise is:

  * Have look at the structure of the file. Load the file and inspect its structure::

      root -l file.root
      [0] .ls
      [1] _file0->Map()
      [2] physics->Print()

  * exit again

  * do neccessary modifications in ``taodr1.C``

  * run it from the shell like this::

      time root -q -b -l taodr1.C++

* We are always using time for the excercises to get basic information on whether time is spent using the CPU or waiting for I/O.

* During running this prints some statistics about what is going on while we run

* At the end, it writes the statistics file TTPS.root

* Plot the TTPS object inside this statistics file to examine what happes during I/O::

    $ root -l TTPS.root
    root [1] tree->Draw()

* Make sure you record all results for each exercise

  * Give different names to the TTPS.root output files

  * Note down wallclock time, cpu efficiency, disk I/O speed for each excercise

1. Effect of System Cache and TTreeCache
----------------------------------------

a) Check the effect of the System Cache
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Run the excercise for the first time using the local file, ``TTreeCache`` turned off

* Run the excercise **again** immediately (changing the name of the output file)

* Compare the disk I/O speed → they should be in the range 1-3 Gb/s now because the file is now cached in RAM by the system. this happens only during tests like this, or development of eg. an analysis, using the same small file over and over again.

  In large scale analysis where typically TB of data are analysed, and read from filesystems not cached in the Linux cache this fortunate case will not appear.  (we will use the TC for that later)

* Flush the cache using the provided::

    releaseFileCache /path/to/file.root

  **Remember to do this after every exercise reading from local disk from now on!**

* Repeat the excercise a third time to get back the original speed

b) Check the effect of the TTreeCache
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Now switch on the TTreeCache - set its size to 10 MB

* Repeat the excercise a third time

2. Sorting and optimising files
-------------------------------

a) Basket Sorting
^^^^^^^^^^^^^^^^^

* Sort the baskets in the file manually by processing it through the provided macro::

    root -q -b -l reorder_baskets.C

* Different groups of students can use different options to explore the different behaviour::

    SortBasketsByEntry
    SortBasketsByBranch

  This operation uses ``fast`` cloning, meaning the baskets are just rearranged (copied), not unzipped and streamed.  It is therefore not very CPU intensive and should be limited by the writing speed - check + record the output of time!

* Repeat the excercises with the ordered file

* Look at the structure of the file and the tree

* Rerun the event loop

* Understand the output of TTreePerfStats and compare to the previous complex example

* Repeat again with TC switched on/off

b) Optimise/auto-flush the file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* In ROOT 5.26 the files are automatically optimised and auto-flushed after a given number of events. We can force to rewrite the unoptimised example file by doing::

    hadd -f2 file.optimised.root file.root

  the -f2 option changes the compression level and therefore forces unzipping/streaming/rezipping (you can also use the reordering macro and omit the "fast" option)

* Repeat the excercise with this file

* Inspect the structure

* Benchmark it with/without TC

3. Understand performance of NFS and GPFS
-----------------------------------------

* Copy the original file and the optimised file to your space on ``/storage/nfs_esc10/studxx``

* Do the following four excercises:

  * Read original file with/without ``TTreeCache``

  * Read optimised file with/without ``TTreeCache``

* Compare to your results from local disk and to results from other students

4. Have a look at what happens during reading
---------------------------------------------

* We run the code in the ``valgrind`` profiler and look at the output using ``kcachegrind``.

* Since profiling runs slower, in order not to spend too much time set the max number of events to a low value, like 1000::

    $ valgrind --tool=callgrind --trace-children=yes root -q -b -l taodr1.C++

* Look at the output file callgrind.out.pid – due to the trace children there are many files from every process ROOT was running, the largest one is the one we want::

    $ kcachegrind callgrind.out.pid

* Explore the call tree and use the callee map to estimate the time spent in

  * Opening the file / reading the streamer info

  * Unzipping the baskets

  * Building the Objects

5. Influence of EDM and Dictionaries
------------------------------------

a) Read a complex file
^^^^^^^^^^^^^^^^^^^^^^

* We use this "official" ROOT example file: http://root.cern.ch/files/atlas20.root

* It is also located in ``/storage/software/gbrandt``

* Load the file to inspect its structure::

    root -l atlas20.root

* Many unknown class dictionaries will be reported – this is not important now

* Repeat the exercise above

* Make dictionary shared lib.  The file is an ATLAS file that needs special libraries to be read.  We can create these within ROOT using::

    _file0->MakeProject("atlas20","*","recreate++");

b) Check the effect of dictionaries
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Comment out the loading of the dictioniary library.  Upon opening the file ROOT will see that there is no custom dictionary available and will create a dictionary of emulated classes in memory.  Let us compare the efficiency of those with the pre-compiled ones.

* Run the excercise again and compare to the case with the prepare dictionary library

c) Performance of different classes for persistency
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* For this exercise we simply run the benchmarking suite provided by ROOT

* Copy it: ``cp -r $ROOTSYS/test``

* Build it: ``make bench``

* Run it

* Compare the different options

  * Which is the most performant container?

  * What is the least performant container?

  * What is the difference in percent for reading/writing these two?

6. Parallel I/O
---------------

In this exercise we will use the installed Torque batch system to read many files in parallel and examine the integrated I/O rate and behaviour of the file system.  We will all read the same files from the same location to put some load on the cluster.

* The files needed are in ``/storage/software/brandt/batch``.  Please copy them to your private working directory.

* Compile the reading executable ``readFiles2`` using ``make``

* This takes a text file with paths of files to read - test it locally on one file::

    readFiles filelist1.txt

  There are other file lists to test reading from different file systems.  You can look into the lists to see what files are used
  ``/storage/nfs_esc10/brandt/*`` (NFS) ``/storage/gpfs_esc10/brandt/*`` (GPFS).

* Open the steering file ``masterSettings.txt``.  Edit the paths to point to your working directory instead of mine.  The parameter filesizeplit determines how many jobs you are sending – leave it on one for now

* Now send the jobs to the batch using::

    sendFiles.py -p masterSettings.txt

* You can check they are running with ``qstat``

* Once they have finished, you can examine the output by using::

    root -q -b -l analyzeIO.C++

* This creates an output root file with a ``TCanvas`` – open it as usual to draw the canvas and look at the plots (macro ``plot_IO.C`` is provided)
