cmake_minimum_required (VERSION 2.8)
project (LEGOLAS CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)

set ( CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_MODULE_PATH} )


set ( LEGOLAS_CXX_Debug_FLAGS   "-g3")
set ( LEGOLAS_CXX_Release_FLAGS "-g")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set ( CMAKE_CXX_FLAGS " -O3 -mtune=native -march=native -Rpass-analysis=loop-vectorize -fno-slp-vectorize -Rpass=loop-vectorize ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set ( CMAKE_CXX_FLAGS " -O3 -mtune=native -march=native  -fopt-info-vec-all -funroll-loops -ftree-vectorize -ftree-vectorizer-verbose=3 ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
  # using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
 set ( CMAKE_CXX_FLAGS " -O3  -fno-alias -mavx2 -xHost -qopt-report=5  ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )
  # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # using Visual Studio C++
endif()

set ( CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -std=c++14 ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )


#include("/home/lolo/Public/tbb-2020.0/cmake/TBBGet.cmake")
#tbb_get(TBB_ROOT tbb_root CONFIG_DIR TBB_DIR)
find_package(TBB REQUIRED)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

#find_package(TBB REQUIRED)
if ( TBB_FOUND )
  include_directories(${TBB_INCLUDE_DIRS})
endif ( TBB_FOUND )


add_subdirectory(Legolas)
add_subdirectory(tst)



