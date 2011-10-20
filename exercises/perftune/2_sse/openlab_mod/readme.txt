"make all" will compile four benchmarks, which are also individual make targets:

"single" - the original SSE vectorised single precision benchmark
"double" - SSE vectorised double precision benchmark
"tbb" - multithreaded benchmark using Intel Threading Building Blocks with parallel_for() (based on "single")
"x87" - uses double precision x87 arithmetic, not vectorised

The "tbb" binary takes a number as argument, which is the number of tasks. #threads <= #tasks, depending #cores. e.g. "./tbb 4" will run 4 threads on 8 cores and 2 threads on 2 cores.


