#pragma once

#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <cmath>

namespace Legolas {

#if defined(__GNUC__) || defined(__clang__)
#define LEGOLAS_HAS_VECTOR_EXTENSIONS 1
#else
#define LEGOLAS_HAS_VECTOR_EXTENSIONS 0
#endif

template <typename T, int P>
class alignas(sizeof(T) * P > 64 ? 64 : sizeof(T) * P) NativeSimd {
public:
  using Scalar = T;
  static constexpr int PackSize = P;

#if LEGOLAS_HAS_VECTOR_EXTENSIONS
  using VecType __attribute__((vector_size(sizeof(T) * P))) = T;
  VecType data_;
#else
  T data_[P];
#endif

  // Constructors
  inline NativeSimd() = default;

  inline NativeSimd(T scalar) {
#if LEGOLAS_HAS_VECTOR_EXTENSIONS
    // Vector broadcast
    for (int i = 0; i < P; ++i) {
      data_[i] = scalar;
    }
#else
    for (int i = 0; i < P; ++i) {
      data_[i] = scalar;
    }
#endif
  }

#if LEGOLAS_HAS_VECTOR_EXTENSIONS
  inline NativeSimd(VecType vec) : data_(vec) {}
#endif

  // Element access
  inline const T& operator[](size_t i) const { return data_[i]; }
  inline T& operator[](size_t i) { return data_[i]; }

  static constexpr size_t size() { return P; }

  // Static builders (Eigen compatibility)
  static inline NativeSimd Constant(T val) {
    return NativeSimd(val);
  }

  static inline NativeSimd Zero() {
    return NativeSimd(T(0));
  }

  // Compound assignments
  inline NativeSimd& operator+=(const NativeSimd& o) {
#if LEGOLAS_HAS_VECTOR_EXTENSIONS
    data_ += o.data_;
#else
    for (int i = 0; i < P; ++i) data_[i] += o.data_[i];
#endif
    return *this;
  }

  inline NativeSimd& operator-=(const NativeSimd& o) {
#if LEGOLAS_HAS_VECTOR_EXTENSIONS
    data_ -= o.data_;
#else
    for (int i = 0; i < P; ++i) data_[i] -= o.data_[i];
#endif
    return *this;
  }

  inline NativeSimd& operator*=(const NativeSimd& o) {
#if LEGOLAS_HAS_VECTOR_EXTENSIONS
    data_ *= o.data_;
#else
    for (int i = 0; i < P; ++i) data_[i] *= o.data_[i];
#endif
    return *this;
  }

  inline NativeSimd& operator/=(const NativeSimd& o) {
#if LEGOLAS_HAS_VECTOR_EXTENSIONS
    data_ /= o.data_;
#else
    for (int i = 0; i < P; ++i) data_[i] /= o.data_[i];
#endif
    return *this;
  }

  inline NativeSimd& operator+=(T s) { return *this += NativeSimd(s); }
  inline NativeSimd& operator-=(T s) { return *this -= NativeSimd(s); }
  inline NativeSimd& operator*=(T s) { return *this *= NativeSimd(s); }
  inline NativeSimd& operator/=(T s) { return *this /= NativeSimd(s); }

  // Unary minus
  inline NativeSimd operator-() const {
#if LEGOLAS_HAS_VECTOR_EXTENSIONS
    return NativeSimd(-data_);
#else
    NativeSimd res;
    for (int i = 0; i < P; ++i) res.data_[i] = -data_[i];
    return res;
#endif
  }

  // Binary operators between vectors
  inline friend NativeSimd operator+(NativeSimd a, const NativeSimd& b) { return a += b; }
  inline friend NativeSimd operator-(NativeSimd a, const NativeSimd& b) { return a -= b; }
  inline friend NativeSimd operator*(NativeSimd a, const NativeSimd& b) { return a *= b; }
  inline friend NativeSimd operator/(NativeSimd a, const NativeSimd& b) { return a /= b; }

  // Binary operators with scalar on right
  inline friend NativeSimd operator+(NativeSimd a, T b) { return a += NativeSimd(b); }
  inline friend NativeSimd operator-(NativeSimd a, T b) { return a -= NativeSimd(b); }
  inline friend NativeSimd operator*(NativeSimd a, T b) { return a *= NativeSimd(b); }
  inline friend NativeSimd operator/(NativeSimd a, T b) { return a /= NativeSimd(b); }

  // Binary operators with scalar on left
  inline friend NativeSimd operator+(T a, const NativeSimd& b) { return NativeSimd(a) + b; }
  inline friend NativeSimd operator-(T a, const NativeSimd& b) { return NativeSimd(a) - b; }
  inline friend NativeSimd operator*(T a, const NativeSimd& b) { return NativeSimd(a) * b; }
  inline friend NativeSimd operator/(T a, const NativeSimd& b) { return NativeSimd(a) / b; }

  // Mathematical methods
  inline NativeSimd inverse() const {
    return NativeSimd(T(1)) / *this;
  }

  inline T sum() const {
    T total = data_[0];
    for (int i = 1; i < P; ++i) {
      total += data_[i];
    }
    return total;
  }
};

// Specialization for P=1 (Scalar transparent wrapper)
template <typename T>
class NativeSimd<T, 1> {
public:
  using Scalar = T;
  static constexpr int PackSize = 1;
  T data_;

  inline NativeSimd() = default;
  inline NativeSimd(T scalar) : data_(scalar) {}

  inline operator T&() { return data_; }
  inline operator const T&() const { return data_; }

  inline const T& operator[](size_t) const { return data_; }
  inline T& operator[](size_t) { return data_; }

  static constexpr size_t size() { return 1; }

  static inline NativeSimd Constant(T val) { return NativeSimd(val); }
  static inline NativeSimd Zero() { return NativeSimd(T(0)); }

  inline NativeSimd& operator+=(const NativeSimd& o) { data_ += o.data_; return *this; }
  inline NativeSimd& operator-=(const NativeSimd& o) { data_ -= o.data_; return *this; }
  inline NativeSimd& operator*=(const NativeSimd& o) { data_ *= o.data_; return *this; }
  inline NativeSimd& operator/=(const NativeSimd& o) { data_ /= o.data_; return *this; }

  inline NativeSimd& operator+=(T s) { data_ += s; return *this; }
  inline NativeSimd& operator-=(T s) { data_ -= s; return *this; }
  inline NativeSimd& operator*=(T s) { data_ *= s; return *this; }
  inline NativeSimd& operator/=(T s) { data_ /= s; return *this; }

  inline NativeSimd operator-() const { return NativeSimd(-data_); }

  inline friend NativeSimd operator+(NativeSimd a, const NativeSimd& b) { return a += b; }
  inline friend NativeSimd operator-(NativeSimd a, const NativeSimd& b) { return a -= b; }
  inline friend NativeSimd operator*(NativeSimd a, const NativeSimd& b) { return a *= b; }
  inline friend NativeSimd operator/(NativeSimd a, const NativeSimd& b) { return a /= b; }

  inline friend NativeSimd operator+(NativeSimd a, T b) { return a += b; }
  inline friend NativeSimd operator-(NativeSimd a, T b) { return a -= b; }
  inline friend NativeSimd operator*(NativeSimd a, T b) { return a *= b; }
  inline friend NativeSimd operator/(NativeSimd a, T b) { return a /= b; }

  inline friend NativeSimd operator+(T a, const NativeSimd& b) { return NativeSimd(a) + b; }
  inline friend NativeSimd operator-(T a, const NativeSimd& b) { return NativeSimd(a) - b; }
  inline friend NativeSimd operator*(T a, const NativeSimd& b) { return NativeSimd(a) * b; }
  inline friend NativeSimd operator/(T a, const NativeSimd& b) { return NativeSimd(a) / b; }

  inline NativeSimd inverse() const { return NativeSimd(T(1) / data_); }
  inline T sum() const { return data_; }
};

} // namespace Legolas
