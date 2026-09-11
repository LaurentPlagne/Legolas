# Legolas++: Building Blocks for Linear Algebra Solvers

<p align="center">
  <em>High-Performance Modern C++ Tensor Engine for Automatic SIMD Vectorization of Recurrences via Data Layout Interleaving (DLI).</em>
</p>

---

## What is Legolas++?

**Legolas++** solves one of the most stubborn performance bottlenecks in scientific and numeric computing: **loop-carried recurrences** ($X_i = f(X_{i-1})$).

Compilers cannot auto-vectorize sequential recurrences because each step strictly requires the output of the preceding step.

Instead of fighting the compiler or rewriting complex assembly, Legolas++ introduces **Data Layout Interleaving (DLI)**:
When computing ensembles of independent problem instances (audio tracks, ADI mesh lines, image channels, option pricing grids), Legolas++ packs elements across instances contiguously into memory. 

The exact same generic algorithm code written using natural scalar math executes on SIMD registers (**ARM NEON**, **x86 AVX2 / AVX-512**) with **zero code modifications** and **zero overhead**.

---

## Why SIMD is the Core Innovation

While multi-threading (thread scheduling across CPU cores) is widely available and commoditized in modern C++, **hardware SIMD vectorization across recurrence loops has historically been an impenetrable barrier**.

Standard compilers systematically fall back to scalar execution on recurrences, leaving up to 90% of the CPU's SIMD arithmetic units idle. Legolas++ solves this at the hardware register level through Data Layout Interleaving.

---

## Key Highlights

- :rocket: **Break the Recurrence Barrier**: Vectorize tridiagonal solvers (Thomas algorithm), IIR digital filters, and depthwise convolutions with 100% hardware SIMD register utilization.
- :package: **100% Header-Only & Zero Dependencies**: Drop into any project with `#include <Legolas/Array/Array.hxx>`. No precompiled libraries (`.a`, `.dylib`, `.so`, `.dll`), no linker flags, and zero external dependencies. Pure standard C++14.
- :zap: **Write Once, Vectorize Everywhere**: A single template implementation works for both scalar types and hardware SIMD vectors (`Legolas::NativeSimd<T, P>`).
- :twisted_right_wards_arrows: **Two-Level Decoupled Parallelism**: 
    1. **Data-Level (SIMD)**: Automatic via Data Layout Interleaving.
    2. **Thread-Level (Multi-Core)**: Built-in, header-only zero-allocation parallel loop scheduler.
- :apple: **Cross-Platform**: Validated on Apple Silicon ARM64 (NEON), Linux x86_64 (AVX2 / AVX-512), and Windows MSVC.

```cmake
# Add to your CMakeLists.txt in 4 lines:
include(FetchContent)
FetchContent_Declare(Legolas GIT_REPOSITORY https://github.com/LaurentPlagne/Legolas.git GIT_TAG master)
FetchContent_MakeAvailable(Legolas)
target_link_libraries(my_project PRIVATE Legolas)
```

<p align="center">
  <img src="assets/images/dli_interleaving_mapping.png" alt="Data Layout Interleaving Memory Mapping" width="700">
</p>

*Figure: Data Layout Interleaving (DLI). Elements at step $i$ across $P=4$ independent systems are interleaved contiguously in memory, enabling direct hardware SIMD vector loads.*

---

## How Legolas++ Compares

| Feature | Compilers (GCC/Clang) | Traditional Linear Algebra (BLAS, Armadillo) | Deep Learning Frameworks (PyTorch, oneDNN) | **Legolas++** |
| :--- | :---: | :---: | :---: | :---: |
| **Vectorize Recurrences ($X_i = f(X_{i-1})$)** | ❌ Fails (scalar $1\times$) | ❌ Scalar loops ($1\times$) | ❌ Custom handwritten kernels | :white_check_mark: **Automatic SIMD ($4\times - 16\times$)** |
| **Write Once, Vectorize Everywhere** | ❌ Manual intrinsics needed | ⚠️ Limited to non-recursive ops | ❌ Separate CPU/GPU implementations | :white_check_mark: **Single generic scalar syntax** |
| **Data Layout Interleaving (DLI)** | ❌ Manual memory transforms | ❌ Fixed row/col major | ⚠️ Heavy tensor reshape overhead | :white_check_mark: **Native in the tensor type** |
| **External Dependencies** | None | BLAS / LAPACK | Heavy (LibTorch, Python, MKL) | :white_check_mark: **Zero (Pure C++14)** |
| **Integration Model** | N/A | Variable | Multi-GB binaries | :white_check_mark: **100% Header-Only** |

---

## Apple Silicon (M1 Max) Benchmark Summary

On tridiagonal recurrence systems ($N_x \in [8, 512]$, up to 262,144 systems, 134 million unknowns):

| Mode | Pack Size ($P$) | Execution | Large Size Throughput ($N_x=512$) | Peak Throughput | Speedup vs Scalar |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Scalar Baseline** | $P=1$ | Sequential (1 Core) | 2.10 GFlops | 6.66 GFlops | 1.00x |
| **Legolas NEON SIMD** | $P=4$ | Sequential (1 Core) | 6.50 GFlops | 16.64 GFlops | **3.10x** |
| **Legolas NEON Unrolled** | $P=8$ | Sequential (1 Core) | 9.58 GFlops | 16.64 GFlops | **4.75x** |
| **Scalar Multi-Thread** | $P=1$ | Parallel (8 Cores) | 16.48 GFlops | 18.43 GFlops | **7.85x** |
| **Legolas NEON Multi-Thread** | $P=4$ | Parallel (8 Cores) | 51.72 GFlops | 52.40 GFlops | **24.63x** |
| **Legolas Hybrid SIMD + Work-Stealing** | $P=8$ | Parallel (8 Cores) | **69.15 GFlops** | **69.40 GFlops** | **33.05x** |

<p align="center">
  <img src="assets/images/Thomas_comparison.png" alt="Throughput Comparison" width="850"/>
</p>

### Real-World Domain Showcases

| Application | Domain | Metric | Scalar Baseline | Legolas (CPU / GPU) | Speedup | Guide |
| :--- | :--- | :--- | :--- | :--- | :---: | :---: |
| **MultiThomas** | Scientific Computing | GFlops | 2.10 GFlops | **69.40 GFlops** (8 Cores) | **33.0x** | [Tutorial 1](tutorials/tridiagonal-thomas.md) |
| **Depthwise 2D Conv** | AI & Edge Vision (MobileNet) | GFlops | 36.40 GFlops | **212.30 GFlops** (8 Cores) | **5.83x** | [Tutorial 2](tutorials/depthwise-conv.md) |
| **Audio IIR Biquad** | Real-Time Audio DSP (64 Tracks) | MSamples/s | 397.9 MS/s | **5,832.2 MS/s** (8 Cores) | **14.66x** | [Tutorial 3](tutorials/audio-biquad.md) |
| **Video Pipeline (CPU)** | Multi-Stream Vision (32 HD Feeds) | FPS | 1,579.6 FPS | **8,442.1 FPS** (7.78 GPix/s) | **5.34x** | [Tutorial 4](tutorials/video-pipeline.md) |
| **Video Pipeline (Metal GPU)** | Multi-Stream Vision (32 HD Feeds) | FPS | 302.1 FPS | **17,263.9 FPS** (15.91 GPix/s) | **57.15x** | [Tutorial 4](tutorials/video-pipeline.md) |

---

## Quick Example in 15 Seconds

```cpp
#include <iostream>
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

// 1. Define an algorithm once using natural scalar math:
struct Scaler {
    template <class A2D>
    void operator()(int begin, int end, A2D X, A2D Y) const {
        using Scalar = typename A2D::RealType;
        Scalar factor(2.5f);
        for (int j = begin; j < end; ++j) {
            for (int i = 0; i < X[j].size(); ++i) {
                Y[j][i] = factor * X[j][i]; // Compiles to NEON/AVX FMA automatically!
            }
        }
    }
};

int main() {
    // 2. Declare 2D array packed across Y with P=4 (4 elements contiguous in memory):
    using Array2D = Legolas::Array<float, 2, 4, 2>; // 1024 systems of size 256
    Array2D X(1024, 256), Y(1024, 256);
    X.fill(1.0f);

    // 3. Parallelize across cores and vectorize in SIMD simultaneously:
    Legolas::parmap(Scaler(), X, Y);

    std::cout << "Done with zero overhead!" << std::endl;
    return 0;
}
```

---

## Citation & Origins

Legolas++ is based on the research presented at **ACM SIGPLAN ARRAY 2017**:

> **Portable vectorization and parallelization of C++ multi-dimensional array computations**  
> Laurent Plagne & Kaveh Bojnourdi  
> *Proceedings of the 4th ACM SIGPLAN International Workshop on Libraries, Languages, and Compilers for Array Programming (ARRAY 2017)*, Pages 47–54.  
> DOI: [10.1145/3091966.3091973](https://doi.org/10.1145/3091966.3091973)
