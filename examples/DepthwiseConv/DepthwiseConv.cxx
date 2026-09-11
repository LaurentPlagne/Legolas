/**
 * DepthwiseConv.cxx - AI Depthwise Separable 2D Convolution with Legolas++
 *
 * In MobileNet (V1/V2/V3), ConvNeXt, and EfficientNet architectures,
 * Depthwise Convolution filters each channel independently with a spatial kernel (e.g. 3x3).
 *
 * In conventional layouts (NCHW), auto-vectorizing across channels is difficult because
 * channels are not contiguous. Vectorizing across spatial dimensions (W) has unaligned loads
 * and boundary conditions.
 *
 * Frameworks like oneDNN, TVM, and ONNX Runtime use blocked layouts: nChw4c (NEON) / nChw8c (AVX).
 * With Legolas++ Data Layout Interleaving (DLI), this layout is native:
 * Legolas::Array<float, 2, PACK_SIZE, 2> groups P channels contiguously at every spatial position!
 *
 * The exact same generic 2D stencil loop executes on SIMD vectors (Eigen::Array<float, P, 1>),
 * computing P channels simultaneously using native NEON fmla.4s instructions.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <iomanip>
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

// Generic 2D 3x3 Depthwise Convolution Kernel
struct DepthwiseConv2D {
    int H, W;
    DepthwiseConv2D(int h, int w) : H(h), W(w) {}

    template <class InA2D, class OutA2D>
    void operator()(int begin, int end, InA2D input, OutA2D output) const {
        typedef typename InA2D::RealType Scalar;

        // 3x3 Gaussian/Sobel-style kernel weights
        Scalar k00(1.0f/16.0f), k01(2.0f/16.0f), k02(1.0f/16.0f);
        Scalar k10(2.0f/16.0f), k11(4.0f/16.0f), k12(2.0f/16.0f);
        Scalar k20(1.0f/16.0f), k21(2.0f/16.0f), k22(1.0f/16.0f);

        for (int c = begin; c < end; ++c) {
            auto in_chan = input[c];
            auto out_chan = output[c];

            // 3x3 stencil over spatial domain [1..H-2] x [1..W-2]
            for (int r = 1; r < H - 1; ++r) {
                const int row_prev = (r - 1) * W;
                const int row_curr = r * W;
                const int row_next = (r + 1) * W;

                for (int col = 1; col < W - 1; ++col) {
                    Scalar sum = k00 * in_chan[row_prev + col - 1]
                                + k01 * in_chan[row_prev + col]
                                + k02 * in_chan[row_prev + col + 1]
                                + k10 * in_chan[row_curr + col - 1]
                                + k11 * in_chan[row_curr + col]
                                + k12 * in_chan[row_curr + col + 1]
                                + k20 * in_chan[row_next + col - 1]
                                + k21 * in_chan[row_next + col]
                                + k22 * in_chan[row_next + col + 1];

                    out_chan[row_curr + col] = sum;
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    std::cout << "========================================================" << std::endl;
    std::cout << " Legolas++ AI Showcase: Depthwise Separable 2D Conv     " << std::endl;
    std::cout << "========================================================" << std::endl;

    const int C = 128;      // Number of feature channels (MobileNet block)
    const int H = 112;      // Height (feature map size)
    const int W = 112;      // Width
    const int S = H * W;    // Spatial elements per channel (12,544)
    const int num_runs = 50;

    std::cout << "Configuration:" << std::endl;
    std::cout << "  Feature Map: " << H << "x" << W << " (" << S << " pixels)" << std::endl;
    std::cout << "  Channels:    " << C << std::endl;
    std::cout << "  FLOPs/run:   " << (18.0 * (H - 2) * (W - 2) * C) / 1e6 << " MFLOPs" << std::endl;
    std::cout << std::endl;

    // ---------------------------------------------------------------
    // 1. Scalar Baseline (PACK_SIZE = 1)
    // ---------------------------------------------------------------
    using ScalarArray = Legolas::Array<float, 2>;
    ScalarArray in_scalar(C, S);
    ScalarArray out_scalar(C, S);
    in_scalar.fill(1.0f);
    out_scalar.fill(0.0f);

    DepthwiseConv2D conv(H, W);

    // Warmup
    Legolas::map(conv, in_scalar, out_scalar);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < num_runs; ++r) {
        Legolas::map(conv, in_scalar, out_scalar);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double time_scalar_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double gflops_scalar = (18.0 * (H - 2) * (W - 2) * C) / (time_scalar_ms * 1e6);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[1] Scalar Baseline (P=1):" << std::endl;
    std::cout << "    Time:   " << time_scalar_ms << " ms" << std::endl;
    std::cout << "    Speed:  " << gflops_scalar << " GFlops" << std::endl;

    // ---------------------------------------------------------------
    // 2. Legolas NEON SIMD (PACK_SIZE = 4, ARM64 NEON native float32x4)
    // ---------------------------------------------------------------
    using NeonArray = Legolas::Array<float, 2, 4, 2>;
    NeonArray in_neon(C, S);
    NeonArray out_neon(C, S);
    in_neon.fill(1.0f);
    out_neon.fill(0.0f);

    // Warmup
    Legolas::map(conv, in_neon, out_neon);

    t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < num_runs; ++r) {
        Legolas::map(conv, in_neon, out_neon);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double time_neon_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double gflops_neon = (18.0 * (H - 2) * (W - 2) * C) / (time_neon_ms * 1e6);
    double speedup_neon = time_scalar_ms / time_neon_ms;

    std::cout << "\n[2] Legolas NEON SIMD (P=4, DLI nChw4c):" << std::endl;
    std::cout << "    Time:    " << time_neon_ms << " ms" << std::endl;
    std::cout << "    Speed:   " << gflops_neon << " GFlops" << std::endl;
    std::cout << "    Speedup: " << speedup_neon << "x vs Scalar Baseline" << std::endl;

    // ---------------------------------------------------------------
    // 3. Legolas Parallel + NEON SIMD (Work-Stealing / TBB)
    // ---------------------------------------------------------------
    NeonArray out_par(C, S);
    out_par.fill(0.0f);

    Legolas::parmap(conv, in_neon, out_par);

    t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < num_runs; ++r) {
        Legolas::parmap(conv, in_neon, out_par);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double time_par_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double gflops_par = (18.0 * (H - 2) * (W - 2) * C) / (time_par_ms * 1e6);
    double speedup_par = time_scalar_ms / time_par_ms;

    std::cout << "\n[3] Legolas Parallel + NEON SIMD (Multi-Core):" << std::endl;
    std::cout << "    Time:    " << time_par_ms << " ms" << std::endl;
    std::cout << "    Speed:   " << gflops_par << " GFlops" << std::endl;
    std::cout << "    Speedup: " << speedup_par << "x vs Scalar Baseline" << std::endl;

    // ---------------------------------------------------------------
    // 4. Verification: Mathematical Exactness
    // ---------------------------------------------------------------
    double max_diff = 0.0;
    for (int c = 0; c < C; ++c) {
        for (int s = 0; s < S; ++s) {
            double diff = std::abs(out_scalar[c][s] - out_neon[c][s]);
            if (diff > max_diff) max_diff = diff;
        }
    }

    std::cout << "\n--------------------------------------------------------" << std::endl;
    std::cout << "Validation Max Error: " << max_diff << std::endl;
    if (max_diff < 1e-5) {
        std::cout << "[SUCCESS] Legolas DepthwiseConv result matches scalar exactly!" << std::endl;
        return 0;
    } else {
        std::cerr << "[FAILURE] Output discrepancy detected!" << std::endl;
        return 1;
    }
}
