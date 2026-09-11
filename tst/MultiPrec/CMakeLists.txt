include_directories(../../)
include_directories(../../Legolas/include)
add_executable(MultiPrec MultiPrec.cxx)
configure_file(plotBench.py plotBench.py COPYONLY)
if (TBB_FOUND AND USE_TBB)
  target_link_libraries(MultiPrec Legolas Threads::Threads ${TBB_LIBRARIES})
else()
  target_link_libraries(MultiPrec Legolas Threads::Threads)
endif()


