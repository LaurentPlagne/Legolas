include_directories(../../)
include_directories(../../Legolas/include)
add_executable(MultiThomas MultiThomas.cxx)
if (TBB_FOUND AND USE_TBB)
  target_link_libraries(MultiThomas ${TBB_LIBRARIES} ${TBB_MALLOC_LIBRARIES})
endif()

configure_file(plotPerf.py plotPerf.py COPYONLY)
configure_file(plotSpeedUp.py plotSpeedUp.py COPYONLY)
configure_file(plotPerfLaplacian.py plotPerfLaplacian.py COPYONLY)
configure_file(plotPerfLDL.py plotPerfLDL.py COPYONLY)
configure_file(plotBench.py plotBench.py COPYONLY)
#target_link_libraries(sub_string_finder ${TBB_IMPORTED_TARGETS})

#target_link_libraries(MultiThomas ${TBB_IMPORTED_TARGETS}${TBB_LIBRARIES} ${TBB_MALLOC_LIBRARIES})
#target_link_libraries(MultiThomas ${TBB_IMPORTED_TARGETS})


