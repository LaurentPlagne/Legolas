# Legolas++ Vulkan Compute Backend — Implementation and Benchmark Report

**Date:** September 12, 2026
**Branch:** `vulkan` (from `master` @ `6fda47c`)
**Scope:** optional, header-only Vulkan compute backend for Linux/Windows; macOS keeps its native Metal path.
**SPIR-V:** precompiled and versioned in-tree (`Legolas/Vulkan/spv/*.hxx`); no shader SDK is required to build or run.

---

## 1. Executive summary

A self-contained Vulkan compute backend was added to Legolas++ without breaking its two core
promises: **header-only** and **zero link-time dependencies**. The Vulkan loader is opened at
runtime (`dlopen`/`LoadLibrary`) and the SPIR-V binaries are embedded as C++ arrays, so enabling
the backend requires only the Vulkan C headers at compile time. When no device is available, the
code silently falls back to the CPU path.

Six workloads were ported and measured on a discrete GPU (NVIDIA RTX 2060 SUPER) against three
CPU baselines (scalar, AVX2 DLI, 12-thread parallel):

| Workload | GPU kernel time | Speedup vs scalar | Speedup vs 12-thread CPU | Verdict |
|---|---:|---:|---:|---|
| VideoPipeline (32×720p Sobel+temporal) | **0.98 ms** | **57.3×** | **18.0×** | excellent |
| DepthwiseConv (128ch, 3×3) | **0.044 ms** | **12.8×** | **4.9×** | excellent |
| MultiThomas, Nx=512 (262,144 systems) | **13.8 ms** (127 GFlops) | **58×** | **9.1×** | excellent |
| MultiThomas, Nx=8 (64 systems) | 0.012 ms | **0.13×** | 6× vs overhead-bound par | too small |
| OptionPricing (16,384 options, N=128) | 20.8 ms | 31.8× | **0.51×** | CPU wins (L3-resident) |
| AudioBiquad (64ch × 960k samples, blocked scan) | **9.9 ms** | **15.7×** | **2.8×** | good |
| Reductions (16.7M floats) | **0.20–0.32 ms** | — | **39–62×** | excellent |

**Key architectural result:** the CPU's Data Layout Interleaving (DLI) pack dimension maps
naturally onto GPU threads. Transposing the ensemble to an `[i][system]` (SoA) layout restores
coalescing, and processing **4 systems per thread with `vec4`** is essential for the tridiagonal
recurrence (4.8× over the scalar mapping at Nx=512).

**Key caveat for discrete GPUs:** the numbers above are *kernel-only*, with buffers resident in
VRAM. Because PCIe transfers run at ~4.5 GB/s on this machine, single-shot end-to-end execution
is transfer-bound and the CPU wins unless data stays on the device (streaming pipelines, repeated
time stepping, or unified-memory systems such as the Apple Silicon target of the Metal backend).

---

## 2. Environment and methodology

| Item | Value |
|---|---|
| CPU | AMD Ryzen 5 3600, 6 cores / 12 threads, 60 GB RAM |
| CPU SIMD | AVX2 + FMA (pack size P=8) |
| GPU | NVIDIA GeForce RTX 2060 SUPER, 8 GB GDDR6, 2176 CUDA cores, ~6.5 TFLOPS FP32, 448 GB/s |
| Driver / loader | NVIDIA 610.57.04 / Vulkan loader 1.4.341 |
| OS / toolchain | Linux x86_64, GCC 15.2.0, `-O3 -march=native` |
| Shader compiler | shaderc/glslc 2026.1, `--target-env=vulkan1.1 -O` (offline) |
| Backend | `LEGOLAS_ENABLE_VULKAN=ON`, Release build |

Methodology:

- **CPU timings**: warmup, then min of 3 samples × 5 inner iterations (VideoPipeline,
  DepthwiseConv, MultiThomas), 3×1 for OptionPricing, 3×3 for AudioBiquad.
- **GPU timings**: kernels are recorded back-to-back in a single command buffer and submitted
  once per sample; min of 3–5 samples after two warmup batches to force clock boost.
  Buffers stay resident; upload/download are measured separately.
- **Accuracy**: every GPU result is compared element-wise against the scalar CPU result.
- MultiThomas uses `13·Nx·Ny` flops; DepthwiseConv `18·(H−2)·(W−2)·C`.
- The benchmark binary is `examples/VulkanBench/VulkanBench.cxx`.

Reproduce:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DLEGOLAS_ENABLE_VULKAN=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
./build/examples/VulkanBench            # full suite
./build/examples/VulkanBench --skip-heavy
```

The complete CTest suite is green: **8/8 tests pass** (7 historical + `VulkanBench`).

---

## 3. Backend architecture

```
Legolas/Vulkan/
  VulkanLoader.hxx      # dlopen/LoadLibrary + vkGetInstanceProcAddr; ~45 function pointers
  VulkanContext.hxx     # Context singleton (device, queue, command pool) + Buffer + staging cache
  VulkanKernel.hxx      # shader module, descriptor sets, pipeline, push constants, dispatch
  Vulkan.hxx            # umbrella include + GPU reduction helpers
  shaders/*.comp        # GLSL reference sources
  shaders/build_shaders.sh, shaders/embed_spirv.py
  spv/*.hxx             # generated, versioned SPIR-V (const uint32_t[])
examples/VulkanBench/VulkanBench.cxx
```

Design decisions:

1. **Zero link dependency.** `VK_NO_PROTOTYPES` is defined and every entry point is resolved
   through `vkGetInstanceProcAddr`. Missing driver ⇒ `Context::available() == false` ⇒ CPU
   fallback.
2. **Versioned SPIR-V.** `Legolas/Vulkan/spv/*.hxx` are generated once and committed, so MSVC,
   CI, and end users never need `glslc`. Regeneration is opt-in via
   `shaders/build_shaders.sh` (requires glslc + Python 3).
3. **Thin, explicit API.** A `Kernel` owns `N` storage-buffer bindings (binding *i* ↔ buffer *i*)
   plus an optional push-constant block. `dispatch(..., repeat)` records `repeat` invocations in
   one command buffer with conservative buffer memory barriers between them.
4. **Memory.** Device-local buffers with a reusable host-visible staging buffer. The staging
   allocator deliberately avoids the small device-local PCIe BAR heap, which is write-combined
   and makes CPU reads catastrophically slow (see §7.1).
5. **GPU mapping of DLI.** A GPU thread replaces one SIMD lane; the ensemble is transposed to
   `[step][system]` (SoA) so that neighbouring threads read contiguous addresses. For the
   tridiagonal kernels each thread processes **4 systems at once via `vec4`**, amortizing address
   arithmetic and providing instruction-level parallelism.
6. **CMake.** `option(LEGOLAS_ENABLE_VULKAN ...)` defaults to `OFF`. When enabled on
   non-Apple platforms, `find_package(Vulkan QUIET)` locates the headers; the `VulkanBench`
   target links only `${CMAKE_DL_LIBS}` and registers a light CTest (`--skip-heavy`).

---

## 4. Results

### 4.1 VideoPipeline — 32 concurrent 720p streams

3×3 Sobel edge detection fused with quadratic temporal differencing; 118 MB per buffer.

| Variant | Time (ms) | FPS | GPix/s | Speedup vs scalar |
|---|---:|---:|---:|---:|
| CPU scalar (1 core) | 56.17 | 570 | 0.525 | 1.00× |
| CPU DLI AVX2 (1 core) | 19.95 | 1,604 | 1.478 | 2.82× |
| CPU parallel + DLI (12 threads) | 17.67 | 1,811 | 1.669 | 3.18× |
| **Vulkan GPU (kernel only)** | **0.980** | **32,659** | **30.10** | **57.3×** |

- GPU vs CPU parallel: **18.0×**.
- Accuracy: `max|diff| = 0.0` — bit-identical to the scalar CPU result.
- Transfers (steady state): 236 MB H2D in 53.6 ms (4.4 GB/s), 118 MB D2H in 26.5 ms (4.5 GB/s).
- End-to-end (upload + kernel + download): ~81 ms ⇒ ~400 FPS, i.e. **4.6× slower than the CPU
  parallel path** for a single isolated batch. The kernel speedup only materializes when frames
  are already resident on the device (camera/DMA path, repeated filtering, or UMA).

### 4.2 DepthwiseConv — 128 channels, 3×3 (MobileNet block)

| Variant | Time (ms) | GFlops | Speedup vs scalar |
|---|---:|---:|---:|
| CPU scalar (1 core) | 0.566 | 49.2 | 1.00× |
| CPU DLI AVX2 (1 core) | 0.522 | 53.4 | 1.09× |
| CPU parallel + DLI | 0.214 | 130.4 | 2.65× |
| **Vulkan GPU (kernel only)** | **0.044** | **628.4** | **12.8×** |

- GPU vs CPU parallel: **4.9×**. Accuracy: `max|diff| = 0.0`.
- DLI does not help this stencil on CPU (GCC already vectorizes the spatial loop): the GPU wins
  by sheer thread count. End-to-end is transfer-bound (~2.9 ms for 12.8 MB), so the GPU only pays
  off if the feature map stays resident across layers.

### 4.3 MultiThomas — ensembles of independent tridiagonal systems

Throughput in GFlops (`13·Nx·Ny`), AVX2 P=8, 12 CPU threads. GPU kernel-only.

| Nx | Systems | CPU P=1 seq | CPU P=8 seq | CPU P=8 par | **Vulkan GPU** | GPU vs par | max abs diff |
|---:|---:|---:|---:|---:|---:|---:|---:|
| 8 | 64 | 4.21 | 27.28 | 0.09 | 0.55 | 0.10× | 9.5e-07 |
| 16 | 256 | 2.88 | 22.87 | 0.59 | 3.24 | 5.4× | 7.6e-06 |
| 32 | 1,024 | 2.28 | 18.79 | 4.43 | 15.50 | 3.5× | 1.1e-04 |
| 64 | 4,096 | 2.22 | 18.05 | 166.71 | 47.90 | 0.29× | 7.9e-04 |
| 128 | 16,384 | 2.19 | 11.74 | 17.72 | **134.54** | **7.6×** | 1.2e-02 |
| 256 | 65,536 | 2.16 | 11.51 | 13.88 | **144.42** | **10.4×** | 2.3e-01 |
| 512 | 262,144 | 2.17 | 12.04 | 13.93 | **126.55** | **9.1×** | 7.2e+00 |

- Accuracy note: the absolute differences grow with the solution magnitude. The 1D Poisson
  system with unit RHS has solutions of order `Nx²/8` (≈ 32,768 at Nx=512), so the Nx=512
  difference of 7.2 corresponds to a **relative error of ~2×10⁻⁴**, consistent with float32
  rounding amplified by the 512-step recurrence and by FMA contraction differences between GCC
  and the GPU.
- Small ensembles are dominated by dispatch overhead and insufficient parallelism (64 systems =
  64 threads). At Nx≤64 the whole working set fits in the CPU's 32 MB L3 (only 5.2 MB at
  Nx=64), where the 12-thread CPU reaches ~167 GFlops and clearly beats the GPU; the GPU
  crossover is at Nx≈128, once the working set exceeds L3.
- **`vec4` matters:** the initial scalar mapping (1 system/thread) peaked at 27 GFlops for
  Nx=512; packing 4 systems per thread raised it to ~130 GFlops (4.8×) and removed a large
  run-to-run variance caused by instruction-issue saturation.
- End-to-end for Nx=512 (2.7 GB H2D + 0.5 GB D2H ≈ 615 ms) is far slower than the CPU; the
  GPU win applies to resident/iterative solvers, not one-shot transfers.

### 4.4 OptionPricing — Black-Scholes Crank-Nicolson, 16,384 options

One thread per option; persistent kernel over `Nt=50` backward time steps; layout `[i][option]`.

| Variant | Time (ms) | Options/s | Tridiag solves/s | Speedup |
|---|---:|---:|---:|---:|
| CPU scalar (1 core) | 661.2 | 24,781 | 1.24 M | 1.00× |
| CPU DLI AVX2 (1 core) | 92.5 | 177,066 | 8.85 M | 7.15× |
| CPU parallel + DLI | 10.7–11.5 | ~1.5 M | ~77 M | 58–62× |
| Vulkan GPU (kernel only) | 20.8 | 787,690 | 39 M | 31.8× |

- Accuracy: `max|diff| = 7.3e-4` on option values of order 10–100 (relative ~10⁻⁵–10⁻⁴).
- **The GPU loses to the 12-thread CPU here (0.51×).** Root cause analysis:
  - 16,384 options give only 16,384 threads = 64 workgroups of 256 on a 30-SM GPU; occupancy is
    low and the long dependent chain (`sm1 → s → sm1`, one division per grid point) is
    latency-bound.
  - The five coefficient/value arrays total ~42 MB, which mostly fits in the Ryzen's 32 MB L3,
    while the GPU's 4 MB L2 forces every one of the 50 time steps to stream from VRAM.
  - A `vec4` variant (4 options/thread) was worse (48.8 ms) because it reduced the thread count
    to 4,096.
  - Fusing the RHS computation into the sweep (removing the RHS array) was also worse
    (73.7 ms) because it lengthened the serial dependency chain for this under-occupied grid.
  - The measured bandwidth-bound floor for this algorithm is ~8 ms (3.4 GB of traffic at
    448 GB/s), so even an ideal parallel-in-space implementation would beat the CPU by only
    ~1.3×. **Conclusion: keep this workload on the CPU.**

### 4.5 AudioBiquad — 64 channels × 960,000 samples (long IIR recurrence)

A 2nd-order IIR filter is a long sequential recurrence per channel; with only 64 channels it is
a poor fit for a GPU. The implemented **blocked scan** splits each channel into `K=469` blocks of
`L=2048` samples and uses the linearity of the filter:

```
state_out(block) = M^L · state_in(block) + c(block)
```

- **Pass 1** (`biquad_block_states`): each thread computes the zero-state response `c` of one
  block (one recurrence per sample).
- **Pass 2** (`biquad_block_prefix`): 64 threads propagate block states sequentially using
  `M^L` (computed once on the host by 2×2 matrix exponentiation).
- **Pass 3** (`biquad_block_apply`): each thread re-runs its block with the exact initial state
  and writes the output.

| Variant | Time (ms) | MSamples/s | Speedup vs scalar |
|---|---:|---:|---:|
| CPU scalar (1 core) | 156.6 | 392.2 | 1.00× |
| CPU DLI AVX2 (1 core) | 29.9 | 2,053 | 5.24× |
| CPU parallel + DLI | 27.8 | 2,212 | 5.64× |
| **Vulkan GPU (3 kernels)** | **9.95** | **6,176** | **15.7×** |

- GPU vs CPU parallel: **2.8×**. Accuracy: `max|diff| = 1.1e-05`.
- The scan adds one extra pass over the input but exposes `64 × 469 = 30,016` independent
  threads, turning a 64-wide recurrence into a wide data-parallel problem.

### 4.6 Reductions — squaredNorm / dot, 16.7M floats (64 MB per array)

Two-pass GPU reduction (per-workgroup partials, then a single-workgroup final pass).

| Reduction | CPU (ms) | GPU (ms) | Speedup | Relative error |
|---|---:|---:|---:|---:|
| `squaredNorm` | 12.33 | 0.200 | 61.6× | < 1e-3 (shown 0.000) |
| `dot` | 12.45 | 0.321 | 38.8× | < 1e-3 (shown 0.000) |

- CPU values are accumulated in `double`; GPU partials in `float`. The relative difference is
  below the 3-decimal display precision.
- End-to-end (upload 128 MB + download) is ~30 ms, so GPU reductions pay off only when the
  arrays are already on the device.

---

## 5. End-to-end vs kernel-only on a discrete GPU

The single most important practical result of this study: **on a discrete GPU, kernel speedups
do not translate into end-to-end speedups unless the data stays resident**. Measured transfer
throughput is ~4.45 GB/s H2D and ~4.5 GB/s D2H (synchronous staging copies).

| Workload | Data per solve | Transfer estimate | Kernel | End-to-end | Best CPU (parallel) |
|---|---:|---:|---:|---:|---:|
| VideoPipeline | 354 MB | ~80 ms | 1.0 ms | ~81 ms | 17.7 ms |
| DepthwiseConv | 12.8 MB | ~2.9 ms | 0.04 ms | ~2.9 ms | 0.21 ms |
| MultiThomas Nx=512 | 3.2 GB | ~602 ms | 13.8 ms | ~616 ms | 125 ms |
| OptionPricing | 42 MB | ~9.4 ms | 20.8 ms | ~30 ms | 10.7 ms |
| AudioBiquad | 488 MB | ~110 ms | 9.9 ms | ~120 ms | 27.8 ms |
| Reductions | 128 MB | ~29 ms | 0.2 ms | ~29 ms | 12.3 ms |

Consequences:

1. The GPU backend targets **resident-data** workloads: real-time pipelines, iterative solvers
   with many passes, batch processing where upload/download is amortized.
2. On unified-memory systems (Apple Silicon, iGPUs), the staging path is replaced by
   `HOST_VISIBLE | DEVICE_LOCAL` mappings and these transfer costs mostly disappear — which is
   exactly why the Metal showcase reaches 57× end-to-end. The Vulkan backend is designed for the
   same zero-copy behavior on UMA hardware (`Context::isUnifiedMemory()`).
3. If needed, a future optimization is asynchronous transfers (double-buffered staging) to
   overlap PCIe with compute; the current implementation is deliberately synchronous for
   simplicity and reproducible measurements.

---

## 6. Cross-workload analysis

**Where the GPU wins decisively**

- Embarrassingly data-parallel kernels (stencils, element-wise): 13–57× kernel speedups.
- Ensembles with ≥ 10⁴ independent recurrences (MultiThomas Nx≥128): 7.6–10.4× over 12 CPU threads.
- Reductions: 39–62×.
- Long IIR recurrences with a blocked scan: 2.8× over 12 threads (the CPU parallel version is
  itself only 5.6× over scalar, because 64 channels cannot fill 12 cores).

**Where the GPU does not win**

- Small workloads (Nx≤32, DepthwiseConv end-to-end): dispatch and transfer overhead dominate.
- Cache-resident CPU workloads (OptionPricing): the CPU's L3 turns 42 MB of working set into
  near-cache bandwidth, while the GPU streams from VRAM; the 16k-thread grid is also too narrow
  to hide the recurrence latency.
- One-shot jobs on discrete GPUs: PCIe is the bottleneck.

**The DLI connection.** The CPU technique packs P independent instances into one SIMD register.
On the GPU, the same dimension is expressed by `vec4` packing (4 systems per thread) plus the
`[step][system]` transposition. Both transformations exist for the same reason: amortize
instruction overhead and expose memory-level parallelism across independent recurrences.

---

## 7. Engineering log: bugs found and fixed

### 7.1 Staging memory type (segfault + 20× slowdown)

- The first implementation allocated staging buffers with the same "prefer device-local" policy
  as compute buffers. On NVIDIA this can select a non-host-visible type and `vkMapMemory`
  returns a null pointer → segfault. Fixed by an explicit `requireHostVisible` path.
- The next fallback selected `HOST_VISIBLE | HOST_COHERENT` without `HOST_CACHED` — on this
  machine that is the write-combined system type, whose CPU **reads** run at ~150 MB/s
  (118 MB download took 765 ms). Preferring `HOST_CACHED` brought downloads to 26 ms (4.5 GB/s).
- Staging buffers are now cached in the `Context` and reused; a full 118 MB H2D+D2H pair costs
  ~26 ms each in steady state instead of ~850 ms.

### 7.2 Thomas recurrence: scalar mapping was instruction-issue-bound

The initial 1-system-per-thread kernel reached only 27 GFlops at Nx=512 with wild run-to-run
variance (12–148 ms per dispatch, quantized to powers of two). `nvidia-smi dmon` showed 100% SM
utilisation but only 15–27% memory utilisation: the kernel was issuing ~8 memory and ~10 ALU
instructions per element. Processing 4 systems per thread with `vec4` reduced the per-system
instruction count, restored stability, and delivered 130 GFlops (4.8×). The current kernel is
bandwidth-bound at ~350 GB/s for large Nx.

### 7.3 OptionPricing: three variants measured

1. Separate RHS pass + Thomas (best): 20.8 ms.
2. RHS fused into the sweep (less traffic, longer dependency chain): 73.7 ms.
3. `vec4` (4 options/thread): 48.8 ms.

Only variant 1 is kept; the alternatives are documented in the shader comments to prevent
regressions.

### 7.4 Benchmark harness bugs

- The reduction helper wrote a 4-byte `float` into an 8-byte `double`, printing subnormal
  garbage; fixed with explicit float temporaries.
- OptionPricing accuracy originally compared the GPU state after 6 in-place solves against one
  CPU solve; the device state is now reset before the accuracy dispatch.
- GPU warmup was increased to two full batches to defeat aggressive idle downclocking on the
  discrete card.

---

## 8. Limitations

1. **Single-precision only.** All kernels are FP32; double precision would need shader variants
   and is bandwidth-limited anyway.
2. **No asynchronous transfers / double buffering.** Transfers are synchronous; fine for
   benchmarks, suboptimal for streaming pipelines.
3. **No subgroup (warp) primitives.** The Thomas solver uses `vec4` ILP rather than
   `GL_KHR_shader_subgroup` shuffles; subgroup-based parallel cyclic reduction could improve
   OptionPricing and very small systems, at the cost of an optional extension.
4. **OptionPricing remains CPU-favorable** on this configuration; a parallel-in-space
   decomposition is the only way to approach the ~8 ms bandwidth floor, which is still not
   compelling versus the 10.7 ms 12-thread CPU.
5. **ImageFilter** was not ported separately: it belongs to the same stencil family as
   VideoPipeline/DepthwiseConv and would reuse the same kernel pattern.
6. **CI without a GPU**: the backend falls back cleanly, but GPU correctness in CI should use
   Mesa's `lavapipe` software driver; this was not wired into GitHub Actions yet.
7. **Validation layers** were not run (no Vulkan SDK on the test machine); the implementation
   was validated by exact/analytic result comparison instead.

---

## 9. Roadmap

1. **CI (lavapipe).** Add a `LEGOLAS_ENABLE_VULKAN=ON` job with `mesa-vulkan-drivers`, run
   `VulkanBench --skip-heavy`, assert accuracy only (no performance assertions).
2. **Async streaming.** Double-buffered staging + fences to overlap PCIe with compute, plus a
   `DeviceArray` RAII helper for resident data.
3. **Subgroup Thomas.** Optional `VK_KHR_shader_subgroup` kernels for small systems.
4. **Fusion.** Merge pass 1 and pass 3 of the biquad scan by storing per-block impulse responses
   instead of re-reading the input.
5. **UMA path validation.** Test on an integrated GPU / Apple MoltenVK-like UMA device to
   quantify zero-copy end-to-end numbers.
6. **Double precision** and **complex** scalar kernels if a scientific use case requires them.

---

## 10. Files added

```
CMakeLists.txt                                  (LEGOLAS_ENABLE_VULKAN option)
examples/CMakeLists.txt                         (VulkanBench target + CTest)
examples/VulkanBench/VulkanBench.cxx            (benchmark suite)
Legolas/Vulkan/Vulkan.hxx
Legolas/Vulkan/VulkanLoader.hxx
Legolas/Vulkan/VulkanContext.hxx
Legolas/Vulkan/VulkanKernel.hxx
Legolas/Vulkan/shaders/*.comp                   (8 GLSL compute shaders)
Legolas/Vulkan/shaders/build_shaders.sh
Legolas/Vulkan/shaders/embed_spirv.py
Legolas/Vulkan/spv/*.hxx                        (versioned SPIR-V)
vulkan.md                                       (this report)
```

The existing Metal backend (`examples/VideoPipeline/VideoPipelineMetal.mm`) is untouched; macOS
continues to use Metal.
