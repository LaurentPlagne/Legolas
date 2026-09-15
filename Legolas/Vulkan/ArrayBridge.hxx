#pragma once

// Legolas::Vulkan overloads of the core reductions with an explicit backend
// policy. These wrappers are self-contained: they reuse the CPU primitives
// and the eligibility trait from Legolas/Array/Reductions.hxx and call the
// generic GPU reductions directly, so they work whether or not the automatic
// core dispatch (LEGOLAS_HAS_VULKAN) is enabled.

#include "Legolas/Array/Reductions.hxx"
#include "Legolas/Vulkan/Backend.hxx"
#include "Legolas/Vulkan/Reductions.hxx"

#include <cstddef>
#include <type_traits>

namespace Legolas {
namespace Vulkan {
namespace detail {

template <class DERIVED>
inline double squaredNormWithPolicy(const BaseArray<DERIVED>& ba, Backend backend,
                                    std::true_type /*eligible*/) {
  const DERIVED& a = ba.getArrayRef();
  const size_t n = static_cast<size_t>(a.size());
  if (shouldUseGpu(backend, n)) {
    double gpuResult = 0.0;
    if (squaredNorm(Context::instance(), a.realDataPtr(), n, &gpuResult)) {
      return gpuResult;
    }
  }
  return Legolas::detail::squaredNormCpu(ba);
}

template <class DERIVED>
inline double squaredNormWithPolicy(const BaseArray<DERIVED>& ba, Backend,
                                    std::false_type /*eligible*/) {
  return Legolas::detail::squaredNormCpu(ba);
}

template <class DERIVED>
inline double dotWithPolicy(const BaseArray<DERIVED>& left,
                            const BaseArray<DERIVED>& right, Backend backend,
                            std::true_type /*eligible*/) {
  const DERIVED& a = left.getArrayRef();
  const DERIVED& b = right.getArrayRef();
  const size_t n = static_cast<size_t>(a.size());
  if (a.size() == b.size() && shouldUseGpu(backend, n)) {
    double gpuResult = 0.0;
    if (dot(Context::instance(), a.realDataPtr(), b.realDataPtr(), n,
            &gpuResult)) {
      return gpuResult;
    }
  }
  return Legolas::detail::dotCpu(left, right);
}

template <class DERIVED>
inline double dotWithPolicy(const BaseArray<DERIVED>& left,
                            const BaseArray<DERIVED>& right, Backend,
                            std::false_type /*eligible*/) {
  return Legolas::detail::dotCpu(left, right);
}

} // namespace detail

// ---------------------------------------------------------------------------
// Core-array reductions routed through the backend policy. The signatures
// mirror Legolas::squaredNorm / Legolas::dot; with the default Backend::Auto
// the GPU is used only for large enough workloads and an unavailable or
// failing device falls back to the CPU implementation transparently.
// ---------------------------------------------------------------------------
template <class DERIVED>
inline double squaredNorm(const BaseArray<DERIVED>& ba,
                          Backend backend = Backend::Auto) {
  return detail::squaredNormWithPolicy(
      ba, backend,
      std::integral_constant<
          bool, Legolas::detail::GpuReductionEligible<DERIVED>::value>());
}

template <class DERIVED>
inline double dot(const BaseArray<DERIVED>& left, const BaseArray<DERIVED>& right,
                  Backend backend = Backend::Auto) {
  return detail::dotWithPolicy(
      left, right, backend,
      std::integral_constant<
          bool, Legolas::detail::GpuReductionEligible<DERIVED>::value>());
}

}} // namespace Legolas::Vulkan
