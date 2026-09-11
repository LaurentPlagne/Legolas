# Two-Level Hybrid Parallelism

Maximizing modern CPU performance requires combining two distinct forms of parallelism:
1. **Thread-Level Parallelism**: Distributing work across multiple physical CPU cores.
2. **Data-Level Parallelism**: Utilizing wide hardware vector units (SIMD) on each core.

Legolas++ implements a decoupled, two-level hybrid execution model:

```text
┌─────────────────────────────────────────────────────────────┐
│                 Level 1: Multi-Core Scaling                 │
│   Legolas::parmap(...) via Native Work-Stealing Loop Engine │
│   Per-worker deque: LIFO local tasks (L1/L2 cache affinity) │
│   FIFO work-stealing when idle (lock-minimized atomics)     │
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

## Level 1: Native Work-Stealing Loop Scheduler

Legolas++ includes a standalone, header-only work-stealing loop parallelism engine ([`Legolas/include/WorkStealing.hxx`](file:///Users/laurentplagne/Projects/Legolas/Legolas/include/WorkStealing.hxx)):

### 1. Persistent Worker Pool
Rather than spawning and tearing down `std::thread` instances on every parallel loop, Legolas++ maintains a lightweight persistent thread pool sized to hardware concurrency.

### 2. Double-Ended Work Queues (Deques)
Each worker thread maintains its own deque of range chunks `[begin, end)`.

### 3. LIFO Local Processing (Cache Locality)
A worker thread pushes and pops sub-tasks from the **tail** of its own deque in Last-In, First-Out (LIFO) order.
- This creates depth-first execution.
- It maximizes data reuse in L1 and L2 CPU caches, because freshly subdivided data is executed immediately while still warm in cache.

### 4. FIFO Work-Stealing (Load Balancing)
When a worker runs out of local work, it becomes a *thief*:
- It randomly chooses another worker (*victim*).
- It attempts to steal from the **head** of the victim's deque in First-In, First-Out (FIFO) order.
- Stealing from the head grabs the *largest available chunk of work*, minimizing scheduling overhead and lock contention.

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
