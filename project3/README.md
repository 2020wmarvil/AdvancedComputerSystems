# Project 3: Memory and Storage Performance Profiling

#### Environment

- OS: Ubuntu 20.04
- CPU: Intel(R) Core(TM) i7-10850H CPU @ 2.70 GHz (12 cores)
- Memory: 2x16 GiB SODIMM DDR4 @ 3200 MHz (Samsung M471A2K43DB1-CWE)
- Storage: 512 GiB NVMe SSD @ 33 MHz (Samsung MZVLB512HBJQ-000L7)

#### Storage Experiments

Random Read, Block Size 4KB
```
sync;fio --randrepeat=1 --ioengine=libaio --direct=1 --name=test --filename=test --bs=4k --size=4G --readwrite=randread --ramp_time=4
```

```
IOPS=19.7k
BW=77.1MiB/s
lat=50.20usec
```

Random Read, Block Size 32KB
```
sync;fio --randrepeat=1 --ioengine=libaio --direct=1 --name=test --filename=test --bs=32k --size=4G --readwrite=randread --ramp_time=4
```

```
IOPS=9794
BW=306MiB/s
lat=101.52usec
```

Random Read, Block Size 128KB
```
sync;fio --randrepeat=1 --ioengine=libaio --direct=1 --name=test --filename=test --bs=128k --size=4G --readwrite=randread --ramp_time=4
```

```
IOPS=6425
BW=803MiB/s
lat=154.57usec
```

#### Analysis: Throughput vs Latency

We can see quite clearly that as the data access size increases, the I/O operations per second falls drastically, but the throughput soars. The latency also grows as throughput increases. This makes sense, given the direct relationship between throughput and latency as dictated by queuing theory.

#### Comparison to Intel Data Center NVMe SSD D7-P5600

The Intel Data Center SSD has a random write-only (4KB) IOPS of 130K. 

I have only run random-read tests on this machine, since I am concerned about the possibility of writing over my partition table and other data. Consequently, I cannot compare my results to the Intel Data Center SSD directly, but I can reason as to why it could theoretically have a lower random-write IOPS than my machine. I think this could be the case because the Intel Data Center SSD is optimized to minimize latency, and they do so by having enterprise grade throughput, rather than a high IOPS count. 