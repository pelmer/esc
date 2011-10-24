Constructors
============

The goal of this exercise is to understand the impact of default constructors
on code size.

Steps
-----

1. Set up your environment::

     . /storage/software/main/env-gcc461.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code:
    * `default-ctors.h <../exercises/c++/default-ctors.h>`_
    * `default-ctors.cxx <../exercises/c++/default-ctors.cxx>`_
    * `default-ctors-main.cxx <../exercises/c++/default-ctors-main.cxx>`_

4. Build the default constructors example from page-25::

     rm -f *.s *.o
     c++ -c -g -W -Wall -Werror -c -Wa,-adhlsc=default-ctors.s default-ctors.cxx
     c++ -c -g -W -Wall -Werror -c -Wa,-adhlsc=default-ctors-main.s default-ctors-main.cxx
     c++ -o default-ctors -g -W -Wall -Werror default-ctors.o default-ctors-main.o

5. Inspect the assembly::

     objdump -rCSd --no-show-raw default-ctors
     c++filt < default-ctors.s | less -SX
     c++filt < default-ctors-main.s | less -SX

6. Modify the program to perform the above inside a for-loop

7. Check the impact on code size (and assembly)

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
