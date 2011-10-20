C++ iostreams and C stdio
=========================

The goal of this exercise is to be aware of the cost of synchronization
between C++ and C I/O systems.  C++ iostreams and C stdio (printf-like) are
synchronized to allow interleaved calls to both I/O systems such as::

  std::cout << "hello" << std::endl;
  std::printf(" world\n");
  std::cout << "bye." << std::endl;

to produce the expected output.  Do a test with and without synchronized I/O
Check if the timings make sense.

References
----------

* `ios_base::sync_with_stdio reference
  <http://www.cplusplus.com/reference/iostream/ios_base/sync_with_stdio/>`_
