Inlining
========

The goal of this exercise is to understand better when functions will be
inlined and the resulting generated code.

1. Compile the FourMom-inlining example from the lectures, first replacing the
   floats with integers, then putting back the floats.

2. Inspect assembly each time

3. Remove the inline hints and inspect assembly (for various levels of
   optimizations)

Code
----

* `fourmom.h <../exercises/c++/fourmom.h>`_
* `fourmom.cxx <../exercises/c++/fourmom.cxx>`_
* `fourmom-main.cxx <../exercises/c++/fourmom-main.cxx>`_

References
----------

* `objdump <http://linux.die.net/man/1/objdump>`_

* `nm <http://linux.die.net/man/1/nm>`_
