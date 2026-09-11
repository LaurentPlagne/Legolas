add_executable(MultiThomas MultiThomas.cxx)
target_link_libraries(MultiThomas Legolas)

configure_file(plotPerf.py plotPerf.py COPYONLY)
configure_file(plotSpeedUp.py plotSpeedUp.py COPYONLY)
configure_file(plotPerfLaplacian.py plotPerfLaplacian.py COPYONLY)
configure_file(plotPerfLDL.py plotPerfLDL.py COPYONLY)
configure_file(plotBench.py plotBench.py COPYONLY)


