#ifndef __LEGOLAS_WORK_STEALING_HXX__
#define __LEGOLAS_WORK_STEALING_HXX__

#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <random>
#include <algorithm>
#include <cassert>

namespace Legolas {
namespace WorkStealing {

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

  // Splitting constructor: splits r into [r.my_begin, mid) and [mid, r.my_end)
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

class ThreadPool {
public:
  using Task = std::function<void()>;

  static ThreadPool& instance() {
    static ThreadPool pool;
    return pool;
  }

  void setNumThreads(int n) {
    if (n < 1) n = 1;
    if (n == num_threads_) return;

    shutdown();
    num_threads_ = n;
    stop_ = false;
    active_jobs_ = 0;
    queues_.resize(num_threads_);
    mutexes_ = std::vector<std::mutex>(num_threads_);

    // Worker 0 is the calling thread itself; spawn num_threads_ - 1 background workers
    for (int i = 1; i < num_threads_; ++i) {
      workers_.emplace_back(&ThreadPool::workerLoop, this, i);
    }
  }

  int getNumThreads() const {
    return num_threads_;
  }

  // Submit and execute tasks across threads with work-stealing
  void execute(const std::vector<Task>& tasks) {
    if (tasks.empty()) return;
    if (num_threads_ <= 1 || tasks.size() == 1) {
      for (const auto& task : tasks) {
        task();
      }
      return;
    }

    const size_t ntasks = tasks.size();
    active_jobs_.store(ntasks, std::memory_order_release);

    // Distribute tasks evenly among worker deques
    for (size_t i = 0; i < ntasks; ++i) {
      int queue_idx = static_cast<int>(i % num_threads_);
      {
        std::lock_guard<std::mutex> lock(mutexes_[queue_idx]);
        queues_[queue_idx].push_back(tasks[i]);
      }
    }

    // Wake up sleeping workers
    cv_.notify_all();

    // Calling thread acts as worker 0
    processWork(0);

    // Wait until all tasks are complete
    std::unique_lock<std::mutex> lock(done_mutex_);
    done_cv_.wait(lock, [this] {
      return active_jobs_.load(std::memory_order_acquire) == 0;
    });
  }

private:
  ThreadPool() : num_threads_(0), stop_(false), active_jobs_(0) {
    int default_n = static_cast<int>(std::thread::hardware_concurrency());
    if (default_n < 1) default_n = 1;
    setNumThreads(default_n);
  }

  ~ThreadPool() {
    shutdown();
  }

  void shutdown() {
    stop_ = true;
    cv_.notify_all();
    for (auto& w : workers_) {
      if (w.joinable()) {
        w.join();
      }
    }
    workers_.clear();
  }

  void workerLoop(int thread_id) {
    while (!stop_) {
      processWork(thread_id);

      std::unique_lock<std::mutex> lock(cv_mutex_);
      if (!stop_ && active_jobs_.load(std::memory_order_acquire) == 0) {
        cv_.wait(lock, [this] {
          return stop_ || active_jobs_.load(std::memory_order_acquire) > 0;
        });
      }
    }
  }

  void processWork(int thread_id) {
    std::mt19937 rng(static_cast<unsigned>(thread_id * 104729 + 17));

    while (active_jobs_.load(std::memory_order_acquire) > 0) {
      Task task;
      bool found = false;

      // 1. Try local deque (LIFO for cache locality)
      {
        std::lock_guard<std::mutex> lock(mutexes_[thread_id]);
        if (!queues_[thread_id].empty()) {
          task = std::move(queues_[thread_id].back());
          queues_[thread_id].pop_back();
          found = true;
        }
      }

      // 2. If no local work, steal from a random victim (FIFO from victim's front)
      if (!found) {
        std::uniform_int_distribution<int> dist(0, num_threads_ - 1);
        for (int attempts = 0; attempts < num_threads_; ++attempts) {
          int victim = dist(rng);
          if (victim == thread_id) continue;

          std::unique_lock<std::mutex> lock(mutexes_[victim], std::try_to_lock);
          if (lock.owns_lock() && !queues_[victim].empty()) {
            task = std::move(queues_[victim].front());
            queues_[victim].pop_front();
            found = true;
            break;
          }
        }
      }

      // 3. Execute stolen or local task
      if (found) {
        task();
        if (active_jobs_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
          std::lock_guard<std::mutex> dlock(done_mutex_);
          done_cv_.notify_all();
        }
      } else {
        // Yield briefly if currently no work found to avoid heavy bus contention
        std::this_thread::yield();
      }
    }
  }

  int num_threads_;
  std::atomic<bool> stop_;
  std::atomic<size_t> active_jobs_;

  std::vector<std::thread> workers_;
  std::vector<std::deque<Task>> queues_;
  std::vector<std::mutex> mutexes_;

  std::mutex cv_mutex_;
  std::condition_variable cv_;

  std::mutex done_mutex_;
  std::condition_variable done_cv_;
};

// task_scheduler_init compatible class
struct task_scheduler_init {
  int nthreads_;

  task_scheduler_init(int nthreads) : nthreads_(nthreads) {
    ThreadPool::instance().setNumThreads(nthreads);
  }

  static int default_num_threads() {
    int n = static_cast<int>(std::thread::hardware_concurrency());
    return n > 0 ? n : 1;
  }

  inline void terminate() const {}
};

// parallel_for over a blocked_range using work-stealing
template <typename Range, typename Body, typename Partitioner = auto_partitioner>
inline void parallel_for(const Range& range, const Body& body, Partitioner = Partitioner()) {
  if (range.empty()) return;

  auto& pool = ThreadPool::instance();
  const int nthreads = pool.getNumThreads();

  if (nthreads <= 1) {
    body(range);
    return;
  }

  // Recursive range subdivision into subtasks
  // Generate approximately 2 to 4 subranges per thread for dynamic load balancing
  size_t target_chunks = static_cast<size_t>(nthreads * 4);
  size_t range_size = range.size();
  size_t grain = range.grainsize();
  size_t chunk_size = std::max(grain, (range_size + target_chunks - 1) / target_chunks);

  std::vector<ThreadPool::Task> tasks;
  auto current_begin = range.begin();
  const auto total_end = range.end();

  while (current_begin < total_end) {
    auto current_end = current_begin + static_cast<decltype(current_begin)>(
      std::min(chunk_size, static_cast<size_t>(total_end - current_begin))
    );
    Range sub_range(current_begin, current_end, grain);
    tasks.push_back([body, sub_range]() {
      body(sub_range);
    });
    current_begin = current_end;
  }

  pool.execute(tasks);
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

} // namespace WorkStealing
} // namespace Legolas

#endif // __LEGOLAS_WORK_STEALING_HXX__
