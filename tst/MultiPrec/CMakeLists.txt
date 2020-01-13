include_directories(../../)
include_directories(../../Legolas/include)
add_executable(MultiPrec MultiPrec.cxx)
configure_file(plotBench.py plotBench.py COPYONLY)
target_link_libraries(MultiPrec Legolas ${TBB_LIBRARIES})


