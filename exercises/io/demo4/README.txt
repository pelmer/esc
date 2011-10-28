PRACTICAL DEMONSTRATION #4

Here we try to compare I/I to local disk, I/O to a NFS filesystem and I/O to
a GPFS filesystem.

Let's run the ROOT macro BenchTree.C that we have already used before.
While program is running, observe the output of dstat on a different
window.

First on the local disk

$ root -b -q -l ~/io_exercises/demo4/BenchTree.C

Then on the NFS server. The performance on the NFS server will depend on
how many of you are executing the program at the same time. You will notice
a slight degradation if running alone, and a horrible performance if you
run in concurrency with all the others. Let's try to make it in sync.
Try to repeat the measurement if it finishes too fast. If it takes too
long, you can CTRL-C the program.

$ cd /storage/nfs_esc11/$USER
$ root -b -q -l ~/io_exercises/demo4/BenchTree.C

Now let's try with GPFS. If you run alone, you will get more or less the
same as the local disk. If you run in parallel with many other students...
well I did not try on this system, let's see. Note that this is a toy system. 
A large system in a computing data centre can be 100 times faster than this. 

$ cd /storage/gpfs_esc11/$USER
$ root -b -q -l ~/io_exercises/demo4/BenchTree.C

