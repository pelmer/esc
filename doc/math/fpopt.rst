(Compiler) Optimization of fp expressions
=========================================

The goal of this exercise is to learn how to control and verify the
optimizations performed by a compiler on fp expressions.

1. Run paranoia with different optimization flags: what's Kahan opinion?

2. Write simple kernels and inspect the generated object code: identify the
   various operation at assembler level

3. Compile code with different optimization options and verify the generated
   code

   * Use also pragmas and function attributes to specify the optimization at
     different level of granularity

   * Find cases when optimization either changes exceptions' behaviour or
     produces wrong results

4. Learn how to avoid common pitfalls
   * undesired conversions
   * redundant calls
   * non inlining
   * loss of precision

5. Verify the gain in performance when taking the "correct" approach

6. Apply all this to the minimization exemple

Code
----

* `kernels.cc <../exercises/math/exercises/kernels.cc>`_

Hints
-----

::

  // setenv OPTFLAGS "-fassociative-math -freciprocal-math -fno-math-errno -fno-signed-zeros -fno-trapping-math -ffinite-math-only"
  // use extern to avoid inlining (and compile with -fPIC)
  // "volatile" can be used to avoid eager optimization
  // compile paranoia with -DVOLATILE and/or -DNOSUB

  pfmon  --long-smpl-period=5000 --resolve-addresses  --smpl-per-function --smpl-show-top=20 ./a.out
  pfmon -e UNHALTED_CORE_CYCLES,ARITH:CYCLES_DIV_BUSY,SSEX_UOPS_RETIRED:SCALAR_SINGLE,SSEX_UOPS_RETIRED:PACKED_SINGLE ./a.out k

  objdump -S -r -C --no-show-raw-insn -w test.o | less
  (on MacOS: otool -t -v  -V -X test.o  | c++filt | less)

References
----------

* `GCC online manual <http://gcc.gnu.org/onlinedocs/gcc-4.5.0/gcc/>`_

  * `Function Attributes section <http://gcc.gnu.org/onlinedocs/gcc-4.5.0/gcc/Function-Attributes.html#Function-Attributes>`_

  * `Optimize Options section <http://gcc.gnu.org/onlinedocs/gcc-4.5.0/gcc/Optimize-Options.html#Optimize-Options>`_

* `man gcc <http://linux.die.net/man/1/gcc>`_

