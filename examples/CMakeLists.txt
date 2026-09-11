include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Legolas/include)

add_executable(DepthwiseConv DepthwiseConv/DepthwiseConv.cxx)
target_include_directories(DepthwiseConv PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(DepthwiseConv Legolas)
if (TBB_FOUND AND USE_TBB)
  target_link_libraries(DepthwiseConv ${TBB_LIBRARIES})
endif()

add_executable(AudioBiquad AudioBiquad/AudioBiquad.cxx)
target_include_directories(AudioBiquad PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(AudioBiquad Legolas)
if (TBB_FOUND AND USE_TBB)
  target_link_libraries(AudioBiquad ${TBB_LIBRARIES})
endif()

add_test(NAME DepthwiseConv COMMAND DepthwiseConv)
add_test(NAME AudioBiquad COMMAND AudioBiquad)
