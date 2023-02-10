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
$ ./ACS_Proj2 <square_matrix_size> <num_iterations> <use_simd?>
```

Run 1 iteration of 1000x1000 with SIMD enabled:
```
$ ./ACS_Proj2 1000 1 1
```

#### Benchmarking Test

These measurements are taken on a 12-core Intel i7-10850H @ 2.70GHz.

```
- 1,000x1,000, Non-SIMD -
Float32: 1.79538s
Float64: 1.79116s
Int16:   1.47309s
Int32:   1.42196s
Int64:   1.5387s

- 10,000x10,000, Non-SIMD -
Float32: DNF
Float64: DNF
Int16:   DNF
Int32:   DNF
Int64:   DNF

- 1,000x1,000, SIMD -
Float32: 0.309114s
Float64: 0.331586s
Int16:   0.262672s
Int32:   0.308254s
Int64:   0.314862s

- 10,000x10,000, SIMD -
Float32: 302.742s
Float64: 347s
Int16:   266.227s
Int32:   305.474s
Int64:   335.092s
```

#### Analysis

Moving to SIMD provides a massive speed-up, significant even for smaller matrix sizes. Without SIMD, it wasn't even poissible to compute the 10,000x10,000 matrices in a reasonable timeframe.
