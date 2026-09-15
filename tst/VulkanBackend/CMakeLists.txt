add_executable(VulkanBackend VulkanBackend.cxx)
target_link_libraries(VulkanBackend Legolas::Vulkan)
add_test(NAME VulkanBackend COMMAND VulkanBackend)

# Second run with the device disabled: exercises the CPU fallback even on
# machines that do have a Vulkan driver.
add_test(NAME VulkanBackendFallback COMMAND VulkanBackend)
set_tests_properties(VulkanBackendFallback PROPERTIES
  ENVIRONMENT "LEGOLAS_DISABLE_VULKAN=1")
