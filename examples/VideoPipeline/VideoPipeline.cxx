/**
 * VideoPipeline.cxx - Real-Time Multi-Stream Video Processing Pipeline with Legolas++
 *
 * In video surveillance (NVR), autonomous vehicles, and broadcast video servers,
 * video processing engines must ingest and process multiple high-definition video streams
 * simultaneously in real time (e.g. 16 to 64 camera streams at 60+ FPS).
 *
 * A typical real-time vision pre-processing pipeline consists of:
 *   1. Spatial Feature Extraction (3x3 Sobel gradient magnitude for edge detection)
 *   2. Temporal Motion Estimation (frame differencing between current frame I_t and previous frame I_{t-1})
 *   3. Motion-Edge Fusion (fusing spatial edges with temporal activity to highlight moving objects)
 *
 * WHY STANDARD COMPUTER VISION LIBS (OpenCV, FFmpeg filters) STRUGGLE TO VECTORIZE ACROSS STREAMS:
 * In conventional frame layouts (planar HxW or interleaved RGB), processing N separate camera feeds
 * processes each frame independently. Within a single frame, 3x3 spatial stencils suffer from
 * unaligned memory accesses, boundary conditions, and horizontal SIMD overhead.
 *
 * LEGOLAS DLI SOLUTION:
 * With Legolas++ Data Layout Interleaving (DLI), multiple video feeds are interleaved at compile-time:
 *   Legolas::Array<float, 2, PACK_SIZE, 2> (N streams, H x W pixels per stream)
 *
 * At every spatial coordinate (r, c), the hardware SIMD vector register (ARM NEON / AVX2 / AVX-512)
 * holds the corresponding pixel from P distinct camera streams.
 *
 * Key advantages:
 *   - 100% aligned contiguous SIMD memory loads and stores across streams.
 *   - Exact same generic stencil C++ code runs seamlessly on scalar floats or SIMD vectors.
 *   - Zero external dependencies (no OpenCV, no IPP, zero external libraries).
 *   - Massive throughput measured in Frames Per Second (FPS) and GigaPixels/second (GPix/s).
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <iomanip>
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

// Generic Real-Time Video Spatial-Temporal Filter
struct VideoSpatialTemporalPipeline {
    int H, W;
    VideoSpatialTemporalPipeline(int h, int w) : H(h), W(w) {}

    template <class InCurrA2D, class InPrevA2D, class OutA2D>
    void operator()(int begin, int end, InCurrA2D curr_frames, InPrevA2D prev_frames, OutA2D out_frames) const {
        typedef typename InCurrA2D::RealType Scalar;

        const Scalar alpha(0.7f); // Temporal blend weight
        const Scalar beta(0.3f);

        for (int s = begin; s < end; ++s) {
            auto curr = curr_frames[s];
            auto prev = prev_frames[s];
            auto out  = out_frames[s];

            // 3x3 Spatial Sobel Stencil + Temporal Differencing across [1..H-2] x [1..W-2]
            for (int r = 1; r < H - 1; ++r) {
                const int row_prev = (r - 1) * W;
                const int row_curr = r * W;
                const int row_next = (r + 1) * W;

                // Prime the sliding window with first two columns
                Scalar p00 = curr[row_prev];
                Scalar p01 = curr[row_prev + 1];

                Scalar p10 = curr[row_curr];
                Scalar p11 = curr[row_curr + 1];

                Scalar p20 = curr[row_next];
                Scalar p21 = curr[row_next + 1];

                for (int col = 1; col < W - 1; ++col) {
                    // Only 3 new loads per pixel instead of 9!
                    Scalar p02 = curr[row_prev + col + 1];
                    Scalar p12 = curr[row_curr + col + 1];
                    Scalar p22 = curr[row_next + col + 1];

                    // Spatial Sobel Horizontal & Vertical gradients
                    Scalar gx = (p02 + Scalar(2.0f) * p12 + p22) - (p00 + Scalar(2.0f) * p10 + p20);
                    Scalar gy = (p20 + Scalar(2.0f) * p21 + p22) - (p00 + Scalar(2.0f) * p01 + p02);

                    // Gradient magnitude approximation
                    Scalar spatial_edge = (gx * gx + gy * gy) * Scalar(0.125f);

                    // Temporal differencing: motion = (I_t - I_{t-1})^2
                    Scalar prev_val = prev[row_curr + col];
                    Scalar diff = p11 - prev_val;
                    Scalar temporal_motion = diff * diff;

                    // Fusion: blended response
                    out[row_curr + col] = alpha * spatial_edge + beta * temporal_motion;

                    // Slide the window forward
                    p00 = p01; p01 = p02;
                    p10 = p11; p11 = p12;
                    p20 = p21; p21 = p22;
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    std::cout << "========================================================" << std::endl;
    std::cout << " Legolas++ Vision Showcase: Real-Time Video Pipeline    " << std::endl;
    std::cout << " Spatial 3x3 Edge Detection + Temporal Motion Fusion    " << std::endl;
    std::cout << "========================================================" << std::endl;

    // Multi-Camera NVR Simulation: 32 Camera Feeds at 720p HD (1280 x 720)
    const int NUM_STREAMS = 32;
    const int H = 720;
    const int W = 1280;
    const int PIXELS_PER_FRAME = H * W;
    const int NUM_WARMUP = 2;
    const int NUM_ITERS  = 10;

    std::cout << "Configuration:" << std::endl;
    std::cout << "  Camera Streams:       " << NUM_STREAMS << " concurrent video feeds" << std::endl;
    std::cout << "  Frame Resolution:     " << W << " x " << H << " (720p HD)" << std::endl;
    std::cout << "  Pixels per Frame:     " << PIXELS_PER_FRAME << " (" << std::fixed << std::setprecision(2)
              << (PIXELS_PER_FRAME / 1e6) << " MPix)" << std::endl;
    std::cout << "  Total Pixels/Batch:   " << (double)NUM_STREAMS * PIXELS_PER_FRAME / 1e6 << " MPix" << std::endl;
    std::cout << "  Benchmark Iterations: " << NUM_ITERS << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

    // Detect native hardware vector length
#if defined(__AVX512F__)
    const int PACK_SIZE = 16;
    const std::string SIMD_NAME = "AVX-512";
#elif defined(__AVX2__)
    const int PACK_SIZE = 8;
    const std::string SIMD_NAME = "AVX2";
#elif defined(__ARM_NEON)
    const int PACK_SIZE = 4;
    const std::string SIMD_NAME = "ARM NEON";
#else
    const int PACK_SIZE = 4;
    const std::string SIMD_NAME = "Generic SIMD";
#endif

    std::cout << "Hardware SIMD Vector:  " << SIMD_NAME << " (Interleaving " << PACK_SIZE << " streams/register)" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

    VideoSpatialTemporalPipeline pipeline(H, W);

    // ---------------------------------------------------------------
    // 1. Scalar Reference Baseline (Standard C++ loop)
    // ---------------------------------------------------------------
    Legolas::Array<float, 2> curr_scalar(NUM_STREAMS, PIXELS_PER_FRAME);
    Legolas::Array<float, 2> prev_scalar(NUM_STREAMS, PIXELS_PER_FRAME);
    Legolas::Array<float, 2> out_scalar(NUM_STREAMS, PIXELS_PER_FRAME);

    // Initialize synthetic camera data
    for (int s = 0; s < NUM_STREAMS; ++s) {
        for (int p = 0; p < PIXELS_PER_FRAME; ++p) {
            curr_scalar[s][p] = std::sin(0.01f * p + 0.5f * s);
            prev_scalar[s][p] = std::sin(0.01f * p + 0.5f * s + 0.05f);
        }
    }
    out_scalar.fill(0.0f);

    for (int it = 0; it < NUM_WARMUP; ++it) {
        pipeline(0, NUM_STREAMS, curr_scalar, prev_scalar, out_scalar);
    }

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int it = 0; it < NUM_ITERS; ++it) {
        pipeline(0, NUM_STREAMS, curr_scalar, prev_scalar, out_scalar);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double time_scalar_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / NUM_ITERS;
    double fps_scalar = (NUM_STREAMS / (time_scalar_ms / 1000.0));
    double gpix_scalar = ((double)NUM_STREAMS * PIXELS_PER_FRAME / (time_scalar_ms / 1000.0)) / 1e9;

    std::cout << "[1] Scalar Reference (Single-Thread, Non-Interleaved):" << std::endl;
    std::cout << "    Latency:    " << std::fixed << std::setprecision(2) << time_scalar_ms << " ms/batch" << std::endl;
    std::cout << "    Throughput: " << std::fixed << std::setprecision(1) << fps_scalar << " FPS" << std::endl;
    std::cout << "    Bandwidth:  " << std::fixed << std::setprecision(3) << gpix_scalar << " GPixels/sec" << std::endl;

    // ---------------------------------------------------------------
    // 2. Legolas DLI SIMD (Single-Thread)
    // ---------------------------------------------------------------
    typedef Legolas::Array<float, 2, PACK_SIZE, 2> SimdArray;
    SimdArray curr_simd(NUM_STREAMS, PIXELS_PER_FRAME);
    SimdArray prev_simd(NUM_STREAMS, PIXELS_PER_FRAME);
    SimdArray out_simd(NUM_STREAMS, PIXELS_PER_FRAME);

    for (int s = 0; s < NUM_STREAMS; ++s) {
        for (int p = 0; p < PIXELS_PER_FRAME; ++p) {
            curr_simd[s][p] = curr_scalar[s][p];
            prev_simd[s][p] = prev_scalar[s][p];
        }
    }
    out_simd.fill(0.0f);

    for (int it = 0; it < NUM_WARMUP; ++it) {
        Legolas::map(pipeline, curr_simd, prev_simd, out_simd);
    }

    t0 = std::chrono::high_resolution_clock::now();
    for (int it = 0; it < NUM_ITERS; ++it) {
        Legolas::map(pipeline, curr_simd, prev_simd, out_simd);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double time_simd_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / NUM_ITERS;
    double fps_simd = (NUM_STREAMS / (time_simd_ms / 1000.0));
    double gpix_simd = ((double)NUM_STREAMS * PIXELS_PER_FRAME / (time_simd_ms / 1000.0)) / 1e9;
    double speedup_simd = time_scalar_ms / time_simd_ms;

    std::cout << "[2] Legolas DLI (" << SIMD_NAME << ", Single-Thread):" << std::endl;
    std::cout << "    Latency:    " << std::fixed << std::setprecision(2) << time_simd_ms << " ms/batch" << std::endl;
    std::cout << "    Throughput: " << std::fixed << std::setprecision(1) << fps_simd << " FPS" << std::endl;
    std::cout << "    Bandwidth:  " << std::fixed << std::setprecision(3) << gpix_simd << " GPixels/sec" << std::endl;
    std::cout << "    Speedup:    " << std::fixed << std::setprecision(2) << speedup_simd << "x vs Scalar Baseline" << std::endl;

    // ---------------------------------------------------------------
    // 3. Legolas Parallel + DLI SIMD (Multi-Core Work-Stealing)
    // ---------------------------------------------------------------
    SimdArray out_par(NUM_STREAMS, PIXELS_PER_FRAME);
    out_par.fill(0.0f);

    for (int it = 0; it < NUM_WARMUP; ++it) {
        Legolas::parmap(pipeline, curr_simd, prev_simd, out_par);
    }

    t0 = std::chrono::high_resolution_clock::now();
    for (int it = 0; it < NUM_ITERS; ++it) {
        Legolas::parmap(pipeline, curr_simd, prev_simd, out_par);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double time_par_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / NUM_ITERS;
    double fps_par = (NUM_STREAMS / (time_par_ms / 1000.0));
    double gpix_par = ((double)NUM_STREAMS * PIXELS_PER_FRAME / (time_par_ms / 1000.0)) / 1e9;
    double speedup_par = time_scalar_ms / time_par_ms;

    std::cout << "[3] Legolas Parallel + DLI (" << SIMD_NAME << " + Multi-Core):" << std::endl;
    std::cout << "    Latency:    " << std::fixed << std::setprecision(2) << time_par_ms << " ms/batch" << std::endl;
    std::cout << "    Throughput: " << std::fixed << std::setprecision(1) << fps_par << " FPS" << std::endl;
    std::cout << "    Bandwidth:  " << std::fixed << std::setprecision(3) << gpix_par << " GPixels/sec" << std::endl;
    std::cout << "    Speedup:    " << std::fixed << std::setprecision(2) << speedup_par << "x vs Scalar Baseline" << std::endl;

    // ---------------------------------------------------------------
    // 4. Numerical Accuracy Verification
    // ---------------------------------------------------------------
    double max_diff_simd = 0.0;
    double max_diff_par  = 0.0;
    for (int s = 0; s < NUM_STREAMS; ++s) {
        for (int p = 0; p < PIXELS_PER_FRAME; ++p) {
            double diff1 = std::abs((double)out_simd[s][p] - (double)out_scalar[s][p]);
            double diff2 = std::abs((double)out_par[s][p]  - (double)out_scalar[s][p]);
            if (diff1 > max_diff_simd) max_diff_simd = diff1;
            if (diff2 > max_diff_par)  max_diff_par  = diff2;
        }
    }

    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "Verification:" << std::endl;
    std::cout << "  Max Abs Diff (SIMD vs Scalar):     " << std::scientific << max_diff_simd << std::endl;
    std::cout << "  Max Abs Diff (Parallel vs Scalar): " << std::scientific << max_diff_par  << std::endl;

    const double TOLERANCE = 1e-4;
    if (max_diff_simd < TOLERANCE && max_diff_par < TOLERANCE) {
        std::cout << "  Status: SUCCESS (Bitwise/Float Accuracy Verified)" << std::endl;
        std::cout << "========================================================" << std::endl;
        return 0;
    } else {
        std::cerr << "  Status: FAILED (Accuracy check exceeded tolerance)" << std::endl;
        std::cout << "========================================================" << std::endl;
        return 1;
    }
}
