# Project 3: Memory and Storage Performance Profiling

#### Environment

OS: Ubuntu 20.04
CPU: Intel(R) Core(TM) i7-10850H CPU @ 2.70 GHz (12 cores)
Memory: 2x16 GiB SODIMM DDR4 @ 3200 MHz (Samsung M471A2K43DB1-CWE)
Storage: 512 GiB NVMe SSD @ 33 MHz (Samsung MZVLB512HBJQ-000L7)

#### Memory Experiments

Variables:
- read/write intensity (read only, write only, 70/30)
- data access size (64B/256B)

#### Storage Experiments

Variables:
- read/write intensity (read only, write only, 70/30)
- data access size (4KB/32KB/128KB)

#### Analysis: Throughput vs Latency

Include some discussions that use queuing theory to explain the throughput vs. latency results you have captured.

#### Comparison to Intel Data Center NVMe SSD D7-P5600

The  specification  of  Intel  Data  Center  NVMe  SSD  D7-P5600  (1.6TB)  lists  a  random  write-only  4KB  IOPS  of  130K. 

Compare your results with this Intel enterprise-grade SSD, and try to explain any unexpected observation (e.g., your client-grade SSD may show higher IOPS than such expensive enterprise-grade SSD, why?). 