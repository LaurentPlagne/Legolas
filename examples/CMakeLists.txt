add_executable(DepthwiseConv DepthwiseConv/DepthwiseConv.cxx)
target_link_libraries(DepthwiseConv Legolas)

add_executable(AudioBiquad AudioBiquad/AudioBiquad.cxx)
target_link_libraries(AudioBiquad Legolas)

add_test(NAME DepthwiseConv COMMAND DepthwiseConv)
add_test(NAME AudioBiquad COMMAND AudioBiquad)
