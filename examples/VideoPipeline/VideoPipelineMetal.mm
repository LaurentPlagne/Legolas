/**
 * VideoPipelineMetal.mm - Apple Silicon Metal GPU Compute Backend vs Legolas++ CPU SIMD
 *
 * Demonstrates:
 *   1. Scalar Baseline (C++ standard single-thread)
 *   2. Legolas DLI ARM NEON (CPU single-thread SIMD)
 *   3. Legolas Parallel + DLI ARM NEON (CPU multi-core Work-Stealing)
 *   4. Apple Silicon Metal GPU Compute (Apple M1 Max GPU, 32 Cores, Zero-Copy Unified Memory)
 *
 * UNIFIED MEMORY ARCHITECTURE (UMA):
 * On Apple Silicon, CPU and GPU share the same physical memory bus (up to 400 GB/s on M1 Max).
 * MTLResourceStorageModeShared buffers allow the CPU to initialize and inspect data
 * without ANY PCIe transfer overhead.
 */

#import <Metal/Metal.h>
#import <Foundation/Foundation.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <iomanip>
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

// ---------------------------------------------------------------------------
// 1. Legolas Generic CPU Pipeline
// ---------------------------------------------------------------------------
struct VideoSpatialTemporalPipeline {
    int H, W;
    VideoSpatialTemporalPipeline(int h, int w) : H(h), W(w) {}

    template <class InCurrA2D, class InPrevA2D, class OutA2D>
    void operator()(int begin, int end, InCurrA2D curr_frames, InPrevA2D prev_frames, OutA2D out_frames) const {
        typedef typename InCurrA2D::RealType Scalar;
        const Scalar alpha(0.7f);
        const Scalar beta(0.3f);

        for (int s = begin; s < end; ++s) {
            auto curr = curr_frames[s];
            auto prev = prev_frames[s];
            auto out  = out_frames[s];

            for (int r = 1; r < H - 1; ++r) {
                const int row_prev = (r - 1) * W;
                const int row_curr = r * W;
                const int row_next = (r + 1) * W;

                Scalar p00 = curr[row_prev];
                Scalar p01 = curr[row_prev + 1];

                Scalar p10 = curr[row_curr];
                Scalar p11 = curr[row_curr + 1];

                Scalar p20 = curr[row_next];
                Scalar p21 = curr[row_next + 1];

                for (int col = 1; col < W - 1; ++col) {
                    Scalar p02 = curr[row_prev + col + 1];
                    Scalar p12 = curr[row_curr + col + 1];
                    Scalar p22 = curr[row_next + col + 1];

                    Scalar gx = (p02 + Scalar(2.0f) * p12 + p22) - (p00 + Scalar(2.0f) * p10 + p20);
                    Scalar gy = (p20 + Scalar(2.0f) * p21 + p22) - (p00 + Scalar(2.0f) * p01 + p02);

                    Scalar spatial_edge = (gx * gx + gy * gy) * Scalar(0.125f);

                    Scalar prev_val = prev[row_curr + col];
                    Scalar diff = p11 - prev_val;
                    Scalar temporal_motion = diff * diff;

                    out[row_curr + col] = alpha * spatial_edge + beta * temporal_motion;

                    p00 = p01; p01 = p02;
                    p10 = p11; p11 = p12;
                    p20 = p21; p21 = p22;
                }
            }
        }
    }
};

// ---------------------------------------------------------------------------
// 2. Metal Shading Language (MSL) Kernel Source
// ---------------------------------------------------------------------------
static const char* metalShaderSource = R"(
#include <metal_stdlib>
using namespace metal;

kernel void video_spatial_temporal_metal(
    device const float* curr_frames   [[buffer(0)]],
    device const float* prev_frames   [[buffer(1)]],
    device float*       out_frames    [[buffer(2)]],
    constant int&       H             [[buffer(3)]],
    constant int&       W             [[buffer(4)]],
    uint3               thread_pos    [[thread_position_in_grid]])
{
    int col = thread_pos.x;
    int r   = thread_pos.y;
    int s   = thread_pos.z;

    if (col >= W || r >= H) return;

    int frame_offset = s * (H * W);
    int idx = frame_offset + r * W + col;

    if (r < 1 || r >= H - 1 || col < 1 || col >= W - 1) {
        out_frames[idx] = 0.0f;
        return;
    }

    int row_prev = frame_offset + (r - 1) * W;
    int row_curr = frame_offset + r * W;
    int row_next = frame_offset + (r + 1) * W;

    float p00 = curr_frames[row_prev + col - 1];
    float p01 = curr_frames[row_prev + col];
    float p02 = curr_frames[row_prev + col + 1];

    float p10 = curr_frames[row_curr + col - 1];
    float p11 = curr_frames[row_curr + col];
    float p12 = curr_frames[row_curr + col + 1];

    float p20 = curr_frames[row_next + col - 1];
    float p21 = curr_frames[row_next + col];
    float p22 = curr_frames[row_next + col + 1];

    float gx = (p02 + 2.0f * p12 + p22) - (p00 + 2.0f * p10 + p20);
    float gy = (p20 + 2.0f * p21 + p22) - (p00 + 2.0f * p01 + p02);

    float spatial_edge = (gx * gx + gy * gy) * 0.125f;

    float prev_val = prev_frames[idx];
    float diff = p11 - prev_val;
    float temporal_motion = diff * diff;

    out_frames[idx] = 0.7f * spatial_edge + 0.3f * temporal_motion;
}
)";

int main(int argc, char* argv[]) {
    @autoreleasepool {
        std::cout << "========================================================" << std::endl;
        std::cout << " Legolas++ Benchmark: CPU vs Apple Silicon Metal GPU    " << std::endl;
        std::cout << " Real-Time Multi-Stream Video Processing (720p HD)      " << std::endl;
        std::cout << "========================================================" << std::endl;

        const int NUM_STREAMS = 32;
        const int H = 720;
        const int W = 1280;
        const int PIXELS_PER_FRAME = H * W;
        const size_t TOTAL_FLOATS = (size_t)NUM_STREAMS * PIXELS_PER_FRAME;
        const size_t BUFFER_BYTES = TOTAL_FLOATS * sizeof(float);
        const int NUM_WARMUP = 3;
        const int NUM_ITERS  = 15;

        std::cout << "Configuration:" << std::endl;
        std::cout << "  Camera Streams:       " << NUM_STREAMS << " concurrent HD feeds" << std::endl;
        std::cout << "  Frame Resolution:     " << W << " x " << H << " (720p HD)" << std::endl;
        std::cout << "  Total Pixels/Batch:   " << TOTAL_FLOATS / 1e6 << " MPix ("
                  << (BUFFER_BYTES / (1024.0 * 1024.0)) << " MB/buffer)" << std::endl;
        std::cout << "  Benchmark Iterations: " << NUM_ITERS << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;

        VideoSpatialTemporalPipeline pipeline(H, W);

        // -----------------------------------------------------------
        // 1. Initialize Host/Shared Buffers
        // -----------------------------------------------------------
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device) {
            std::cout << "Notice: No Metal GPU device available in this environment (e.g. headless CI VM). Skipping Metal benchmark." << std::endl;
            return 0;
        }

        std::cout << "Hardware:" << std::endl;
        std::cout << "  CPU SIMD Engine:     ARM NEON (4 lanes FP32)" << std::endl;
        std::cout << "  GPU Metal Device:    " << [[device name] UTF8String] << std::endl;
        std::cout << "  Unified Memory (UMA):" << ([device hasUnifiedMemory] ? " YES (Zero-Copy)" : " NO") << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;

        // Allocate shared memory buffers: directly accessible by CPU and GPU
        id<MTLBuffer> bufCurr = [device newBufferWithLength:BUFFER_BYTES options:MTLResourceStorageModeShared];
        id<MTLBuffer> bufPrev = [device newBufferWithLength:BUFFER_BYTES options:MTLResourceStorageModeShared];
        id<MTLBuffer> bufOutGPU = [device newBufferWithLength:BUFFER_BYTES options:MTLResourceStorageModeShared];

        float* curr_raw = (float*)[bufCurr contents];
        float* prev_raw = (float*)[bufPrev contents];
        float* out_gpu_raw = (float*)[bufOutGPU contents];

        for (int s = 0; s < NUM_STREAMS; ++s) {
            for (int p = 0; p < PIXELS_PER_FRAME; ++p) {
                int idx = s * PIXELS_PER_FRAME + p;
                curr_raw[idx] = std::sin(0.01f * p + 0.5f * s);
                prev_raw[idx] = std::sin(0.01f * p + 0.5f * s + 0.05f);
                out_gpu_raw[idx] = 0.0f;
            }
        }

        // -----------------------------------------------------------
        // 2. Scalar CPU Baseline
        // -----------------------------------------------------------
        Legolas::Array<float, 2> curr_scalar(NUM_STREAMS, PIXELS_PER_FRAME);
        Legolas::Array<float, 2> prev_scalar(NUM_STREAMS, PIXELS_PER_FRAME);
        Legolas::Array<float, 2> out_scalar(NUM_STREAMS, PIXELS_PER_FRAME);

        for (int s = 0; s < NUM_STREAMS; ++s) {
            for (int p = 0; p < PIXELS_PER_FRAME; ++p) {
                curr_scalar[s][p] = curr_raw[s * PIXELS_PER_FRAME + p];
                prev_scalar[s][p] = prev_raw[s * PIXELS_PER_FRAME + p];
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
        double fps_scalar = NUM_STREAMS / (time_scalar_ms / 1000.0);
        double gpix_scalar = (TOTAL_FLOATS / (time_scalar_ms / 1000.0)) / 1e9;

        std::cout << "[1] CPU Scalar (1 Core, Standard C++):" << std::endl;
        std::cout << "    Latency:    " << std::fixed << std::setprecision(2) << time_scalar_ms << " ms" << std::endl;
        std::cout << "    Throughput: " << std::fixed << std::setprecision(1) << fps_scalar << " FPS" << std::endl;
        std::cout << "    Bandwidth:  " << std::fixed << std::setprecision(3) << gpix_scalar << " GPixels/sec" << std::endl;

        // -----------------------------------------------------------
        // 3. Legolas DLI NEON SIMD (1 Core)
        // -----------------------------------------------------------
        const int PACK_SIZE = 4;
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
        double fps_simd = NUM_STREAMS / (time_simd_ms / 1000.0);
        double gpix_simd = (TOTAL_FLOATS / (time_simd_ms / 1000.0)) / 1e9;

        std::cout << "[2] CPU Legolas DLI (1 Core ARM NEON SIMD):" << std::endl;
        std::cout << "    Latency:    " << std::fixed << std::setprecision(2) << time_simd_ms << " ms" << std::endl;
        std::cout << "    Throughput: " << std::fixed << std::setprecision(1) << fps_simd << " FPS" << std::endl;
        std::cout << "    Bandwidth:  " << std::fixed << std::setprecision(3) << gpix_simd << " GPixels/sec" << std::endl;
        std::cout << "    Speedup:    " << std::fixed << std::setprecision(2) << (time_scalar_ms / time_simd_ms) << "x" << std::endl;

        // -----------------------------------------------------------
        // 4. Legolas Parallel + DLI NEON SIMD (Multi-Core)
        // -----------------------------------------------------------
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
        double fps_par = NUM_STREAMS / (time_par_ms / 1000.0);
        double gpix_par = (TOTAL_FLOATS / (time_par_ms / 1000.0)) / 1e9;

        std::cout << "[3] CPU Legolas Parallel + DLI (Multi-Core + ARM NEON):" << std::endl;
        std::cout << "    Latency:    " << std::fixed << std::setprecision(2) << time_par_ms << " ms" << std::endl;
        std::cout << "    Throughput: " << std::fixed << std::setprecision(1) << fps_par << " FPS" << std::endl;
        std::cout << "    Bandwidth:  " << std::fixed << std::setprecision(3) << gpix_par << " GPixels/sec" << std::endl;
        std::cout << "    Speedup:    " << std::fixed << std::setprecision(2) << (time_scalar_ms / time_par_ms) << "x" << std::endl;

        // -----------------------------------------------------------
        // 5. Apple Silicon Metal GPU Compute
        // -----------------------------------------------------------
        NSError* error = nil;
        NSString* srcString = [NSString stringWithUTF8String:metalShaderSource];
        id<MTLLibrary> library = [device newLibraryWithSource:srcString options:nil error:&error];
        if (!library) {
            std::cerr << "Metal compile error: " << [[error localizedDescription] UTF8String] << std::endl;
            return 1;
        }

        id<MTLFunction> kernelFunc = [library newFunctionWithName:@"video_spatial_temporal_metal"];
        id<MTLComputePipelineState> pipelineState = [device newComputePipelineStateWithFunction:kernelFunc error:&error];
        if (!pipelineState) {
            std::cerr << "Pipeline error: " << [[error localizedDescription] UTF8String] << std::endl;
            return 1;
        }

        id<MTLCommandQueue> commandQueue = [device newCommandQueue];

        // Metal Dispatch configuration
        MTLSize gridSize = MTLSizeMake(W, H, NUM_STREAMS);
        NSUInteger threadGroupWidth = 16;
        NSUInteger threadGroupHeight = 16;
        NSUInteger threadGroupDepth = 1;
        MTLSize threadGroupSize = MTLSizeMake(threadGroupWidth, threadGroupHeight, threadGroupDepth);

        // Warmup
        for (int it = 0; it < NUM_WARMUP; ++it) {
            id<MTLCommandBuffer> cmdBuffer = [commandQueue commandBuffer];
            id<MTLComputeCommandEncoder> encoder = [cmdBuffer computeCommandEncoder];
            [encoder setComputePipelineState:pipelineState];
            [encoder setBuffer:bufCurr offset:0 atIndex:0];
            [encoder setBuffer:bufPrev offset:0 atIndex:1];
            [encoder setBuffer:bufOutGPU offset:0 atIndex:2];
            int h_val = H, w_val = W;
            [encoder setBytes:&h_val length:sizeof(int) atIndex:3];
            [encoder setBytes:&w_val length:sizeof(int) atIndex:4];
            [encoder dispatchThreads:gridSize threadsPerThreadgroup:threadGroupSize];
            [encoder endEncoding];
            [cmdBuffer commit];
            [cmdBuffer waitUntilCompleted];
        }

        // Timed Metal Benchmark
        t0 = std::chrono::high_resolution_clock::now();
        for (int it = 0; it < NUM_ITERS; ++it) {
            id<MTLCommandBuffer> cmdBuffer = [commandQueue commandBuffer];
            id<MTLComputeCommandEncoder> encoder = [cmdBuffer computeCommandEncoder];
            [encoder setComputePipelineState:pipelineState];
            [encoder setBuffer:bufCurr offset:0 atIndex:0];
            [encoder setBuffer:bufPrev offset:0 atIndex:1];
            [encoder setBuffer:bufOutGPU offset:0 atIndex:2];
            int h_val = H, w_val = W;
            [encoder setBytes:&h_val length:sizeof(int) atIndex:3];
            [encoder setBytes:&w_val length:sizeof(int) atIndex:4];
            [encoder dispatchThreads:gridSize threadsPerThreadgroup:threadGroupSize];
            [encoder endEncoding];
            [cmdBuffer commit];
            [cmdBuffer waitUntilCompleted];
        }
        t1 = std::chrono::high_resolution_clock::now();
        double time_gpu_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / NUM_ITERS;
        double fps_gpu = NUM_STREAMS / (time_gpu_ms / 1000.0);
        double gpix_gpu = (TOTAL_FLOATS / (time_gpu_ms / 1000.0)) / 1e9;

        std::cout << "[4] GPU Apple Metal Compute (M1 Max 32-Core GPU, Zero-Copy):" << std::endl;
        std::cout << "    Latency:    " << std::fixed << std::setprecision(2) << time_gpu_ms << " ms" << std::endl;
        std::cout << "    Throughput: " << std::fixed << std::setprecision(1) << fps_gpu << " FPS" << std::endl;
        std::cout << "    Bandwidth:  " << std::fixed << std::setprecision(3) << gpix_gpu << " GPixels/sec" << std::endl;
        std::cout << "    Speedup:    " << std::fixed << std::setprecision(2) << (time_scalar_ms / time_gpu_ms) << "x vs Scalar Baseline" << std::endl;
        std::cout << "    GPU vs CPU: " << std::fixed << std::setprecision(2) << (fps_gpu / fps_par) << "x vs CPU Multi-Core" << std::endl;

        // -----------------------------------------------------------
        // 6. Accuracy Verification (GPU vs CPU Scalar)
        // -----------------------------------------------------------
        double max_diff_gpu = 0.0;
        for (int s = 0; s < NUM_STREAMS; ++s) {
            for (int r = 1; r < H - 1; ++r) {
                for (int c = 1; c < W - 1; ++c) {
                    int idx = s * PIXELS_PER_FRAME + r * W + c;
                    double diff = std::abs((double)out_gpu_raw[idx] - (double)out_scalar[s][r * W + c]);
                    if (diff > max_diff_gpu) max_diff_gpu = diff;
                }
            }
        }

        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "Verification:" << std::endl;
        std::cout << "  Max Abs Diff (Metal GPU vs CPU Scalar): " << std::scientific << max_diff_gpu << std::endl;
        if (max_diff_gpu < 1e-4) {
            std::cout << "  Status: SUCCESS (Float Accuracy Verified between GPU & CPU)" << std::endl;
        } else {
            std::cerr << "  Status: FAILED (Accuracy check exceeded tolerance)" << std::endl;
            return 1;
        }

        std::cout << "========================================================" << std::endl;
        std::cout << " Summary Comparison Table:                             " << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "| Engine                                | Throughput | GPixels/s | Speedup |" << std::endl;
        std::cout << "|:--------------------------------------|:----------:|:---------:|:-------:|" << std::endl;
        std::cout << "| [1] CPU Scalar (1 Core)               | " << std::setw(7) << (int)fps_scalar << " FPS | " << std::fixed << std::setprecision(2) << std::setw(5) << gpix_scalar << "     |   1.00x |" << std::endl;
        std::cout << "| [2] CPU Legolas DLI (1 Core NEON)     | " << std::setw(7) << (int)fps_simd << " FPS | " << std::fixed << std::setprecision(2) << std::setw(5) << gpix_simd << "     | " << std::setw(6) << std::setprecision(2) << (time_scalar_ms / time_simd_ms) << "x |" << std::endl;
        std::cout << "| [3] CPU Legolas Parallel + DLI (8 Cores)| " << std::setw(7) << (int)fps_par << " FPS | " << std::fixed << std::setprecision(2) << std::setw(5) << gpix_par << "     | " << std::setw(6) << std::setprecision(2) << (time_scalar_ms / time_par_ms) << "x |" << std::endl;
        std::cout << "| [4] GPU Metal Compute (M1 Max 32 Cores)| " << std::setw(7) << (int)fps_gpu << " FPS | " << std::fixed << std::setprecision(2) << std::setw(5) << gpix_gpu << "     | " << std::setw(6) << std::setprecision(2) << (time_scalar_ms / time_gpu_ms) << "x |" << std::endl;
        std::cout << "========================================================" << std::endl;
    }
    return 0;
}
