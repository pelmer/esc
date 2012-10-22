C++11 final and devirtualization
================================

The goal of this exercise is to investigate how the ``final``
identifier can be used to devirtualize method calls.

Steps
-----

1. Set up your environment::

     . /storage/software/main/env-gcc472.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code, making sure you understand everything that is being done:
    * `cxx11-devirtualize.cxx <../exercises/c++/cxx11-devirtualize.cxx>`_

4. Compile and inspect ``cxx11-devirtualize.cxx`` ::

     $ g++ -Wall -pedantic -c cxx11-devirtualization.cxx -std=c++11 -O2 -ggdb
     $ objdump -S -r ./cxx11-devirtualize.o | c++filt | less

5. Now, re-compile with::

     $ g++ -Wall -pedantic -c cxx11-devirtualization.cxx -std=c++11 -O2 -ggdb -DWITH_FINAL=1
     $ objdump -S -r ./cxx11-devirtualize.o | c++filt | less

6. Can you spot the differences ? Make sure you do understand them.

References
----------

* `objdump <http://linux.die.net/man/1/objdump>`_ (for disassembly of binary
  code)

* `readelf <http://linux.die.net/man/1/readelf>`_ (for general inspection of
  ELF binaries)

* `GCC Inline Assembly HOWTO
  <http://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html>`_

* `PC Assembly Language <http://www.drpaulcarter.com/pcasm/>`_ (PDF document
  in `English <http://www.drpaulcarter.com/pcasm/pcasm-book-pdf.zip>`_,
  `French <http://www.drpaulcarter.com/pcasm/pcasm-book-french-pdf.zip>`_,
  `Italian <http://www.drpaulcarter.com/pcasm/pcasm-book-italian-pdf.zip>`_,
  etc.)

* `Intel® 64 and IA-32 Architectures Software Developer's Manual, Volume 1:
  Basic Architecture <http://www.intel.com/Assets/PDF/manual/253665.pdf>`_
  (Don't imagine reading this cover to cover, but a good reference to search
  for particular assembly instructions.)

* `C++11 final specifier <http://en.cppreference.com/w/cpp/language/final>`_
