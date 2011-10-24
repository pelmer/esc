Template meta-programming
=========================

The goal of this exercise is to understand the costs of template
meta-programming in terms of code size and CPU performance.

Steps
-----

1. Set up your environment::

     . /storage/software/main/env-gcc461.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code, examine and understand the templated factorial
   function and dot-product
    * `factorial.cxx <../exercises/c++/factorial.cxx>`_

4. Compile and inspect assembly code to make sure the proper literals have
   been correctly directly inserted into the assembly::

     c++ -o factorial -g -W -Wall -Werror -save-temps factorial.cxx
     objdump -rCSd --no-show-raw factorial # or: less -SX factorial.s

     c++ -o factorialO2 -g -O2 -W -Wall -Werror -save-temps factorial.cxx
     objdump -rCSd --no-show-raw factorialO2 # or: less -SX factorial.s

References
----------

* `Technical report on C++ performance (TR-18015)
  <http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf>`_

* `Template meta-programming [wikipedia]
  <http://en.wikipedia.org/wiki/Template_metaprogramming>`_

* `C++ Templates - The Complete Guide - by David Vandervoorde and
  Nicolai Josuttis <http://www.josuttis.com/tmplbook/>`_
