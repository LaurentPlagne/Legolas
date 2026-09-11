cmake_minimum_required (VERSION 3.5)
project (LEGOLAS CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set ( CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_MODULE_PATH} )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
  set(CMAKE_CXX_FLAGS "/O2 /W3 /EHsc /D_USE_MATH_DEFINES ${CMAKE_CXX_FLAGS}")
  add_definitions(-D_USE_MATH_DEFINES)
else()
  set ( LEGOLAS_CXX_Debug_FLAGS   "-g3")
  set ( LEGOLAS_CXX_Release_FLAGS "-g")

  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64")
      set ( CMAKE_CXX_FLAGS " -O3 -mtune=native -Rpass-analysis=loop-vectorize -fno-slp-vectorize -Rpass=loop-vectorize ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
    else()
      set ( CMAKE_CXX_FLAGS " -O3 -mtune=native -march=native -Rpass-analysis=loop-vectorize -fno-slp-vectorize -Rpass=loop-vectorize ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
    endif()
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set ( CMAKE_CXX_FLAGS " -O3 -mtune=native -march=native  -fopt-info-vec-all -funroll-loops -ftree-vectorize ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set ( CMAKE_CXX_FLAGS " -O3  -fno-alias -mavx2 -xHost -qopt-report=5  ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
  endif()

  set ( CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
endif()

find_package(Threads REQUIRED)

option(USE_TBB "Use Intel TBB library (otherwise uses native WorkStealing)" ON)
option(USE_EIGEN "Use Eigen library for SIMD arrays (otherwise uses native NativeSimd)" ON)

if (USE_TBB)
  find_package(TBB QUIET)
endif()

if (TBB_FOUND AND USE_TBB)
  message(STATUS "Found Intel TBB: ${TBB_LIBRARIES}")
  add_definitions(-DUSING_TBB=1)
  include_directories(${TBB_INCLUDE_DIRS})
else()
  message(STATUS "Using native WorkStealing loop parallelizer (TBB disabled or not found)")
  add_definitions(-DUSING_TBB=0)
endif()

find_package(Eigen3 QUIET)
if (EIGEN3_FOUND)
  include_directories(${EIGEN3_INCLUDE_DIR})
endif()

if (USE_EIGEN)
  if (EIGEN3_FOUND)
    message(STATUS "Using Eigen backend for SIMD arrays: ${EIGEN3_INCLUDE_DIR}")
    add_definitions(-DUSE_EIGEN=1)
  else()
    message(WARNING "Eigen3 requested but not found, falling back to NativeSimd")
    set(USE_EIGEN OFF)
  endif()
else()
  message(STATUS "Using native NativeSimd vector packs (Eigen disabled)")
endif()

enable_testing()

add_subdirectory(Legolas)
add_subdirectory(tst)
add_subdirectory(examples)



