Loop vectorization using SIMD instructions
==========================================

The goal of this excercise is to speed-up loop execution streaming fp data in
parallel (four by four) through SIMD instructions.

1. Vectorize the generation of normally (gaussian) distributed random numbers
2. Vectorize the chi2 calculation (including kahan summation)
3. Vectorize "vl_fast_atan2_f"
4. Try to "convince" the compiler to vectorize loops using ftree-vectorize
5. Measure speed-up
6. Apply all this to the minimization example
7. A long term project: vectorize the Wallace's normal random number generator in C++

Code
----

* `SSEArray.h <../exercises/math/include/SSEArray.h>`_
* `gaussian_ziggurat.h <../exercises/math/include/gaussian_ziggurat.h>`_
* `SSEMathFun_t.cpp <../exercises/math/examples/SSEMathFun_t.cpp>`_ ("the vectorized loop")
* `approxPhi.cpp <../exercises/math/examples/approxPhi.cpp>`_ (approximated atan2 to vectorize)
* `exRandom.cpp <../exercises/math/examples/exRandom.cpp>`_
  (example of random generators' usage and blending of a 16-wide vector)

Hints
-----

::

  pfmon  --long-smpl-period=5000 --resolve-addresses --smpl-per-function --smpl-show-top=20 ./a.out
  pfmon -e UNHALTED_CORE_CYCLES,ARITH:CYCLES_DIV_BUSY,SSEX_UOPS_RETIRED:SCALAR_SINGLE,SSEX_UOPS_RETIRED:PACKED_SINGLE ./a.out k

References
----------

* `a partial SSE port of the cephes floating point algorithms <http://gruntthepeon.free.fr/ssemath/>`_
* `Mersenne Twister Home Page <http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html>`_
* `Normal Distribution on wikipedia <http://en.wikipedia.org/wiki/Normal_distribution#Generating_values_from_normal_distribution>`_
* `David B. Thomas; Philip G.W. Leong; Wayne Luk; John D. Villasenor
  (October 2007). "Gaussian Random Number Generators" (pdf) <http://www.cse.cuhk.edu.hk/~phwl/mt/public/archives/papers/grng_acmcs07.pdf>`_
* `A fast vectorised implementation of Wallace's normal random number generator <http://arxiv.org/abs/1004.3114>`_
* `Random Number Generation on GPUs <http://http.developer.nvidia.com/GPUGems3/gpugems3_ch37.html>`_
