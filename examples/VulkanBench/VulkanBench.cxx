/**
 * VulkanBench.cxx - Legolas++ GPU (Vulkan) vs CPU benchmark suite.
 *
 * Workloads:
 *   1. VideoPipeline  - 3x3 Sobel + temporal differencing (32 x 720p streams)
 *   2. DepthwiseConv  - 3x3 depthwise convolution (128 channels, 112x112)
 *   3. MultiThomas    - ensemble of independent tridiagonal systems
 *   4. OptionPricing  - Black-Scholes Crank-Nicolson portfolio (16384 options)
 *   5. Reductions     - squaredNorm / dot product (16.7M floats)
 *
 * For each workload the CPU scalar, CPU DLI SIMD, CPU multi-core and Vulkan
 * GPU variants are measured. GPU kernel times exclude host<->device transfers
 * (buffers stay resident); upload/download are reported separately.
 */

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"
#include "Legolas/Vulkan/Vulkan.hxx"

using Clock = std::chrono::high_resolution_clock;

#if defined(__AVX512F__)
static const int NATIVE_PACK = 16;
static const char* NATIVE_SIMD = "AVX-512";
#elif defined(__AVX2__)
static const int NATIVE_PACK = 8;
static const char* NATIVE_SIMD = "AVX2";
#elif defined(__ARM_NEON)
static const int NATIVE_PACK = 4;
static const char* NATIVE_SIMD = "NEON";
#else
static const int NATIVE_PACK = 4;
static const char* NATIVE_SIMD = "generic";
#endif

static double elapsedMs(Clock::time_point a, Clock::time_point b) {
  return std::chrono::duration<double, std::milli>(b - a).count();
}

template <class F>
static double cpuTimeMinMs(F f, int samples = 5, int inner = 5) {
  f();
  double best = 1e300;
  for (int s = 0; s < samples; ++s) {
    auto t0 = Clock::now();
    for (int i = 0; i < inner; ++i) f();
    auto t1 = Clock::now();
    best = std::min(best, elapsedMs(t0, t1) / double(inner));
  }
  return best;
}

// f(repeat) must record and submit `repeat` back-to-back dispatches.
template <class F>
static double gpuTimeMinMs(F f, int samples = 5, int inner = 10) {
  // Warm up GPU clocks and caches before sampling: discrete GPUs downclock
  // aggressively when idle, and the benchmark alternates long CPU-only
  // phases with short GPU bursts. A time-based warmup is more robust than a
  // fixed iteration count.
  const auto warmStart = Clock::now();
  do {
    f(inner);
  } while (elapsedMs(warmStart, Clock::now()) < 250.0);

  double best = 1e300;
  for (int s = 0; s < samples; ++s) {
    auto t0 = Clock::now();
    f(inner);
    auto t1 = Clock::now();
    best = std::min(best, elapsedMs(t0, t1) / double(inner));
  }
  return best;
}

static double maxAbsDiff(const float* a, const float* b, size_t n) {
  double worst = 0.0;
  for (size_t i = 0; i < n; ++i) {
    double d = std::fabs(double(a[i]) - double(b[i]));
    if (d > worst) worst = d;
  }
  return worst;
}

static void printHeader(const char* title) {
  std::cout << "\n================================================================================\n";
  std::cout << " " << title << "\n";
  std::cout << "================================================================================\n";
}

// ===========================================================================
// 1. VideoPipeline: 3x3 Sobel edge detection + temporal motion differencing
// ===========================================================================
struct VideoSpatialTemporalPipeline {
  int H, W;
  VideoSpatialTemporalPipeline(int h, int w) : H(h), W(w) {}

  template <class InCurrA2D, class InPrevA2D, class OutA2D>
  void operator()(int begin, int end, InCurrA2D curr_frames, InPrevA2D prev_frames,
                  OutA2D out_frames) const {
    typedef typename InCurrA2D::RealType Scalar;
    const Scalar alpha(0.7f);
    const Scalar beta(0.3f);

    for (int s = begin; s < end; ++s) {
      auto curr = curr_frames[s];
      auto prev = prev_frames[s];
      auto out = out_frames[s];

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

static void benchVideo() {
  printHeader("1. VideoPipeline: Sobel + temporal motion (32 streams, 720p)");
  const int NUM_STREAMS = 32;
  const int H = 720;
  const int W = 1280;
  const int PIX = H * W;
  const size_t TOTAL = size_t(NUM_STREAMS) * size_t(PIX);

  Legolas::Array<float, 2> curr(NUM_STREAMS, PIX);
  Legolas::Array<float, 2> prev(NUM_STREAMS, PIX);
  Legolas::Array<float, 2> out(NUM_STREAMS, PIX);

  for (int s = 0; s < NUM_STREAMS; ++s) {
    for (int p = 0; p < PIX; ++p) {
      curr[s][p] = std::sin(0.01f * p + 0.5f * s);
      prev[s][p] = std::sin(0.01f * p + 0.5f * s + 0.05f);
    }
  }
  out.fill(0.0f);

  VideoSpatialTemporalPipeline pipeline(H, W);

  double tScalar = cpuTimeMinMs([&] { Legolas::map(pipeline, curr, prev, out); }, 5, 3);

  using SimdArray = Legolas::Array<float, 2, NATIVE_PACK, 2>;
  SimdArray currS(NUM_STREAMS, PIX), prevS(NUM_STREAMS, PIX), outS(NUM_STREAMS, PIX);
  for (int s = 0; s < NUM_STREAMS; ++s)
    for (int p = 0; p < PIX; ++p) {
      currS[s][p] = curr[s][p];
      prevS[s][p] = prev[s][p];
    }
  outS.fill(0.0f);

  double tSimd = cpuTimeMinMs([&] { Legolas::map(pipeline, currS, prevS, outS); }, 5, 3);
  double tPar = cpuTimeMinMs([&] { Legolas::parmap(pipeline, currS, prevS, outS); }, 5, 3);

  double tGpuKernel = -1.0, tUpload = -1.0, tDownload = -1.0, diff = -1.0;
  bool gpuOk = false;

  Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();
  if (ctx.available()) {
    Legolas::Vulkan::Buffer dCurr(ctx, TOTAL * sizeof(float));
    Legolas::Vulkan::Buffer dPrev(ctx, TOTAL * sizeof(float));
    Legolas::Vulkan::Buffer dOut(ctx, TOTAL * sizeof(float));
    Legolas::Vulkan::Kernel kernel(ctx, Legolas::Vulkan::spv::video_pipeline,
                                   Legolas::Vulkan::spv::video_pipeline_words, 3, 8);

    // Warm up the reusable staging buffer, then measure steady-state upload.
    dCurr.upload(curr.realDataPtr(), TOTAL * sizeof(float));
    dPrev.upload(prev.realDataPtr(), TOTAL * sizeof(float));
    auto t0 = Clock::now();
    dCurr.upload(curr.realDataPtr(), TOTAL * sizeof(float));
    dPrev.upload(prev.realDataPtr(), TOTAL * sizeof(float));
    auto t1 = Clock::now();
    tUpload = elapsedMs(t0, t1);

    struct Params { int H; int W; } params{H, W};
    const Legolas::Vulkan::Buffer* buffers[3] = {&dCurr, &dPrev, &dOut};
    const uint32_t gx = (W + 15) / 16, gy = (H + 15) / 16, gz = NUM_STREAMS;

    if (kernel.valid()) {
      tGpuKernel = gpuTimeMinMs(
          [&](int rep) { kernel.dispatch(buffers, 3, gx, gy, gz, &params, sizeof(params), rep); },
          5, 10);

      kernel.dispatch(buffers, 3, gx, gy, gz, &params, sizeof(params), 1);
      std::vector<float> gpuOut(TOTAL);
      dOut.download(gpuOut.data(), TOTAL * sizeof(float));  // warm staging
      t0 = Clock::now();
      dOut.download(gpuOut.data(), TOTAL * sizeof(float));
      t1 = Clock::now();
      tDownload = elapsedMs(t0, t1);

      diff = maxAbsDiff(gpuOut.data(), out.realDataPtr(), TOTAL);
      gpuOk = true;
    }
  }

  const double scalarFps = NUM_STREAMS / (tScalar / 1000.0);
  const double simdFps = NUM_STREAMS / (tSimd / 1000.0);
  const double parFps = NUM_STREAMS / (tPar / 1000.0);
  const double gpuFps = tGpuKernel > 0 ? NUM_STREAMS / (tGpuKernel / 1000.0) : 0.0;

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "| Variant                 | Time (ms) | Throughput (FPS) | GPix/s | Speedup vs scalar |\n";
  std::cout << "|:------------------------|----------:|-----------------:|-------:|------------------:|\n";
  std::cout << "| CPU scalar (1 core)     | " << std::setw(9) << tScalar << " | "
            << std::setw(16) << scalarFps << " | " << std::setw(6)
            << (double(TOTAL) / (tScalar / 1000.0) / 1e9) << " | " << std::setw(17) << 1.00 << " |\n";
  std::cout << "| CPU DLI " << NATIVE_SIMD << " (1 core)   | " << std::setw(9) << tSimd << " | "
            << std::setw(16) << simdFps << " | " << std::setw(6)
            << (double(TOTAL) / (tSimd / 1000.0) / 1e9) << " | " << std::setw(17)
            << (tScalar / tSimd) << " |\n";
  std::cout << "| CPU parallel + DLI      | " << std::setw(9) << tPar << " | "
            << std::setw(16) << parFps << " | " << std::setw(6)
            << (double(TOTAL) / (tPar / 1000.0) / 1e9) << " | " << std::setw(17)
            << (tScalar / tPar) << " |\n";
  if (gpuOk) {
    std::cout << "| Vulkan GPU (kernel)     | " << std::setw(9) << tGpuKernel << " | "
              << std::setw(16) << gpuFps << " | " << std::setw(6)
              << (double(TOTAL) / (tGpuKernel / 1000.0) / 1e9) << " | " << std::setw(17)
              << (tScalar / tGpuKernel) << " |\n";
    std::cout << "GPU upload: " << tUpload << " ms, download: " << tDownload
              << " ms, max|diff| vs scalar: " << std::scientific << diff << std::fixed << "\n";
  }
  std::cout << "CSV,VideoPipeline,scalar," << tScalar << ",FPS," << scalarFps << "\n";
  std::cout << "CSV,VideoPipeline,dli," << tSimd << ",FPS," << simdFps << "\n";
  std::cout << "CSV,VideoPipeline,parallel," << tPar << ",FPS," << parFps << "\n";
  if (gpuOk) std::cout << "CSV,VideoPipeline,vulkan," << tGpuKernel << ",FPS," << gpuFps << "\n";
}

// ===========================================================================
// 2. DepthwiseConv: 3x3 depthwise convolution
// ===========================================================================
struct DepthwiseConv2D {
  int H, W;
  DepthwiseConv2D(int h, int w) : H(h), W(w) {}

  template <class InA2D, class OutA2D>
  void operator()(int begin, int end, InA2D input, OutA2D output) const {
    typedef typename InA2D::RealType Scalar;

    Scalar k00(1.0f / 16.0f), k01(2.0f / 16.0f), k02(1.0f / 16.0f);
    Scalar k10(2.0f / 16.0f), k11(4.0f / 16.0f), k12(2.0f / 16.0f);
    Scalar k20(1.0f / 16.0f), k21(2.0f / 16.0f), k22(1.0f / 16.0f);

    for (int c = begin; c < end; ++c) {
      auto in_chan = input[c];
      auto out_chan = output[c];

      for (int r = 1; r < H - 1; ++r) {
        const int row_prev = (r - 1) * W;
        const int row_curr = r * W;
        const int row_next = (r + 1) * W;

        for (int col = 1; col < W - 1; ++col) {
          Scalar sum = k00 * in_chan[row_prev + col - 1] + k01 * in_chan[row_prev + col] +
                       k02 * in_chan[row_prev + col + 1] + k10 * in_chan[row_curr + col - 1] +
                       k11 * in_chan[row_curr + col] + k12 * in_chan[row_curr + col + 1] +
                       k20 * in_chan[row_next + col - 1] + k21 * in_chan[row_next + col] +
                       k22 * in_chan[row_next + col + 1];
          out_chan[row_curr + col] = sum;
        }
      }
    }
  }
};

static void benchDepthwiseConv() {
  printHeader("2. DepthwiseConv: 3x3 depthwise convolution (128 channels, 112x112)");
  const int C = 128;
  const int H = 112;
  const int W = 112;
  const int S = H * W;
  const double flops = 18.0 * (H - 2) * (W - 2) * C;

  Legolas::Array<float, 2> in(C, S), out(C, S);
  in.fill(1.0f);
  out.fill(0.0f);

  DepthwiseConv2D conv(H, W);
  double tScalar = cpuTimeMinMs([&] { Legolas::map(conv, in, out); }, 5, 20);

  using SimdArray = Legolas::Array<float, 2, NATIVE_PACK, 2>;
  SimdArray inS(C, S), outS(C, S);
  for (int c = 0; c < C; ++c)
    for (int s = 0; s < S; ++s) inS[c][s] = in[c][s];
  outS.fill(0.0f);

  double tSimd = cpuTimeMinMs([&] { Legolas::map(conv, inS, outS); }, 5, 20);
  double tPar = cpuTimeMinMs([&] { Legolas::parmap(conv, inS, outS); }, 5, 20);

  double tGpuKernel = -1.0, diff = -1.0;
  bool gpuOk = false;

  Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();
  if (ctx.available()) {
    const size_t total = size_t(C) * size_t(S);
    Legolas::Vulkan::Buffer dIn(ctx, total * sizeof(float));
    Legolas::Vulkan::Buffer dOut(ctx, total * sizeof(float));
    Legolas::Vulkan::Kernel kernel(ctx, Legolas::Vulkan::spv::depthwise_conv,
                                   Legolas::Vulkan::spv::depthwise_conv_words, 2, 8);
    dIn.upload(in.realDataPtr(), total * sizeof(float));

    struct Params { int H; int W; } params{H, W};
    const Legolas::Vulkan::Buffer* buffers[2] = {&dIn, &dOut};
    const uint32_t gx = (W + 15) / 16, gy = (H + 15) / 16, gz = C;

    if (kernel.valid()) {
      tGpuKernel = gpuTimeMinMs(
          [&](int rep) { kernel.dispatch(buffers, 2, gx, gy, gz, &params, sizeof(params), rep); },
          5, 20);

      kernel.dispatch(buffers, 2, gx, gy, gz, &params, sizeof(params), 1);
      std::vector<float> gpuOut(total);
      dOut.download(gpuOut.data(), total * sizeof(float));
      diff = maxAbsDiff(gpuOut.data(), out.realDataPtr(), total);
      gpuOk = true;
    }
  }

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "| Variant                 | Time (ms) | GFlops | Speedup vs scalar |\n";
  std::cout << "|:------------------------|----------:|-------:|------------------:|\n";
  std::cout << "| CPU scalar (1 core)     | " << std::setw(9) << tScalar << " | "
            << std::setw(6) << (flops / (tScalar * 1e6)) << " | " << std::setw(17) << 1.00 << " |\n";
  std::cout << "| CPU DLI " << NATIVE_SIMD << " (1 core)   | " << std::setw(9) << tSimd << " | "
            << std::setw(6) << (flops / (tSimd * 1e6)) << " | " << std::setw(17)
            << (tScalar / tSimd) << " |\n";
  std::cout << "| CPU parallel + DLI      | " << std::setw(9) << tPar << " | "
            << std::setw(6) << (flops / (tPar * 1e6)) << " | " << std::setw(17)
            << (tScalar / tPar) << " |\n";
  if (gpuOk) {
    std::cout << "| Vulkan GPU (kernel)     | " << std::setw(9) << tGpuKernel << " | "
              << std::setw(6) << (flops / (tGpuKernel * 1e6)) << " | " << std::setw(17)
              << (tScalar / tGpuKernel) << " |\n";
    std::cout << "max|diff| vs scalar: " << std::scientific << diff << std::fixed << "\n";
  }
  std::cout << "CSV,DepthwiseConv,scalar," << tScalar << ",GFlops," << (flops / (tScalar * 1e6)) << "\n";
  std::cout << "CSV,DepthwiseConv,dli," << tSimd << ",GFlops," << (flops / (tSimd * 1e6)) << "\n";
  std::cout << "CSV,DepthwiseConv,parallel," << tPar << ",GFlops," << (flops / (tPar * 1e6)) << "\n";
  if (gpuOk) std::cout << "CSV,DepthwiseConv,vulkan," << tGpuKernel << ",GFlops," << (flops / (tGpuKernel * 1e6)) << "\n";
}

// ===========================================================================
// 3. MultiThomas: ensemble of tridiagonal systems
// ===========================================================================
struct ThomasSolver {
  template <class A2D>
  void operator()(int begin, int end, A2D D2D, A2D U2D, A2D L2D, A2D B2D, A2D X2D) const {
    typedef typename A2D::Element Element;
    typedef typename A2D::RealType Scalar;
    Element S(X2D[0].shape());
    Scalar one(1.0), s, sm1;

    for (int j = begin; j < end; j++) {
      auto D = D2D[j];
      auto U = U2D[j];
      auto L = L2D[j];
      auto B = B2D[j];
      auto X = X2D[j];
      s = D[0];
      sm1 = one / s;
      const int size = X.size();
      X[0] = B[0] * sm1;
      for (int i = 1; i < size; i++) {
        S[i] = U[i - 1] * sm1;
        s = D[i] - L[i] * S[i];
        X[i] = B[i] - L[i] * X[i - 1];
        sm1 = one / s;
        X[i] *= sm1;
      }
      for (int i = (size - 2); i >= 0; i--) {
        X[i] -= S[i + 1] * X[i + 1];
      }
    }
  }
};

static void benchMultiThomas() {
  printHeader("3. MultiThomas: independent tridiagonal systems (13*Nx*Ny flops)");
  const std::vector<int> sizes = {8, 16, 32, 64, 128, 256, 512};

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "| Nx  | Systems  | CPU P=1 seq | CPU P=" << NATIVE_PACK
            << " seq | CPU P=" << NATIVE_PACK << " par | Vulkan GPU | GPU speedup (vs par) | max|diff| |\n";
  std::cout << "|----:|---------:|------------:|-------------:|--------------:|-----------:|---------------------:|-----------|\n";

  for (int nx : sizes) {
    const int ny = nx * nx;
    const double flops = 13.0 * nx * ny;

    Legolas::Array<float, 2> X(ny, nx), B(ny, nx), U(ny, nx), D(ny, nx), L(ny, nx);
    X.fill(1.0f);
    B.fill(1.0f);
    U.fill(-1.0f);
    D.fill(2.0f);
    L.fill(-1.0f);

    ThomasSolver solver;
    double tP1 = cpuTimeMinMs([&] { Legolas::map(solver, D, U, L, B, X); }, 3, 5);

    using PackedArray = Legolas::Array<float, 2, NATIVE_PACK, 2>;
    PackedArray Xp(ny, nx), Bp(ny, nx), Up(ny, nx), Dp(ny, nx), Lp(ny, nx);
    Xp.fill(1.0f);
    Bp.fill(1.0f);
    Up.fill(-1.0f);
    Dp.fill(2.0f);
    Lp.fill(-1.0f);

    double tPn = cpuTimeMinMs([&] { Legolas::map(solver, Dp, Up, Lp, Bp, Xp); }, 3, 5);
    double tPar = cpuTimeMinMs([&] { Legolas::parmap(solver, Dp, Up, Lp, Bp, Xp); }, 3, 5);

    double tGpu = -1.0, diff = -1.0;
    bool gpuOk = false;

    Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();
    if (ctx.available()) {
      const size_t n = size_t(nx) * size_t(ny);
      std::vector<float> hD(n, 2.0f), hU(n, -1.0f), hL(n, -1.0f), hB(n, 1.0f), hX(n, 1.0f);
      // hD/hU/hL/hB are constant: already filled in [i][j] layout.
      (void)hD; (void)hU; (void)hL; (void)hB; (void)hX;

      Legolas::Vulkan::Buffer dD(ctx, n * sizeof(float));
      Legolas::Vulkan::Buffer dU(ctx, n * sizeof(float));
      Legolas::Vulkan::Buffer dL(ctx, n * sizeof(float));
      Legolas::Vulkan::Buffer dB(ctx, n * sizeof(float));
      Legolas::Vulkan::Buffer dX(ctx, n * sizeof(float));
      Legolas::Vulkan::Buffer dS(ctx, n * sizeof(float));
      Legolas::Vulkan::Kernel kernel(ctx, Legolas::Vulkan::spv::thomas,
                                     Legolas::Vulkan::spv::thomas_words, 6, 8);

      dD.upload(hD.data(), n * sizeof(float));
      dU.upload(hU.data(), n * sizeof(float));
      dL.upload(hL.data(), n * sizeof(float));
      dB.upload(hB.data(), n * sizeof(float));
      dX.upload(hX.data(), n * sizeof(float));

      struct Params { int N; int ny; } params{nx, ny};
      const Legolas::Vulkan::Buffer* buffers[6] = {&dD, &dU, &dL, &dB, &dX, &dS};
      const uint32_t gx = ((uint32_t(ny) / 4) + 255) / 256;

      if (kernel.valid()) {
        tGpu = gpuTimeMinMs(
            [&](int rep) { kernel.dispatch(buffers, 6, gx, 1, 1, &params, sizeof(params), rep); },
            3, 5);

        kernel.dispatch(buffers, 6, gx, 1, 1, &params, sizeof(params), 1);
        std::vector<float> gpuX(n);
        dX.download(gpuX.data(), n * sizeof(float));

        // Compare a few systems (CPU result in X).
        double worst = 0.0;
        for (int j = 0; j < std::min(ny, 64); ++j) {
          for (int i = 0; i < nx; ++i) {
            double d = std::fabs(double(gpuX[size_t(i) * ny + j]) - double(X[j][i]));
            if (d > worst) worst = d;
          }
        }
        diff = worst;
        gpuOk = true;
      }
    }

    std::cout << "| " << std::setw(3) << nx << " | " << std::setw(8) << ny << " | "
              << std::setw(11) << (flops / (tP1 * 1e6)) << " | " << std::setw(12)
              << (flops / (tPn * 1e6)) << " | " << std::setw(13) << (flops / (tPar * 1e6)) << " | ";
    if (gpuOk) {
      std::cout << std::setw(10) << (flops / (tGpu * 1e6)) << " | " << std::setw(20)
                << (tPar / tGpu) << " | " << std::setw(9) << std::scientific << diff
                << std::fixed << " |\n";
    } else {
      std::cout << "         - |                    - |         - |\n";
    }

    std::cout << "CSV,MultiThomas," << nx << ",P1_seq," << (flops / (tP1 * 1e6)) << "\n";
    std::cout << "CSV,MultiThomas," << nx << ",P" << NATIVE_PACK << "_seq," << (flops / (tPn * 1e6)) << "\n";
    std::cout << "CSV,MultiThomas," << nx << ",P" << NATIVE_PACK << "_par," << (flops / (tPar * 1e6)) << "\n";
    if (gpuOk) std::cout << "CSV,MultiThomas," << nx << ",vulkan," << (flops / (tGpu * 1e6)) << "\n";
  }
}

// ===========================================================================
// 4. OptionPricing: Black-Scholes Crank-Nicolson portfolio
// ===========================================================================
struct BlackScholesSolver {
  int N;
  int Nt;
  BlackScholesSolver(int n, int nt) : N(n), Nt(nt) {}

  template <class A2D>
  void operator()(int begin, int end, A2D V2D, A2D D2D, A2D U2D, A2D L2D, A2D RHS2D) const {
    typedef typename A2D::Element Element;
    typedef typename A2D::RealType Scalar;

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

      for (int t = 0; t < Nt; ++t) {
        for (int i = 1; i < N - 1; ++i) {
          RHS[i] = (-L[i]) * V[i - 1] + (two - D[i]) * V[i] + (-U[i]) * V[i + 1];
        }
        RHS[0] = V[0];
        RHS[N - 1] = V[N - 1];

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

        for (int i = N - 2; i >= 0; --i) {
          V[i] -= S_diag[i + 1] * V[i + 1];
        }
      }
    }
  }
};

static void benchOptionPricing() {
  printHeader("4. OptionPricing: Black-Scholes Crank-Nicolson (16384 options)");
  const int NUM_OPTIONS = 16384;
  const int N = 128;
  const int Nt = 50;
  const float S_max = 300.0f;
  const float dS = S_max / float(N - 1);
  const float dt = 1.0f / float(Nt);
  const float r = 0.05f;

  std::vector<float> hV(size_t(NUM_OPTIONS) * N);
  std::vector<float> hD(size_t(NUM_OPTIONS) * N);
  std::vector<float> hU(size_t(NUM_OPTIONS) * N);
  std::vector<float> hL(size_t(NUM_OPTIONS) * N);
  std::vector<float> hS(size_t(NUM_OPTIONS) * N, 0.0f);

  // Build transposed [i][option] arrays directly, matching the CPU contract.
  for (int m = 0; m < NUM_OPTIONS; ++m) {
    const float K = 80.0f + 40.0f * (float(m) / float(NUM_OPTIONS));
    const float sigma = 0.15f + 0.30f * (float(m % 256) / 256.0f);
    const float sig2 = sigma * sigma;

    std::vector<float> dCoef(N, 0.0f), uCoef(N, 0.0f), lCoef(N, 0.0f);
    for (int i = 1; i < N - 1; ++i) {
      const float fi = float(i);
      const float alpha = 0.5f * sig2 * fi * fi - 0.5f * r * fi;
      const float beta = -(sig2 * fi * fi + r);
      const float gamma = 0.5f * sig2 * fi * fi + 0.5f * r * fi;
      lCoef[i] = -0.5f * dt * alpha;
      dCoef[i] = 1.0f - 0.5f * dt * beta;
      uCoef[i] = -0.5f * dt * gamma;
    }
    dCoef[0] = 1.0f; uCoef[0] = 0.0f; lCoef[0] = 0.0f;
    dCoef[N - 1] = 1.0f; uCoef[N - 1] = 0.0f; lCoef[N - 1] = 0.0f;

    for (int i = 0; i < N; ++i) {
      const float Si = float(i) * dS;
      const size_t idx = size_t(i) * NUM_OPTIONS + m;
      hV[idx] = std::max(Si - K, 0.0f);
      hD[idx] = dCoef[i];
      hU[idx] = uCoef[i];
      hL[idx] = lCoef[i];
    }
  }

  // CPU arrays in natural [option][i] layout.
  Legolas::Array<float, 2> V_cpu(NUM_OPTIONS, N), D_cpu(NUM_OPTIONS, N);
  Legolas::Array<float, 2> U_cpu(NUM_OPTIONS, N), L_cpu(NUM_OPTIONS, N);
  Legolas::Array<float, 2> R_cpu(NUM_OPTIONS, N);
  for (int m = 0; m < NUM_OPTIONS; ++m)
    for (int i = 0; i < N; ++i) {
      V_cpu[m][i] = hV[size_t(i) * NUM_OPTIONS + m];
      D_cpu[m][i] = hD[size_t(i) * NUM_OPTIONS + m];
      U_cpu[m][i] = hU[size_t(i) * NUM_OPTIONS + m];
      L_cpu[m][i] = hL[size_t(i) * NUM_OPTIONS + m];
    }

  BlackScholesSolver pricer(N, Nt);
  double tP1 = cpuTimeMinMs([&] { Legolas::map(pricer, V_cpu, D_cpu, U_cpu, L_cpu, R_cpu); }, 3, 1);

  using PackedArray = Legolas::Array<float, 2, NATIVE_PACK, 2>;
  PackedArray V_p(NUM_OPTIONS, N), D_p(NUM_OPTIONS, N);
  PackedArray U_p(NUM_OPTIONS, N), L_p(NUM_OPTIONS, N), R_p(NUM_OPTIONS, N);
  for (int m = 0; m < NUM_OPTIONS; ++m)
    for (int i = 0; i < N; ++i) {
      V_p[m][i] = hV[size_t(i) * NUM_OPTIONS + m];
      D_p[m][i] = hD[size_t(i) * NUM_OPTIONS + m];
      U_p[m][i] = hU[size_t(i) * NUM_OPTIONS + m];
      L_p[m][i] = hL[size_t(i) * NUM_OPTIONS + m];
    }

  double tPn = cpuTimeMinMs([&] { Legolas::map(pricer, V_p, D_p, U_p, L_p, R_p); }, 3, 1);
  double tPar = cpuTimeMinMs([&] { Legolas::parmap(pricer, V_p, D_p, U_p, L_p, R_p); }, 3, 1);

  double tGpu = -1.0, diff = -1.0;
  bool gpuOk = false;

  Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();
  if (ctx.available()) {
    const size_t n = size_t(NUM_OPTIONS) * N;
    Legolas::Vulkan::Buffer dV(ctx, n * sizeof(float));
    Legolas::Vulkan::Buffer dD(ctx, n * sizeof(float));
    Legolas::Vulkan::Buffer dU(ctx, n * sizeof(float));
    Legolas::Vulkan::Buffer dL(ctx, n * sizeof(float));
    Legolas::Vulkan::Buffer dR(ctx, n * sizeof(float));
    Legolas::Vulkan::Buffer dS(ctx, n * sizeof(float));
    Legolas::Vulkan::Kernel kernel(ctx, Legolas::Vulkan::spv::option_pricing,
                                   Legolas::Vulkan::spv::option_pricing_words, 6, 12);

    dV.upload(hV.data(), n * sizeof(float));
    dD.upload(hD.data(), n * sizeof(float));
    dU.upload(hU.data(), n * sizeof(float));
    dL.upload(hL.data(), n * sizeof(float));

    struct Params { int N; int Nt; int ny; } params{N, Nt, NUM_OPTIONS};
    const Legolas::Vulkan::Buffer* buffers[6] = {&dV, &dD, &dU, &dL, &dR, &dS};
    const uint32_t gx = (uint32_t(NUM_OPTIONS) + 255) / 256;

    if (kernel.valid()) {
      tGpu = gpuTimeMinMs(
          [&](int rep) { kernel.dispatch(buffers, 6, gx, 1, 1, &params, sizeof(params), rep); },
          3, 1);

      // Reset the device state and run a single solve for the accuracy check.
      dV.upload(hV.data(), n * sizeof(float));
      kernel.dispatch(buffers, 6, gx, 1, 1, &params, sizeof(params), 1);
      std::vector<float> gpuV(n);
      dV.download(gpuV.data(), n * sizeof(float));

      // Re-run CPU once from the same initial state for the accuracy check.
      for (int m = 0; m < NUM_OPTIONS; ++m)
        for (int i = 0; i < N; ++i) V_cpu[m][i] = hV[size_t(i) * NUM_OPTIONS + m];
      Legolas::map(pricer, V_cpu, D_cpu, U_cpu, L_cpu, R_cpu);

      double worst = 0.0;
      for (int m = 0; m < 64; ++m)
        for (int i = 0; i < N; ++i) {
          double d = std::fabs(double(gpuV[size_t(i) * NUM_OPTIONS + m]) - double(V_cpu[m][i]));
          if (d > worst) worst = d;
        }
      diff = worst;
      gpuOk = true;
    }
  }

  const double solves = double(NUM_OPTIONS) * Nt;
  std::cout << std::fixed << std::setprecision(3);
  std::cout << "| Variant                 | Time (ms) | Options/s | Tridiag solves/s | Speedup |\n";
  std::cout << "|:------------------------|----------:|----------:|-----------------:|--------:|\n";
  std::cout << "| CPU scalar (1 core)     | " << std::setw(9) << tP1 << " | " << std::setw(9)
            << (NUM_OPTIONS / (tP1 / 1000.0)) << " | " << std::setw(16)
            << (solves / (tP1 / 1000.0)) << " | " << std::setw(7) << 1.00 << " |\n";
  std::cout << "| CPU DLI " << NATIVE_SIMD << " (1 core)   | " << std::setw(9) << tPn << " | "
            << std::setw(9) << (NUM_OPTIONS / (tPn / 1000.0)) << " | " << std::setw(16)
            << (solves / (tPn / 1000.0)) << " | " << std::setw(7) << (tP1 / tPn) << " |\n";
  std::cout << "| CPU parallel + DLI      | " << std::setw(9) << tPar << " | " << std::setw(9)
            << (NUM_OPTIONS / (tPar / 1000.0)) << " | " << std::setw(16)
            << (solves / (tPar / 1000.0)) << " | " << std::setw(7) << (tP1 / tPar) << " |\n";
  if (gpuOk) {
    std::cout << "| Vulkan GPU (kernel)     | " << std::setw(9) << tGpu << " | " << std::setw(9)
              << (NUM_OPTIONS / (tGpu / 1000.0)) << " | " << std::setw(16)
              << (solves / (tGpu / 1000.0)) << " | " << std::setw(7) << (tP1 / tGpu) << " |\n";
    std::cout << "GPU speedup vs CPU parallel: " << (tPar / tGpu)
              << "x, max|diff| vs scalar: " << std::scientific << diff << std::fixed << "\n";
  }
  std::cout << "CSV,OptionPricing,scalar," << tP1 << ",options_per_s," << (NUM_OPTIONS / (tP1 / 1000.0)) << "\n";
  std::cout << "CSV,OptionPricing,dli," << tPn << ",options_per_s," << (NUM_OPTIONS / (tPn / 1000.0)) << "\n";
  std::cout << "CSV,OptionPricing,parallel," << tPar << ",options_per_s," << (NUM_OPTIONS / (tPar / 1000.0)) << "\n";
  if (gpuOk) std::cout << "CSV,OptionPricing,vulkan," << tGpu << ",options_per_s," << (NUM_OPTIONS / (tGpu / 1000.0)) << "\n";
}

// ===========================================================================
// 5. AudioBiquad: 64-track IIR biquad filter (blocked scan on GPU)
// ===========================================================================
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
        Scalar y0 = c_b0 * x0 + c_b1 * x1 + c_b2 * x2 - c_a1 * y1 - c_a2 * y2;

        out_track[n] = y0;

        x2 = x1;
        x1 = x0;
        y2 = y1;
        y1 = y0;
      }
    }
  }
};

struct Mat2 {
  double m[4];
};

static Mat2 matMul(const Mat2& a, const Mat2& b) {
  Mat2 r;
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      r.m[i * 2 + j] = a.m[i * 2] * b.m[j] + a.m[i * 2 + 1] * b.m[2 + j];
  return r;
}

static Mat2 matPow(Mat2 base, int e) {
  Mat2 r{{1.0, 0.0, 0.0, 1.0}};
  while (e > 0) {
    if (e & 1) r = matMul(r, base);
    base = matMul(base, base);
    e >>= 1;
  }
  return r;
}

static void benchAudioBiquad() {
  printHeader("5. AudioBiquad: 64-track IIR biquad (blocked scan on GPU)");
  const int num_channels = 64;
  const int sample_rate = 96000;
  const int num_samples = sample_rate * 10;

  const float omega = 2.0f * float(M_PI) * 1000.0f / sample_rate;
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

  Legolas::Array<float, 2> in(num_channels, num_samples);
  Legolas::Array<float, 2> out(num_channels, num_samples);
  for (int ch = 0; ch < num_channels; ++ch)
    for (int n = 0; n < num_samples; ++n)
      in[ch][n] = std::sin(0.01f * n + 0.3f * ch) + 0.4f * std::sin(0.23f * n + ch);
  out.fill(0.0f);

  double tScalar = cpuTimeMinMs([&] { Legolas::map(filter, in, out); }, 3, 3);

  using SimdArray = Legolas::Array<float, 2, NATIVE_PACK, 2>;
  SimdArray inS(num_channels, num_samples), outS(num_channels, num_samples);
  for (int ch = 0; ch < num_channels; ++ch)
    for (int n = 0; n < num_samples; ++n) inS[ch][n] = in[ch][n];
  outS.fill(0.0f);

  double tSimd = cpuTimeMinMs([&] { Legolas::map(filter, inS, outS); }, 3, 3);
  double tPar = cpuTimeMinMs([&] { Legolas::parmap(filter, inS, outS); }, 3, 3);

  double tGpu = -1.0, diff = -1.0;
  bool gpuOk = false;

  Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();
  if (ctx.available()) {
    const int L = 2048;
    const int K = (num_samples + L - 1) / L;
    const int lastLen = num_samples - (K - 1) * L;

    const double a1 = double(filter.a1);
    const double a2 = double(filter.a2);
    Mat2 M{{-a1, -a2, 1.0, 0.0}};
    Mat2 mFull = matPow(M, L);
    Mat2 mLast = matPow(M, lastLen);

    const size_t total = size_t(num_channels) * size_t(num_samples);
    Legolas::Vulkan::Buffer dIn(ctx, total * sizeof(float));
    Legolas::Vulkan::Buffer dOut(ctx, total * sizeof(float));
    Legolas::Vulkan::Buffer dC(ctx, size_t(num_channels) * K * 2 * sizeof(float));
    Legolas::Vulkan::Buffer dState(ctx, size_t(num_channels) * K * 2 * sizeof(float));
    dIn.upload(in.realDataPtr(), total * sizeof(float));

    Legolas::Vulkan::Kernel k1(ctx, Legolas::Vulkan::spv::biquad_block_states,
                               Legolas::Vulkan::spv::biquad_block_states_words, 2, 36);
    Legolas::Vulkan::Kernel k2(ctx, Legolas::Vulkan::spv::biquad_block_prefix,
                               Legolas::Vulkan::spv::biquad_block_prefix_words, 2, 40);
    Legolas::Vulkan::Kernel k3(ctx, Legolas::Vulkan::spv::biquad_block_apply,
                               Legolas::Vulkan::spv::biquad_block_apply_words, 3, 36);

    struct BlockParams {
      int N, L, C, K;
      float b0, b1, b2, a1, a2;
    } p{num_samples, L, num_channels, K,
         filter.b0, filter.b1, filter.b2, filter.a1, filter.a2};
    struct PrefixParams {
      int K, C;
      float m00, m01, m10, m11, l00, l01, l10, l11;
    } pp{K, num_channels,
         float(mFull.m[0]), float(mFull.m[1]), float(mFull.m[2]), float(mFull.m[3]),
         float(mLast.m[0]), float(mLast.m[1]), float(mLast.m[2]), float(mLast.m[3])};

    const Legolas::Vulkan::Buffer* b1[2] = {&dIn, &dC};
    const Legolas::Vulkan::Buffer* b2[2] = {&dC, &dState};
    const Legolas::Vulkan::Buffer* b3[3] = {&dIn, &dState, &dOut};
    const uint32_t gx = (uint32_t(K) + 127) / 128;
    const uint32_t gxC = (uint32_t(num_channels) + 63) / 64;

    auto runGpu = [&]() {
      k1.dispatch(b1, 2, gx, num_channels, 1, &p, sizeof(p), 1);
      k2.dispatch(b2, 2, gxC, 1, 1, &pp, sizeof(pp), 1);
      k3.dispatch(b3, 3, gx, num_channels, 1, &p, sizeof(p), 1);
    };

    if (k1.valid() && k2.valid() && k3.valid()) {
      tGpu = gpuTimeMinMs([&](int) { runGpu(); }, 5, 3);

      runGpu();
      std::vector<float> gpuOut(total);
      dOut.download(gpuOut.data(), total * sizeof(float));
      diff = maxAbsDiff(gpuOut.data(), out.realDataPtr(), total);
      gpuOk = true;
    }
  }

  const double mspsScalar = (double(num_channels) * num_samples) / (tScalar * 1e3);
  const double mspsSimd = (double(num_channels) * num_samples) / (tSimd * 1e3);
  const double mspsPar = (double(num_channels) * num_samples) / (tPar * 1e3);
  const double mspsGpu = gpuOk ? (double(num_channels) * num_samples) / (tGpu * 1e3) : 0.0;

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "| Variant                 | Time (ms) | MSamples/s | Speedup vs scalar |\n";
  std::cout << "|:------------------------|----------:|-----------:|------------------:|\n";
  std::cout << "| CPU scalar (1 core)     | " << std::setw(9) << tScalar << " | " << std::setw(10)
            << mspsScalar << " | " << std::setw(17) << 1.00 << " |\n";
  std::cout << "| CPU DLI " << NATIVE_SIMD << " (1 core)   | " << std::setw(9) << tSimd << " | "
            << std::setw(10) << mspsSimd << " | " << std::setw(17) << (tScalar / tSimd) << " |\n";
  std::cout << "| CPU parallel + DLI      | " << std::setw(9) << tPar << " | " << std::setw(10)
            << mspsPar << " | " << std::setw(17) << (tScalar / tPar) << " |\n";
  if (gpuOk) {
    std::cout << "| Vulkan GPU (3 kernels)  | " << std::setw(9) << tGpu << " | " << std::setw(10)
              << mspsGpu << " | " << std::setw(17) << (tScalar / tGpu) << " |\n";
    std::cout << "GPU speedup vs CPU parallel: " << (tPar / tGpu)
              << "x, max|diff| vs scalar: " << std::scientific << diff << std::fixed << "\n";
  }
  std::cout << "CSV,AudioBiquad,scalar," << tScalar << ",MSamples_per_s," << mspsScalar << "\n";
  std::cout << "CSV,AudioBiquad,dli," << tSimd << ",MSamples_per_s," << mspsSimd << "\n";
  std::cout << "CSV,AudioBiquad,parallel," << tPar << ",MSamples_per_s," << mspsPar << "\n";
  if (gpuOk) std::cout << "CSV,AudioBiquad,vulkan," << tGpu << ",MSamples_per_s," << mspsGpu << "\n";
}

// ===========================================================================
// 6. Reductions: squaredNorm and dot product
// ===========================================================================
static void benchReductions() {
  printHeader("6. Reductions: squaredNorm / dot (16.7M floats, 64 MB per array)");
  const size_t n = size_t(1) << 24;

  Legolas::Array<float, 1> a(n), b(n);
  for (size_t i = 0; i < n; ++i) {
    a[i] = std::sin(0.001f * float(i));
    b[i] = std::cos(0.002f * float(i));
  }

  double cpuNorm = Legolas::squaredNorm(a);
  double cpuDot = Legolas::dot(a, b);
  double tNormCpu = cpuTimeMinMs([&] { volatile double r = Legolas::squaredNorm(a); (void)r; }, 5, 3);
  double tDotCpu = cpuTimeMinMs([&] { volatile double r = Legolas::dot(a, b); (void)r; }, 5, 3);

  double gpuNorm = 0.0, gpuDot = 0.0, tNormGpu = -1.0, tDotGpu = -1.0;
  bool gpuOk = false;

  Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();
  if (ctx.available()) {
    const uint32_t numWorkgroups = uint32_t(std::min<size_t>((n + 255) / 256, 4096));
    Legolas::Vulkan::Buffer dA(ctx, n * sizeof(float));
    Legolas::Vulkan::Buffer dB(ctx, n * sizeof(float));
    Legolas::Vulkan::Buffer dPartials(ctx, numWorkgroups * sizeof(float));
    Legolas::Vulkan::Buffer dOut(ctx, sizeof(float));
    Legolas::Vulkan::Kernel partial(ctx, Legolas::Vulkan::spv::reduce_partial,
                                    Legolas::Vulkan::spv::reduce_partial_words, 3, 8);
    Legolas::Vulkan::Kernel finalK(ctx, Legolas::Vulkan::spv::reduce_final,
                                   Legolas::Vulkan::spv::reduce_final_words, 2, 4);
    dA.upload(a.realDataPtr(), n * sizeof(float));
    dB.upload(b.realDataPtr(), n * sizeof(float));

    const Legolas::Vulkan::Buffer* pBuffers[3] = {&dA, &dB, &dPartials};
    const Legolas::Vulkan::Buffer* fBuffers[2] = {&dPartials, &dOut};

    if (partial.valid() && finalK.valid()) {
      struct P1 { uint32_t n; uint32_t mode; };
      uint32_t partialCount = numWorkgroups;

      auto run = [&](uint32_t mode, int rep) {
        P1 p{uint32_t(n), mode};
        partial.dispatch(pBuffers, 3, numWorkgroups, 1, 1, &p, sizeof(p), rep);
        finalK.dispatch(fBuffers, 2, 1, 1, 1, &partialCount, sizeof(partialCount), 1);
      };

      tNormGpu = gpuTimeMinMs([&](int rep) { run(0u, rep); }, 5, 10);
      run(0u, 1);
      float gpuNormF = 0.0f;
      dOut.download(&gpuNormF, sizeof(float));
      gpuNorm = double(gpuNormF);

      tDotGpu = gpuTimeMinMs([&](int rep) { run(1u, rep); }, 5, 10);
      run(1u, 1);
      float gpuDotF = 0.0f;
      dOut.download(&gpuDotF, sizeof(float));
      gpuDot = double(gpuDotF);
      gpuOk = true;
    }
  }

  std::cout << std::fixed << std::setprecision(3);
  std::cout << "| Reduction    | CPU time (ms) | CPU value     | GPU time (ms) | GPU value     | Speedup | Rel. error |\n";
  std::cout << "|:-------------|--------------:|--------------:|--------------:|--------------:|--------:|-----------:|\n";
  std::cout << "| squaredNorm  | " << std::setw(13) << tNormCpu << " | " << std::setw(13)
            << std::scientific << cpuNorm << std::fixed << " | " << std::setw(13)
            << (gpuOk ? tNormGpu : -1.0) << " | " << std::setw(13) << std::scientific << gpuNorm
            << std::fixed << " | " << std::setw(7)
            << (gpuOk ? tNormCpu / tNormGpu : 0.0) << " | " << std::setw(10)
            << (gpuOk ? std::fabs(gpuNorm - cpuNorm) / std::fabs(cpuNorm) : -1.0) << " |\n";
  std::cout << "| dot          | " << std::setw(13) << tDotCpu << " | " << std::setw(13)
            << std::scientific << cpuDot << std::fixed << " | " << std::setw(13)
            << (gpuOk ? tDotGpu : -1.0) << " | " << std::setw(13) << std::scientific << gpuDot
            << std::fixed << " | " << std::setw(7)
            << (gpuOk ? tDotCpu / tDotGpu : 0.0) << " | " << std::setw(10)
            << (gpuOk ? std::fabs(gpuDot - cpuDot) / std::fabs(cpuDot) : -1.0) << " |\n";
  std::cout << "CPU values are double-precision accumulations; GPU values are float (two-pass).\n";
  std::cout << "CSV,Reductions,squaredNorm,cpu," << tNormCpu << "\n";
  if (gpuOk) std::cout << "CSV,Reductions,squaredNorm,vulkan," << tNormGpu << "\n";
  std::cout << "CSV,Reductions,dot,cpu," << tDotCpu << "\n";
  if (gpuOk) std::cout << "CSV,Reductions,dot,vulkan," << tDotGpu << "\n";
}

// ===========================================================================
int main(int argc, char** argv) {
  bool skipHeavy = false;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--skip-heavy") skipHeavy = true;
  }

  std::cout << "================================================================================\n";
  std::cout << " Legolas++ Vulkan GPU Benchmark Suite\n";
  std::cout << "================================================================================\n";
  std::cout << "CPU SIMD: " << NATIVE_SIMD << " (P=" << NATIVE_PACK << ")\n";

  Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();
  if (ctx.available()) {
    const char* typeName = "other";
    switch (ctx.deviceType()) {
      case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: typeName = "discrete GPU"; break;
      case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: typeName = "integrated GPU"; break;
      case VK_PHYSICAL_DEVICE_TYPE_CPU: typeName = "CPU (software Vulkan)"; break;
      default: break;
    }
    std::cout << "Vulkan device: " << ctx.deviceName() << " (" << typeName << ")\n";
  } else {
    std::cout << "Vulkan device: NONE (GPU benchmarks skipped)\n";
  }
  std::cout << std::flush;

  benchVideo();
  benchDepthwiseConv();
  if (!skipHeavy) {
    benchMultiThomas();
    benchOptionPricing();
    benchAudioBiquad();
    benchReductions();
  }

  std::cout << "\nDone.\n";
  return 0;
}
