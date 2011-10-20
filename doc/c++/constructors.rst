Constructors
============

The goal of this exercise is to understand the impact of default constructors
on code size.

1. Implement the example from page-25 (default constructors) and use it in a
   simple program::

     Z z;
     z.foo(); //> to force compiler to use z (foo should use some method of E,F)

2. Inspect the assembly

3. Modify the program to perform the above inside a for-loop

4. Check the impact on code size (and assembly)

Code
----

* `default-ctors.h <../exercises/c++/default-ctors.h>`_
* `default-ctors.cxx <../exercises/c++/default-ctors.cxx>`_
* `default-ctors-main.cxx <../exercises/c++/default-ctors-main.cxx>`_

References
----------

* `Technical report on C++ performance (TR-18015)
  <http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf>`_

* `objdump <http://linux.die.net/man/1/objdump>`_

* `nm <http://linux.die.net/man/1/nm>`_

* `GCC Inline Assembly HOWTO
  <http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html>`_

* `PC Assembly Language <http://www.drpaulcarter.com/pcasm/>`_ (PDF document
  in `English <http://www.drpaulcarter.com/pcasm/pcasm-book-pdf.zip>`_,
  `French <http://www.drpaulcarter.com/pcasm/pcasm-book-french-pdf.zip>`_,
  `Italian <http://www.drpaulcarter.com/pcasm/pcasm-book-italian-pdf.zip>`_,
  etc.)
