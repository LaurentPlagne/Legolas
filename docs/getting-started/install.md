# Installation & Build

Legolas++ is a **100% header-only, zero-dependency** C++14 template engine. It requires no compiled libraries (`.a`, `.so`, `.dylib`, `.dll`), no linker flags, and no third-party package managers.

---

## Prerequisites

- **C++ Compiler**: A standard C++14 compliant compiler:
  - Apple Clang $\ge 12.0$ (macOS Apple Silicon / Intel)
  - GCC $\ge 7.0$ (Linux)
  - LLVM Clang $\ge 8.0$ (Linux / macOS)
  - Microsoft Visual C++ (MSVC) $\ge 2017$ (Windows)
- **CMake**: Version $\ge 3.5$
- **External Dependencies**: **None** (zero external dependencies). Both SIMD packing (`Legolas::NativeSimd`) and thread scheduling (`Legolas::WorkStealingThreadPool`) are fully native standard C++.

---

## Compiling & Running Tests

```bash
# Clone the repository
git clone https://github.com/laurentplagne/Legolas.git
cd Legolas

# Configure and compile tests
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

# Run the automated test suite (100% passing in < 6 seconds)
ctest --test-dir build --output-on-failure
```

Expected output:
```text
Test project .../Legolas/build
    Start 1: MultiThomasExample
1/5 Test #1: MultiThomasExample ...............   Passed    0.02 sec
    Start 2: StaticArrays
2/5 Test #2: StaticArrays .....................   Passed    0.01 sec
    Start 3: MultiPrec
3/5 Test #3: MultiPrec ........................   Passed    0.02 sec
    Start 4: DepthwiseConv
4/5 Test #4: DepthwiseConv ....................   Passed    0.42 sec
    Start 5: AudioBiquad
5/5 Test #5: AudioBiquad ......................   Passed    4.80 sec

100% tests passed, 0 tests failed out of 5
```

---

## Integrating Legolas++ into Your Project

### Method 1: Modern CMake Target (Recommended)

Add Legolas as a subdirectory and link the `INTERFACE` target:

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(path/to/Legolas)

add_executable(my_solver main.cpp)
target_link_libraries(my_solver PRIVATE Legolas)
```

The `Legolas` target automatically propagates all necessary include directories and thread flags.

### Method 2: Header-Only Include

Simply include the headers and compile with your compiler:

```bash
c++ -O3 -std=c++14 -I/path/to/Legolas -I/path/to/Legolas/Legolas/include main.cpp -o my_solver
```

