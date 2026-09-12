#pragma once

// Legolas++ Vulkan compute backend (optional, opt-in).
//
// Header-only: the Vulkan loader is resolved at runtime and the SPIR-V
// binaries are embedded in Legolas/Vulkan/spv/*.hxx. When no Vulkan device
// is available, Context::instance().available() is false and the caller can
// transparently fall back to the CPU implementation.
//
// Supported platforms: Linux and Windows. macOS keeps its native Metal path.

#include "Legolas/Vulkan/VulkanLoader.hxx"
#include "Legolas/Vulkan/VulkanContext.hxx"
#include "Legolas/Vulkan/VulkanKernel.hxx"

#include "Legolas/Vulkan/spv/video_pipeline.hxx"
#include "Legolas/Vulkan/spv/depthwise_conv.hxx"
#include "Legolas/Vulkan/spv/thomas.hxx"
#include "Legolas/Vulkan/spv/option_pricing.hxx"
#include "Legolas/Vulkan/spv/biquad_block_states.hxx"
#include "Legolas/Vulkan/spv/biquad_block_prefix.hxx"
#include "Legolas/Vulkan/spv/biquad_block_apply.hxx"
#include "Legolas/Vulkan/spv/reduce_partial.hxx"
#include "Legolas/Vulkan/spv/reduce_final.hxx"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace Legolas {
namespace Vulkan {

inline const char* backendName() { return "Vulkan"; }

namespace detail {

inline bool reduceImpl(Context& ctx, const float* a, const float* b, size_t n,
                       uint32_t mode, double* result) {
  if (!ctx.available() || a == nullptr || n == 0) return false;

  const uint32_t wgSize = 256;
  uint32_t numWorkgroups =
      static_cast<uint32_t>(std::min<size_t>((n + wgSize - 1) / wgSize, 4096));
  if (numWorkgroups == 0) numWorkgroups = 1;

  Buffer devA(ctx, n * sizeof(float));
  Buffer devB(ctx, b ? n * sizeof(float) : sizeof(float));
  Buffer devPartials(ctx, numWorkgroups * sizeof(float));
  Buffer devOut(ctx, sizeof(float));
  if (!devA.valid() || !devB.valid() || !devPartials.valid() || !devOut.valid()) {
    return false;
  }

  devA.upload(a, n * sizeof(float));
  if (b) devB.upload(b, n * sizeof(float));

  Kernel partialKernel(ctx, spv::reduce_partial, spv::reduce_partial_words, 3,
                       sizeof(uint32_t) * 2);
  Kernel finalKernel(ctx, spv::reduce_final, spv::reduce_final_words, 2,
                     sizeof(uint32_t));
  if (!partialKernel.valid() || !finalKernel.valid()) return false;

  struct PartialParams {
    uint32_t n;
    uint32_t mode;
  } partialParams{static_cast<uint32_t>(n), mode};
  const Buffer* partialBuffers[3] = {&devA, &devB, &devPartials};
  partialKernel.dispatch(partialBuffers, 3, numWorkgroups, 1, 1, &partialParams,
                         sizeof(partialParams), 1);

  uint32_t partialCount = numWorkgroups;
  const Buffer* finalBuffers[2] = {&devPartials, &devOut};
  finalKernel.dispatch(finalBuffers, 2, 1, 1, 1, &partialCount,
                       sizeof(partialCount), 1);

  float out = 0.0f;
  devOut.download(&out, sizeof(float));
  *result = static_cast<double>(out);
  return true;
}

} // namespace detail

// Squared Euclidean norm of a host float array, computed on the GPU.
inline bool reduceSquaredNorm(Context& ctx, const float* hostData, size_t n,
                              double* result) {
  return detail::reduceImpl(ctx, hostData, nullptr, n, 0u, result);
}

// Dot product of two host float arrays, computed on the GPU.
inline bool reduceDot(Context& ctx, const float* hostA, const float* hostB,
                      size_t n, double* result) {
  return detail::reduceImpl(ctx, hostA, hostB, n, 1u, result);
}

}} // namespace Legolas::Vulkan
