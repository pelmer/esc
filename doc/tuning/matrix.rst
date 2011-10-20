Matrix multiplication
=====================

*The goal of this exercise is to demonstrate the viability of theoretical
analysis to predict the behavior of complex x86 hardware.  An additional goal
is to demonstrate the optimization steps of a modern compiler.*

Two double-precision matrices, A and B, are being multiplied to produce matrix
C.  There are two source files available, and one of them contains the
multiplication routine (``multiply.c``), while the other the management and
time measurement functions (``matrix.c``).  Consider what you learned during
the lectures, and try to establish the maximum speed at which a modern
processor could perform this task.

* **Establish the approximate number of operations to multiply two NxN
  matrices**

* **Establish the formula for the minimum number of cycles to do the above**

* **Re-calculate the formula from the 2nd bullet assuming N=1024 and double
  precision data**

Examine the source, compile and run it::

  icc –o matmul matrix.c multiply.c
  ./matmul

What do you think of the performance?

Now run ``rebuild_everything.sh``, which will generate optimized files for
several different scenarios.  Compare the percentage of peak throughput,
cycles, instructions, CPI, loads, stores and floating point work across
different scenarios.  You can use a supplied script to do that::

  ./measure.sh ./matmul

Or measure everything at once::

  ./measure.sh all

What are the speedups? What is the speedup if you compare the best case to the
DEFAULT case (no options!)?

What can you observe regarding the ``–g`` flag?
