#pragma once

// Expression evaluation on the GPU: automatic GLSL generation from Legolas
// expression templates (GlslEmitter) plus runtime SPIR-V compilation
// (SpirVCompiler).
//
// The CPU path is always available: when the expression is not GPU-eligible,
// no device is available or no shader compiler is installed, `evaluate` runs
// the regular host assignment `output = expression`.

#include "Legolas/Array/Expression.hxx"
#include "Legolas/Vulkan/Backend.hxx"
#include "Legolas/Vulkan/DeviceArray.hxx"
#include "Legolas/Vulkan/GlslEmitter.hxx"
#include "Legolas/Vulkan/SpirVCompiler.hxx"
#include "Legolas/Vulkan/VulkanKernel.hxx"

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <vector>

namespace Legolas {
namespace Vulkan {
namespace detail {

template <class DERIVED>
inline bool evaluateToBuffer(Context& ctx, const BaseArray<DERIVED>& expression,
                             Buffer& output, size_t n) {
  if (!ctx.available() || !output.valid() || n == 0) return false;
  if (!SpirVCompiler::available()) return false;

  GlslProgram program;
  GlslEmitter<DERIVED>::emit(expression.getArrayRef(), program);
  if (program.inputs.empty()) return false;
  for (size_t size : program.sizes) {
    if (size < n) return false;
  }

  std::vector<uint32_t> spirv;
  if (!SpirVCompiler::compile(buildGlslSource(program), spirv)) return false;

  std::vector<Buffer> deviceInputs;
  deviceInputs.reserve(program.inputs.size());
  for (size_t i = 0; i < program.inputs.size(); ++i) {
    deviceInputs.emplace_back(ctx, program.sizes[i] * sizeof(float));
    if (!deviceInputs.back().valid()) return false;
    deviceInputs.back().upload(program.inputs[i],
                               program.sizes[i] * sizeof(float));
  }

  std::vector<const Buffer*> buffers;
  buffers.reserve(deviceInputs.size() + 1);
  for (const Buffer& buffer : deviceInputs) buffers.push_back(&buffer);
  buffers.push_back(&output);

  const uint32_t bindingCount = static_cast<uint32_t>(buffers.size());
  Kernel kernel(ctx, spirv.data(), spirv.size(), bindingCount,
                sizeof(uint32_t));
  if (!kernel.valid()) return false;

  const uint32_t count = static_cast<uint32_t>(n);
  const uint32_t groups = (count + 255u) / 256u;
  kernel.dispatch(buffers.data(), bindingCount, groups, 1, 1, &count,
                  sizeof(count));
  return true;
}

template <class DERIVED>
inline bool evaluateDevice(Context& ctx, const BaseArray<DERIVED>& expression,
                           DeviceArray<float>& output, std::true_type) {
  const size_t n = static_cast<size_t>(expression.getArrayRef().size());
  if (n == 0 || !output.valid() || output.size() < n) return false;
  return evaluateToBuffer(ctx, expression, output.buffer(), n);
}

template <class DERIVED>
inline bool evaluateDevice(Context&, const BaseArray<DERIVED>&,
                           DeviceArray<float>&, std::false_type) {
  return false;
}

template <class DERIVED, class HOST>
inline bool evaluateHost(const BaseArray<DERIVED>& expression, HOST& output,
                         Backend backend, std::true_type) {
  const size_t n = static_cast<size_t>(expression.getArrayRef().size());
  if (n > 0 && shouldUseGpu(backend, n) && SpirVCompiler::available()) {
    Context& ctx = Context::instance();
    DeviceArray<float> device(ctx, n);
    if (device.valid() &&
        evaluateToBuffer(ctx, expression, device.buffer(), n) &&
        toHost(device, output)) {
      return true;
    }
  }
  output = expression;
  return false;
}

template <class DERIVED, class HOST>
inline bool evaluateHost(const BaseArray<DERIVED>& expression, HOST& output,
                         Backend, std::false_type) {
  output = expression;
  return false;
}

} // namespace detail

// True when an external GLSL compiler (glslc or glslangValidator) was found
// and the generated expression shaders can be compiled at runtime.
inline bool runtimeShaderCompilerAvailable() {
  return detail::SpirVCompiler::available();
}

// ---------------------------------------------------------------------------
// Evaluate a level-1 float expression into a resident device array. Returns
// false when the expression is not GPU-eligible, no device or shader compiler
// is available, or any dispatch step fails.
// ---------------------------------------------------------------------------
template <class DERIVED>
inline bool evaluate(Context& ctx, const BaseArray<DERIVED>& expression,
                     DeviceArray<float>& output) {
  return detail::evaluateDevice(
      ctx, expression, output,
      std::integral_constant<
          bool, detail::GpuExpressionEligible<DERIVED>::value>());
}

// ---------------------------------------------------------------------------
// Evaluate an expression into a host array (must be assignable from
// BaseArray<DERIVED>, e.g. Legolas::Array). Backend::Auto (default) uses the
// GPU when available and large enough; the CPU assignment
// `output = expression` is always the fallback. Returns true when the GPU
// path produced the result.
// ---------------------------------------------------------------------------
template <class DERIVED, class HOST>
inline bool evaluate(const BaseArray<DERIVED>& expression, HOST& output,
                     Backend backend = Backend::Auto) {
  return detail::evaluateHost(
      expression, output, backend,
      std::integral_constant<
          bool, detail::GpuExpressionEligible<DERIVED>::value>());
}

}} // namespace Legolas::Vulkan
