# HCC microkernels

This is a collection of bandwidth and latency mini benchmarks for a system with
AMD GPU devices and the ROCm stack.

## System info
```
00_show_accelerators
```
Not an actual benchmark. Shows all GPU devices available in the system

## Bandwidth of large memory copies between CPU and GPU
```
01_memcopy_bandwidth h2d pinned -s 1g -r 10
```

Shows bandwidth for a host to device copy of 1Gb of data. Host memory
is pinned (default: unpinned). Bandwidth is averaged over 10 repetitions.

```
01_memcopy_bandwidth d2h -s 512m
```

Shows bandwidth for a device to host copy of 512Mb of data

```
01_memcopy_bandwidth h2d d2h -s 1g
```

Shows bandwidth for a concurrent host to device and device to host copy of 1Gb of data. 
