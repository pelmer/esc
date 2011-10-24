Inlining
========

The goal of this exercise is to understand better when functions will be
inlined and the resulting generated code.

Steps
-----

1. Set up your environment::

     . /storage/software/main/env-gcc461.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code:
    * `fourmom.h <../exercises/c++/fourmom.h>`_
    * `fourmom.cxx <../exercises/c++/fourmom.cxx>`_
    * `fourmom-main.cxx <../exercises/c++/fourmom-main.cxx>`_

4. Compile the FourMom-inlining example from the lectures, first replacing the
   floats with integers, then putting back the floats. To compile::

     c++ -o fourmom -O2 -W -Wall -Werror fourmom.cxx fourmom-main.cxx

5. Inspect assembly each time::

     objdump -rCSd --no-show-raw fourmom

6. Remove the inline hints and inspect assembly, for various levels of
   optimizations

References
----------

* `objdump <http://linux.die.net/man/1/objdump>`_

* `nm <http://linux.die.net/man/1/nm>`_
