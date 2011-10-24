Exceptions
==========

The goal of this exercise is to understand the cost coming from the use of
exceptions.

Steps
-----

1. Set up your environment::

     . /storage/software/main/env-gcc461.sh

2. Go to the exercise directory::

     cd esc/exercises/c++

3. Review exercise code:
    * `exception-lib1.h <../exercises/c++/exception-lib1.h>`_
    * `exception-lib1.cxx <../exercises/c++/exception-lib1.cxx>`_
    * `exception-lib2.h <../exercises/c++/exception-lib2.h>`_
    * `exception-lib2.cxx <../exercises/c++/exception-lib2.cxx>`_
    * `exception-liberr.h <../exercises/c++/exception-liberr.h>`_
    * `exception-liberr.cxx <../exercises/c++/exception-liberr.cxx>`_
    * `exception-main.cxx <../exercises/c++/exception-main.cxx>`_

4. Build a little program based on the lectures (page 13-14) and modify it
   to work with and without exceptions (but with error handling)::

     rm -f *.s *.o *.ii *.so
     c++ -shared -o libexc-test1.so -g -W -Wall -Werror -fPIC exception-lib1.cxx
     c++ -shared -o libexc-test2.so -g -W -Wall -Werror -fPIC exception-lib2.cxx
     c++ -o exception-main -g -W -Wall -Werror exception-main.cxx -L. -lexc-test{1,2}

5. Compile with various optimization levels

6. Inspect the assembly, try to make sense of everything::

     objdump -rCSd --no-show-raw exception-main libexc-test{1,2}.so

7. Check the speed with and without exceptions to ensure the zero-cost overhead

8. Check the speed when an exception is thrown

References
----------

* `Technical report on C++ performance (TR-18015)
  <http://www.open-std.org/jtc1/sc22/wg21/docs/TR18015.pdf>`_

* `Optimizing Away C++ Exception Handling
  <http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.116.8337&rep=rep1&type=pdf>`_

* `Exception handling [wikipedia]
  <http://en.wikipedia.org/wiki/Exception_handling>`_

* `Disabling/Using C++ exceptions in GCC
  <http://gcc.gnu.org/onlinedocs/libstdc++/manual/using_exceptions.html>`_
