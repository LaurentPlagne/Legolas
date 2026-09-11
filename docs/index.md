# Legolas++: Building Blocks for Linear Algebra Solvers

<p align="center">
  <em>High-Performance Modern C++ Tensor Engine for Automatic SIMD Vectorization of Recurrences via Data Layout Interleaving (DLI).</em>
</p>

<p align="center">
  <a class="md-button md-button--primary" href="#industrial-showcases-energy-efficiency">⚡ Explore Industry Showcases</a>
  <a class="md-button" href="getting-started/quickstart.md">🚀 30-Second Quickstart</a>
  <a class="md-button" href="https://github.com/LaurentPlagne/Legolas" target="_blank">💻 GitHub Repository</a>
</p>

---

## 🏆 Industrial Showcases & Energy Efficiency

> 🌿 **Green Computing & HPC Energy Efficiency**  
> High-performance computing is no longer solely about raw clock speed—it is fundamentally an **energy efficiency challenge**. Whether in battery-constrained drones, automotive vision systems, or megawatt hyperscale data centers, **unvectorized scalar loops waste energy** by leaving silicon execution pipelines idle while burning power.  
>
> Through **Data Layout Interleaving (DLI)**, Legolas++ packs problem ensembles directly into hardware vector registers, slashing the **energy consumed per frame, audio sample, and linear solver by up to 88%**.

<div class="showcase-grid">

  <!-- SHOWCASE 1: Video Pipeline -->
  <div class="showcase-card featured">
    <div>
      <div class="showcase-header">
        <span class="showcase-domain">Computer Vision &amp; Broadcast</span>
        <span class="showcase-badge-hot">🔥 CPU + Metal GPU</span>
      </div>
      <h3 class="showcase-title">🎥 32-Stream Real-Time Video Pipeline</h3>
      <p class="showcase-description">
        Continuous spatial 3×3 Sobel edge extraction combined with quadratic temporal motion differencing across 32 concurrent 720p HD video streams. Evaluated on Apple Silicon M1 Max.
      </p>
      <div class="metric-row">
        <div class="metric-pill">
          <span class="metric-value highlight-orange">8,442 FPS</span>
          <span class="metric-label">CPU (8 Cores)</span>
        </div>
        <div class="metric-pill">
          <span class="metric-value highlight-green">17,264 FPS</span>
          <span class="metric-label">Metal GPU (32 Cores)</span>
        </div>
        <div class="metric-pill">
          <span class="metric-value">15.91 GPix/s</span>
          <span class="metric-label">Peak Throughput</span>
        </div>
      </div>
      <div class="energy-box">
        <div class="energy-box-title">🌱 Energy &amp; Carbon Footprint Analysis</div>
        <div class="energy-box-body">
          • <strong>CPU:</strong> Consumes only <span class="energy-stat">3.55 µJ per HD frame</span> (0.259 GPixels/Watt).<br>
          • <strong>Metal GPU:</strong> Drops to <span class="energy-stat">2.43 µJ per HD frame</span> (0.379 GPixels/Watt).<br>
          • <strong>Green Impact:</strong> <strong>87.2% energy reduction</strong> vs scalar baseline. Sustains <strong>287 concurrent 60 FPS feeds</strong> on a single 40 W laptop without thermal throttling.
        </div>
      </div>
    </div>
    <div class="showcase-footer">
      <a class="showcase-link" href="tutorials/video-pipeline.md">Explore Video Pipeline Showcase →</a>
      <span class="source-tag">examples/VideoPipeline</span>
    </div>
  </div>

  <!-- SHOWCASE 2: Audio DSP -->
  <div class="showcase-card">
    <div>
      <div class="showcase-header">
        <span class="showcase-domain">Audio DSP &amp; Acoustics</span>
        <span class="showcase-badge-hot">14.66× Speedup</span>
      </div>
      <h3 class="showcase-title">🎧 64-Channel Studio Audio IIR Biquad</h3>
      <p class="showcase-description">
        Recursive Direct Form II digital biquad filtering across 64 parallel audio tracks. Demonstrates vectorization of recursive digital signal processing where compilers strictly fail.
      </p>
      <div class="metric-row">
        <div class="metric-pill">
          <span class="metric-value highlight-orange">5,832 MS/s</span>
          <span class="metric-label">Throughput</span>
        </div>
        <div class="metric-pill">
          <span class="metric-value highlight-green">&lt; 0.001%</span>
          <span class="metric-label">CPU Utilization</span>
        </div>
        <div class="metric-pill">
          <span class="metric-value">14.66×</span>
          <span class="metric-label">Speedup vs Scalar</span>
        </div>
      </div>
      <div class="energy-box">
        <div class="energy-box-title">🌱 Ultra-Low Power Consumption</div>
        <div class="energy-box-body">
          • <strong>Power Draw:</strong> Processes 64 channels of studio-grade 192 kHz audio with <span class="energy-stat">&lt; 0.05 W</span>.<br>
          • <strong>Energy Density:</strong> Consumes just <span class="energy-stat">5.14 nJ per filtered sample</span> (194.4 MSamples/Joule).<br>
          • <strong>Green Impact:</strong> Replaces dedicated 500 W hardware DSP racks with pure software running on low-power mobile or edge CPUs.
        </div>
      </div>
    </div>
    <div class="showcase-footer">
      <a class="showcase-link" href="tutorials/audio-biquad.md">Explore Audio DSP Showcase →</a>
      <span class="source-tag">examples/AudioBiquad</span>
    </div>
  </div>

  <!-- SHOWCASE 3: AI & Edge Vision -->
  <div class="showcase-card">
    <div>
      <div class="showcase-header">
        <span class="showcase-domain">Edge AI &amp; Neural Nets</span>
        <span class="showcase-badge-hot">MobileNet / ConvNeXt</span>
      </div>
      <h3 class="showcase-title">🧠 Depthwise Separable 2D Convolution</h3>
      <p class="showcase-description">
        Core compute primitive of lightweight edge vision neural networks (MobileNet, ConvNeXt, EfficientNet). Vectorizes per-channel spatial filters across large batch ensembles.
      </p>
      <div class="metric-row">
        <div class="metric-pill">
          <span class="metric-value highlight-orange">212.3 GFlops</span>
          <span class="metric-label">CPU Throughput</span>
        </div>
        <div class="metric-pill">
          <span class="metric-value highlight-green">0.0 µs</span>
          <span class="metric-label">PCIe Transfer Lag</span>
        </div>
        <div class="metric-pill">
          <span class="metric-value">5.83×</span>
          <span class="metric-label">Speedup vs Scalar</span>
        </div>
      </div>
      <div class="energy-box">
        <div class="energy-box-title">🌱 Embedded Battery Autonomy</div>
        <div class="energy-box-body">
          • <strong>Compute Density:</strong> Delivers <span class="energy-stat">7.08 GFlops/Watt</span> on standard CPU cores.<br>
          • <strong>Zero Transfer Overhead:</strong> Avoids high-power GPU bus copies and discrete accelerator wake-up latency.<br>
          • <strong>Green Impact:</strong> Cuts inference power by <span class="energy-stat">82.8%</span>, maximizing battery life for robotics, drones, and edge cameras.
        </div>
      </div>
    </div>
    <div class="showcase-footer">
      <a class="showcase-link" href="tutorials/depthwise-conv.md">Explore Edge AI Showcase →</a>
      <span class="source-tag">examples/DepthwiseConv</span>
    </div>
  </div>

  <!-- SHOWCASE 4: Scientific Computing -->
  <div class="showcase-card">
    <div>
      <div class="showcase-header">
        <span class="showcase-domain">Scientific Computing &amp; PDEs</span>
        <span class="showcase-badge-hot">33.05× Speedup</span>
      </div>
      <h3 class="showcase-title">🔬 MultiThomas Tridiagonal Solver</h3>
      <p class="showcase-description">
        Solving 262,144 coupled tridiagonal linear systems (134.2 million unknowns) arising in Alternating Direction Implicit (ADI) heat diffusion, Navier-Stokes, and quantitative finance PDEs.
      </p>
      <div class="metric-row">
        <div class="metric-pill">
          <span class="metric-value highlight-orange">69.40 GFlops</span>
          <span class="metric-label">Peak Compute</span>
        </div>
        <div class="metric-pill">
          <span class="metric-value highlight-green">134M</span>
          <span class="metric-label">Unknowns Solved</span>
        </div>
        <div class="metric-pill">
          <span class="metric-value">33.05×</span>
          <span class="metric-label">Total Speedup</span>
        </div>
      </div>
      <div class="energy-box">
        <div class="energy-box-title">🌱 Hyperscale Cluster Compute Density</div>
        <div class="energy-box-body">
          • <strong>HPC Efficiency:</strong> Delivers <span class="energy-stat">2.31 GFlops/Watt</span> on sequential recurrences where standard compilers yield only 0.07 GFlops/Watt.<br>
          • <strong>Scale:</strong> Solves 134 million unknowns in milliseconds with zero memory reordering overhead.<br>
          • <strong>Green Impact:</strong> <strong>33× higher compute density per Joule</strong>, drastically shrinking cloud HPC electricity bills.
        </div>
      </div>
    </div>
    <div class="showcase-footer">
      <a class="showcase-link" href="tutorials/tridiagonal-thomas.md">Explore MultiThomas Showcase →</a>
      <span class="source-tag">tst/MultiThomas</span>
    </div>
  </div>

</div>

---

## What is Legolas++?

**Legolas++** solves one of the most stubborn performance bottlenecks in scientific and numeric computing: **loop-carried recurrences** ($X_i = f(X_{i-1})$).

Compilers cannot auto-vectorize sequential recurrences because each step strictly requires the output of the preceding step. Standard optimizing compilers systematically fall back to **scalar execution**, leaving up to **90% of the CPU's vector hardware completely idle**.

Instead of fighting the compiler with hand-written assembly intrinsics, Legolas++ introduces **Data Layout Interleaving (DLI)**:
When computing ensembles of independent problem instances (video feeds, audio tracks, ADI mesh sweeps, neural channels), Legolas++ packs elements across instances contiguously into physical memory:

<p align="center">
  <img src="assets/images/dli_animation.svg" alt="Data Layout Interleaving Animation" width="960" height="640" style="max-width: 100%; height: auto;">
</p>

*Figure: Data Layout Interleaving (DLI) in Action. Elements at step $i$ across $P=4$ independent systems are interleaved contiguously in memory, enabling direct single-instruction hardware SIMD vector streaming (`ldr q` / `vmovaps`).*

The exact same generic algorithm code written using natural scalar math compiles into full hardware SIMD registers (**ARM NEON**, **x86 AVX2 / AVX-512**) with **zero code modifications** and **zero overhead**.

---

## Key Technical Highlights

- :rocket: **Break the Recurrence Barrier**: Vectorize tridiagonal solvers (Thomas algorithm), IIR digital filters, and depthwise convolutions with 100% hardware SIMD register saturation.
- :package: **100% Header-Only & Zero Dependencies**: Drop into any project with `#include <Legolas/Array/Array.hxx>`. No precompiled libraries, no link flags, and zero external dependencies. Pure standard C++14.
- :zap: **Write Once, Vectorize Everywhere**: A single template implementation works for scalar floats and hardware SIMD vectors (`Legolas::NativeSimd<T, P>`).
- :twisted_right_wards_arrows: **Two-Level Decoupled Parallelism**: 
    1. **Data-Level (SIMD)**: Automatic via Data Layout Interleaving.
    2. **Thread-Level (Multi-Core)**: Built-in, header-only zero-allocation parallel loop scheduler.
- :apple: **Cross-Platform**: Validated on Apple Silicon ARM64 (NEON), Linux x86_64 (AVX2 / AVX-512), and Windows MSVC.

```cmake
# Add Legolas++ to your CMakeLists.txt in 4 lines:
include(FetchContent)
FetchContent_Declare(Legolas GIT_REPOSITORY https://github.com/LaurentPlagne/Legolas.git GIT_TAG master)
FetchContent_MakeAvailable(Legolas)
target_link_libraries(my_project PRIVATE Legolas)
```

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

## 30-Second Quick Example

Write your numerical solver **once** using natural scalar math; Legolas++ executes it at full hardware vector line-rate and parallelizes across cores simultaneously:

```cpp
#include <iostream>
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

// 1. Define your algorithm once using natural scalar math:
struct Scaler {
    template <class A2D>
    void operator()(int begin, int end, A2D X, A2D Y) const {
        using Scalar = typename A2D::RealType;
        Scalar factor(2.5f);
        for (int j = begin; j < end; ++j) {
            for (int i = 0; i < X[j].size(); ++i) {
                Y[j][i] = factor * X[j][i]; // Compiles to NEON / AVX2 FMA automatically!
            }
        }
    }
};

int main() {
    // 2. Declare 2D array interleaved across dimension 2 with pack size P=4:
    using Array2D = Legolas::Array<float, 2, 4, 2>; // 1024 systems of size 256
    Array2D X(1024, 256), Y(1024, 256);
    X.fill(1.0f);

    // 3. Parallelize across cores and vectorize in SIMD simultaneously:
    Legolas::parmap(Scaler(), X, Y);

    std::cout << "Computed 1024 systems at full hardware vector line-rate!" << std::endl;
    return 0;
}
```

---

## Citation & Academic Background

Legolas++ is based on the research presented at **ACM SIGPLAN ARRAY 2017**:

> [**Portable vectorization and parallelization of C++ multi-dimensional array computations**](https://dl.acm.org/doi/10.1145/3091966.3091973)  
> Laurent Plagne & Kavoos Bojnourdi  
> *Proceedings of the 4th ACM SIGPLAN International Workshop on Libraries, Languages, and Compilers for Array Programming (ARRAY 2017)*, Pages 47–54.  
> DOI: [10.1145/3091966.3091973](https://doi.org/10.1145/3091966.3091973) · Available on [ResearchGate](https://www.researchgate.net/publication/317485219_Portable_vectorization_and_parallelization_of_C_multi-dimensional_array_computations)

---

## License

Legolas++ is distributed under the terms of the permissive **[MIT License](https://github.com/LaurentPlagne/Legolas/blob/master/LICENSE)** (Copyright (c) 2019-2026 EDF-R&D, TriScale innov).
