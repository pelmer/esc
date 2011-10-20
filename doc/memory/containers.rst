Playing with Containers
=======================

The goal of this exercise is to understand the memory use and performance of
STL containers. You will get familiar with the `IgProf
<http://igprof.sourceforge.net>`_ profiler tool.

Steps
-----

1. Copy the exercise to your own working area, such as ~/esc10::

       cd $HOME && mkdir -p esc10 && cd esc10
       cp -rp /storage/software/tuura/2-containers .
       cd 2-containers

2. Source the exercise environment::

       . /storage/software/tuura/environment.sh

3. Examine the little test programs in this directory.  You use them for
   dedicated little studies on container memory allocation behaviour:

   - `ls-build.cc <../exercises/memory/ls-build.cc>`_: Build a
     ``std::list<std::string>`` one at a time.

   - `vs-build.cc <../exercises/memory/vs-build.cc>`_: Build a
     ``std::vector<std::string>`` one at a time.

   - `vvvi-build-and-copy.cc <../exercises/memory/vvvi-build-and-copy.cc>`_:
     Build triply nested vector of ints, and make a copy of it; drops profile
     heap snapshots part way.  This example was used in the presentation.

   - `vvvi-reserve.cc <../exercises/memory/vvvi-reserve.cc>`_: Same as
     ``vvvi-build-and-snapshot.cc``, but uses the ``reserve()`` method
     correctly to avoid vector resizes.  This example was used in the
     presentation.

   - `vvs-build-and-copy.cc <../exercises/memory/vvs-build-and-copy.cc>`_:
     Same as ``vvvi-build-and-copy.cc``, but using doubly nested vector of
     strings.  Behaves differently because strings are copy-on-write -- less
     memory copies but more reference counting.

4. Build each program.  The command is mentioned in each source file.  To
   build all in one go::

       grep -h Compile *.cc | sed 's|// Compile: ||' | sh -x

5. Run the programs under the igprof profiler::

       igprof -mp ./ls-build
       igprof -mp ./vs-build
       igprof -mp ./vvs-build-and-copy
       igprof -mp ./vvvi-build-and-copy
       igprof -mp ./vvvi-reserve

6. Analyse the profiles using::

       for f in *.gz; do
         igprof-analyse -d -v -g -r MEM_TOTAL $f 2>&1 | less -SX
       done
