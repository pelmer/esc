Basic Tools, System Aspects
===========================

The goal of this exercise is to understand how the operating system allocates
memory for simple processes.  You will use basic system tools including
``readelf``, ``size``, ``pmap``, ``top``, ``numactl`` and ``/proc`` linux
pseudo-filesystem to investigate virtual and physical memory layout of basic
applications.

Steps
-----

1. Open a terminal window. For the exercises on this page, we are using
   tools from the operating system so it is not necessary to source any
   extra environment script (although it should not hurt if you do).

2. Execute the following command.  It tells you which shell you are executing
   in your terminal; it should print out /bin/bash::

       echo $SHELL

   We will be using the /bin/bash binary as an example of an executable
   in the following exercises.

3. Execute the following command.  It tells how the shell executable is laid
   out for the operating system to load into memory.  Make note of the dynamic
   linker to be used, and which file byte ranges will get memory mapped and
   with which alignments::

       readelf -Wl $SHELL

4. Execute both commands below.  The ``readelf -Wd`` command tells you which
   additional shared libraries the dynamic linker will load when the program
   first starts up.  Compare the output to the ``ldd`` command output.  It
   should look similar, but not quite the same.  Run the commands on at least
   two other test servers; again you should see some changes in the output.
   How do you explain the differences?::

       readelf -Wd $SHELL
       ldd $SHELL

5. Execute both commands below.  They tell you the actual makeup of the
   executable (and shared libraries).  Compare the output to the output from
   #3 (``readelf -Wl``) and understand all the columns.  What are the
   differences between the ``readelf`` and ``size`` output?  What are the
   different sections for, which ones will be loaded, and with what sort of
   memory permissions?  How will this data be shared across multiple
   executions?::

       readelf -WS $SHELL
       size -A $SHELL

6. Execute all the commands below.  They give you detailed information about
   the shell process you are executing in the terminal window.  Work out the
   similarities and differences with the first ``readelf`` (-Wl) and ``ldd``
   commands, the ``/proc`` maps and ``pmap`` output.  How much of allocated
   address space is actually resident.  What additional libraries and other
   files are listed mapped in \*maps?  What are the items in brackets? (Search
   google for the ones named v<something>!) How much heap has been allocated
   to this process?  How big is the stack at the moment, and how big could it
   get?::

       ls -laF /proc/$$
       less -X /proc/$$/maps
       less -X /proc/$$/smaps
       pmap -x $$
       pmap -d $$

7. Execute ``top``.  Type the following letters one at a time to get enhanced
   output: ``z x c b f j 1 o J J J J J J J J 1 1 1``.  That's letter J eight
   times, to move the P column just before %CPU, and letter 1 (one) three
   times, first to get out of option mode, then to toggle CPU state display
   twice.

   Type ``O`` to change the sort order, try at least memory use (``n``), virtual
   image (``o``) and CPU use (``k``).  You can also use ``<`` and ``>``
   characters in live display to switch sort column leftwards and rightwards,
   respectively.

   You can save your settings with ``W``.

8. Execute steps 3-7 for more significant programs, for example
   ``/usr/bin/gvim`` and ``firefox`` -- launch both programs and see with
   ``ps`` which exact program firefox actually runs.  Why do ``readelf -Wd``,
   ``ldd`` and ``/proc/pid/maps`` now show so different results?  In general,
   compare outputs.

9. EXTRA: Execute the command below to produce the physical memory map and
   BIOS NUMA configuration of the system you are running on, and to get
   detailed information about the NUMA features of the system, in particular
   how many nodes the operating system thinks it is scheduling for::

       grep -e '^ BIOS' /var/log/dmesg
       awk '/^NUMA/,/^ACPI/ {print} {}' /var/log/dmesg
       numactl --hardware
       numactl --show
       numastat
       less -X /proc/$$/numa_maps

11. EXTRA: see how much more you can learn using ``/proc/$$/pagemap``

12. EXTRA: Execute the following. Work out how many physical CPUs and cores
    you have, and how many "soft" CPUs (hyperthreads), and how they are
    arranged for operating system scheduling::

        less -X /proc/cpuinfo
        grep -e '^CPU' -e ' node' -e NODE -e domain -e group /var/log/dmesg

References
----------

* `readelf man page <http://linux.die.net/man/1/readelf>`_
* `ldd man page <http://linux.die.net/man/1/ldd>`_
* `size man page <http://linux.die.net/man/1/size>`_
* `pmap man page <http://linux.die.net/man/1/pmap>`_
* `numactl man page <http://linux.die.net/man/8/numactl>`_
* `numastat man page <http://manpages.ubuntu.com/manpages/lucid/man8/numastat.8.html>`_
* `numa_maps man page <http://linux.die.net/man/5/numa_maps>`_
* `/proc filesystem man page <http://linux.die.net/man/5/proc>`_
* `/proc/pid/pagemap overview <http://lxr.linux.no/linux+v2.6.31/Documentation/vm/pagemap.txt>`_
