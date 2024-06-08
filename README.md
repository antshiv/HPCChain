# HPC-Chain

## Introduction
This repository is a library like langchain and lamaindex but optimized for high performance computing. Currently this code will work only on platforms that have the AVX-512 instructions set.

## Installation
You can use this library using docker.

```bash
docker run -it --rm -v $(pwd):/app -w /app ghcr.io/chain-lang/hpc-chain:latest
```
or you can use make to run the make file
    
```bash
make
```
