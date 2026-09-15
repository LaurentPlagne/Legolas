add_executable(DepthwiseConv DepthwiseConv/DepthwiseConv.cxx)
target_link_libraries(DepthwiseConv Legolas)

add_executable(AudioBiquad AudioBiquad/AudioBiquad.cxx)
target_link_libraries(AudioBiquad Legolas)

add_executable(VideoPipeline VideoPipeline/VideoPipeline.cxx)
target_link_libraries(VideoPipeline Legolas)

add_executable(OptionPricing OptionPricing/OptionPricing.cxx)
target_link_libraries(OptionPricing Legolas)

add_test(NAME DepthwiseConv COMMAND DepthwiseConv)
add_test(NAME AudioBiquad COMMAND AudioBiquad)
add_test(NAME VideoPipeline COMMAND VideoPipeline)
add_test(NAME OptionPricing COMMAND OptionPricing)

if (APPLE)
    add_executable(VideoPipelineMetal VideoPipeline/VideoPipelineMetal.mm)
    target_link_libraries(VideoPipelineMetal Legolas "-framework Metal" "-framework Foundation")
    add_test(NAME VideoPipelineMetal COMMAND VideoPipelineMetal)
endif()

# Optional Vulkan GPU benchmark. The core exposes Legolas::Vulkan only when
# LEGOLAS_ENABLE_VULKAN is ON and the Vulkan headers were found; the backend
# itself is header-only and loads libvulkan at runtime. macOS keeps its native
# Metal path and is excluded there.
if (TARGET LegolasVulkan)
    add_executable(VulkanBench VulkanBench/VulkanBench.cxx)
    target_link_libraries(VulkanBench Legolas::Vulkan)
    add_test(NAME VulkanBench COMMAND VulkanBench --skip-heavy)
endif()
