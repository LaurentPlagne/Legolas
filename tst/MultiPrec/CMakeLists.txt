add_executable(MultiPrec MultiPrec.cxx)
configure_file(plotBench.py plotBench.py COPYONLY)
target_link_libraries(MultiPrec Legolas)


