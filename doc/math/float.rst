Floating Point Representation
=============================

Goal of this exercise is to better understand the IEEE 754 representation of
floating points, the behavior of operations among them and fp-exceptions.

1. Run paranoia: is your system IEEE 754 conformant?

2. Using the "float-as-int union" print exponent and mantissa of a given
   number, find "next and previous float", verify the representation of
   infinites, subnormals and NaN.

   * Suggest code to identify subnormals, NaN and infinites either checking
     their binary representation or using their "properties"

3. Write simple operations among fp and verify the results w.r.t. the
   expectations of the equivalent expression using "real numbers".

   * Measure the ulp-gap (and its size as fp-number!) between expectations and
     results 

   * Make them generating exceptions and verity which exceptions have been
     actually raised

   * Trap the exceptions using SIGFPE

4. Improve the accuracy of operations using refactoring, Kahan summation or
   other technique.

   * Compare the speed w.r.t. using double-precision

5. Apply all this to the minimization exemple

Code
----

* `lookInFloat.cpp <../exercises/math/exercises/lookInFloat.cpp>`_
* `fpe.cpp <../exercises/math/exercises/fpe.cpp>`_
* `kernels.cc <../exercises/math/exercises/kernels.cc>`_
* `loglike.cpp <../exercises/math/exercises/loglike.cpp>`_
* `paranoia.c <../exercises/math/examples/paranoia.c>`_

Hints
-----

::

  export CXX=g++-451
  export CXXFLAGS="-O2 -std=gnu++0x -msse4 -ftree-vectorize -ftree-vectorizer-verbose=1 -pthread -fPIC -fopenmp"
  export LD_LIBRARY_PATH=${PWD}/lib:${LD_LIBRARY_PATH}

  // use extern to avoid inlining (compile with -fPIC)
  // "volatile" can be used to avoid eager optimization

  objdump -S -r -C --no-show-raw-insn -w test.o | less
  (on MacOS: otool -t -v  -V -X test.o  | c++filt | less)

References
----------

* `IEEE 754 on Wikipedia <http://en.wikipedia.org/wiki/IEEE_754>`_

* `Single precision floating point format <http://en.wikipedia.org/wiki/Single_precision_floating-point_format>`_

* `man fetestexcept <http://linux.die.net/man/3/fetestexcept>`_

* `std::numerical_limits <http://www.cplusplus.com/reference/std/limits/numeric_limits/>`_
