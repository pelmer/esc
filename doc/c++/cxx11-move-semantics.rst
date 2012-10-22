C++11 move semantics
====================

The goal of this exercise is to investigate how to implement move
operations for a class and figure out its impact on the code.

Steps
-----

1. Set up your environment::

     . /storage/software/env-gcc472.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code, making sure you understand everything that is being done:
    * `big.hxx <../exercises/c++/big.hxx>`_
    * `big.cxx <../exercises/c++/big.cxx>`_
    * `big-main.cxx <../exercises/c++/big-main.cxx>`_

4. Compile and run ``big-main`` ::

     $ make -f big-Makefile

5. Note the number of ``Big`` instances being created and copied.
   You can also look at the assembly like so::

     $ objdump -S -r ./big-main | c++filt | less

6. Now, implement the move operations (i.e. the move constructor and
   the move assignmenent operator)

7. Compile and run ``big-main``, compare with the number of ``Big``
   instances being created "from scratch".

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

* `C++11 move constructor <http://en.cppreference.com/w/cpp/language/move_constructor>`_

* `C++11 move operator <http://en.cppreference.com/w/cpp/language/move_operator>`_
