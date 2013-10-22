Vector memory use
=================

The goal of this exercise is to understand the memory use and performance of
`std::vector <http://www.cplusplus.com/reference/stl/vector/>`_. You will get
familiar with the `IgProf <http://igprof.org>`_ profiler tool.

Steps
-----

1. Make sure your environment is correct::

     c++ -v 2>&1 | grep version  # should say 'gcc version 4.8.2 (GCC)'
     igprof -h                   # should print simple help message

2. Go to the exercise directory::

     cd esc/exercises/memory

3. Examine the little test programs in this directory.  You use them for
   dedicated little studies on container memory allocation behaviour:

   - `vvvi-build-and-copy.cc <../exercises/memory/vvvi-build-and-copy.cc>`_:
     Build triply nested vector of ints, and make a copy of it; drops profile
     heap snapshots part way.  This example was used in the presentation.

   - `vvvi-reserve.cc <../exercises/memory/vvvi-reserve.cc>`_: Same as
     ``vvvi-build-and-snapshot.cc``, but uses the ``reserve()`` method
     correctly to avoid vector resizes.  This example was used in the
     presentation.

4. Build each program.  The command is mentioned in each source file.  To
   build all in one go::

     grep -h Compile *.cc | sed 's|// Compile: ||' | sh -x

5. Run the programs under the igprof profiler::

     rm -f *.gz
     igprof -mp ./vvvi-build-and-copy
     igprof -mp ./vvvi-reserve

6. Analyse the profiles using::

     for f in *.gz; do
       igprof-analyse --sqlite -d -v -g -r MEM_TOTAL $f |
         sqlite3 ~/public_html/cgi-bin/data/${f}_tot.sql3
       igprof-analyse --sqlite -d -v -g -r MEM_LIVE $f |
         sqlite3 ~/public_html/cgi-bin/data/${f}_live.sql3
       igprof-analyse --sqlite -d -v -g -r MEM_LIVE --value peak $f |
         sqlite3 ~/public_html/cgi-bin/data/${f}_live_peak.sql3
     done

7. This results in profiles such as::

   http://131.154.193.30/~<student>/cgi-bin/igprof-navigator.py/vvvi-copy.gz_tot/o_tot/


