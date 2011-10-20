Catching memory errors
======================

The goal of this exercise is to locate some common memory errors using
`Valgrind <http://valgrind.org>`_.

Steps
-----

1. Copy the exercise to your own working area, such as ``~/esc10``::

       cd $HOME && mkdir -p esc10 && cd esc10
       cp -rp /storage/software/tuura/3-valgrind .
       cd 3-valgrind

2. Source the exercise environment::

       . /storage/software/tuura/environment32.sh

3. Examine the little test programs in this directory.  You use them for
   dedicated little studies on container memory allocation behaviour:

   - `uninitialised.cc <../exercises/memory/uninitialised.cc>`_: Catch use
     of uninitialised memory.

   - `array-smash.cc <../exercises/memory/array-smash.cc>`_: Catch writing
     past the end of an object.

   - `stack-smash.cc <../exercises/memory/stack-smash.cc>`_: Catch
     destruction of stack frame caused by overwriting outside bounds.

4. Build each program.  The command is mentioned in each source file.  To
   build all in one go::

       grep -h Compile *.cc | sed 's|// Compile: ||' | sh -x

5. Run the programs under valgrind::

       valgrind --num-callers=50 --leak-check=full ./uninitialised
       valgrind --num-callers=50 --leak-check=full ./array-smash
       valgrind --num-callers=50 --leak-check=full ./stack-smash

6. Find the problems, fix them, recompile and repeat until the issues are
   fixed.

References
----------

* `Valgrind <http://valgrind.org>`_
