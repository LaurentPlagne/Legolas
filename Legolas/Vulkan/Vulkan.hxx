#pragma once

// Legolas++ Vulkan compute backend (optional, opt-in).
//
// Header-only: the Vulkan loader is resolved at runtime and the SPIR-V
// binaries are embedded as C++ arrays. When no Vulkan device is available,
// Context::instance().available() is false and every helper falls back to the
// CPU implementation.
//
// This umbrella exposes the *generic* backend only. Workload kernels (the
// benchmark suite shaders) live in Legolas/Vulkan/Kernels.hxx, and custom
// SPIR-V can be fed directly to Vulkan::Kernel.
//
// Supported platforms: Linux and Windows. macOS keeps its native Metal path.

#include "Legolas/Vulkan/VulkanLoader.hxx"
#include "Legolas/Vulkan/VulkanContext.hxx"
#include "Legolas/Vulkan/VulkanKernel.hxx"
#include "Legolas/Vulkan/Backend.hxx"
#include "Legolas/Vulkan/DeviceArray.hxx"
#include "Legolas/Vulkan/Reductions.hxx"
#include "Legolas/Vulkan/ArrayBridge.hxx"
#include "Legolas/Vulkan/GlslEmitter.hxx"
#include "Legolas/Vulkan/SpirVCompiler.hxx"
#include "Legolas/Vulkan/Evaluate.hxx"

namespace Legolas {
namespace Vulkan {

inline const char* backendName() { return "Vulkan"; }

}} // namespace Legolas::Vulkan
