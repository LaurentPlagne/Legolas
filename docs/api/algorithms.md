# API Reference: Parallel Algorithms (`map` & `parmap`)

Header: `#include "Legolas/Array/Map.hxx"`

Legolas++ provides higher-order algorithm dispatchers that automatically handle SIMD vectorization unpacking and multi-threaded work distribution.

---

## `Legolas::map` (Sequential SIMD)

Applies an algorithm functor across one or more arrays sequentially on a single thread, while automatically vectorizing operations over packed elements ($P > 1$).

```cpp
template <class ALGO, typename... ARRAYS>
void map(ALGO algo, ARRAYS... rest);
```

### Example
```cpp
struct InvertElements {
    template <class A1D>
    void operator()(int begin, int end, A1D in, A1D out) const {
        using Scalar = typename A1D::RealType;
        Scalar one(1.0f);
        for (int i = begin; i < end; ++i) {
            out[i] = one / in[i];
        }
    }
};

Legolas::Array<float, 1, 4> In(1024), Out(1024);
In.fill(2.0f);

Legolas::map(InvertElements(), In, Out);
```

---

## `Legolas::parmap` (Multi-Core + SIMD)

Distributes problem instance ranges across worker threads using the native **Work-Stealing engine** (or Intel oneTBB), while executing packed SIMD vector instructions inside each chunk.

```cpp
template <class ALGO, typename... ARRAYS>
void parmap(ALGO algo, ARRAYS... rest);
```

### Functor Contract
Your algorithm functor must provide an `operator()` with the signature:

```cpp
struct MyAlgorithm {
    template <class InArray, class OutArray>
    void operator()(int begin, int end, InArray in, OutArray out) const;
};
```

- `begin`, `end`: The range of outer problem instances assigned to this thread.
- `in`, `out`: The array views (either packed views of type `Eigen::Array<T, P, 1>` or scalar views for the remainder loop).
- The functor must be `const` and thread-safe (instances in $[begin, end)$ must be independent).

---

## Thread Pool Configuration

When using the native Work-Stealing scheduler:

### Automatic Sizing
By default, Legolas++ sizes its worker thread pool to match `std::thread::hardware_concurrency()` (all available physical performance cores).

### Environment Variable
You can override the number of worker threads without recompiling by setting the `SPN_THREAD_NUMBER` environment variable:

```bash
# Force Legolas++ to use exactly 4 threads:
export SPN_THREAD_NUMBER=4
./my_program
```

### Programmatic Sizing
You can also configure the number of threads programmatically:

```cpp
#include "Legolas/include/my_tbb_parallel_for.hxx"

// Set number of worker threads:
my_tbb::task_scheduler_init init(4);
```
