PRACTICAL DEMONSTRATION #1

This demo is intended to get in toucht with the Linux page cache.

First of all let's have a look at the simple source clearcache.c

$ cat clearcache.c

This simple program:
1) opens the file specified on the command line;
2) uses the function fdatasync() (see man fdatasync) to flush file to disk 
if it still resides on "dirty" pages in the cache;
3) uses posix_fadvise() to tell (see man posix_fadvise) to tell the kernel 
that we do not intend to use the file soon;

The kernel will immediately remove this file from the page cache, such 
that every new access to the file will require read access from the disk

Compile the program with

$ gcc clearcache.c -o clearcache

Remember the position of this binary, as this will be used next.

Now issue the command

$ dd if=/dev/zero of=test.dat bs=1M count=1024

For who does not know dd yet (see man dd), this is a utility which allows 
one to make some simple operation on devices or files.

We have created a 1 GB file on the local disk, filled with zeroes, with 
sequential writes of 1 MB size each.

How fast was it? 1 GB/s? Your disk local can actually make something like 
150 MB/s, hence 1 GB/s is impossible.

Now on a different window, launch

$ top -d1

and keep an eye on the value you read on the top right, named as "cached". 
Issue the command

$ ~/io_exercises/demo1/clearcache test.dat

You should see on the top window that the cached size decreased of exactly 
1 GB (maybe 2 GB if you machine-mate made it simultaneusly with you).

You got 1 GB/s because you were writing straight to the cache. The OS will 
write the file to disk asynchronously for you.

Now let's read back the file (and throw it to the null device)

$ dd of=/dev/null if=test.dat bs=1M count=1024

Depending on the synchronicity with you machine-mate you should get a 
number between 75/s and 150 MB/s. This is what really your local disk can 
sustain.

Now try to reread it

$ dd of=/dev/null if=test.dat bs=1M count=1024

you should get an amazing number, say 4 GB/s or so. You were reading from 
the page cache, there was no real I/O on disk.

How much memory your machine can dedicate to caching?

$ cat /proc/meminfo  | head -n4
MemTotal:     37037656 kB
MemFree:      29837268 kB
Buffers:        428636 kB
Cached:        2127116 kB

It can use the total available memory. In this example you can still load 
your cache with other 29 GB of files if you want. This looks a lot, but 
your machines have 12 cores with 36 GB total RAM, i.e. 3 GB per core. Once 
you start 12 applications consuming their own HEAP memory, you might end 
up with limited availability for your page cache.

Now locate your directory studxx inside /storage/gpfs_esc11 and change
directory into it. Repeat the dd write writing on GPFS. What changes?

Now you are writing into the network. GPFS has its own internal caching
strategy, and bypasses completely the Linux page cache. Whether or not the
Linux page cache is used, depends on the filesystem implementation!

