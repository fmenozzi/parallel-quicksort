# Parallel Quicksort
Implementation of parallel quicksort using the Cilk Plus parallel runtime for my parallel and distributed computing class. Both the implemetations of `quicksort` itself and (more importantly) `partition` were implemented in parallel. Results below were generated on a 20-core Xeon E5-2650 server with eight attached Intel Xeon Phi 5110P accelerators. For now, the code explicitly assumes a 20-core system, as quicksort will default to the C function `qsort` when the max level of parallelism has been achieved (i.e. when `n < N/P`).

Requires Intel C/C++ compiler (`icc`) to run.

The following results were obtained for sorting pseudo-random, 64-bit floating point numbers:

**Sequential (qsort)**
    
            N | Running Time (s)
-------------:|----------------:
           10 |   0.000008
          100 |   0.000018
        1,000 |   0.000382
       10,000 |   0.002048
      100,000 |   0.017653
    1,000,000 |   0.203481
   10,000,000 |   1.998444
  100,000,000 |  20.931379
1,000,000,000 | 235.277135

**Parallel**
    
            N | Running Time (s)
-------------:|----------------:
           10 |  0.038327
          100 |  0.011083
        1,000 |  0.019024
       10,000 |  0.038175
      100,000 |  0.059049
    1,000,000 |  0.139344
   10,000,000 |  0.743794
  100,000,000 |  4.779819
1,000,000,000 | 39.562650

## Build, run, clean
    $ make
    $ ./seq N
    $ ./par N
    $ make clean
