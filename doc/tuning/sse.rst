SSE instructions
================

*The goal of this exercise is to demonstrate the microarchitectural behavior
of vector optimized programs, and the response of the performance counters to
such optimizations.*

In this exercise, you will use three programs, which can be built by running
``make all``:

* scalar: uses SSE, the data type is scalar double

* double: uses SSE, the data type is packed double

* single: uses SSE, the data type is packed single

Use ``pfmon_deluxe`` and **fill in Table 2 from the course materials.**

The "instructions retired" count does not decrease linearly on a logarithmic
scale, however there is one figure which decreases almost linearly.  Which one
and why?

When comparing the 3 examples, can you explain why the CPI went up and why
despite of that the execution time is much lower?
