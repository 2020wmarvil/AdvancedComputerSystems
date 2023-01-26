# AdvancedComputerSystems

#### Generating a Large Test File
```
tr -dc "A-Za-z 0-9" < /dev/urandom | fold -w100|head -n 100000 > bigfile.txt
```


#### Building the Project
```
sudo apt install cmake
cd project1
mkdir build
cd build
cmake ..
make -j8
```

#### Running the Project
```
./ACS_Proj1 <input_filename> <compression_level> <num_threads>
```

#### Benchmarking Test
```
---- L1 Compression ----
1 Thread:
real	0m13.846s
user	0m12.751s
sys	0m5.018s

2 Thread:
real	0m10.171s
user	0m12.981s
sys	0m5.160s

4 Thread:
real	0m9.613s
user	0m14.013s
sys	0m5.212s

8 Thread:
real	0m9.528s
user	0m14.858s
sys	0m5.297s

16 Threads:
real	0m9.936s
user	0m14.933s
sys	0m5.373s

---- L2 Compression ----
1 Thread:
real	0m14.432s
user	0m13.287s
sys	0m5.053s

2 Thread:
real	0m10.361s
user	0m13.816s
sys	0m5.273s

4 Thread:
real	0m9.619s
user	0m14.297s
sys	0m5.091s

8 Thread:
real	0m9.916s
user	0m15.618s
sys	0m5.384s

16 Threads:
real	0m9.864s
user	0m15.222s
sys	0m5.456s

---- L3 Compression ----
1 Thread:
real	0m15.505s
user	0m14.585s
sys	0m5.416s

2 Thread:
real	0m10.806s
user	0m14.916s
sys	0m5.197s

4 Thread:
real	0m9.703s
user	0m16.063s
sys	0m5.165s

8 Thread:
real	0m10.038s
user	0m17.576s
sys	0m5.453s

16 Threads:
real	0m9.696s
user	0m17.913s
sys	0m5.431s

---- L4 Compression ----
1 Thread:
real	0m17.205s
user	0m15.866s
sys	0m5.781s

2 Thread:
real	0m11.711s
user	0m15.756s
sys	0m5.613s

4 Thread:
real	0m10.532s
user	0m18.331s
sys	0m5.656s

8 Thread:
real	0m10.160s
user	0m19.857s
sys	0m5.722s

16 Threads:
real	0m10.859s
user	0m21.521s
sys	0m5.898s

---- L5 Compression ----
1 Thread:
real	0m47.693s
user	0m48.079s
sys	0m5.620s

2 Thread:
real	0m27.408s
user	0m52.978s
sys	0m6.048s

4 Thread:
real	0m24.290s
user	1m31.466s
sys	0m7.198s

8 Thread:
real	0m41.484s
user	4m50.500s
sys	0m11.628s

16 Threads:
real	0m55.196s
user	9m14.772s
sys	0m18.593s
```

#### Analysis

There is a consistent 2x speed up when going from 1 thread to 2 threads. This is what one would expect. Perf improvements are not consistent, though, as doubling again from 4, to 8, and to 16 yield increasingly negligble improvements. This is most likely due to the overhead in managing thread scheduling, or perhaps there is some shared resource that is being locked consistently. The reduced improvement in performance could also be due to my machine using those threads for other applications running on my machine, and thus the program has to fight for control over the available CPU cores.  
