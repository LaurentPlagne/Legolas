include_directories(../../)
include_directories(../../Legolas/include)
add_executable(MultiThomas MultiThomas.cxx)
target_link_libraries(MultiThomas Legolas ${TBB_LIBRARIES} ${TBB_MALLOC_LIBRARIES})


