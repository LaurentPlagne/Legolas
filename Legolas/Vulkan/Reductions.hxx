#pragma once

#include "Legolas/Vulkan/DeviceArray.hxx"
#include "Legolas/Vulkan/VulkanKernel.hxx"
#include "Legolas/Vulkan/spv/reduce_final.hxx"
#include "Legolas/Vulkan/spv/reduce_partial.hxx"

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace Legolas {
namespace Vulkan {

namespace detail {

// Two-pass reduction (per-workgroup partials then a single final workgroup).
// `b` may be null: mode 0 (sum of squares) never reads it and binding `a` as
// the second descriptor is safe.
inline bool reduceDispatch(Context& ctx, const Buffer& a, const Buffer* b,
                           size_t n, uint32_t mode, Buffer& out) {
  if (!ctx.available() || n == 0 || !a.valid() || !out.valid()) return false;

  const uint32_t wgSize = 256;
  uint32_t numWorkgroups =
      static_cast<uint32_t>(std::min<size_t>((n + wgSize - 1) / wgSize, 4096));
  if (numWorkgroups == 0) numWorkgroups = 1;

  Buffer partials(ctx, numWorkgroups * sizeof(float));
  if (!partials.valid()) return false;

  Kernel partialKernel(ctx, spv::reduce_partial, spv::reduce_partial_words, 3,
                       sizeof(uint32_t) * 2);
  Kernel finalKernel(ctx, spv::reduce_final, spv::reduce_final_words, 2,
                     sizeof(uint32_t));
  if (!partialKernel.valid() || !finalKernel.valid()) return false;

  struct PartialParams {
    uint32_t n;
    uint32_t mode;
  } partialParams{static_cast<uint32_t>(n), mode};
  const Buffer* partialBuffers[3] = {&a, b != nullptr ? b : &a, &partials};
  partialKernel.dispatch(partialBuffers, 3, numWorkgroups, 1, 1, &partialParams,
                         sizeof(partialParams), 1);

  uint32_t partialCount = numWorkgroups;
  const Buffer* finalBuffers[2] = {&partials, &out};
  finalKernel.dispatch(finalBuffers, 2, 1, 1, 1, &partialCount,
                       sizeof(partialCount), 1);
  return true;
}

inline bool reduceImpl(Context& ctx, const float* a, const float* b, size_t n,
                       uint32_t mode, double* result) {
  if (!ctx.available() || a == nullptr || n == 0 || result == nullptr) {
    return false;
  }

  Buffer deviceA(ctx, n * sizeof(float));
  Buffer deviceOut(ctx, sizeof(float));
  if (!deviceA.valid() || !deviceOut.valid()) return false;
  deviceA.upload(a, n * sizeof(float));

  Buffer deviceB;
  const Buffer* deviceBptr = nullptr;
  if (b != nullptr) {
    deviceB = Buffer(ctx, n * sizeof(float));
    if (!deviceB.valid()) return false;
    deviceB.upload(b, n * sizeof(float));
    deviceBptr = &deviceB;
  }

  if (!reduceDispatch(ctx, deviceA, deviceBptr, n, mode, deviceOut)) return false;

  float out = 0.0f;
  deviceOut.download(&out, sizeof(float));
  *result = static_cast<double>(out);
  return true;
}

} // namespace detail

// ---------------------------------------------------------------------------
// One-shot reductions on host memory: upload, reduce, download. Return false
// (without touching *result) when no device is available, so callers can fall
// back to the CPU implementation.
// ---------------------------------------------------------------------------
inline bool squaredNorm(Context& ctx, const float* data, size_t n,
                        double* result) {
  return detail::reduceImpl(ctx, data, nullptr, n, 0u, result);
}

inline bool dot(Context& ctx, const float* a, const float* b, size_t n,
                double* result) {
  return detail::reduceImpl(ctx, a, b, n, 1u, result);
}

// ---------------------------------------------------------------------------
// Resident-data reductions: no transfer, the caller keeps the DeviceArray
// alive. This is the recommended path for iterative algorithms.
// ---------------------------------------------------------------------------
inline bool squaredNorm(const DeviceArray<float>& data, double* result) {
  if (!data.valid() || result == nullptr) return false;
  Buffer out(Context::instance(), sizeof(float));
  if (!out.valid()) return false;
  if (!detail::reduceDispatch(Context::instance(), data.buffer(), nullptr,
                              data.size(), 0u, out)) {
    return false;
  }
  float value = 0.0f;
  out.download(&value, sizeof(float));
  *result = static_cast<double>(value);
  return true;
}

inline bool dot(const DeviceArray<float>& a, const DeviceArray<float>& b,
                double* result) {
  if (!a.valid() || !b.valid() || a.size() != b.size() || result == nullptr) {
    return false;
  }
  Buffer out(Context::instance(), sizeof(float));
  if (!out.valid()) return false;
  if (!detail::reduceDispatch(Context::instance(), a.buffer(), &b.buffer(),
                              a.size(), 1u, out)) {
    return false;
  }
  float value = 0.0f;
  out.download(&value, sizeof(float));
  *result = static_cast<double>(value);
  return true;
}

}} // namespace Legolas::Vulkan
