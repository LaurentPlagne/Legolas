include_directories(../../)
include_directories(../../Legolas/include)
add_executable(StaticArrays StaticArrays.cxx)
target_link_libraries(StaticArrays Threads::Threads)
if (TBB_FOUND AND USE_TBB)
  target_link_libraries(StaticArrays ${TBB_LIBRARIES})
endif()


