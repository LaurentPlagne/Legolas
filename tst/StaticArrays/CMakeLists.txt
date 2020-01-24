include_directories(../../)
include_directories(../../Legolas/include)
add_executable(StaticArrays StaticArrays.cxx)
target_link_libraries(StaticArrays ${TBB_LIBRARIES} ${TBB_MALLOC_LIBRARIES})


