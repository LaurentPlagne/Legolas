cmake_minimum_required (VERSION 3.5)
project (LEGOLAS CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MACOSX_RPATH ON)

set ( CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_MODULE_PATH} )

set ( LEGOLAS_CXX_Debug_FLAGS   "-g3")
set ( LEGOLAS_CXX_Release_FLAGS "-g")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64")
    set ( CMAKE_CXX_FLAGS " -O3 -mtune=native -Rpass-analysis=loop-vectorize -fno-slp-vectorize -Rpass=loop-vectorize ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
  else()
    set ( CMAKE_CXX_FLAGS " -O3 -mtune=native -march=native -Rpass-analysis=loop-vectorize -fno-slp-vectorize -Rpass=loop-vectorize ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set ( CMAKE_CXX_FLAGS " -O3 -mtune=native -march=native  -fopt-info-vec-all -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=3 ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set ( CMAKE_CXX_FLAGS " -O3  -fno-alias -mavx2 -xHost -qopt-report=5  ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
endif()

set ( CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -std=c++14 ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )

option(USE_TBB "Use Intel TBB library (otherwise uses native WorkStealing)" ON)

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

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

enable_testing()

add_subdirectory(Legolas)
add_subdirectory(tst)
add_subdirectory(examples)



