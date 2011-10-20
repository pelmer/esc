Vector Algebra using SIMD instructions
======================================

The goal of this excercise is to learn how to implement and use 3- and
4-dimensional Vector Algebra using SIMD instructions.

1. Implement scalar product (invariant mass) between two Lorentz vectors
2. Implement Lorenz boost transformation
3. Measure speed-up

Code
----

* `SSEVec.h <../exercises/math/include/SSEVec.h>`_
* `SSERot.h <../exercises/math/include/SSERot.h>`_
* `SSEVec_t.cpp <../exercises/math/examples/SSEVec_t.cpp>`_

Hints
-----

::

  pfmon --long-smpl-period=5000 --resolve-addresses --smpl-per-function --smpl-show-top=20 ./a.out
  pfmon -e UNHALTED_CORE_CYCLES,ARITH:CYCLES_DIV_BUSY,SSEX_UOPS_RETIRED:SCALAR_SINGLE,SSEX_UOPS_RETIRED:PACKED_SINGLE ./a.out k

References
----------

* `Compiler intrinsics (by Visual Studio 2010) <http://msdn.microsoft.com/en-us/library/26td21ds.aspx>`_
* `Lorentz boost in Root::Math <http://project-mathlibs.web.cern.ch/project-mathlibs/sw/5_22_00/html/classROOT_1_1Math_1_1Boost.html#_details>`_
