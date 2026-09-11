include_directories(..)
include_directories(include)
file(GLOB Legolas_SRC
  "BlockMatrix/*.cxx"
  "include/*.cxx"
  "BlockMatrix/Structures/*/*.cxx"
  )
add_library(Legolas STATIC ${Legolas_SRC})
target_link_libraries(Legolas Threads::Threads)
if ( TBB_FOUND AND USE_TBB )
  target_link_libraries(Legolas ${TBB_LIBRARIES})
endif ()


