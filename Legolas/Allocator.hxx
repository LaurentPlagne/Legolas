#pragma once

#include "dkconfig.hxx"

#if ! defined (USING_TBB)
#error C macro USING_TBB is not defined
#endif

#if USING_TBB == 1

//#warning  USING_TBB est positionnée a 1
#include "tbb/cache_aligned_allocator.h"

#elif defined(USE_EIGEN)

#include <Eigen/Core>

#else

#include <cstdlib>
#include <memory>
#include <new>

#endif

namespace Legolas{

#if !defined(USE_EIGEN) && (USING_TBB == 0)
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
#endif

#if USING_TBB == 1
template <class T> using allocator=tbb::cache_aligned_allocator<T>;
#elif defined(USE_EIGEN)
template <class T> using allocator=Eigen::aligned_allocator<T>;
#else
template <class T> using allocator=AlignedAllocator<T, 64>;
#endif

}
