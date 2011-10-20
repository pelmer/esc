Shared libraries
================

The goal of this exercise is to understand the memory footprint resulting from
shared libraries.

Make 100 dummy shared libraries, e.g. each with a single function inside,
e.g. ``foo001()``, ``foo002()``, or something similar.  Link these all to a
simple standalone main which waits for a bit of input such that you can look
at it with ``top``, ``pmap``, etc.  Do you understand what you see?  Try the
same test for both 32bit and 64bit.

You probably want to write a small ``perl``, ``python`` or shell script to
help you here!
