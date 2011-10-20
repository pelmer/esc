Template meta-programming
=========================

The goal of this exercise is to understand the costs of template
meta-programming in terms of code size and CPU performance.

1. Examine and understand the templated factorial function and dot-product in
   factorial.cxx below

2. Compile with -save-temps and inspect assembly code to make sure the proper
   lierals have been correctly directly inserted into the assembly

Code
----

* `factorial.cxx <../exercises/c++/factorial.cxx>`_

References
----------

* `Technical report on C++ performance (TR-18015)
  <http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf>`_

* `Template meta-programming [wikipedia]
  <http://en.wikipedia.org/wiki/Template_metaprogramming>`_

* `C++ Templates - The Complete Guide - by David Vandervoorde and
  Nicolai Josuttis <http://www.josuttis.com/tmplbook/>`_
