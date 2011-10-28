PRACTICAL DEMONSTRATION #2

This demo is intended to get in touch with some I/O monitoring and 
profiling tools.

Open 4 different terminals on 4 windows, if possible, and move them to 
your screen so that you can have a look at all of them at the same time.

On different windows, issue the following 4 commands:

$ top -d1

$ dstat -D sdb

$ iostat -N -m 1

$ iotop

Now on a fifth window issue the command

$ dd if=/dev/zero of=test.dat bs=1M count=1024 oflag=sync

When the command finishes, it lasts 10-20 seconds, issue it again if 
needed. The oflag=sync tells dd to sync to disk after every write 
operation. In this case the flush to disk will be synchronous.

Observe what happens on the 4 monitoring windows:
1) top (and also dstat) tells you that there is a non negligible I/O wait, 
i.e. the disk is saturating and the usable CPU load is very low
2) dstat gives you the disk I/O versus time in some unit per second
3) iostat tells you specifically which devices are being used
4) finally iotop gives you how much I/O is done on a process-by-process 
basis

Now reread it
$ dd of=/dev/null if=test.dat bs=1M count=1024

clear the file from the cache
$ ~/io_exercises/demo1/clearcache test.dat

Reread it again. If you want try to change some parameters in dd (but 
avoid writing 100 GB files!) and see what happens.


Now let's see how strace and ltrace work. Issue the command

$ strace dd if=/dev/zero of=test.dat bs=1M count=1024 oflag=sync

You will see a dump of all the system calls. In particular you will see a 
long sequence of read() calls (from special device /dev/zero) and write() 
calls (to your test.dat file). CTRL-C to interrupt.

Then try with

$ strace -c dd if=/dev/zero of=test.dat bs=1M count=1024 oflag=sync

Upon completion, after a few seconds (you can interrupt always with 
CTRL-C) it will write a summary of system calls. In this trivial example 
you will see that most of the time is spent in write() calls. You can see 
how many calls of each type are issued, and the average duration.

Now try with

$ ltrace -c dd if=/dev/zero of=test.dat bs=1M count=1024 oflag=sync

ltrace is very similar, but intended to trace library calls instead of 
system calls. Some I/O library calls are directly mapped to system calls. 
You will find in the summary also any other library call has been issued.


Now, the interesting thing with strace detailed dump, is that it can be 
used to diagnose very accurately what the application is doing. You can 
decide to dump timing of all I/O calls, including random movements of the 
file descriptor pointer, e.g. lseek() calls, size of the read() or write() 
requests, etc.. By analysing the trace you can produce useful plots to 
understand e.g. how randomic is your access pattern.

Launch the command

$ strace -q -a1 -s0 -f -tttT -o strace.out -e \
  trace=file,desc,process,socket dd if=/dev/zero of=test.1.dat bs=64k \
  count=16384 oflag=sync

See man strace for details on the various options.

When finished, have a look at the file strace.out. In particular now it 
contains timestamp of every operation. You might use the information to 
build useful plots. Here we use a graphical tool which is able to 
interpret this output.

Launch the command

$ ioprofiler.py

On the GUI, go to File -> Open, then change "Files of type" to "Pure 
strace output", and select your strace.out. Now switch the button 
at the bottom of the main window to "Writes" and select on the top the 
"test.1.dat" row. Finally click on the "Details" button. Default plot 
is "Pattern diagram". You can switch to "Size histogram", "Time 
histogram", "Speed histogram" by selecting the appropriate buttons.

The sheet on the left gives you details of all the I/O calls, writes in 
this case.

Now have a look at the small test program randread.c, and compile it with

$ gcc -O2 randread.c -o randread

Clear the cache

$ ~/io_exercises/demo1/clearcache test.1.dat

$ strace -q -a1 -s0 -f -tttT -o strace.out -e \
  trace=file,desc,process,socket ./randread test.1.dat 65536

Open ioprofiler.py as before and select the strace.out. Note that now you 
are reading the file, not writing. You should find that the "time vs 
offset" is completely scattered, there's no pattern, this is a purely 
random access pattern. Check also the other plots with time and speed 
distributions. How to interpret?


Finally do the same with a simple ROOT macro

$ strace -q -a1 -s0 -f -tttT -o strace.out -e \
  trace=file,desc,process,socket root -b -q -l BenchTree.C

and analyse it with ioprofiler.py
