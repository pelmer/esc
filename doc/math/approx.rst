Approximate Math
================

Goal of this exercise is to learn how to speed-up software choosing the
appropriate precision in floating point math

1. use reciprocal instructions to speed-up divisions and square-roots

   * check if the compiler can generate the same code (hint: try fast-math)

2. using algorithm based on look-up tables or small-polynomial expansions
   find the best accuracy

   * to fill a typical eta-phi histogram (user defined bins)

   * to build and fill a log scale "histogram" (either optimized or
     user-defined bins)

   * to sample and interpolate a function in log scale

Code
----

* `approxMath.cpp <../exercises/math/examples/approxMath.cpp>`_
* `approxLog.cpp <../exercises/math/examples/approxLog.cpp>`_
* `approxPhi.cpp <../exercises/math/examples/approxPhi.cpp>`_

References
----------

* `Fast inverse square root on Wikipedia (and references therein)
  <http://en.wikipedia.org/wiki/Fast_inverse_square_root>`_

* `icsi-log technical paper
  <http://www.icsi.berkeley.edu/pubs/techreports/TR-07-002.pdf>`_

* `fast math in vlfeat <http://www.vlfeat.org/api/mathop_8h.html>`_
