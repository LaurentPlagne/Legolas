/**
 * OptionPricing.cxx - Real-Time Portfolio Option Pricing & Greeks via Black-Scholes PDE
 *
 * =========================================================================================
 * FINANCIAL DERIVATIVES & QUANTITATIVE RISK CHALLENGE:
 * =========================================================================================
 * Investment banks, market-making desks, and hedge funds (e.g. Goldman Sachs, Citadel,
 * BNP Paribas) revalue portfolios containing tens of thousands of options contracts in
 * real-time for:
 *   1. Value-at-Risk (VaR) & Historical Simulation
 *   2. Regulatory Credit / Debit Valuation Adjustments (XVA, CVA, FVA)
 *   3. Real-time Greeks hedging (Delta, Gamma, Vega) across volatility surfaces
 *
 * The mathematical foundation is the 1D Black-Scholes Partial Differential Equation (PDE):
 *
 *      dV/dt + 0.5 * sigma^2 * S^2 * (d^2V / dS^2) + r * S * (dV / dS) - r * V = 0
 *
 * Discretizing backward in time (tau = T - t) with the unconditionally stable Crank-Nicolson
 * finite difference scheme on a spatial grid S_i = i * dS (i = 0 .. N-1) yields a tridiagonal
 * linear system at every time step m -> m+1:
 *
 *      L_i * V_{i-1}^{m+1} + D_i * V_i^{m+1} + U_i * V_{i+1}^{m+1} = RHS_i^m
 *
 * =========================================================================================
 * WHY TRADITIONAL QUANT ENGINES (QuantLib, etc.) FAIL TO VECTORIZE:
 * =========================================================================================
 * Each tridiagonal system is solved via the classical O(N) Thomas algorithm (Gaussian
 * elimination for tridiagonal matrices). The forward elimination sweep:
 *
 *      X_i = (RHS_i - L_i * X_{i-1}) / s_i
 *
 * has an unbreakable recursive data dependency across space: X_i requires X_{i-1}.
 * Standard C++ compilers (GCC, Clang, MSVC) are mathematically incapable of auto-vectorizing
 * this spatial loop.
 *
 * Consequently, production quant engines process options one contract at a time using 64-bit
 * scalar instructions. On modern hardware with ARM NEON (128-bit) or x86 AVX-512 (512-bit),
 * 75% to 87.5% of CPU vector compute units sit completely IDLE.
 *
 * =========================================================================================
 * THE LEGOLAS++ DLI INNOVATION:
 * =========================================================================================
 * Legolas++ overcomes this barrier through Data Layout Interleaving (DLI):
 * Rather than attempting to vectorize along the spatial dimension S (blocked by recurrence),
 * Legolas++ interleaves P independent option contracts directly into SIMD vector registers:
 *
 *      Legolas::Array<float, 2, PACK_SIZE, 2> V(NUM_OPTIONS, GRID_POINTS);
 *
 * 1. The quant writes the natural scalar Crank-Nicolson + Thomas algorithm ONCE.
 * 2. Legolas++ compiles the generic scalar functor into hardware SIMD registers (NativeSimd<float, P>).
 * 3. 4 options (NEON / AVX2) or 8/16 options (AVX-512) are solved simultaneously in 1 clock cycle.
 * 4. Legolas::parmap transparently distributes contract batches across all physical CPU cores
 *    using its zero-allocation, lock-minimized work-stealing scheduler.
 * =========================================================================================
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <cassert>

#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

// =========================================================================================
// Generic Crank-Nicolson Black-Scholes PDE Solver
// =========================================================================================
// Written in 100% natural scalar C++.
// When instantiated with PACK_SIZE=1: executes as scalar float.
// When instantiated with PACK_SIZE=4/8: automatically executes as hardware SIMD vector registers!
// =========================================================================================
struct BlackScholesSolver {
    int N;   // Number of spatial grid points
    int Nt;  // Number of time steps

    BlackScholesSolver(int n, int nt) : N(n), Nt(nt) {}

    template <class A2D>
    void operator()(int begin, int end,
                    A2D V2D, A2D D2D, A2D U2D, A2D L2D, A2D RHS2D) const {
        typedef typename A2D::Element Element;
        typedef typename A2D::RealType Scalar;

        // Temporary diagonal buffer for the Thomas forward sweep
        Element S_diag(V2D[0].shape());
        Scalar one(1.0f);
        Scalar two(2.0f);
        Scalar s, sm1;

        for (int opt = begin; opt < end; ++opt) {
            auto V = V2D[opt];
            auto D = D2D[opt];
            auto U = U2D[opt];
            auto L = L2D[opt];
            auto RHS = RHS2D[opt];

            // March backward in time from maturity T to present t=0
            for (int t = 0; t < Nt; ++t) {
                // -------------------------------------------------------------
                // 1. Explicit Crank-Nicolson step: RHS = B * V^m
                // In Crank-Nicolson:
                //   A = I - 0.5 * dt * M   (Implicit matrix: sub=L, diag=D, sup=U)
                //   B = I + 0.5 * dt * M   (Explicit matrix)
                // Due to symmetry:
                //   B_sub  = -L_i
                //   B_diag = 2.0 - D_i
                //   B_sup  = -U_i
                // -------------------------------------------------------------
                for (int i = 1; i < N - 1; ++i) {
                    RHS[i] = (-L[i]) * V[i - 1] + (two - D[i]) * V[i] + (-U[i]) * V[i + 1];
                }
                // Boundary conditions at S = 0 and S = S_max
                RHS[0] = V[0];
                RHS[N - 1] = V[N - 1];

                // -------------------------------------------------------------
                // 2. Implicit Crank-Nicolson step: Solve A * V^{m+1} = RHS
                // Solved via the Thomas algorithm (tridiagonal Gaussian elimination).
                // Vectorized across P options simultaneously via Legolas DLI!
                // -------------------------------------------------------------
                // Forward elimination sweep:
                s = D[0];
                sm1 = one / s;
                V[0] = RHS[0] * sm1;

                for (int i = 1; i < N; ++i) {
                    S_diag[i] = U[i - 1] * sm1;
                    s = D[i] - L[i] * S_diag[i];
                    V[i] = RHS[i] - L[i] * V[i - 1];
                    sm1 = one / s;
                    V[i] *= sm1;
                }

                // Backward substitution sweep:
                for (int i = N - 2; i >= 0; --i) {
                    V[i] -= S_diag[i + 1] * V[i + 1];
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    std::cout << "================================================================================" << std::endl;
    std::cout << "  Legolas++ Quantitative Finance: Real-Time Portfolio Option Pricing Engine    " << std::endl;
    std::cout << "  Black-Scholes PDE Finite Difference (Crank-Nicolson) & Greek Sensitivities    " << std::endl;
    std::cout << "================================================================================" << std::endl;

    // -------------------------------------------------------------------------
    // Portfolio Configuration
    // -------------------------------------------------------------------------
    const int NUM_OPTIONS = 16384; // 16,384 distinct options in the trading book
    const int N = 128;             // Spatial grid resolution (asset price points)
    const int Nt = 50;             // Time steps (backward integration)
    const float S_max = 300.0f;    // Maximum asset price on the grid
    const float dS = S_max / float(N - 1);
    const float T = 1.0f;          // 1-year maturity
    const float dt = T / float(Nt);
    const float r = 0.05f;         // 5% risk-free interest rate
    const float S_spot = 100.0f;   // Current underlying asset price (ATM spot)
    const int spot_idx = int(std::round(S_spot / dS));

    const int num_runs = 5;

    std::cout << "\n[Portfolio Setup]" << std::endl;
    std::cout << "  Contracts in Portfolio:  " << NUM_OPTIONS << " options" << std::endl;
    std::cout << "  Spatial Grid Points (N): " << N << " (dS = $" << std::fixed << std::setprecision(2) << dS << ")" << std::endl;
    std::cout << "  Time Steps (Nt):         " << Nt << " (dt = " << dt << " years)" << std::endl;
    std::cout << "  Underlying Spot Price:   $" << S_spot << " (Grid node index: " << spot_idx << ")" << std::endl;
    std::cout << "  Risk-Free Rate (r):      " << (r * 100.0f) << "%" << std::endl;
    std::cout << "  Tridiagonal Solves:      " << (long(NUM_OPTIONS) * Nt) << " systems ("
              << (long(NUM_OPTIONS) * Nt * N / 1000000) << " million linear equations solved)" << std::endl;

    // -------------------------------------------------------------------------
    // Helper: Initialize tridiagonal coefficients and initial payoffs for option m
    // -------------------------------------------------------------------------
    auto init_contract_data = [&](int m, float& K_out, float& sigma_out,
                                  std::vector<float>& init_v,
                                  std::vector<float>& d_coef,
                                  std::vector<float>& u_coef,
                                  std::vector<float>& l_coef) {
        // Diversified portfolio parameters
        K_out = 80.0f + 40.0f * (float(m) / float(NUM_OPTIONS)); // Strikes from $80 to $120
        sigma_out = 0.15f + 0.30f * (float(m % 256) / 256.0f);   // Volatility from 15% to 45%

        init_v.resize(N);
        d_coef.resize(N);
        u_coef.resize(N);
        l_coef.resize(N);

        // Initial condition: European Call payoff V(S, 0) = max(S - K, 0)
        for (int i = 0; i < N; ++i) {
            float S_i = float(i) * dS;
            init_v[i] = std::max(S_i - K_out, 0.0f);
        }

        // Interior tridiagonal matrix coefficients
        const float sig2 = sigma_out * sigma_out;
        for (int i = 1; i < N - 1; ++i) {
            float i_flt = float(i);
            float alpha_i = 0.5f * sig2 * i_flt * i_flt - 0.5f * r * i_flt;
            float beta_i  = -(sig2 * i_flt * i_flt + r);
            float gamma_i = 0.5f * sig2 * i_flt * i_flt + 0.5f * r * i_flt;

            l_coef[i] = -0.5f * dt * alpha_i;
            d_coef[i] = 1.0f - 0.5f * dt * beta_i;
            u_coef[i] = -0.5f * dt * gamma_i;
        }

        // Boundary conditions at i = 0 (S = 0) and i = N - 1 (S = S_max)
        d_coef[0] = 1.0f; u_coef[0] = 0.0f; l_coef[0] = 0.0f;
        d_coef[N - 1] = 1.0f; u_coef[N - 1] = 0.0f; l_coef[N - 1] = 0.0f;
    };

    BlackScholesSolver pricer(N, Nt);

    // -------------------------------------------------------------------------
    // 1. Scalar Baseline (PACK_SIZE = 1)
    // Standard industry implementation: One option at a time, scalar loops.
    // -------------------------------------------------------------------------
    using ScalarArray = Legolas::Array<float, 2>;
    ScalarArray V_scalar(NUM_OPTIONS, N);
    ScalarArray D_scalar(NUM_OPTIONS, N);
    ScalarArray U_scalar(NUM_OPTIONS, N);
    ScalarArray L_scalar(NUM_OPTIONS, N);
    ScalarArray RHS_scalar(NUM_OPTIONS, N);

    std::vector<float> tmp_v, tmp_d, tmp_u, tmp_l;
    float dummy_k, dummy_sig;
    for (int m = 0; m < NUM_OPTIONS; ++m) {
        init_contract_data(m, dummy_k, dummy_sig, tmp_v, tmp_d, tmp_u, tmp_l);
        for (int i = 0; i < N; ++i) {
            V_scalar[m][i] = tmp_v[i];
            D_scalar[m][i] = tmp_d[i];
            U_scalar[m][i] = tmp_u[i];
            L_scalar[m][i] = tmp_l[i];
        }
    }

    // Warmup
    Legolas::map(pricer, V_scalar, D_scalar, U_scalar, L_scalar, RHS_scalar);

    // Benchmark Scalar
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int run = 0; run < num_runs; ++run) {
        // Reset initial payoffs
        for (int m = 0; m < NUM_OPTIONS; ++m) {
            float K_val = 80.0f + 40.0f * (float(m) / float(NUM_OPTIONS));
            for (int i = 0; i < N; ++i) {
                V_scalar[m][i] = std::max(float(i) * dS - K_val, 0.0f);
            }
        }
        Legolas::map(pricer, V_scalar, D_scalar, U_scalar, L_scalar, RHS_scalar);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double time_scalar_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double opts_per_sec_scalar = double(NUM_OPTIONS) / (time_scalar_ms * 1e-3);

    std::cout << "\n[1] Traditional Scalar Baseline (P=1, e.g. QuantLib):" << std::endl;
    std::cout << "    Execution Time: " << std::fixed << std::setprecision(2) << time_scalar_ms << " ms" << std::endl;
    std::cout << "    Throughput:     " << std::fixed << std::setprecision(0) << opts_per_sec_scalar << " options/sec" << std::endl;

    // -------------------------------------------------------------------------
    // 2. Legolas++ SIMD Single-Thread (hardware width: NEON/SSE=4, AVX2=8,
    //    AVX-512=16)
    // -------------------------------------------------------------------------
#if defined(__AVX512F__)
    const int PACK_SIZE = 16;
#elif defined(__AVX2__)
    const int PACK_SIZE = 8;
#else
    const int PACK_SIZE = 4;
#endif
    using SimdArray = Legolas::Array<float, 2, PACK_SIZE, 2>;
    SimdArray V_simd(NUM_OPTIONS, N);
    SimdArray D_simd(NUM_OPTIONS, N);
    SimdArray U_simd(NUM_OPTIONS, N);
    SimdArray L_simd(NUM_OPTIONS, N);
    SimdArray RHS_simd(NUM_OPTIONS, N);

    // Populate SIMD interleaved arrays from scalar initial data
    for (int m = 0; m < NUM_OPTIONS; ++m) {
        init_contract_data(m, dummy_k, dummy_sig, tmp_v, tmp_d, tmp_u, tmp_l);
        for (int i = 0; i < N; ++i) {
            V_simd[m][i] = tmp_v[i];
            D_simd[m][i] = tmp_d[i];
            U_simd[m][i] = tmp_u[i];
            L_simd[m][i] = tmp_l[i];
        }
    }

    // Warmup
    Legolas::map(pricer, V_simd, D_simd, U_simd, L_simd, RHS_simd);

    t0 = std::chrono::high_resolution_clock::now();
    for (int run = 0; run < num_runs; ++run) {
        for (int m = 0; m < NUM_OPTIONS; ++m) {
            float K_val = 80.0f + 40.0f * (float(m) / float(NUM_OPTIONS));
            for (int i = 0; i < N; ++i) {
                V_simd[m][i] = std::max(float(i) * dS - K_val, 0.0f);
            }
        }
        Legolas::map(pricer, V_simd, D_simd, U_simd, L_simd, RHS_simd);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double time_simd_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double opts_per_sec_simd = double(NUM_OPTIONS) / (time_simd_ms * 1e-3);
    double speedup_simd = time_scalar_ms / time_simd_ms;

    std::cout << "\n[2] Legolas++ DLI SIMD (P=" << PACK_SIZE << ", Single Core):" << std::endl;
    std::cout << "    Execution Time: " << std::fixed << std::setprecision(2) << time_simd_ms << " ms" << std::endl;
    std::cout << "    Throughput:     " << std::fixed << std::setprecision(0) << opts_per_sec_simd << " options/sec" << std::endl;
    std::cout << "    Speedup:        " << std::fixed << std::setprecision(2) << speedup_simd << "x vs Scalar Baseline" << std::endl;

    // -------------------------------------------------------------------------
    // 3. Legolas++ Multi-Core Parallel + SIMD (Parmap Work-Stealing)
    // -------------------------------------------------------------------------
    SimdArray V_par(NUM_OPTIONS, N);
    SimdArray RHS_par(NUM_OPTIONS, N);

    for (int m = 0; m < NUM_OPTIONS; ++m) {
        float K_val = 80.0f + 40.0f * (float(m) / float(NUM_OPTIONS));
        for (int i = 0; i < N; ++i) {
            V_par[m][i] = std::max(float(i) * dS - K_val, 0.0f);
        }
    }

    // Warmup
    Legolas::parmap(pricer, V_par, D_simd, U_simd, L_simd, RHS_par);

    t0 = std::chrono::high_resolution_clock::now();
    for (int run = 0; run < num_runs; ++run) {
        for (int m = 0; m < NUM_OPTIONS; ++m) {
            float K_val = 80.0f + 40.0f * (float(m) / float(NUM_OPTIONS));
            for (int i = 0; i < N; ++i) {
                V_par[m][i] = std::max(float(i) * dS - K_val, 0.0f);
            }
        }
        Legolas::parmap(pricer, V_par, D_simd, U_simd, L_simd, RHS_par);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double time_par_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double opts_per_sec_par = double(NUM_OPTIONS) / (time_par_ms * 1e-3);
    double speedup_par = time_scalar_ms / time_par_ms;

    std::cout << "\n[3] Legolas++ DLI Multi-Core + SIMD (All CPU Cores, Work-Stealing):" << std::endl;
    std::cout << "    Execution Time: " << std::fixed << std::setprecision(2) << time_par_ms << " ms" << std::endl;
    std::cout << "    Throughput:     " << std::fixed << std::setprecision(0) << opts_per_sec_par << " options/sec" << std::endl;
    std::cout << "    Speedup:        " << std::fixed << std::setprecision(2) << speedup_par << "x vs Scalar Baseline" << std::endl;

    // -------------------------------------------------------------------------
    // Numerical Verification: Max Absolute Error between Scalar and SIMD
    // -------------------------------------------------------------------------
    float max_diff = 0.0f;
    for (int m = 0; m < NUM_OPTIONS; ++m) {
        for (int i = 0; i < N; ++i) {
            float diff = std::abs(V_scalar[m][i] - V_par[m][i]);
            if (diff > max_diff) max_diff = diff;
        }
    }

    std::cout << "\n[Numerical Validation]" << std::endl;
    std::cout << "  Max Discrepancy (|V_SIMD - V_Scalar|): " << std::scientific << max_diff << std::endl;
    if (max_diff < 5e-3f) {
        std::cout << "  Status: PASSED (Numerically equivalent within float32 FMA precision)" << std::endl;
    } else {
        std::cerr << "  Status: FAILED (Numerical divergence detected)" << std::endl;
        return 1;
    }

    // -------------------------------------------------------------------------
    // Greek Sensitivity Analysis (Delta, Gamma) at Spot Price $100
    // -------------------------------------------------------------------------
    std::cout << "\n[Sample Option Valuation & Greeks (Spot = $" << S_spot << ")]" << std::endl;
    std::cout << "  -------------------------------------------------------------------" << std::endl;
    std::cout << "  Contract ID | Strike (K) | Price V(S) | Delta (dV/dS) | Gamma (d2V/dS2) " << std::endl;
    std::cout << "  -------------------------------------------------------------------" << std::endl;

    int sample_indices[4] = {0, NUM_OPTIONS / 4, NUM_OPTIONS / 2, (3 * NUM_OPTIONS) / 4};
    for (int idx : sample_indices) {
        float K_val = 80.0f + 40.0f * (float(idx) / float(NUM_OPTIONS));
        float price = V_par[idx][spot_idx];
        float price_up = V_par[idx][spot_idx + 1];
        float price_dn = V_par[idx][spot_idx - 1];

        // Central finite difference sensitivities:
        // Delta = dV / dS
        float delta = (price_up - price_dn) / (2.0f * dS);
        // Gamma = d^2V / dS^2
        float gamma = (price_up - 2.0f * price + price_dn) / (dS * dS);

        std::cout << "  Option #" << std::setw(5) << idx
                  << " | $" << std::fixed << std::setprecision(2) << std::setw(6) << K_val
                  << " | $" << std::fixed << std::setprecision(2) << std::setw(8) << price
                  << " | " << std::fixed << std::setprecision(4) << std::setw(10) << delta
                  << " | " << std::fixed << std::setprecision(6) << std::setw(12) << gamma
                  << std::endl;
    }
    std::cout << "  -------------------------------------------------------------------" << std::endl;

    // -------------------------------------------------------------------------
    // Summary Performance Table
    // -------------------------------------------------------------------------
    std::cout << "\n================================================================================" << std::endl;
    std::cout << "                     PERFORMANCE SUMMARY (16,384 OPTIONS)                       " << std::endl;
    std::cout << "================================================================================" << std::endl;
    std::cout << "  Configuration               | Time (ms) | Speedup  | Throughput (Options/sec)" << std::endl;
    std::cout << "  ----------------------------+-----------+----------+--------------------------" << std::endl;
    std::cout << "  Scalar Baseline (P=1)       | " << std::setw(9) << std::fixed << std::setprecision(2) << time_scalar_ms
              << " | " << std::setw(7) << "1.00x"
              << "  | " << std::setw(15) << std::fixed << std::setprecision(0) << opts_per_sec_scalar << std::endl;
    std::cout << "  Legolas DLI SIMD (P=" << PACK_SIZE << ")     | " << std::setw(9) << std::fixed << std::setprecision(2) << time_simd_ms
              << " | " << std::setw(7) << std::fixed << std::setprecision(2) << speedup_simd << "x"
              << "  | " << std::setw(15) << std::fixed << std::setprecision(0) << opts_per_sec_simd << std::endl;
    std::cout << "  Legolas DLI Multi-Core (P=" << PACK_SIZE << ")| " << std::setw(9) << std::fixed << std::setprecision(2) << time_par_ms
              << " | " << std::setw(7) << std::fixed << std::setprecision(2) << speedup_par << "x"
              << "  | " << std::setw(15) << std::fixed << std::setprecision(0) << opts_per_sec_par << std::endl;
    std::cout << "================================================================================\n" << std::endl;

    return 0;
}
