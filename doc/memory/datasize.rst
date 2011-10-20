Data Sizes
==========

The goal of this exercise is to understand better the size of C++ objects in
memory, to understand some differences between 32bit and 64bit and to learn to
use some basic tools.

1. Examine the size of basic data types in memory, ``int``, ``long``,
   ``float``, ``double`` and pointers, for example.  Hint: use the
   ``sizeof()`` function.

2. Examine the size of the following class in memory (not the generated object
   file size) after compiling for both 32bit and 64bit::

       class foo
       {
         double a;
         bool agood;
         double b;
         bool bgood;
         double c;
         bool cgood;
         double d;
         bool dgood;
       };

   Do you understand any difference that you see?

3. Write a little program to create a very large vector of such objects.  Add
   something to the program, for example "``std::cin >> i``", such that it
   subsequently stops waiting for input in order that you can examine the
   process with command line tools like ``top`` or ``pmap`` from a separate
   shell, etc.  What do you see?  Try it for both 32bit and 64bit.

4. Do you see how to reduce the memory usage (for both 32bit and 64bit) by
   modifications to the class?

5. What happens if the class has a virtual function? (Again, for both 32bit
   and 64bit.)

6. Write a small program that makes a very large number (i.e.  millions) of
   small memory allocations (try 4 bytes, ``class foo`` above, 100 bytes in
   subsequent runs).  Again stop the program each time with some trick like
   waiting for input such that you can examine it with ``top``, ``pmap``, etc.
   Do you understand what you see?  Try this test for both 32bit and 64bit.

References
----------

* `man page for top <http://linux.die.net/man/1/top>`_
* `man page for pmap <http://linux.die.net/man/1/pmap>`_
