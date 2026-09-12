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

# Optional Vulkan GPU benchmark. The backend is header-only and loads
# libvulkan at runtime: only the Vulkan C headers are needed at build time.
# macOS keeps its native Metal path and is excluded here.
if (LEGOLAS_ENABLE_VULKAN AND NOT APPLE)
    find_package(Vulkan QUIET)
    if (Vulkan_FOUND)
        add_executable(VulkanBench VulkanBench/VulkanBench.cxx)
        target_link_libraries(VulkanBench Legolas ${CMAKE_DL_LIBS})
        target_include_directories(VulkanBench PRIVATE ${VULKAN_INCLUDE_DIRS})
        add_test(NAME VulkanBench COMMAND VulkanBench --skip-heavy)
    else()
        message(WARNING "LEGOLAS_ENABLE_VULKAN=ON but Vulkan headers were not found: VulkanBench skipped.")
    endif()
endif()
