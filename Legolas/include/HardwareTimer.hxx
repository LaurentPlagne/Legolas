#ifndef __LEGOLAS_HARDWARE_TIMER_HXX__
#define __LEGOLAS_HARDWARE_TIMER_HXX__

#include <cstdint>
#include <chrono>

#if defined(__APPLE__)
#  include <mach/mach_time.h>
#endif

#if defined(__linux__) && (defined(__i386__) || defined(__x86_64__))
#  if __has_include(<asm/msr.h>)
#    include <asm/msr.h>
#  endif
#endif

namespace Legolas {

struct HardwareTimer {
  // Read hardware cycle counter / timestamp counter
  static inline uint64_t readCycles() {
#if defined(__aarch64__) || defined(_M_ARM64)
    uint64_t val;
    asm volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
#elif defined(__x86_64__) || defined(__i386__)
    uint32_t lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return (static_cast<uint64_t>(hi) << 32) | lo;
#elif defined(__APPLE__)
    return mach_absolute_time();
#else
    return static_cast<uint64_t>(
      std::chrono::high_resolution_clock::now().time_since_epoch().count()
    );
#endif
  }

  // Frequency of the cycle counter in Hz
  static inline uint64_t readFrequency() {
#if defined(__aarch64__) || defined(_M_ARM64)
    uint64_t freq;
    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
    return freq;
#elif defined(__APPLE__)
    static mach_timebase_info_data_t info = {0, 0};
    if (info.denom == 0) {
      mach_timebase_info(&info);
    }
    // mach_absolute_time * info.numer / info.denom = nanoseconds
    // frequency = 1e9 * info.denom / info.numer
    return static_cast<uint64_t>(1000000000ULL * info.denom / info.numer);
#else
    // Fallback: 1 GHz default resolution for standard chrono
    return 1000000000ULL;
#endif
  }

  // Elapsed time in seconds between two cycle counts
  static inline double cyclesToSeconds(uint64_t start, uint64_t stop) {
    const uint64_t freq = readFrequency();
    if (freq > 0) {
      return static_cast<double>(stop - start) / static_cast<double>(freq);
    }
    return 0.0;
  }
};

} // namespace Legolas

#endif // __LEGOLAS_HARDWARE_TIMER_HXX__
