Reimplementing ``C++`` in ``C``
===============================

The goal of this exercise is to understand the relative cost of various C++
idioms by reimplementing them in ``C``, more or less as what was done with
``CFront``.

Steps
-----

1. Set up your environment::

     . /storage/software/main/env-gcc461.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code:
    * `class-a.cc <../exercises/c++/class-a.cc>`_
    * `class-b.cc <../exercises/c++/class-b.cc>`_

4. Reimplement the static method and the virtual/non-virtual ones in plain
   ``C`` functions operating on a simple ``C`` struct.

Use the same techniques as glossed over during the lecture to implement
virtual methods.
