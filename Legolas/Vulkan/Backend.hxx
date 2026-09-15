#pragma once

#include "Legolas/Vulkan/VulkanContext.hxx"

#include <cstddef>
#include <cstdlib>

namespace Legolas {
namespace Vulkan {

// ---------------------------------------------------------------------------
// Backend: execution policy shared by the generic helpers of this module.
//
// Core algorithms can opt into the GPU without hard-wiring a dependency: the
// policy is passed explicitly, and an unavailable device (or a failed
// allocation/dispatch) always falls back to the CPU implementation.
//
//   CPU    : never touch the device.
//   Vulkan : try the GPU, fall back to the CPU if anything fails.
//   Auto   : GPU only when a device is available and the workload is large
//            enough to amortize the transfer (see minGpuElements()).
// ---------------------------------------------------------------------------
enum class Backend { CPU, Vulkan, Auto };

inline bool available() { return Context::instance().available(); }

inline Backend preferredBackend() {
  return available() ? Backend::Vulkan : Backend::CPU;
}

inline const char* backendName(Backend backend) {
  switch (backend) {
    case Backend::Vulkan:
      return "Vulkan";
    case Backend::Auto:
      return "Auto";
    default:
      return "CPU";
  }
}

// Minimum element count above which a one-shot host->device->host helper is
// expected to amortize the PCIe transfer. Overridable through the
// LEGOLAS_VULKAN_MIN_ELEMENTS environment variable.
inline size_t minGpuElements() {
  static const size_t value = []() -> size_t {
    const char* env = std::getenv("LEGOLAS_VULKAN_MIN_ELEMENTS");
    if (env != nullptr && *env != '\0') {
      char* end = nullptr;
      const unsigned long long parsed = std::strtoull(env, &end, 10);
      if (end != env && parsed > 0) return static_cast<size_t>(parsed);
    }
    return static_cast<size_t>(1) << 20;
  }();
  return value;
}

inline bool shouldUseGpu(Backend backend, size_t elements) {
  if (backend == Backend::CPU) return false;
  if (backend == Backend::Vulkan) return true;
  return available() && elements >= minGpuElements();
}

}} // namespace Legolas::Vulkan
