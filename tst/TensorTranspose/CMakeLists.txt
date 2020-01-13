include_directories(../../)
include_directories(../../Legolas/include)
include_directories(/home/lolo/Public/hptt/src)
link_directories(/home/lolo/Public/hptt/lib)
add_executable(TensorTranspose TensorTranspose.cxx)
target_link_libraries(TensorTranspose Legolas ${TBB_LIBRARIES} hptt ${OpenMP_CXX_LIBRARIES})
# set the path to the library folder


