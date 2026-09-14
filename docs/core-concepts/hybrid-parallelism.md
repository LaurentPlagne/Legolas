# Two-Level Hybrid Parallelism

Maximizing modern CPU performance requires combining two distinct forms of parallelism:
1. **Thread-Level Parallelism**: Distributing work across multiple physical CPU cores.
2. **Data-Level Parallelism**: Utilizing wide hardware vector units (SIMD) on each core.

Legolas++ implements a decoupled, two-level hybrid execution model:

```text
┌─────────────────────────────────────────────────────────────┐
│                 Level 1: Multi-Core Scaling                 │
│   Legolas::parmap(...) via persistent StaticThreadPool      │
│   Dynamic chunks claimed from an atomic counter (~4/worker) │
│   Hybrid spin-then-sleep idle policy (no hot-path locks)    │
└──────────────────────────────┬──────────────────────────────┘
                               │ (blocked ranges)
┌──────────────────────────────▼──────────────────────────────┐
│                  Level 2: SIMD Vectorization                │
│   Legolas::Array<T, D, P, DP> (Data Layout Interleaving)    │
│   Packed views mapped directly to ARM NEON / x86 AVX2/512   │
│   Zero memory permute overhead during computation           │
└─────────────────────────────────────────────────────────────┘
```

---

## Level 1: Persistent Thread Pool with Dynamic Chunking

The multi-core engine is `Legolas::StaticThreadPool` (header [`Legolas/include/StaticScheduler.hxx`](https://github.com/LaurentPlagne/Legolas/blob/master/Legolas/include/StaticScheduler.hxx)):

### 1. Persistent Worker Pool
Rather than spawning and tearing down `std::thread` instances on every parallel loop, Legolas++ maintains a lightweight persistent thread pool sized to hardware concurrency. The calling thread participates as worker 0; the remaining `N-1` threads are background workers.

### 2. Dynamic Chunking (Lock-Free on the Hot Path)
Every parallel loop is split into chunks sized to about four chunks per worker, so asymmetries between performance and efficiency cores are absorbed by dynamic scheduling. Workers claim the next chunk by incrementing a single atomic counter — there are no per-worker queues and no locks taken while executing user code.

### 3. Hybrid Spin-Then-Sleep Waiting
An idle worker first spins (`pause`/`yield`, default 4000 iterations) and then blocks on a condition variable. Short loops therefore avoid OS scheduling latency, while long idle periods do not burn CPU cycles.

### 4. Nested Parallel Regions
A `parallel_for` issued from inside a pool worker executes inline: nesting is safe and cannot deadlock, but nested work does not add threads.

### 5. Runtime Configuration
The scheduler can be tuned without recompiling:

* `LEGOLAS_NUM_THREADS` (or standard `OMP_NUM_THREADS`): number of worker threads.
* `LEGOLAS_SPIN_COUNT`: spin iterations before a worker sleeps.
* `LEGOLAS_PARALLEL_THRESHOLD`: minimum element count for bulk operations (`fill`, assignment) to be distributed across workers.

> **Note**: an alternative deque-based work-stealing pool (LIFO local tasks, FIFO stealing) is provided in [`Legolas/include/WorkStealing.hxx`](https://github.com/LaurentPlagne/Legolas/blob/master/Legolas/include/WorkStealing.hxx) for experimentation, but `Legolas::parmap` uses the `StaticThreadPool` described above.

---

## Level 2: Fine-Grained SIMD Vectorization

Within each chunk executed by a worker thread:
- The algorithm processes packets of size $P$.
- The compiler maps all arithmetic directly to native SIMD instructions (`fdiv.4s`, `fmul.4s`, `fsub.4s`, `fmla.4s` on ARM NEON).
- No thread synchronization or locking occurs at this level.

---

## Zero Remainder Waste

What happens if the total number of problem instances $M$ is not a multiple of the pack size $P$?

Legolas++ automatically splits the loop:
1. The vectorized loop processes the bulk range $[0, \lfloor M / P \rfloor \cdot P)$ using packed SIMD vector registers.
2. The remaining scalar tail range $[\lfloor M / P \rfloor \cdot P, M)$ is executed using standard scalar math by the exact same template operator.

You never have to write a separate "scalar cleanup loop".
