#pragma once

#include "Legolas/Vulkan/VulkanContext.hxx"

#include <cstddef>
#include <type_traits>
#include <utility>

namespace Legolas {
namespace Vulkan {

// ---------------------------------------------------------------------------
// DeviceArray<T>: RAII device-side array of `count` trivially copyable
// scalars. This is the generic bridge between the core containers (whose
// payload is a contiguous scalar buffer) and device memory: the caller owns
// residency, so repeated kernels can run without re-uploading.
// ---------------------------------------------------------------------------
template <class T>
class DeviceArray {
  static_assert(std::is_trivially_copyable<T>::value,
                "DeviceArray<T> requires a trivially copyable T");

public:
  DeviceArray() = default;

  DeviceArray(Context& ctx, size_t count)
      : count_(count), buffer_(ctx, count * sizeof(T)) {}

  DeviceArray(DeviceArray&&) noexcept = default;
  DeviceArray& operator=(DeviceArray&&) noexcept = default;

  DeviceArray(const DeviceArray&) = delete;
  DeviceArray& operator=(const DeviceArray&) = delete;

  bool valid() const { return buffer_.valid(); }
  size_t size() const { return count_; }
  Buffer& buffer() { return buffer_; }
  const Buffer& buffer() const { return buffer_; }

  bool upload(const T* data, size_t count) {
    if (!valid() || data == nullptr || count > count_) return false;
    buffer_.upload(data, count * sizeof(T));
    return true;
  }

  bool download(T* data, size_t count) const {
    if (!valid() || data == nullptr || count > count_) return false;
    buffer_.download(data, count * sizeof(T));
    return true;
  }

private:
  size_t count_ = 0;
  Buffer buffer_;
};

namespace detail {

// Core containers expose realDataPtr(); standard containers expose data().
template <class HOST>
inline auto hostConstData(const HOST& host, int) -> decltype(host.realDataPtr()) {
  return host.realDataPtr();
}

template <class HOST>
inline auto hostConstData(const HOST& host, long) -> decltype(host.data()) {
  return host.data();
}

template <class HOST>
inline auto hostConstData(const HOST& host) -> decltype(hostConstData(host, 0)) {
  return hostConstData(host, 0);
}

template <class HOST>
inline auto hostMutableData(HOST& host, int) -> decltype(host.realDataPtr()) {
  return host.realDataPtr();
}

template <class HOST>
inline auto hostMutableData(HOST& host, long) -> decltype(host.data()) {
  return host.data();
}

template <class HOST>
inline auto hostMutableData(HOST& host) -> decltype(hostMutableData(host, 0)) {
  return hostMutableData(host, 0);
}

template <class HOST>
inline size_t hostSize(const HOST& host) {
  return static_cast<size_t>(host.size());
}

template <class HOST>
struct HostScalar {
  typedef typename std::remove_cv<typename std::remove_pointer<
      decltype(hostConstData(std::declval<const HOST&>()))>::type>::type Type;
};

} // namespace detail

// Upload a host container (Legolas::Array, std::vector, ...) to the device.
template <class HOST>
inline DeviceArray<typename detail::HostScalar<HOST>::Type> toDevice(
    Context& ctx, const HOST& host) {
  typedef typename detail::HostScalar<HOST>::Type Scalar;
  DeviceArray<Scalar> device(ctx, detail::hostSize(host));
  if (device.valid()) {
    device.upload(detail::hostConstData(host), device.size());
  }
  return device;
}

// Download a device array back into a host container of matching size.
template <class HOST, class T>
inline bool toHost(const DeviceArray<T>& device, HOST& host) {
  if (!device.valid() || detail::hostSize(host) != device.size()) return false;
  return device.download(detail::hostMutableData(host), device.size());
}

}} // namespace Legolas::Vulkan
