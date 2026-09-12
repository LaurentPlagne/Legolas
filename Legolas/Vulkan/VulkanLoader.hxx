#pragma once

// Minimal Vulkan function loader.
//
// Legolas++ keeps its "zero dependency" promise: the Vulkan loader is opened
// at runtime (dlopen / LoadLibrary) and every entry point is resolved through
// vkGetInstanceProcAddr. Nothing is linked at build time, and a machine
// without a Vulkan driver simply falls back to the CPU path.
//
// Only the ~40 commands required for compute + buffer transfers are loaded.

#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <windows.h>
#else
#  include <dlfcn.h>
#endif

#ifndef VK_NO_PROTOTYPES
#  define VK_NO_PROTOTYPES 1
#endif
#include <vulkan/vulkan_core.h>

#include <cstdint>

namespace Legolas {
namespace Vulkan {

#define LEGOLAS_VK_GLOBAL_FUNCTIONS(X)          \
  X(vkCreateInstance)                           \
  X(vkEnumerateInstanceExtensionProperties)     \
  X(vkEnumerateInstanceLayerProperties)

#define LEGOLAS_VK_INSTANCE_FUNCTIONS(X)        \
  X(vkDestroyInstance)                          \
  X(vkEnumeratePhysicalDevices)                 \
  X(vkGetPhysicalDeviceProperties)              \
  X(vkGetPhysicalDeviceMemoryProperties)        \
  X(vkGetPhysicalDeviceQueueFamilyProperties)   \
  X(vkCreateDevice)                             \
  X(vkGetDeviceProcAddr)

#define LEGOLAS_VK_DEVICE_FUNCTIONS(X)          \
  X(vkDestroyDevice)                            \
  X(vkDeviceWaitIdle)                           \
  X(vkGetDeviceQueue)                           \
  X(vkCreateBuffer)                             \
  X(vkDestroyBuffer)                            \
  X(vkGetBufferMemoryRequirements)              \
  X(vkAllocateMemory)                           \
  X(vkFreeMemory)                               \
  X(vkBindBufferMemory)                         \
  X(vkMapMemory)                                \
  X(vkUnmapMemory)                              \
  X(vkCreateShaderModule)                       \
  X(vkDestroyShaderModule)                      \
  X(vkCreateDescriptorSetLayout)                \
  X(vkDestroyDescriptorSetLayout)               \
  X(vkCreatePipelineLayout)                     \
  X(vkDestroyPipelineLayout)                    \
  X(vkCreateComputePipelines)                   \
  X(vkDestroyPipeline)                          \
  X(vkCreateDescriptorPool)                     \
  X(vkDestroyDescriptorPool)                    \
  X(vkAllocateDescriptorSets)                   \
  X(vkUpdateDescriptorSets)                     \
  X(vkCreateCommandPool)                        \
  X(vkDestroyCommandPool)                       \
  X(vkAllocateCommandBuffers)                   \
  X(vkFreeCommandBuffers)                       \
  X(vkBeginCommandBuffer)                       \
  X(vkEndCommandBuffer)                         \
  X(vkCmdBindPipeline)                          \
  X(vkCmdBindDescriptorSets)                    \
  X(vkCmdDispatch)                              \
  X(vkCmdPushConstants)                         \
  X(vkCmdCopyBuffer)                            \
  X(vkCmdPipelineBarrier)                       \
  X(vkQueueSubmit)                              \
  X(vkQueueWaitIdle)                            \
  X(vkCreateFence)                              \
  X(vkDestroyFence)                             \
  X(vkWaitForFences)                            \
  X(vkResetFences)

class VulkanApi {
public:
  VulkanApi() = default;
  ~VulkanApi() { unloadLibrary(); }

  VulkanApi(const VulkanApi&) = delete;
  VulkanApi& operator=(const VulkanApi&) = delete;

  bool loadLibrary() {
#if defined(_WIN32)
    lib_ = reinterpret_cast<void*>(::LoadLibraryA("vulkan-1.dll"));
#else
    lib_ = ::dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
    if (!lib_) lib_ = ::dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
#endif
    if (!lib_) return false;

    vkGetInstanceProcAddr =
        reinterpret_cast<PFN_vkGetInstanceProcAddr>(symbol("vkGetInstanceProcAddr"));
    if (!vkGetInstanceProcAddr) {
      unloadLibrary();
      return false;
    }

#define LEGOLAS_VK_LOAD_GLOBAL(name)                                              \
    name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(nullptr, #name));   \
    if (!name) {                                                                  \
      unloadLibrary();                                                            \
      return false;                                                               \
    }
    LEGOLAS_VK_GLOBAL_FUNCTIONS(LEGOLAS_VK_LOAD_GLOBAL)
#undef LEGOLAS_VK_LOAD_GLOBAL

    return true;
  }

  bool loadInstance(VkInstance instance) {
#define LEGOLAS_VK_LOAD_INSTANCE(name)                                            \
    name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(instance, #name));  \
    if (!name) return false;
    LEGOLAS_VK_INSTANCE_FUNCTIONS(LEGOLAS_VK_LOAD_INSTANCE)
    LEGOLAS_VK_DEVICE_FUNCTIONS(LEGOLAS_VK_LOAD_INSTANCE)
#undef LEGOLAS_VK_LOAD_INSTANCE
    return true;
  }

  void unloadLibrary() {
    if (lib_) {
#if defined(_WIN32)
      ::FreeLibrary(reinterpret_cast<HMODULE>(lib_));
#else
      ::dlclose(lib_);
#endif
      lib_ = nullptr;
    }
  }

  void* symbol(const char* name) {
#if defined(_WIN32)
    return reinterpret_cast<void*>(
        ::GetProcAddress(reinterpret_cast<HMODULE>(lib_), name));
#else
    return ::dlsym(lib_, name);
#endif
  }

  void* lib_ = nullptr;
  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;

#define LEGOLAS_VK_DECLARE(name) PFN_##name name = nullptr;
  LEGOLAS_VK_GLOBAL_FUNCTIONS(LEGOLAS_VK_DECLARE)
  LEGOLAS_VK_INSTANCE_FUNCTIONS(LEGOLAS_VK_DECLARE)
  LEGOLAS_VK_DEVICE_FUNCTIONS(LEGOLAS_VK_DECLARE)
#undef LEGOLAS_VK_DECLARE
};

}} // namespace Legolas::Vulkan
