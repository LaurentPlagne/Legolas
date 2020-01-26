include_directories(../../)
include_directories(../../Legolas/include)
add_executable(MultiThomasExample MultiThomasExample.cxx)
#target_link_libraries(MultiThomasExample Legolas ${TBB_LIBRARIES} ${TBB_MALLOC_LIBRARIES})
target_link_libraries(MultiThomasExample ${TBB_LIBRARIES} ${TBB_MALLOC_LIBRARIES})
#target_link_libraries(sub_string_finder ${TBB_IMPORTED_TARGETS})

#target_link_libraries(MultiThomasExample ${TBB_IMPORTED_TARGETS}${TBB_LIBRARIES} ${TBB_MALLOC_LIBRARIES})
#target_link_libraries(MultiThomasExample ${TBB_IMPORTED_TARGETS})


