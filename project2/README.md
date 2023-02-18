# Project 2: SIMD Matrix Multiplication

#### Building the Project
```
$ cd project2
$ mkdir build
$ cd build
$ cmake ..
$ make
```

#### Running the Project

Template:
```
$ ./ACS_Proj2 <square_matrix_size> <use_simd?>
```

Run 1000x1000 mult with SIMD enabled:
```
$ ./ACS_Proj2 1000 1
```

#### Benchmarking Test

These measurements are taken on a 12-core Intel i7-10850H @ 2.70GHz.

```
Performing 100x100 matrix calculations.
Float32 NonSIMD: 0.0002668s
Float32 SIMD: 7.69e-05s
Int16 NonSIMD: 0.000256s
Int16 SIMD: 0.0003113s

Performing 1000x1000 matrix calculations.
Float32 NonSIMD: 0.431881s
Float32 SIMD: 0.137632s
Int16 NonSIMD: 0.367756s
Int16 SIMD: 0.32713s

Performing 10,000x10,000 matrix calculations.
Float32 NonSIMD: DNF
Float32 SIMD: 1307.85s
Int16 NonSIMD: DNF
Int16 SIMD: 1678.37s
```

#### Analysis

Moving to SIMD provides a massive speed-up, significant even for smaller matrix sizes. Without SIMD, it wasn't even poissible to compute the 10,000x10,000 matrices in a reasonable timeframe. There are also some interesting nuances to the data access patterns that allow for better cache coherency than a naive approach. 
