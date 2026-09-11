# Legolas++

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-14%2F20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B14)
[![Documentation](https://img.shields.io/badge/Docs-GitHub%20Pages-informational.svg)](https://laurentplagne.github.io/Legolas/)
[![Platforms](https://img.shields.io/badge/Platform-macOS%20ARM64%20(Apple%20Silicon)%20%7C%20Linux%20x86%20(AVX2%2FAVX512)-success.svg)]()
[![Build & Test](https://img.shields.io/badge/CTest-100%25%20Passing%20(5%2F5)-brightgreen.svg)]()
[![Zero Dependencies](https://img.shields.io/badge/Parallelizer-Native%20Work--Stealing%20(Header--Only)-orange.svg)]()
[![License](https://img.shields.io/badge/License-GPL%20v2-lightgrey.svg)](License.md)

*High-Performance Modern C++ Tensor Engine for Automatic SIMD Vectorization & Multi-Core Work-Stealing via Data Layout Interleaving (DLI).*

---

## 1. The Fundamental Barrier: Why Compilers Fail on Recurrences

Many essential algorithms in science, digital signal processing, finance, and AI feature **strict loop-carried data dependencies** where iteration $i$ inherently depends on the result of iteration $i-1$:

```cpp
// Tridiagonal elimination (Thomas algorithm), IIR filters, Gauss-Seidel, Mamba SSMs:
for (int i = 1; i < N; ++i) {
    X[i] = (B[i] - L[i] * X[i-1]) * invD[i]; // Strict recurrence: X[i] requires X[i-1]!
}
```

Because of this recurrence, **no compiler (Clang, GCC, MSVC, Intel) can auto-vectorize this loop sequentially**, no matter what `#pragma omp simd` or `-O3` flags are used. 

---

## 2. The Legolas++ Solution: Data Layout Interleaving (DLI)

In practice, production systems rarely solve just one isolated recurrence. Instead, they process **ensembles of independent problem instances**:
* **Scientific Computing**: Solving thousands of 1D tridiagonal systems across 2D/3D ADI meshes or diffusion equations.
* **Audio Engineering & DAWs**: Filtering 32, 64, or 128 audio tracks simultaneously with recursive IIR/Biquad filters.
* **Edge AI & Computer Vision**: Computing Depthwise Separable Convolutions across $C$ feature channels (MobileNet, ConvNeXt).
* **Quantitative Finance**: Calibrating option prices across thousands of strikes and maturities via Crank-Nicolson PDE grids.

### Transposing Data at the Memory Level
Rather than trying to vectorize sequentially along $i$, **Legolas++ interleaves $P$ problem instances directly in memory**:

```
Standard Memory Layout (Sequential, Cannot Vectorize):
Instance 0: [x0, x1, x2, ... xN-1]
Instance 1: [x0, x1, x2, ... xN-1]  ==> Accessing x[i] across instances requires slow gather loads!
Instance 2: [x0, x1, x2, ... xN-1]

Legolas++ Interleaved Layout (DLI):
[x0_p0, x0_p1, ... x0_pP-1] [x1_p0, x1_p1, ... x1_pP-1] ...
└─────── 1 SIMD Load ───────┘ └─────── 1 SIMD Load ───────┘
```

### Zero-Overhead Abstraction: Write Once, Vectorize Everywhere
1. **Define the tensor**: `Legolas::Array<T, D, P, DP>` defines a tensor of dimension `D` with packing factor `P` along dimension `DP`.
2. **Zero-cost view**: `.getPackedView()` reinterprets interleaved data as native SIMD vector registers (`Eigen::Array<T, P, 1>`).
3. **Write natural scalar code**: The numerical solver is written **once** using standard scalar syntax. The same generic function compiles into high-throughput SIMD instructions (ARM NEON or x86 AVX) without writing assembly or intrinsics:

```cpp
struct ThomasSolver {
  template <class A2D>
  void operator()(int begin, int end, A2D D, A2D U, A2D L, A2D B, A2D X) const {
    using Scalar = typename A2D::RealType;
    Scalar one(1.0), s, sm1;

    for (int j = begin; j < end; ++j) {
      s = D[j][0];
      sm1 = one / s;
      X[j][0] = B[j][0] * sm1;

      // Forward sweep: executes on scalar floats OR vector NEON/AVX registers!
      for (int i = 1; i < X[j].size(); ++i) {
        s = D[j][i] - L[j][i] * (U[j][i-1] * sm1);
        X[j][i] = (B[j][i] - L[j][i] * X[j][i-1]);
        sm1 = one / s;
        X[j][i] *= sm1;
      }
      // Backward substitution...
    }
  }
};
```

---

## 3. Benchmarks on Apple Silicon (M1 Max ARM64)

The benchmark evaluates the Thomas tridiagonal algorithm across $N_y = N_x^2$ systems of size $N_x \in [8, 512]$ (up to 262,144 systems, 134M unknowns):

| Configuration | Pack Size ($P$) | Execution Mode | Peak Throughput | Speedup vs Scalar |
| :--- | :---: | :---: | :---: | :---: |
| **Scalar Baseline** | $P=1$ | Sequential (1 Core) | **2.10 GFlops** | 1.0x (Baseline) |
| **Legolas NEON SIMD** | $P=4$ | Sequential (1 Core) | **6.50 GFlops** | **3.10x** |
| **Legolas NEON Unrolled** | $P=8$ | Sequential (1 Core) | **9.97 GFlops** | **4.75x** |
| **Scalar Multi-Thread** | $P=1$ | Parallel (8 Cores) | **18.43 GFlops** | **8.78x** |
| **Legolas NEON Multi-Thread** | $P=4$ | Parallel (8 Cores) | **52.40 GFlops** | **24.95x** |
| **Legolas Hybrid SIMD + Work-Stealing** | $P=8$ | Parallel (8 Cores) | **69.40 GFlops** | **33.05x** |

> **Key takeaway**: Legolas++ delivers an overall **33x speedup** on Apple Silicon M1 Max compared to standard scalar execution, sustaining **65 to 69.4 GFlops** on a tridiagonal recurrence solver.

### Performance Curves

#### 1. Throughput Scaling ($N_x = 8 \dots 512$)
![Throughput Scaling (Apple M1 Max ARM64)](Thomas_comparison.png)

*High-resolution vector format: [Thomas_comparison.svg](Thomas_comparison.svg) | Interactive web report: [`tst/MultiThomas/benchmarks_report.html`](tst/MultiThomas/benchmarks_report.html)*

#### 2. Multi-Core Scaling (1 to 8 Threads on Apple M1 Max)
![Multi-Core Speedup Scaling](Thomas_speedup.png)

*High-resolution vector format: [Thomas_speedup.svg](Thomas_speedup.svg)*

```
Multi-Core Speedup (Apple M1 Max Firestorm P-Cores, P=8 NEON):
1 Thread:  9.58 GFlops (1.00x)
2 Threads: 19.04 GFlops (1.99x) -> 99.4% parallel efficiency
4 Threads: 37.59 GFlops (3.92x) -> 98.1% parallel efficiency
8 Threads: 69.15 GFlops (7.22x)
```

---

## 4. Real-World Showcases & Examples

### 4.1 AI & Edge Computer Vision: Depthwise Separable 2D Convolution
*Location: [`examples/DepthwiseConv/DepthwiseConv.cxx`](examples/DepthwiseConv/DepthwiseConv.cxx)*

Depthwise convolutions in MobileNet (V1/V2/V3), ConvNeXt, and EfficientNet filter each channel independently with a $3 \times 3$ kernel.
In standard `NCHW` layout, vectorizing across channels is hindered by memory strides. Industry runtimes (Intel oneDNN, Apache TVM) convert tensors into **blocked channel formats** like `nChw4c` (NEON) or `nChw8c` (AVX).
* In Legolas++, this layout is native: `Legolas::Array<float, 2, 4, 2>` packs 4 channels contiguously.
* Every spatial multiply-accumulate computes across 4 channels simultaneously with NEON `fmla.4s` instructions.
* **Result**: **212.3 GFlops**, **5.83x speedup** over scalar on Apple M1 Max (0.00 mathematical error).

### 4.2 Digital Audio Processing: 64-Track IIR Biquad Filter
*Location: [`examples/AudioBiquad/AudioBiquad.cxx`](examples/AudioBiquad/AudioBiquad.cxx)*

A 2nd-order Direct Form I/II IIR Biquad filter ($y[n] = b_0 x[n] + b_1 x[n-1] + b_2 x[n-2] - a_1 y[n-1] - a_2 y[n-2]$) contains an unavoidable temporal feedback loop.
* Legolas++ interleaves audio channels into SIMD vectors: `Legolas::Array<float, 2, 4, 2>` (64 tracks, 960,000 samples at 96 kHz).
* The sample loop proceeds sequentially through time, but evaluates $P=4$ channels simultaneously in hardware registers.
* **Result**: **5,832 Megasamples/sec** (46.7 GFlops), **14.7x speedup** over scalar processing (0.00 mathematical error).

---

## 5. Two-Level Hybrid Parallelism Engine

Legolas++ implements two decoupled layers of parallelism:

```
┌─────────────────────────────────────────────────────────────┐
│                 Level 1: Multi-Core Scaling                 │
│   Legolas::parmap(...) via Native Work-Stealing Loop Engine │
│   Per-worker deque: LIFO local tasks (L1/L2 cache affinity) │
│   FIFO work-stealing when idle (lock-minimized atomics)     │
└──────────────────────────────┬──────────────────────────────┘
                               │ (blocked ranges)
┌──────────────────────────────▼──────────────────────────────┐
│                  Level 2: SIMD Vectorization                │
│   Legolas::Array<T, D, P, DP> (Data Layout Interleaving)    │
│   Packed views mapped directly to ARM NEON / x86 AVX2/512   │
│   Zero memory permute overhead during computation           │
└─────────────────────────────────────────────────────────────┘
```

### Zero-Dependency Native Work-Stealing
Legolas++ includes a standalone, header-only work-stealing scheduler (`Legolas/include/WorkStealing.hxx`):
* No external runtime libraries required (no link against TBB or OpenMP necessary).
* Automatically sizes thread pool to hardware concurrency (supports `SPN_THREAD_NUMBER`).
* Can be toggled with Intel TBB using `-DUSE_TBB=ON/OFF`.

---

## 6. Quickstart & Build Instructions

### Prerequisites
* C++14 compliant compiler (AppleClang $\ge 12$, GCC $\ge 7$, Clang $\ge 8$)
* CMake $\ge 3.5$
* Eigen 3.3 or 3.4 (`brew install eigen`)

### 1. Build with Zero External Dependencies (Native Work-Stealing)
```bash
cmake -B build -DUSE_TBB=OFF
cmake --build build -j8
ctest --test-dir build --output-on-failure
```

### 2. Build with Intel oneTBB (Optional)
```bash
cmake -B build -DUSE_TBB=ON
cmake --build build -j8
ctest --test-dir build --output-on-failure
```

### 3. Run Showcases
```bash
# AI Depthwise 2D Convolution showcase:
./build/examples/DepthwiseConv

# Multi-channel Audio IIR Biquad showcase:
./build/examples/AudioBiquad

# MultiThomas Tridiagonal benchmark suite:
./build/tst/MultiThomas/MultiThomas
python3 tst/MultiThomas/plotPerfModern.py build/tst/MultiThomas
```

---

## 7. Documentation Website

Comprehensive tutorials, mathematical explanations, architecture guides, and API reference are available on the official documentation website:

:link: **[https://laurentplagne.github.io/Legolas/](https://laurentplagne.github.io/Legolas/)**

To preview the documentation site locally:
```bash
pip install -r requirements-docs.txt
mkdocs serve
# Open http://127.0.0.1:8000 in your browser
```

---

## 8. Citation & Academic Background

Legolas++ is based on the research presented at ACM SIGPLAN ARRAY 2017:

> **Portable vectorization and parallelization of C++ multi-dimensional array computations**  
> Laurent Plagne & Kaveh Bojnourdi  
> *Proceedings of the 4th ACM SIGPLAN International Workshop on Libraries, Languages, and Compilers for Array Programming (ARRAY 2017)*, Pages 47–54.  
> DOI: [10.1145/3091966.3091973](https://doi.org/10.1145/3091966.3091973)

---

## 9. License

This project is distributed under the terms of the GNU General Public License v2 (GPL-2.0). See [License.md](License.md) for details.