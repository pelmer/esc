Using ``gdb`` to inspect a ``C++`` class layout
===============================================

The goal of this exercise is to understand how ``C++`` classes are
implemented to get a handle on the cost of various C++ idioms.

Steps
-----

1. Set up your environment::

     . /storage/software/env-gcc472.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code and make sure you understand all of it:
    * `gdb-ex-1.cc <../exercises/c++/gdb-ex-1.cc>`_
    * `gdb-ex-2.cc <../exercises/c++/gdb-ex-2.cc>`_
    * `gdb-ex-3.cc <../exercises/c++/gdb-ex-3.cc>`_

4. Build the ``gdb-ex-1.cc`` to get all debugging information in ``gdb``::

     c++ -ggdb -O2 -W -Wall gdb-ex-1.cc -o gdb-ex-1

5. Run the code under ``gdb``, insert a break point at ``main`` and
   inspect its assembly::

     $ gdb ./gdb-ex-1
     (gdb) br main
     (gdb) run
     (gdb) disas

6. Make sure you understand the differences between the calls to ``f`` and
   ``inline_f`` and how this reflects into the assembly.

7. Inspect the code of ``gdb-ex-2.cc``, compile and run it under ``gdb``,
   disassemble ``main`` and ``f``

8. Modify ``gdb-ex-2.cc`` so that the method ``m`` is now virtual. 

9. Compare with the previous case.

10. Inspect the code of ``gdb-ex-3.cc``, compile it, run it and make sure you
    understand what it does, and what it implies for the binary layout of classes
    with polymorphism.

Notes
-----

It is possible to have ``gdb`` output ``Intel`` assembly syntax instead of
the default ``AT&T`` one::

  (gdb) set disassembly-flavor intel


References
----------

* `GDB online documentation <http://sources.redhat.com/gdb/current/onlinedocs/gdb.html>`_

* `Introduction to Linux Intel Assembly language <http://heather.cs.ucdavis.edu/~matloff/50/LinuxAssembly.html>`_

* `Using Assembly Language in Linux <http://asm.sourceforge.net/articles/linasm.html>`_
