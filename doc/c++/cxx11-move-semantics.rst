C++11 move semantics
====================

The goal of this exercise is to investigate how to implement move
operations for a class and figure out its impact on the code.

Steps
-----

1. Set up your environment::

     . /storage/software/main/env-gcc472.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code, making sure you understand everything that is being done:
    * `big.hxx <../exercises/c++/big.hxx>`_
    * `big.cxx <../exercises/c++/big.cxx>`_
    * `main-big.cxx <../exercises/c++/main-big.cxx>`_

4. Compile with and without the "DIFFERENT" macro defined and compile with
   different optimization levels from -O0 to -O3, and "DIFFERENT" also with
   the linker folding identical code::

     for opt in O{0,1,2,3}; do
       c++ -o templates-$opt-diff-icf -DDIFFERENT -Wl,--icf=safe -$opt -W -Wall templates.cxx
       c++ -o templates-$opt-diff -DDIFFERENT -$opt -W -Wall templates.cxx
       c++ -o templates-$opt-same -$opt -W -Wall templates.cxx
     done

5. Compare the size of the results

6. Compare with the -save-temps switch -- which stores the usual "temporary"
   intermedate files permanently -- the largest and the leanest programs from
   above

7. Look at the assembly code and try to figure out if everything makes sense

8. Copy the templates.cxx file to explicit-templates-inst.cxx and edit the new
   file to use explicit template instantiation

9. Compare the code size resulting from compilation of each of templates.cxx
   and explicit-templates-inst.cxx 

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
