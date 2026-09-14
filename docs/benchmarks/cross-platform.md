# Cross-Platform & Cloud Performance Evaluation

Legolas++ is designed for high portability across CPU microarchitectures and operating systems. Because performance characteristics vary significantly between vector engines (ARM NEON vs. x86 AVX2 vs. x86 AVX-512) and memory subsystems, this guide details how to benchmark Legolas++ on modern server architectures.

> 📈 **Theoretical Performance Limits & Roofline Modeling**  
> To understand how close these benchmark numbers operate to the absolute physical limits of the silicon (memory bandwidth saturation vs. peak FMA compute capacity), explore our dedicated [**Roofline Performance Model & Hardware Efficiency Guide**](roofline-model.md).

---

## 1. Architectural Matrix

| Architecture | Vector Extension | Vector Width | Native Float Pack ($P$) | Target Hardware |
| :--- | :---: | :---: | :---: | :---: |
| **Apple Silicon** | ARM NEON | 128-bit | $P=4$ ($P=8$ unrolled) | Apple M1/M2/M3/M4 |
| **ARM Server** | NEON / SVE | 128-bit / 256-bit | $P=4$ / $P=8$ | AWS Graviton 3/4, Ampere Altra |
| **x86-64 (Standard)** | AVX2 + FMA | 256-bit | $P=8$ | Intel Core 4th-14th gen, AMD Zen 2/3 |
| **x86-64 (Server/HPC)** | AVX-512 | 512-bit | **$P=16$** | Intel Xeon Scalable (Ice Lake, Sapphire Rapids), AMD Zen 4/5 |

> [!TIP]
> **The AVX-512 Advantage ($P=16$)**:  
> On AVX-512 architectures, Legolas++ packs $P=16$ independent problem instances into a single 512-bit `zmm` register. A single vectorized operation simultaneously computes 16 reciprocal square roots, divisions, or multiply-accumulates across 16 problem instances.

---

## 2. Running On-Demand Cloud Benchmarks (AWS / Hetzner / Scaleway)

To generate reference publication curves on target server hardware without maintaining dedicated bare-metal infrastructure, use on-demand cloud spot instances.

### 2.1 Recommended Cloud Instances

1. **Intel Xeon Scalable (AVX-512, $P=16$)**:
   * **AWS EC2**: `c6i.4xlarge` (Ice Lake) or `c7i.4xlarge` (Sapphire Rapids)
   * **Cost**: ~$0.08 to $0.15 per hour (spot pricing).
2. **AMD EPYC (Zen 4, AVX-512)**:
   * **AWS EC2**: `c7a.4xlarge` (Genoa)
3. **ARM Neoverse (SVE, 256-bit)**:
   * **AWS EC2**: `c7g.4xlarge` (Graviton 3, Neoverse V1)

### 2.2 Automated Execution

Once connected to your cloud instance (Ubuntu 24.04 LTS), execute the one-shot benchmark script:

```bash
git clone https://github.com/LaurentPlagne/Legolas.git
cd Legolas
./scripts/run_cloud_bench.sh
```

The script automatically:
1. Configures standard build tools (`cmake`, `ninja`, modern C++14 compiler).
2. Detects available vector instruction sets (`avx512f`, `avx2`, `neon`, `sve`).
3. Compiles the native zero-dependency benchmarks (`Legolas::NativeSimd` and the header-only `StaticThreadPool` scheduler).
4. Executes the full `MultiThomas` benchmark resolution sweep ($N_x \in [8, 512]$) and thread scaling benchmarks.
5. Produces publication-grade SVG/PNG plots and interactive HTML reports.

---

## 3. GitHub Actions CI Matrix

Continuous integration is automated on every commit and pull request via [`.github/workflows/ci.yml`](https://github.com/LaurentPlagne/Legolas/blob/master/.github/workflows/ci.yml).

The CI matrix verifies compilation and runs the complete test suite across:
* **macOS 14 ARM64** (Apple Silicon Clang runner)
* **Ubuntu 24.04 x86_64** (Linux GCC 14 runner)
* **Windows x86_64** (Microsoft Visual C++ MSVC runner)

```yaml
strategy:
  matrix:
    os: [ubuntu-latest, macos-14, windows-latest]
```

Every build validates 100% test passage across all test suites (`MultiThomasExample`, `StaticArrays`, `MultiPrec`, `DepthwiseConv`, `AudioBiquad`).

---

## 4. Automated Live Benchmark Results (Continuous Cloud Runs)

> ⚡ **Continuously Measured on GitHub Actions Cloud Runners**  
> Every execution of the automated benchmark pipeline measures real-world throughput across cloud runners and dynamically updates this table.  
> 🔗 [View all benchmark runs on GitHub Actions](https://github.com/LaurentPlagne/Legolas/actions/workflows/benchmarks.yml)

<!-- BEGIN_BENCHMARK_RESULTS -->
| Workload | Metric | 🐧 **Linux x86_64** | 🍏 **macOS** |
| :--- | :--- | :---: | :---: |
| **Hardware Target** | CPU / Arch | AMD EPYC 7763 64-Core Processor | Apple M1 (Virtual) (3 cores) |
| **MultiThomas** ($N_x=64$) | Throughput | **15.1 GFlops** | **20.6 GFlops** |
| **VideoPipeline (CPU)** | Throughput | **3,270 FPS** | **2,989 FPS** |
| **AudioBiquad (64ch)** | Throughput | **4,399 MS/s** | **2,446 MS/s** |
| **DepthwiseConv (128ch)** | Throughput | **165.7 GFlops** | **64.8 GFlops** |
| **OptionPricing** (16k options) | Throughput | **483,453 opt/s** | **127,152 opt/s** |
<!-- END_BENCHMARK_RESULTS -->

