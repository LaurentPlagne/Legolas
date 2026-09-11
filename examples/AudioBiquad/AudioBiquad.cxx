/**
 * AudioBiquad.cxx - Multi-Channel Digital Audio IIR Biquad Filtering with Legolas++
 *
 * Direct Form I / II Biquad filters are fundamental building blocks in Digital Audio Workstations
 * (equalizers, crossovers, speaker correction, Dolby Atmos multichannel processing).
 *
 * The standard difference equation is:
 *   y[n] = b0 * x[n] + b1 * x[n-1] + b2 * x[n-2] - a1 * y[n-1] - a2 * y[n-2]
 *
 * WHY COMPILERS FAIL TO VECTORIZE:
 * The loop across audio samples n has a strict recursive feedback dependency (y[n] depends on y[n-1]).
 * No auto-vectorizer can vectorize across time!
 *
 * LEGOLAS DLI SOLUTION:
 * Modern audio engines process multiple audio channels simultaneously (stereo, 7.1 surround, multitrack).
 * Legolas interleaves P channels into SIMD vectors:
 *   Legolas::Array<float, 2, PACK_SIZE, 2> (M channels, N audio samples per channel)
 *
 * The inner sample loop remains strictly sequential across time, but computes P audio channels
 * simultaneously in hardware SIMD registers (NEON / AVX).
 */

#include <iostream>
#include <vector>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iomanip>
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

// Generic Biquad IIR Audio Filter
struct BiquadFilter {
    float b0, b1, b2;
    float a1, a2;

    BiquadFilter(float b0_, float b1_, float b2_, float a1_, float a2_)
        : b0(b0_), b1(b1_), b2(b2_), a1(a1_), a2(a2_) {}

    template <class InA2D, class OutA2D>
    void operator()(int begin, int end, InA2D input, OutA2D output) const {
        typedef typename InA2D::RealType Scalar;

        Scalar c_b0(b0), c_b1(b1), c_b2(b2);
        Scalar c_a1(a1), c_a2(a2);

        for (int ch = begin; ch < end; ++ch) {
            auto in_track = input[ch];
            auto out_track = output[ch];
            const int num_samples = in_track.size();

            Scalar x1(0.0f), x2(0.0f);
            Scalar y1(0.0f), y2(0.0f);

            for (int n = 0; n < num_samples; ++n) {
                Scalar x0 = in_track[n];
                // Direct Form I Biquad recurrence
                Scalar y0 = c_b0 * x0 + c_b1 * x1 + c_b2 * x2 - c_a1 * y1 - c_a2 * y2;

                out_track[n] = y0;

                // State shift
                x2 = x1;
                x1 = x0;
                y2 = y1;
                y1 = y0;
            }
        }
    }
};

int main(int argc, char* argv[]) {
    std::cout << "========================================================" << std::endl;
    std::cout << " Legolas++ Audio DSP: Multi-Channel IIR Biquad Filter    " << std::endl;
    std::cout << "========================================================" << std::endl;

    const int num_channels = 64;           // 64 audio tracks in a studio session
    const int sample_rate = 96000;         // 96 kHz high-resolution audio
    const int duration_sec = 10;           // 10 seconds of audio buffer
    const int num_samples = sample_rate * duration_sec; // 960,000 samples per track
    const int num_runs = 20;

    // Butterworth 2nd-order lowpass filter at fc = 1000 Hz, Q = 0.707
    const float omega = 2.0f * M_PI * 1000.0f / sample_rate;
    const float alpha = std::sin(omega) / (2.0f * 0.7071f);
    const float cos_w = std::cos(omega);

    const float b0_norm = (1.0f - cos_w) / 2.0f;
    const float b1_norm = 1.0f - cos_w;
    const float b2_norm = (1.0f - cos_w) / 2.0f;
    const float a0_norm = 1.0f + alpha;
    const float a1_norm = -2.0f * cos_w;
    const float a2_norm = 1.0f - alpha;

    BiquadFilter filter(b0_norm / a0_norm, b1_norm / a0_norm, b2_norm / a0_norm,
                        a1_norm / a0_norm, a2_norm / a0_norm);

    std::cout << "Configuration:" << std::endl;
    std::cout << "  Audio Tracks:   " << num_channels << " channels" << std::endl;
    std::cout << "  Sample Rate:    " << sample_rate / 1000 << " kHz (960,000 samples/track)" << std::endl;
    std::cout << "  Total Samples:  " << (double(num_channels) * num_samples) / 1e6 << " Megasamples" << std::endl;
    std::cout << "  FLOPs per run:  " << (8.0 * num_channels * num_samples) / 1e6 << " MFLOPs" << std::endl;
    std::cout << std::endl;

    // ---------------------------------------------------------------
    // 1. Scalar Baseline (P=1)
    // ---------------------------------------------------------------
    using ScalarAudio = Legolas::Array<float, 2>;
    ScalarAudio in_scalar(num_channels, num_samples);
    ScalarAudio out_scalar(num_channels, num_samples);

    // Fill with synthetic multi-frequency audio
    for (int ch = 0; ch < num_channels; ++ch) {
        for (int n = 0; n < num_samples; ++n) {
            in_scalar[ch][n] = std::sin(2.0f * M_PI * (440.0f + ch * 10.0f) * n / sample_rate);
        }
    }
    out_scalar.fill(0.0f);

    // Warmup
    Legolas::map(filter, in_scalar, out_scalar);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < num_runs; ++r) {
        Legolas::map(filter, in_scalar, out_scalar);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    double time_scalar_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double msps_scalar = (double(num_channels) * num_samples) / (time_scalar_ms * 1e3); // MSamples/sec
    double gflops_scalar = (8.0 * num_channels * num_samples) / (time_scalar_ms * 1e6);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[1] Scalar Baseline (P=1):" << std::endl;
    std::cout << "    Time:       " << time_scalar_ms << " ms" << std::endl;
    std::cout << "    Throughput: " << msps_scalar << " MSamples/s (" << gflops_scalar << " GFlops)" << std::endl;

    // ---------------------------------------------------------------
    // 2. Legolas NEON SIMD (P=4, DLI multi-channel)
    // ---------------------------------------------------------------
    using NeonAudio = Legolas::Array<float, 2, 4, 2>;
    NeonAudio in_neon(num_channels, num_samples);
    NeonAudio out_neon(num_channels, num_samples);

    for (int ch = 0; ch < num_channels; ++ch) {
        for (int n = 0; n < num_samples; ++n) {
            in_neon[ch][n] = in_scalar[ch][n];
        }
    }
    out_neon.fill(0.0f);

    // Warmup
    Legolas::map(filter, in_neon, out_neon);

    t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < num_runs; ++r) {
        Legolas::map(filter, in_neon, out_neon);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double time_neon_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double msps_neon = (double(num_channels) * num_samples) / (time_neon_ms * 1e3);
    double gflops_neon = (8.0 * num_channels * num_samples) / (time_neon_ms * 1e6);
    double speedup_neon = time_scalar_ms / time_neon_ms;

    std::cout << "\n[2] Legolas NEON SIMD (P=4, 4 Audio Channels Interleaved):" << std::endl;
    std::cout << "    Time:       " << time_neon_ms << " ms" << std::endl;
    std::cout << "    Throughput: " << msps_neon << " MSamples/s (" << gflops_neon << " GFlops)" << std::endl;
    std::cout << "    Speedup:    " << speedup_neon << "x vs Scalar Baseline" << std::endl;

    // ---------------------------------------------------------------
    // 3. Legolas Parallel + NEON SIMD (Multi-Core)
    // ---------------------------------------------------------------
    NeonAudio out_par(num_channels, num_samples);
    out_par.fill(0.0f);

    Legolas::parmap(filter, in_neon, out_par);

    t0 = std::chrono::high_resolution_clock::now();
    for (int r = 0; r < num_runs; ++r) {
        Legolas::parmap(filter, in_neon, out_par);
    }
    t1 = std::chrono::high_resolution_clock::now();
    double time_par_ms = std::chrono::duration<double, std::milli>(t1 - t0).count() / num_runs;
    double msps_par = (double(num_channels) * num_samples) / (time_par_ms * 1e3);
    double gflops_par = (8.0 * num_channels * num_samples) / (time_par_ms * 1e6);
    double speedup_par = time_scalar_ms / time_par_ms;

    std::cout << "\n[3] Legolas Parallel + NEON SIMD (Multi-Core Work-Stealing):" << std::endl;
    std::cout << "    Time:       " << time_par_ms << " ms" << std::endl;
    std::cout << "    Throughput: " << msps_par << " MSamples/s (" << gflops_par << " GFlops)" << std::endl;
    std::cout << "    Speedup:    " << speedup_par << "x vs Scalar Baseline" << std::endl;

    // ---------------------------------------------------------------
    // 4. Verification
    // ---------------------------------------------------------------
    double max_diff = 0.0;
    for (int ch = 0; ch < num_channels; ++ch) {
        for (int n = 0; n < num_samples; ++n) {
            double diff = std::abs(out_scalar[ch][n] - out_neon[ch][n]);
            if (diff > max_diff) max_diff = diff;
        }
    }

    std::cout << "\n--------------------------------------------------------" << std::endl;
    std::cout << "Validation Max Error: " << max_diff << std::endl;
    if (max_diff < 1e-4) {
        std::cout << "[SUCCESS] Multi-Channel IIR Biquad output matches scalar reference!" << std::endl;
        return 0;
    } else {
        std::cerr << "[FAILURE] Output discrepancy detected!" << std::endl;
        return 1;
    }
}
