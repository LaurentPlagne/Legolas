#ifndef __LEGOLAS_STATIC_SCHEDULER_HXX__
#define __LEGOLAS_STATIC_SCHEDULER_HXX__

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <chrono>

#if defined(__x86_64__) || defined(_M_X64)
#include <immintrin.h>
#define LEGOLAS_CPU_PAUSE() _mm_pause()
#elif defined(__aarch64__)
#define LEGOLAS_CPU_PAUSE() asm volatile("yield")
#else
#define LEGOLAS_CPU_PAUSE() std::this_thread::yield()
#endif

namespace Legolas {
namespace StaticScheduler {

struct split {};

template <typename Value>
class blocked_range {
public:
  using size_type = std::size_t;

  blocked_range(Value begin, Value end, size_type grainsize = 1)
      : my_begin(begin), my_end(end), my_grainsize(grainsize) {}

  inline Value begin() const { return my_begin; }
  inline Value end() const { return my_end; }
  inline size_type size() const { return static_cast<size_type>(my_end > my_begin ? my_end - my_begin : 0); }
  inline size_type grainsize() const { return my_grainsize; }
  inline bool empty() const { return my_end <= my_begin; }
  inline bool is_divisible() const { return size() > my_grainsize; }

  // Splitting constructor
  blocked_range(blocked_range& r, split)
      : my_end(r.my_end), my_grainsize(r.my_grainsize) {
    Value mid = r.my_begin + static_cast<Value>((r.size() + 1u) / 2u);
    my_begin = mid;
    r.my_end = mid;
  }

private:
  Value my_begin;
  Value my_end;
  size_type my_grainsize;
};

struct auto_partitioner {};
struct simple_partitioner {};
struct static_partitioner {};

// Type-erased work interface (zero heap allocation)
struct IWork {
  virtual void execute(int thread_id) = 0;
};

template <typename Range, typename Body>
struct DynamicChunkWork : public IWork {
  const Range& range_;
  const Body& body_;
  std::atomic<size_t> next_index_{0};
  size_t chunk_size_{1};

  DynamicChunkWork(const Range& r, const Body& b, int nthreads)
      : range_(r), body_(b), next_index_(0) {
    const size_t total = range_.size();
    // 4 chunks per thread perfectly balances asymmetric P-cores and E-cores
    const size_t target_chunks = static_cast<size_t>(nthreads * 4);
    const size_t grain = range_.grainsize();
    chunk_size_ = std::max(grain, (total + target_chunks - 1) / target_chunks);
  }

  void execute(int /*thread_id*/) override {
    const size_t total = range_.size();
    const size_t grain = range_.grainsize();
    const auto b = range_.begin();

    while (true) {
      size_t start = next_index_.fetch_add(chunk_size_, std::memory_order_relaxed);
      if (start >= total) break;

      size_t count = std::min(chunk_size_, total - start);
      auto my_begin = b + static_cast<decltype(b)>(start);
      auto my_end = my_begin + static_cast<decltype(b)>(count);
      Range sub(my_begin, my_end, grain);
      body_(sub);
    }
  }
};

class StaticThreadPool {
public:
  static StaticThreadPool& instance() {
    static StaticThreadPool pool;
    return pool;
  }

  void setNumThreads(int n) {
    if (n < 1) n = 1;
    if (n == num_threads_) return;

    shutdown();

    num_threads_ = n;
    stop_.store(false, std::memory_order_relaxed);
    generation_.store(0, std::memory_order_relaxed);
    remaining_workers_.store(0, std::memory_order_relaxed);
    sleeping_workers_.store(0, std::memory_order_relaxed);
    current_work_.store(nullptr, std::memory_order_relaxed);

    // Worker 0 is the calling thread; spawn num_threads_ - 1 background workers
    workers_.reserve(num_threads_ - 1);
    for (int i = 1; i < num_threads_; ++i) {
      workers_.emplace_back(&StaticThreadPool::workerLoop, this, i);
    }
  }

  int getNumThreads() const {
    return num_threads_;
  }

  void dispatch(IWork* work) {
    if (num_threads_ <= 1) {
      work->execute(0);
      return;
    }

    const int bg_workers = num_threads_ - 1;
    remaining_workers_.store(bg_workers, std::memory_order_relaxed);
    current_work_.store(work, std::memory_order_release);

    // Increment generation (lock-free)
    generation_.fetch_add(1, std::memory_order_release);

    // Wake workers if any are sleeping
    if (sleeping_workers_.load(std::memory_order_acquire) > 0) {
      std::lock_guard<std::mutex> lock(cv_mutex_);
      cv_.notify_all();
    }

    // Calling thread acts as worker 0
    work->execute(0);

    // Wait for all background workers: hybrid spin-then-sleep
    int spins = 0;
    while (remaining_workers_.load(std::memory_order_acquire) > 0) {
      if (++spins < 4000) {
        LEGOLAS_CPU_PAUSE();
      } else {
        std::unique_lock<std::mutex> lock(done_mutex_);
        done_cv_.wait_for(lock, std::chrono::microseconds(50), [this] {
          return remaining_workers_.load(std::memory_order_acquire) == 0;
        });
      }
    }
  }

private:
  StaticThreadPool()
      : num_threads_(0), stop_(false), generation_(0),
        remaining_workers_(0), sleeping_workers_(0), current_work_(nullptr) {
    int default_n = static_cast<int>(std::thread::hardware_concurrency());
    if (default_n < 1) default_n = 1;
    setNumThreads(default_n);
  }

  ~StaticThreadPool() {
    shutdown();
  }

  void shutdown() {
    stop_.store(true, std::memory_order_relaxed);
    generation_.fetch_add(1, std::memory_order_release);
    {
      std::lock_guard<std::mutex> lock(cv_mutex_);
      cv_.notify_all();
    }

    for (auto& w : workers_) {
      if (w.joinable()) {
        w.join();
      }
    }
    workers_.clear();
  }

  void workerLoop(int thread_id) {
    uint64_t local_gen = 0;

    while (!stop_.load(std::memory_order_relaxed)) {
      // 1. Fast spin-wait for new work
      int spins = 0;
      bool got_work = false;
      while (spins < 4000) {
        uint64_t gen = generation_.load(std::memory_order_acquire);
        if (gen > local_gen) {
          local_gen = gen;
          got_work = true;
          break;
        }
        if (stop_.load(std::memory_order_relaxed)) return;
        LEGOLAS_CPU_PAUSE();
        ++spins;
      }

      // 2. Sleep on CV if no work arrived during spin window
      if (!got_work) {
        std::unique_lock<std::mutex> lock(cv_mutex_);
        sleeping_workers_.fetch_add(1, std::memory_order_relaxed);
        cv_.wait(lock, [this, &local_gen] {
          return stop_.load(std::memory_order_relaxed) ||
                 generation_.load(std::memory_order_acquire) > local_gen;
        });
        sleeping_workers_.fetch_sub(1, std::memory_order_relaxed);

        if (stop_.load(std::memory_order_relaxed)) break;
        local_gen = generation_.load(std::memory_order_acquire);
      }

      // 3. Execute work
      IWork* work = current_work_.load(std::memory_order_acquire);
      if (work) {
        work->execute(thread_id);
      }

      // 4. Signal completion
      if (remaining_workers_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        std::lock_guard<std::mutex> lock(done_mutex_);
        done_cv_.notify_one();
      }
    }
  }

  int num_threads_;
  std::vector<std::thread> workers_;
  std::atomic<bool> stop_;
  std::atomic<uint64_t> generation_;
  std::atomic<int> remaining_workers_;
  std::atomic<int> sleeping_workers_;
  std::atomic<IWork*> current_work_;

  std::mutex cv_mutex_;
  std::condition_variable cv_;

  std::mutex done_mutex_;
  std::condition_variable done_cv_;
};

// task_scheduler_init compatible class
struct task_scheduler_init {
  int nthreads_;

  task_scheduler_init(int nthreads) : nthreads_(nthreads) {
    StaticThreadPool::instance().setNumThreads(nthreads);
  }

  static int default_num_threads() {
    int n = static_cast<int>(std::thread::hardware_concurrency());
    return n > 0 ? n : 1;
  }

  inline void terminate() const {}
};

// parallel_for over a blocked_range using dynamic chunking
template <typename Range, typename Body, typename Partitioner = auto_partitioner>
inline void parallel_for(const Range& range, const Body& body, Partitioner = Partitioner()) {
  if (range.empty()) return;

  auto& pool = StaticThreadPool::instance();
  const int nthreads = pool.getNumThreads();

  if (nthreads <= 1 || range.size() <= 1) {
    body(range);
    return;
  }

  // Stack-allocated work functor: zero heap allocation
  DynamicChunkWork<Range, Body> work(range, body, nthreads);
  pool.dispatch(&work);
}

// parallel_for over an index interval [first, last)
template <typename Index, typename Function>
inline void parallel_for(Index first, Index last, const Function& f) {
  parallel_for(blocked_range<Index>(first, last), [&f](const blocked_range<Index>& r) {
    for (Index i = r.begin(); i < r.end(); ++i) {
      f(i);
    }
  });
}

} // namespace StaticScheduler
} // namespace Legolas

#endif // __LEGOLAS_STATIC_SCHEDULER_HXX__
