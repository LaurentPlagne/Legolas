# Installation & Build

Legolas++ is a **100% header-only, zero-dependency** C++14 template engine. It requires no compiled binary libraries (`.a`, `.so`, `.dylib`, `.dll`), no complex linker flags, and no third-party package managers.

---

## 📋 Prerequisites

- **C++ Compiler**: A standard C++14 compliant compiler:
  - Apple Clang $\ge 12.0$ (macOS Apple Silicon / Intel)
  - GCC $\ge 7.0$ (Linux)
  - LLVM Clang $\ge 8.0$ (Linux / macOS)
  - Microsoft Visual C++ (MSVC) $\ge 2017$ (Windows)
- **CMake**: Version $\ge 3.5$
- **External Dependencies**: **None** (zero external dependencies). Both SIMD packing (`Legolas::NativeSimd`) and multi-core scheduling (`Legolas::StaticThreadPool`) are pure standard C++.

---

## 🚀 Compiling & Running Tests

Clone the repository and build the automated test suite with standard CMake:

```bash
# Clone the repository
git clone https://github.com/laurentplagne/Legolas.git
cd Legolas

# Configure Release build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

# Run the automated test suite (100% passing)
ctest --test-dir build --output-on-failure
```

Expected output:
```text
Test project .../Legolas/build
    Start 1: MultiThomasExample
1/9 Test #1: MultiThomasExample ...............   Passed    0.02 sec
    Start 2: StaticArrays
2/9 Test #2: StaticArrays .....................   Passed    0.01 sec
    Start 3: MultiPrec
3/9 Test #3: MultiPrec ........................   Passed    0.02 sec
    Start 4: DepthwiseConv
4/9 Test #4: DepthwiseConv ....................   Passed    0.42 sec
    Start 5: AudioBiquad
5/9 Test #5: AudioBiquad ......................   Passed    4.80 sec
    Start 6: MultiThomasVulkanAccuracy
6/9 Test #6: MultiThomasVulkanAccuracy ........   Passed    0.15 sec
    Start 7: VulkanBench
7/9 Test #7: VulkanBench ......................   Passed    0.28 sec
...
100% tests passed, 0 tests failed out of 9
```

---

## 📦 Integrating Legolas++ into Your Project

### Method 1: CMake FetchContent (Recommended)

Add Legolas++ to your `CMakeLists.txt` in 4 lines without managing submodules or manual clones:

```cmake
include(FetchContent)

FetchContent_Declare(
  Legolas
  GIT_REPOSITORY https://github.com/LaurentPlagne/Legolas.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(Legolas)

add_executable(my_solver main.cpp)
target_link_libraries(my_solver PRIVATE Legolas)
```

### Method 2: CMake add_subdirectory (Local Clone or Git Submodule)

Add Legolas as a subdirectory and link the `INTERFACE` target:

```cmake
cmake_minimum_required(VERSION 3.5)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(path/to/Legolas)

add_executable(my_solver main.cpp)
target_link_libraries(my_solver PRIVATE Legolas)
```

The `Legolas` target automatically propagates all required include directories and threading flags.

### Method 3: Install + `find_package` (Prefix / System-Wide)

Install the headers and the CMake package files with a single command:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --install build --prefix /path/to/legolas-install
```

Then consume the installed package from any project:

```cmake
find_package(Legolas CONFIG REQUIRED)

add_executable(my_solver main.cpp)
target_link_libraries(my_solver PRIVATE Legolas::Legolas)  # or `Legolas`
```

Set `CMAKE_PREFIX_PATH=/path/to/legolas-install` (or install to a standard prefix) so that `find_package` can locate `LegolasConfig.cmake`.

### Method 4: Header-Only Include Path

Simply pass the include paths directly to your compiler:

```bash
c++ -O3 -std=c++14 -I/path/to/Legolas -I/path/to/Legolas/Legolas/include main.cpp -o my_solver
```

---

## 🔥 GPU Acceleration: Optional Vulkan Compute Backend (Linux & Windows)

Starting with version 2.1, Legolas++ provides an optional, self-contained **Vulkan compute backend** for Linux and Windows platforms (while macOS leverages native Apple Metal).

### 1. Architectural Philosophy: Zero Link-Time Dependencies

The Vulkan backend adheres strictly to Legolas++'s core design principles:

1. **Zero Link-Time Dependencies**:
   - `VK_NO_PROTOTYPES` is defined internally; every Vulkan API entry point is dynamically resolved at runtime via `vkGetInstanceProcAddr` through `dlopen` (Linux) or `LoadLibrary` (Windows).
   - The library links only against `${CMAKE_DL_LIBS}`. You **never** need to link against `libvulkan.so` or `vulkan-1.lib`.
2. **Versioned In-Tree SPIR-V**:
   - Compute shaders are precompiled to SPIR-V binaries and committed directly in-tree as standard C++ `const uint32_t[]` arrays in `Legolas/Vulkan/spv/*.hxx`.
   - **No Vulkan SDK and no shader compiler (`glslc` / `shaderc`) is required to build or execute.**
3. **Transparent CPU Fallback**:
   - If no compatible Vulkan GPU, driver, or ICD loader is present on the machine, `Legolas::Vulkan::Context::available()` evaluates to `false`. The engine detects this instantly and falls back cleanly to the multi-core CPU SIMD path without crashing.

### 2. How Data Layout Interleaving (DLI) Maps to GPU

* **GPU Thread as SIMD Lane**: On CPU, DLI groups $P$ independent problem instances into adjacent vector lanes. On the GPU, each thread represents one instance, and an entire warp (32 threads) or wavefront (64 threads) streams data in unison.
* **SoA Layout for 100% Coalesced Memory**: Problem ensembles are transposed to an `[step][system]` (Structure-of-Arrays) layout in VRAM, guaranteeing that adjacent GPU threads access contiguous memory addresses with 100% cache-line efficiency.
* **Instruction-Level Parallelism via `vec4`**: For tridiagonal recurrences, each GPU thread solves **4 systems simultaneously using `vec4` registers**. This amortizes index calculations and eliminates instruction-issue bottlenecks, delivering a **4.8× speedup** over naive scalar thread mappings.

### 3. Benchmark Highlights (NVIDIA RTX 2060 SUPER vs. 12-Thread CPU)

Measured with Release build (`-O3 -march=native`), resident VRAM buffers:

| Workload | GPU Kernel Time | Speedup vs Scalar | Speedup vs 12-Thread CPU | Performance Metric |
| :--- | :---: | :---: | :---: | :---: |
| **VideoPipeline** (32×720p Sobel + Temporal) | **0.98 ms** | **57.3×** | **18.0×** | **32,659 FPS** (30.1 GPix/s) |
| **DepthwiseConv** (128ch, 3×3 MobileNet) | **0.044 ms** | **12.8×** | **4.9×** | **628.4 GFlops** |
| **MultiThomas** ($N_x=512$, 262,144 systems) | **13.8 ms** | **58.0×** | **9.1×** | **126.6 GFlops** |
| **AudioBiquad** (64ch × 960k samples, blocked scan) | **9.95 ms** | **15.7×** | **2.8×** | **6,176 MSamples/s** |
| **Reductions** (`squaredNorm`, `dot`, 16.7M floats) | **0.20–0.32 ms** | — | **39–62×** | Bandwidth saturated (~350 GB/s) |

> ⚠️ **Resident Data vs. Discrete PCIe Transfers**  
> On discrete GPUs, data transfers over PCIe run at ~4.5 GB/s. For a single one-shot isolated execution, PCIe transfer overhead dominates and the CPU parallel path wins. The GPU delivers massive real-world gains when **buffers stay resident in VRAM** across time steps or frames (iterative PDE solvers, multi-pass rendering/vision pipelines, or audio streaming), or on **Unified Memory Architectures (UMA)** where zero-copy host-visible memory eliminates transfers entirely.

### 4. Enabling and Building the Vulkan Backend

Enabling Vulkan is opt-in via the `LEGOLAS_ENABLE_VULKAN` CMake flag (requires standard Vulkan C headers `vulkan/vulkan.h` on Linux/Windows):

```bash
# 1. Configure with Vulkan enabled
cmake -B build -DCMAKE_BUILD_TYPE=Release -DLEGOLAS_ENABLE_VULKAN=ON

# 2. Build the project and benchmark suite
cmake --build build -j

# 3. Run all CTest verification tests
ctest --test-dir build --output-on-failure

# 4. Run the comprehensive Vulkan benchmark suite
./build/examples/VulkanBench                # Full suite
./build/examples/VulkanBench --skip-heavy    # Fast smoke test
```

---

## 🍎 GPU Acceleration: Native Apple Metal Backend (macOS)

On macOS platforms, Legolas++ leverages native **Apple Metal** (`examples/VideoPipeline/VideoPipelineMetal.mm`).

Apple Silicon features a **Unified Memory Architecture (UMA)** where CPU cores and GPU cores share the same physical memory pool:
- **Zero-Copy Memory**: Interleaved tensors are allocated in shared host-accessible buffers (`MTLResourceStorageModeShared`). Data written by CPU threads is immediately visible to GPU compute pipelines with **zero PCIe copy latency**.
- **Performance**: The 32-stream 720p HD Video Pipeline reaches **17,264 FPS** (15.91 GPixels/s) at only **2.43 µJ per frame** (**87.2% energy reduction** vs. scalar baseline), sustaining 287 concurrent 60 FPS streams on a 40 W laptop.
