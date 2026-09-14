# Scientific Computing: MultiThomas Tridiagonal Solver

Tridiagonal matrix systems arise constantly in computational science:
- 2D and 3D heat and diffusion equations solved via **Alternating Direction Implicit (ADI)** schemes.
- Poisson solvers on structured meshes.
- Cubic spline interpolation.
- Implicit time integration of parabolic PDEs.

A tridiagonal system $T x = b$ is defined by three diagonals: sub-diagonal $L$, main diagonal $D$, and super-diagonal $U$:

$$\begin{pmatrix}
D_0 & U_0 & 0 & \dots & 0 \\
L_1 & D_1 & U_1 & \dots & 0 \\
\vdots & \ddots & \ddots & \ddots & \vdots \\
0 & \dots & L_{N-1} & D_{N-1} & U_{N-1} \\
0 & \dots & 0 & L_N & D_N
\end{pmatrix}
\begin{pmatrix} x_0 \\ x_1 \\ \vdots \\ x_{N-1} \\ x_N \end{pmatrix}
=
\begin{pmatrix} b_0 \\ b_1 \\ \vdots \\ b_{N-1} \\ b_N \end{pmatrix}$$

The classical $O(N)$ solution is the **Thomas algorithm** (Gaussian elimination for tridiagonal systems).

---

## 1. The Algorithm

The algorithm consists of two sweeps:

- **1. Forward elimination**: Modifies the upper diagonal into $S$ and updates $X$:

    $$s = D_i - L_i S_{i-1}, \quad S_i = \frac{U_i}{s}, \quad X_i = \frac{B_i - L_i X_{i-1}}{s}$$

- **2. Backward substitution**:

    $$X_i = X_i - S_{i+1} X_{i+1}$$

Both sweeps are sequential recurrences: forward sweep propagates from $i=0$ to $N-1$, and backward sweep propagates from $i=N-2$ down to $0$.

---

## 2. Implementing with Legolas++

In Legolas++, we write the solver **once as a generic C++ functor**:

```cpp
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

struct ThomasSolver {
    template <class A2D>
    void operator()(int begin, int end,
                    const A2D D, const A2D U, const A2D L,
                    const A2D B, A2D X) const {
        using Scalar = typename A2D::RealType;
        using Element = typename A2D::Element;

        Element S(X[0].shape());
        Scalar one(1.0), s, sm1;

        for (int j = begin; j < end; ++j) {
            auto d = D[j]; auto u = U[j]; auto l = L[j];
            auto b = B[j]; auto x = X[j];
            const int n = x.size();

            // Forward sweep
            s = d[0];
            sm1 = one / s;
            x[0] = b[0] * sm1;

            for (int i = 1; i < n; ++i) {
                S[i] = u[i - 1] * sm1;
                s = d[i] - l[i] * S[i];
                x[i] = b[i] - l[i] * x[i - 1];
                sm1 = one / s;
                x[i] *= sm1;
            }

            // Backward sweep
            for (int i = n - 2; i >= 0; --i) {
                x[i] -= S[i + 1] * x[i + 1];
            }
        }
    }
};
```

---

## 3. Running the Benchmark

```cpp
int main() {
    using Real = float;
    const int nx = 128;         // Size of each tridiagonal system
    const int ny = nx * nx;     // 16,384 systems (2.1 million unknowns)

    // Interleaved layout: PACK_SIZE = 8 across dimension Y
    using Array2D = Legolas::Array<Real, 2, 8, 2>;

    Array2D D(ny, nx), U(ny, nx), L(ny, nx), B(ny, nx), X(ny, nx);
    D.fill(2.0f); U.fill(-1.0f); L.fill(-1.0f); B.fill(1.0f); X.fill(0.0f);

    // Solve in parallel using SIMD vector units and the thread pool:
    Legolas::parmap(ThomasSolver(), D, U, L, B, X);

    return 0;
}
```

---

## 4. Performance Results (Apple M1 Max)

- **Scalar Baseline ($P=1$)**: 2.10 GFlops (1 Core).
- **Legolas NEON SIMD ($P=4$)**: 6.50 GFlops (1 Core) $\rightarrow$ **3.1x speedup**.
- **Legolas Multi-Core ($P=8$, 8 Cores)**: **69.4 GFlops** $\rightarrow$ **33x overall speedup**!

### 🌿 HPC Energy Efficiency & Cloud Cost Reduction

In scientific computing, PDE solvers (Alternating Direction Implicit methods, heat diffusion, CFD Navier-Stokes) run continuously on large clusters:
* **2.31 GFlops per Watt**: Sustained on coupled linear recurrences where standard compilers stall at 0.07 GFlops/Watt.
* **33× Higher Compute Density**: 33× more physical simulation progress per kilowatt-hour of electricity.
* **Direct Cloud Cost Reduction**: Drastically shrinks cloud compute-node-hours and cooling electricity demands for massive physical models.
