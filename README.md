# sysc-fir-tb

A SystemC simulation testbench for a 5-tap FIR digital filter, based on Forte's SystemC training material. Thanks to Zim.

## Overview

Simulates a **Finite Impulse Response (FIR) filter** with coefficients `{18, 77, 107, 77, 18}` using 16-bit fixed-point arithmetic. The testbench drives 64 input samples (a pulse pattern), collects outputs, and reports average latency and throughput.

Communication between the testbench and filter uses a **ready-valid handshake** on both input and output sides.

## Requirements

- [Docker](https://www.docker.com/)

No local SystemC installation needed — everything runs inside a pre-configured Docker container.

## Setup

```bash
# Pull the SystemC Docker image (once)
docker pull learnwithexamples/systemc

# Create and enter the container
docker run -it --name systemc learnwithexamples/systemc /bin/bash

# On subsequent runs, restart the existing container
docker start -i systemc
```

All commands below are run **inside the container**.

## Build & Run

```bash
# Build the simulation binary
make

# Build, run simulation, and compare against golden output
make run

# Clean all build artifacts and output
make clean
```

The simulation writes results to `output.dat`. `make run` automatically diffs it against `golden/ref_output.dat` and prints `Simulation passed` or `Simulation failed`.

### Overriding the SystemC path

If SystemC is installed somewhere other than `/usr/local/systemc-2.3.3`:

```bash
SCPATH=/path/to/systemc make
```

The Makefile also auto-detects the platform (`lib-linux64` on Linux, `lib-macosx64` on macOS).

## Project Structure

```
├── main.cpp          # Top-level: wires tb and fir, runs sc_start()
├── fir.h / fir.cpp   # FIR filter (DUT)
├── tb.h  / tb.cpp    # Testbench: stimulus source + output sink
├── golden/
│   └── ref_output.dat  # Golden reference for regression
└── Makefile.bak      # Original Makefile kept for reference
```
