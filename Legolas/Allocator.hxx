#pragma once

#if !defined(_MSC_VER)
#  ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#  endif
#  ifndef _GNU_SOURCE
#    define _GNU_SOURCE 1
#  endif
#  include <stdlib.h>
#endif

#include <cstdlib>
#include <memory>
#include <new>

namespace Legolas {

template <class T, size_t Alignment = 64>
struct AlignedAllocator {
  using value_type = T;
  AlignedAllocator() noexcept = default;
  template <class U> AlignedAllocator(const AlignedAllocator<U, Alignment>&) noexcept {}
  
  T* allocate(size_t n) {
    if (n == 0) return nullptr;
    void* ptr = nullptr;
#if defined(_MSC_VER)
    ptr = _aligned_malloc(n * sizeof(T), Alignment);
    if (!ptr) throw std::bad_alloc();
#else
    if (posix_memalign(&ptr, Alignment, n * sizeof(T)) != 0) {
      throw std::bad_alloc();
    }
#endif
    return static_cast<T*>(ptr);
  }
  
  void deallocate(T* p, size_t) noexcept {
#if defined(_MSC_VER)
    _aligned_free(p);
#else
    free(p);
#endif
  }
  
  template <class U> struct rebind { using other = AlignedAllocator<U, Alignment>; };
  bool operator==(const AlignedAllocator&) const noexcept { return true; }
  bool operator!=(const AlignedAllocator&) const noexcept { return false; }
};

template <class T> using allocator = AlignedAllocator<T, 64>;

} // namespace Legolas
