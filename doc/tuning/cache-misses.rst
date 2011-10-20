Cache misses
============

*The goal of this exercise is a practical demonstration of cache miss behavior
and the usefulness of triggering functionality in performance monitoring
tools.  An additional goal is the demonstration of various optimization paths
of compilers.*

The exercise program contains a simple linked list, which is first being
filled with some numbers and then being walked through in a loop.  Use pfmon
to observe cache miss behavior.  For example::

  pfmon –e MEM_LOAD_RETIRED:L3_MISS,MEM_LOAD_RETIRED:L3_UNSHARED_HIT \
     --trigger-code-start=walk ./linkedthrash 7000

**Fill in Table 4.**

What is a good solution (in general) to walk a linked list?  What kind of
optimizations could you think of?

Remove the pointer printing at the end of ``walk()`` and recompile and rerun.
**Fill in Table 5.**

What do you think happened?  You can use ``objdump`` to verify your
assumptions (ask the lecturers for help when in doubt).
