# HPC-Chain

## Introduction
HPC-Chain is a high performance computing library designed and optmized to run on server grade hardware. Currently the code is optmized on Xeon 2nd gen processors and above. Though the higher the generation the better the performance.

## Features
This repository is a library like langchain and lamaindex but optimized for high performance computing. Currently this code will work only on platforms that have the AVX-512 instructions set, NUMA support and MPI support.

It would work best when the server has two or more sockets per node as it tries to optmize the code to run on multiple sockets.

The code, tries to do optmization at these few levels:
1. SRAM (L1, L2, L3 cache) Memory optmization
2. SIKMD (Single Instruction Multiple Data) optmization using intrinsics like the AVX-512 instruction set
3. OpenMP optmization
4. NUMA optmization
4. MPI optmization

To get the most performance there are few hardware requirements that needs to be met:
1. The server should have at least 2 sockets per node
2. The server should have AVX-512 instruction set
3. The server should have NUMA support
4. The server should have MPI support

E.g Hardware that meets these requirements are:
1. Intel Xeon 2nd gen and above

## Installation
You can use this library using docker.

```bash
docker run -it --rm -v $(pwd):/app -w /app ghcr.io/chain-lang/hpc-chain:latest
```
or you can use make to run the make file
    
```bash
make
```
