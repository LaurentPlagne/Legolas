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

Distributes problem instance ranges across worker threads using the native **`StaticThreadPool` engine**, while executing packed SIMD vector instructions inside each chunk.

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
- `in`, `out`: The array views (either packed views of type `Legolas::NativeSimd<T, P>` or scalar views for the remainder loop).
- The functor must be `const` and thread-safe (instances in $[begin, end)$ must be independent).

---

## Thread Pool Configuration

When using the native `StaticThreadPool` scheduler:

### Automatic Sizing
By default, Legolas++ sizes its worker thread pool to match `std::thread::hardware_concurrency()` (all available physical performance cores).

### Environment Variable
You can override the number of worker threads without recompiling by setting the `LEGOLAS_NUM_THREADS` (or standard `OMP_NUM_THREADS`) environment variable:

```bash
# Force Legolas++ to use exactly 4 threads:
export LEGOLAS_NUM_THREADS=4
./my_program
```

Additional tuning knobs:
- `LEGOLAS_SPIN_COUNT`: spin iterations before an idle worker sleeps (default `4000`).
- `LEGOLAS_PARALLEL_THRESHOLD`: minimum element count for bulk operations (`fill`, assignment) to be distributed across workers (default `32768`).

### Programmatic Sizing
You can also configure the number of threads programmatically:

```cpp
#include "Legolas/include/Parallel.hxx"

// Set number of worker threads (Legolas:: or LGS::):
LGS::task_scheduler_init init(4);
```
