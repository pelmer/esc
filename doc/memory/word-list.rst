Word List
=========

In this exercise you will work on a small test program,
`word-list.cc <../exercises/memory/word-list.cc>`_, which sorts a
filtered list of unique words, then prints out middle *N* of them.
The program is inefficient, and even has bugs.  You will use basic
tools such as ``top``, ``ps``, ``pmap`` and ``pstack``, and profiling
and debugging tools ``valgrind`` and ``igprof``, to understand,
correct and optimise the program.

1. Make sure your environment is correct::

     c++ -v 2>&1 | grep version  # should say 'gcc version 4.8.2 (GCC)'
     valgrind --version          # should say 'valgrind-3.8.1'
     igprof -h                   # should print simple help message

2. Go to the exercise directory::

     cd esc/exercises/memory

3. Make yourself a test dataset of 16 random 30'000-word files::

     perl -ne '
       chomp; $W{$_}++;
       END { use List::Util "shuffle";
             print map { "$_\n$_\n" } (shuffle keys %W)[0...99999] }
       ' < /usr/share/dict/words > words-random

     split -a 1 -l 25000 words-random words-part-
     head -12 words-random > words-short

4. Compile the program::

     c++ -g -W -Wall -Werror -ansi -pedantic -o word-list word-list.cc -ldl -lpthread

5. Run the program with different arguments::

     time ./word-list N 'gle' words-part-*
     time ./word-list N '[ceix]' words-part-*
     time ./word-list N '[badrx}' words-part-*
     time ./word-list N 'gle' words-short

6. While the program is running, use other terminal windows to examine it::

     top
     ps wuf -C word-list
     pmap $(ps -o pid= -C word-list)
     pstack $(ps -o pid= -C word-list)

7. Check for memory access errors and leaks. Translate the diagnostics to the
   specific problems in the source code::

     valgrind --num-callers=50 --leak-check=full \
       ./word-list N 'gle' words-part-[abc]

     valgrind --num-callers=50 --leak-check=full \
       ./word-list N '[ceix]' words-part-a

     valgrind --num-callers=50 --leak-check=full \
       ./word-list N '[badrx}' words-part-[abc]

     valgrind --num-callers=50 --leak-check=full \
       ./word-list N 'gle' words-short

8. Profile where the program spends it's time. Again translate the profile
   information to specific issues in the source code::

     igprof -o perf-simple.gz -d -pp ./word-list N 'gle' words-part-*
     igprof -o perf-pattern.gz -d -pp ./word-list N '[ceix]' words-part-*

     igprof-analyse -d -v -g perf-simple.gz > perf-simple.res 2>&1
     igprof-analyse -d -v -g perf-pattern.gz > perf-pattern.res 2>&1

     less -X perf-*.res

9. Profile program total memory allocations, leaks and heap snapshots.  For
   one stage, ``sort_words_uniq``, compare difference profile for pre and post
   heap snapshots::

     igprof -o mem-simple.gz -d -mp ./word-list Y 'gle' words-part-*
     igprof -o mem-pattern.gz -d -mp ./word-list Y '[ceix]' words-part-*
     igprof -o mem-bad.gz -d -mp ./word-list Y '[badrx}' words-part-*

     igprof-analyse -d -v -g -r MEM_TOTAL mem-simple.gz > mem-simple.res 2>&1
     igprof-analyse -d -v -g -r MEM_TOTAL mem-pattern.gz > mem-pattern.res 2>&1
     igprof-analyse -d -v -g -r MEM_LIVE  mem-bad.gz > mem-bad.res 2>&1

     less -X mem-*.res

     for f in igprof.*.gz; do
       igprof-analyse -d -v -g -r MEM_LIVE $f > $f.res 2>&1
     done

     less -X igprof*.res

     for f in igprof.*.sort_words_uniq.pre.gz; do
       igprof-analyse -d -v -g -r MEM_LIVE \
         -b $f $(echo $f | sed 's/\.pre\./.post./') \
         > $f.res.sortdiff 2>&1
     done

     less -X igprof*.res.sortdiff

10. Optimise the program and remove memory leaks from it as you best see
    fit. Cross-check your results always by running::

      grep -iv <rx> <file> | uniq | wc -l

11. Submit as your final result the output from the command (step #5),
    valgrind messages from step #7, overall timing comparison with grep/uniq
    (% faster/slower), output of ``head -25 perf-*.res mem-*.res`` and how you
    might further improve performance or quality, or if why that would no
    longer be possible or make sense.

References
----------

* `IgProf <http://igprof.org>`_
* `Valgrind <http://valgrind.org>`_
