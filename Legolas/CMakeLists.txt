include_directories(..)
include_directories(include)
file(GLOB Legolas_SRC
  "BlockMatrix/*.cxx"
  "include/*.cxx"
  "BlockMatrix/Structures/*/*.cxx"
  )
add_library(Legolas SHARED ${Legolas_SRC})
if ( TBB_FOUND )
  target_link_libraries(Legolas ${TBB_LIBRARIES})
endif ( TBB_FOUND)


