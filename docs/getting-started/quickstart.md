# Quickstart Guide

This 5-minute tutorial demonstrates how to define a multi-dimensional interleaved tensor, apply an operation in parallel across CPU cores, and let the SIMD vector units execute the code at hardware line rate.

---

## 1. Anatomy of a Legolas Tensor

In standard C++, a 2D array is typically laid out linearly row-by-row (`[row 0], [row 1], ...`).

With Legolas++, you specify an **interleaving factor** `P` along a designated dimension `DP`:

```cpp
#include "Legolas/Array/Array.hxx"

// Legolas::Array<ScalarType, Dimension, PackSize, PackDimension>
using MyTensor = Legolas::Array<float, 2, 4, 2>;
```

- `float`: Element type.
- `2`: Rank of the tensor (2D matrix of shape `ny x nx`).
- `4`: Pack size $P=4$ (4 elements grouped together contiguously).
- `2`: Pack dimension ($DP=2$, packing happens across the first/outer dimension $Y$).

```cpp
// Allocate 1024 independent systems of length 64 floats
MyTensor X(1024, 64);
X.fill(1.0f);
```

---

## 2. Writing a Generic Algorithm

The core principle of Legolas++ is that **your numerical algorithm is written once using standard scalar syntax**:

```cpp
struct MultiplyAccumulate {
    template <class A2D>
    void operator()(int begin, int end, const A2D A, const A2D B, A2D C) const {
        using Scalar = typename A2D::RealType;
        Scalar alpha(2.0f);

        for (int j = begin; j < end; ++j) {
            auto a_row = A[j];
            auto b_row = B[j];
            auto c_row = C[j];
            const int n = a_row.size();

            for (int i = 0; i < n; ++i) {
                // Natural scalar notation!
                c_row[i] = alpha * a_row[i] + b_row[i];
            }
        }
    }
};
```

---

## 3. Executing with `map` and `parmap`

You execute algorithms using Legolas dispatchers:

=== "Multi-Core Parallel (SIMD + Threads)"

    ```cpp
    #include "Legolas/Array/Map.hxx"

    // Distributes chunks of problem instances across worker threads
    // while executing SIMD vector instructions inside each chunk:
    Legolas::parmap(MultiplyAccumulate(), A, B, C);
    ```

=== "Sequential (Single-Core SIMD)"

    ```cpp
    #include "Legolas/Array/Map.hxx"

    // Runs on a single core, but processes P elements per clock cycle in SIMD:
    Legolas::map(MultiplyAccumulate(), A, B, C);
    ```

---

## 4. What Happens Under the Hood?

When `Legolas::parmap` is invoked:
1. It automatically inspects the template arguments of `A, B, C`.
2. It detects that $P=4$.
3. Instead of passing scalar references, it automatically passes `.getPackedView()` to `MultiplyAccumulate`.
4. In `a_row[i]`, the returned type is `Legolas::NativeSimd<float, 4>`.
5. The compiler translates `alpha * a_row[i] + b_row[i]` directly into an ARM NEON `fmla.4s` or x86 AVX `_mm256_fmadd_ps` instruction.
6. The outer instances are distributed dynamically across CPU cores by the lock-minimized **Work-Stealing scheduler**.

---

## 5. Explore Industrial Showcases & Energy Efficiency

Ready to see how Legolas++ slashes compute latency and energy consumption in production? Explore our comprehensive showcases:

- :movie_camera: **[Real-Time Video Pipeline (CPU & Metal GPU)](../tutorials/video-pipeline.md)**  
  Processes 32 concurrent 720p HD streams at **8,442 FPS on CPU** and **17,264 FPS on Metal GPU** (15.91 GPixels/s). Consumes only **2.43 µJ per HD frame** (87.2% energy reduction vs scalar baseline), enabling 287 concurrent 60 FPS feeds on a 40 W laptop.
- :musical_note: **[Real-Time Audio DSP (64 Tracks)](../tutorials/audio-biquad.md)**  
  Processes 64 high-resolution studio audio channels at **5,832 MSamples/sec** with under 0.001% CPU load, consuming just **5.14 nJ per sample** (< 0.05 W power draw).
- :brain: **[AI & Edge Vision (Depthwise Conv)](../tutorials/depthwise-conv.md)**  
  Delivers **212.3 GFlops** on 8 CPU cores (**7.08 GFlops/Watt**) for MobileNet/ConvNeXt neural layers with zero GPU memory transfer latency.
- :bar_chart: **[Scientific Computing (Thomas Recurrence)](../tutorials/tridiagonal-thomas.md)**  
  Solves 262,144 coupled linear systems (134 million unknowns) at **69.40 GFlops** (**33.05× speedup** and **2.31 GFlops/Watt**) for ADI heat diffusion and Navier-Stokes simulation.
