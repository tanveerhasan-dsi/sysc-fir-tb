# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Environment

This project runs inside a Docker container with SystemC pre-installed. The host machine does not need SystemC installed.

```bash
# Pull the image (once)
docker pull learnwithexamples/systemc

# First run — creates and enters the container
docker run -it --name systemc learnwithexamples/systemc /bin/bash

# Subsequent runs — restart the existing container
docker start -i systemc
```

All commands below are meant to be run **inside the container**.

## Build & Run

```bash
# Build only
make out

# Build, run simulation, and compare against golden output
make all

# Clean build artifacts
make clean

# Run regression check separately (after ./out has been executed)
make cmp_result
```

The simulation binary is `./out`. Running it produces `output.dat`, which `make cmp_result` diffs against `golden/ref_output.dat`.

## Architecture

The design follows a standard SystemC DUT + testbench pattern. All modules are clocked on the positive edge of a 10 ns (100 MHz) clock with an active-high synchronous reset.

**Module hierarchy (`main.cpp`):**
- `SYSTEM` (top) instantiates `tb` and `fir`, wires them together via `sc_signal`s, and calls `sc_start()`.

**FIR filter (`fir.h` / `fir.cpp`):**
- Single `SC_CTHREAD` process (`fir_main`) clocked on `clk.pos()`.
- 5-tap delay line with coefficients `{18, 77, 107, 77, 18}` and `sc_int<16>` fixed-point arithmetic.
- Handshake loop: assert `inp_rdy`, wait for `inp_vld`; compute; assert `outp_vld`, wait for `outp_rdy`.

**Testbench (`tb.h` / `tb.cpp`):**
- Two `SC_CTHREAD` processes sharing the same module:
  - `source`: drives reset, then sends 64 samples (value `256` at indices 24–28, `0` elsewhere); records `start_time[]` per sample; calls `sc_stop()` after a 10 000-cycle drain timeout.
  - `sink`: reads 64 output samples; records `end_time[]`; writes results to `output.dat`; prints average latency and throughput; calls `sc_stop()`.

**Handshake protocol** (used on both input and output sides):
- Producer asserts `*_vld`; consumer asserts `*_rdy`; transfer occurs on the clock edge where both are high.

## Key Files

| File | Purpose |
|------|---------|
| `fir.h` / `fir.cpp` | DUT — 5-tap FIR filter |
| `tb.h` / `tb.cpp` | Testbench (stimulus + checker) |
| `main.cpp` | Top-level wiring and `sc_main` |
| `golden/ref_output.dat` | Golden reference for regression |
| `Makefile` | Primary build (use this one; `Makefile.1` has syntax errors) |

## SystemC Conventions Used

- `SC_CTHREAD` (not `SC_METHOD`) for all sequential logic — supports `wait()` calls.
- `reset_signal_is(rst, true)` in `fir`'s constructor — code before the first `wait()` in `fir_main` is the reset handler.
- `sc_int<16>` for data signals; `sc_uint<8>` for coefficients.
- `dynamic_cast<sc_clock*>(clk.get_interface())` in `tb::sink` to retrieve the clock period at runtime.
