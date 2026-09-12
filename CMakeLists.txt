cmake_minimum_required (VERSION 3.5)
project (LEGOLAS
  VERSION 1.0.0
  DESCRIPTION "Building blocks for Linear Algebra Solvers"
  LANGUAGES CXX
)

set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set ( CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_MODULE_PATH} )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opt-in build knobs. The defaults keep release builds fast while avoiding
# the previous pitfalls: Debug was silently compiled with -O3, vectorization
# reports were always emitted and -march=native was always forced.
option(LEGOLAS_NATIVE_OPTIMIZATION
  "Tune the build for the host CPU (-march=native / -mtune=native)" ON)
option(LEGOLAS_VECTORIZATION_REPORT
  "Emit compiler vectorization reports (noisy, slows down compilation)" OFF)
option(LEGOLAS_VERBOSE_MAKEFILE
  "Enable verbose makefiles" OFF)

if (LEGOLAS_VERBOSE_MAKEFILE)
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

# Release optimization level. Generator expressions keep Debug builds
# debuggable (no -O3) with both single- and multi-config generators.
set(LEGOLAS_RELEASE_OPT_FLAG $<$<NOT:$<CONFIG:Debug>>:-O3>)

if (MSVC)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  add_compile_options(/W3 /EHsc)
  add_compile_options($<$<NOT:$<CONFIG:Debug>>:/O2>)
  add_definitions(-D_USE_MATH_DEFINES)
else()
  add_compile_options(-Wall -Wno-deprecated-declarations)

  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(${LEGOLAS_RELEASE_OPT_FLAG})
    add_compile_options(-fno-slp-vectorize)
    if (LEGOLAS_NATIVE_OPTIMIZATION)
      add_compile_options($<$<NOT:$<CONFIG:Debug>>:-mtune=native>)
      if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64")
        add_compile_options($<$<NOT:$<CONFIG:Debug>>:-march=native>)
      endif()
    endif()
    if (LEGOLAS_VECTORIZATION_REPORT)
      add_compile_options(-Rpass-analysis=loop-vectorize -Rpass=loop-vectorize)
    endif()
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(${LEGOLAS_RELEASE_OPT_FLAG})
    add_compile_options($<$<NOT:$<CONFIG:Debug>>:-funroll-loops>
                        $<$<NOT:$<CONFIG:Debug>>:-ftree-vectorize>)
    if (LEGOLAS_NATIVE_OPTIMIZATION)
      add_compile_options($<$<NOT:$<CONFIG:Debug>>:-mtune=native>
                          $<$<NOT:$<CONFIG:Debug>>:-march=native>)
    endif()
    if (LEGOLAS_VECTORIZATION_REPORT)
      add_compile_options(-fopt-info-vec-all)
    endif()
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    add_compile_options(${LEGOLAS_RELEASE_OPT_FLAG} -fno-alias)
    if (LEGOLAS_NATIVE_OPTIMIZATION)
      add_compile_options($<$<NOT:$<CONFIG:Debug>>:-mavx2>
                          $<$<NOT:$<CONFIG:Debug>>:-xHost>)
    endif()
    if (LEGOLAS_VECTORIZATION_REPORT)
      add_compile_options(-qopt-report=5)
    endif()
  endif()

  add_definitions(-D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L)
endif()

option(LEGOLAS_ENABLE_VULKAN "Build the optional Vulkan GPU backend benchmark (Linux/Windows, header-only, runtime-loaded)" OFF)

find_package(Threads REQUIRED)

enable_testing()

add_subdirectory(Legolas)
add_subdirectory(tst)
add_subdirectory(examples)



