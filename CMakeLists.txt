cmake_minimum_required (VERSION 2.8)
project (LEGOLAS CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)

set ( CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_MODULE_PATH} )


set ( LEGOLAS_CXX_Debug_FLAGS   "-g3 -mtune=native -march=native")
set ( LEGOLAS_CXX_Release_FLAGS "-g -O3  -std=c++14 -mtune=native -march=native") 

set ( CMAKE_CXX_FLAGS "-Wall -Wno-deprecated-declarations -std=c++14 ${LEGOLAS_CXX_${CMAKE_BUILD_TYPE}_FLAGS} ${CMAKE_CXX_FLAGS}" )



find_package(Eigen3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR})

find_package(TBB REQUIRED)
if ( TBB_FOUND )
  include_directories(${TBB_INCLUDE_DIRS})
endif ( TBB_FOUND )


add_subdirectory(Legolas)
add_subdirectory(tst)



