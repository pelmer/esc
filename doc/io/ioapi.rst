I/O API techniques
==================

In this demonstration we will examine some examples which make use of the 
I/O techniques we have discussed in the lecture.

Basic I/O with changing offset
------------------------------

Let's start with basics. Have a look at the file basic_io.c

Now compile it with::

  $ gcc -O2 basic_io.c -o basic_io

Generate the input file::

  $ dd if=/dev/zero of=bigfile.dat bs=1M count=1024

Now clear the cache::

  $ ~/io_exercises/demo1/clearcache bigfile.dat

In a separate window launch the command::

  $ iotop -b -a -d0.1 -u $USER 2>&1 | grep basic_io | egrep -v grep

Then launch in your main window::

  # ./basic_io

How much does it read from disk? Change the offset to 8192 in basic_io.c, 
recompile, clean the cache and retry. How much does it read now?

Vectored (scatter-gather) I/O
-----------------------------

This is a simple demonstration of vectored I/O over the network. The same
readv() and writev() calls can in fact be used also in network 
communication. In this example writev.c sends a list of vectors and 
readv() receives them. Let's have a look at the programs.

Compile with::

  $ gcc -O2 readv.c -o readv
  $ gcc -O2 writev.c -o writev

and run::

  $ ./readv &
  $ ./writev

Vectored (scatter-gather) I/O
-----------------------------

Let's first have a look at mmap_simple.c

Compile it with::

  $ gcc -O2 mmap_simple.c -o map_simple

Generate a zeroed input file with dd::

  $ dd if=/dev/zero of=mmapped.bin bs=1M count=1024

Check the file is zeroed::

  $ hexdump mmapped.bin | less

Launch mmap_simple::

  $ ./mmap_simple

Check again the content of the file::

  $ hexdump mmapped.bin | less

Launch again mmap_simple and check again. As you can see, with simple 
memory accesses we are actually writing a file.

Now another example which uses mmap to make a copy of the file

Compile it::

  $ gcc -O2 mmap_copy.c -o mmap_copy

and execute it::

  $ ./mmap_copy mmapped.bin newfile.bin

Check they are equal::

  $ diff mmapped.bin newfile.bin 

Asynchronous I/O
----------------

First example, use AIO to copy a source file to a destination file.
Let's look at aio_copy.c

Compile with::

  $ gcc -O2 aio_copy.c -o aio_copy -lrt

and run with::

  $ ./aio_copy newfile.bin newfile.2.bin 64
