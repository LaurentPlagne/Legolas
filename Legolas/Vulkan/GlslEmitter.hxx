#pragma once

// Automatic GLSL generation from Legolas array expressions.
//
// The expression templates built by Legolas/Array/Expression.hxx
// (ArrayExpression<L,OP,R> and ScaledArray<A>) form a binary tree that is
// fully known at compile time. GlslEmitter walks that tree and emits the
// corresponding GLSL expression, collecting the leaf arrays as storage-buffer
// inputs. The generated source is then compiled to SPIR-V at runtime by
// SpirVCompiler (see Evaluate.hxx for the public entry points).

#include "Legolas/Array/Expression.hxx"

#include <cstddef>
#include <cstdio>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Legolas {
namespace Vulkan {
namespace detail {

template <class...>
struct MakeVoid {
  typedef void Type;
};
template <class... Ts>
using VoidT = typename MakeVoid<Ts...>::Type;

// ---------------------------------------------------------------------------
// Eligibility: only level-1 (stride-1) float expressions can be mapped to the
// flat float kernels without a repacking pass. Leaves must expose a scalar
// data pointer; expression nodes recurse.
// ---------------------------------------------------------------------------
template <class T, class = void>
struct GpuExpressionEligible : std::false_type {};

template <class T>
struct GpuExpressionEligible<
    T, VoidT<decltype(std::declval<const T&>().realDataPtr())>>
    : std::integral_constant<bool, (T::level == 1) &&
                                       std::is_same<typename T::RealType,
                                                    float>::value> {};

template <class L, class OP, class R>
struct GpuExpressionEligible<ArrayExpression<L, OP, R>, void>
    : std::integral_constant<bool, GpuExpressionEligible<L>::value &&
                                       GpuExpressionEligible<R>::value> {};

template <class A>
struct GpuExpressionEligible<ScaledArray<A>, void>
    : GpuExpressionEligible<A> {};

// ---------------------------------------------------------------------------
// GLSL spelling of the Legolas binary operators.
// ---------------------------------------------------------------------------
template <class OP>
struct GlslOperator;

template <>
struct GlslOperator<AddArray> {
  static const char* symbol() { return "+"; }
};

template <>
struct GlslOperator<MinusArray> {
  static const char* symbol() { return "-"; }
};

template <>
struct GlslOperator<MultArray> {
  static const char* symbol() { return "*"; }
};

// ---------------------------------------------------------------------------
// Collected program: the GLSL body plus the list of input buffers (deduplicated
// by data pointer, so `a*a` binds `a` only once).
// ---------------------------------------------------------------------------
struct GlslProgram {
  std::vector<const float*> inputs;
  std::vector<size_t> sizes;
  std::string body;

  int input(const float* data, size_t count) {
    for (size_t i = 0; i < inputs.size(); ++i) {
      if (inputs[i] == data) return static_cast<int>(i);
    }
    inputs.push_back(data);
    sizes.push_back(count);
    return static_cast<int>(inputs.size() - 1);
  }
};

inline std::string glslFloat(float value) {
  char buffer[64];
  std::snprintf(buffer, sizeof(buffer), "%.9g", static_cast<double>(value));
  std::string text(buffer);
  // GLSL floating literals must contain a '.' or an exponent.
  if (text.find_first_of(".eE") == std::string::npos) text += ".0";
  return text;
}

// ---------------------------------------------------------------------------
// Emitter. The primary template handles leaves (concrete arrays); the two
// specializations handle expression nodes.
// ---------------------------------------------------------------------------
template <class T>
struct GlslEmitter {
  static void emit(const T& array, GlslProgram& program) {
    const int index =
        program.input(array.realDataPtr(), static_cast<size_t>(array.size()));
    program.body += "in" + std::to_string(index) + "[gid]";
  }
};

template <class L, class OP, class R>
struct GlslEmitter<ArrayExpression<L, OP, R>> {
  static void emit(const ArrayExpression<L, OP, R>& expression,
                   GlslProgram& program) {
    program.body += "(";
    GlslEmitter<L>::emit(expression.left_, program);
    program.body += GlslOperator<OP>::symbol();
    GlslEmitter<R>::emit(expression.right_, program);
    program.body += ")";
  }
};

template <class A>
struct GlslEmitter<ScaledArray<A>> {
  static void emit(const ScaledArray<A>& expression, GlslProgram& program) {
    program.body += "(" + glslFloat(expression.coef_) + "*";
    GlslEmitter<A>::emit(expression.array_, program);
    program.body += ")";
  }
};

// ---------------------------------------------------------------------------
// Shader template: binding i is input i, the last binding is the output.
// ---------------------------------------------------------------------------
inline std::string buildGlslSource(const GlslProgram& program) {
  std::string source;
  source += "#version 450\n";
  source += "layout(local_size_x = 256, local_size_y = 1, local_size_z = 1) in;\n";
  for (size_t i = 0; i < program.inputs.size(); ++i) {
    source += "layout(std430, binding = " + std::to_string(i) +
              ") readonly buffer In" + std::to_string(i) + " { float in" +
              std::to_string(i) + "[]; };\n";
  }
  source += "layout(std430, binding = " + std::to_string(program.inputs.size()) +
            ") writeonly buffer Out { float outv[]; };\n";
  source += "layout(push_constant) uniform Params { uint n; } params;\n";
  source += "void main() {\n";
  source += "  uint gid = gl_GlobalInvocationID.x;\n";
  source += "  if (gid >= params.n) return;\n";
  source += "  outv[gid] = " + program.body + ";\n";
  source += "}\n";
  return source;
}

} // namespace detail
}} // namespace Legolas::Vulkan
