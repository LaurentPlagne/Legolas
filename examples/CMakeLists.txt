add_executable(DepthwiseConv DepthwiseConv/DepthwiseConv.cxx)
target_link_libraries(DepthwiseConv Legolas)

add_executable(AudioBiquad AudioBiquad/AudioBiquad.cxx)
target_link_libraries(AudioBiquad Legolas)

add_executable(VideoPipeline VideoPipeline/VideoPipeline.cxx)
target_link_libraries(VideoPipeline Legolas)

add_test(NAME DepthwiseConv COMMAND DepthwiseConv)
add_test(NAME AudioBiquad COMMAND AudioBiquad)
add_test(NAME VideoPipeline COMMAND VideoPipeline)

if (APPLE)
    add_executable(VideoPipelineMetal VideoPipeline/VideoPipelineMetal.mm)
    target_link_libraries(VideoPipelineMetal Legolas "-framework Metal" "-framework Foundation")
    add_test(NAME VideoPipelineMetal COMMAND VideoPipelineMetal)
endif()
