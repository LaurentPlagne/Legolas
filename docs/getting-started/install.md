# Installation & Build

Legolas++ is designed to be lightweight, portable, and easy to integrate into modern C++ projects.

---

## Prerequisites

- **C++ Compiler**: A modern C++14 compliant compiler:
  - AppleClang $\ge 12.0$ (macOS Apple Silicon / Intel)
  - GCC $\ge 7.0$ (Linux)
  - Clang $\ge 8.0$ (Linux / macOS)
- **CMake**: Version $\ge 3.5$
- **Eigen**: Version $\ge 3.3$ (recommended $3.4+$).
  - macOS: `brew install eigen`
  - Ubuntu/Debian: `sudo apt-get install libeigen3-dev`

---

## Build Options

Legolas++ provides a dual-backend parallel engine via the CMake flag `USE_TBB`:

```bash
# Option 1: Native Work-Stealing (Zero External Dependencies, Default recommended)
cmake -B build -DUSE_TBB=OFF

# Option 2: Intel oneTBB Backend
cmake -B build -DUSE_TBB=ON
```

---

## Compiling & Running Tests

```bash
# Clone the repository
git clone https://github.com/laurentplagne/Legolas.git
cd Legolas

# Configure and compile with all available CPU cores
cmake -B build -DUSE_TBB=OFF
cmake --build build -j8

# Run the automated test suite (5/5 tests passing in < 5 seconds)
ctest --test-dir build --output-on-failure
```

Expected output:
```text
Test project .../Legolas/build
    Start 1: MultiThomasExample
1/5 Test #1: MultiThomasExample ...............   Passed    0.01 sec
    Start 2: StaticArrays
2/5 Test #2: StaticArrays .....................   Passed    0.01 sec
    Start 3: MultiPrec
3/5 Test #3: MultiPrec ........................   Passed    0.01 sec
    Start 4: DepthwiseConv
4/5 Test #4: DepthwiseConv ....................   Passed    0.07 sec
    Start 5: AudioBiquad
5/5 Test #5: AudioBiquad ......................   Passed    4.90 sec

100% tests passed, 0 tests failed out of 5
```

---

## Integrating Legolas++ into Your CMake Project

Because Legolas++ arrays are header-only, integrating it into an existing CMake project takes only a few lines:

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.5)
project(MyProject CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Add Legolas include directories
include_directories(/path/to/Legolas /path/to/Legolas/Legolas/include)

add_executable(my_app main.cpp)
```
