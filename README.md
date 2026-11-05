# Deterministic vs Randomised Quicksort

**UTS 41052 Advanced Algorithms | Assignment 2**  
**Authors:** Felix Herrmann and Hein Htet Aung

## Overview

This project compares two implementations of Quicksort: **deterministic last-element pivot selection** and **uniformly randomised pivot selection**. Both use the same in-place Lomuto partitioning scheme, allowing the effect of pivot selection to be evaluated across six input types: random, sorted, reverse-sorted, nearly sorted, many duplicates, and all equal.

The benchmark measures execution time and separately records element comparisons, swaps, maximum recursion depth, and initial partition imbalance. It uses fixed seeds, warm-up rounds, repeated timings, shuffled execution order, and correctness checks against `std::sort`.

## Presentation Video Link

The following is the YouTube video link to our presentation video.
https://youtu.be/aSnGIHinGY0

## Minimum Requirements

- **CMake 3.16 or later**
- **A C++17-compatible compiler** (such as Clang, GCC, or MSVC)
- A system capable of compiling and running a native C++ executable, with sufficient memory and stack space for recursive Quicksort on arrays of up to **20,000 integers**

No third-party C++ libraries are required. The project uses the C++ standard library.

> **Tested environment:** macOS on an Apple M3 Pro with 18 GB RAM, Apple Clang, and a Release build. This is the benchmark environment, **not** a minimum hardware requirement. Exact minimum RAM, CPU, and OS versions have not been established.

## Build and Run

From the repository root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

**macOS / Linux:**

```bash
./build/benchmark
```

**Windows (with a multi-configuration CMake generator, such as Visual Studio):**

```powershell
.\build\Release\benchmark.exe
```

The benchmark executes all configured cases automatically; no command-line arguments are required. The executable name is `benchmark`.

## Benchmark Configuration

The default configuration in `benchmarks/main.cpp` uses:

- **Array sizes:** 100, 1,000, 5,000, 10,000, 15,000, and 20,000
- **Input types:** random, sorted, reverse, nearly sorted, many duplicates, and all equal
- **Input seeds:** 5 for randomly generated input families
- **Randomised pivot seeds:** 5
- **Warm-up rounds:** 5 (not included in measured results)
- **Measured repetitions:** 5

To change these settings, edit `BenchmarkConfig` in `benchmarks/main.cpp` and rebuild.

## Output

Running the benchmark creates a `results/` directory containing:

- `random.csv`
- `sorted.csv`
- `reverse.csv`
- `nearly_sorted.csv`
- `many_duplicates.csv`
- `all_equal.csv`
- `dashboard.csv`

The six input-specific files contain individual performance and analysis runs. `dashboard.csv` summarises timing statistics, algorithmic measurements, and correctness counts by input type, algorithm, and array size.

The benchmark displays a summary of total, passed, and failed runs in the terminal. Incorrect results are recorded as failures and excluded from the aggregated performance and analysis measurements.

## Project Structure

```text
benchmarks/main.cpp    Benchmark configuration, execution, validation, and CSV export
include/               Quicksort headers and measurement definitions
src/                   Deterministic and randomised Quicksort implementations
CMakeLists.txt         CMake build configuration
reportResults/         Results used in the report and presentation video for the analysis of data
report/                Includes our project report
```
