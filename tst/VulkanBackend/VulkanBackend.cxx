#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Legolas/Array/Array.hxx"
#include "Legolas/Vulkan/Vulkan.hxx"

// Linking Legolas::Vulkan must propagate the feature macro so that core code
// can guard optional integration.
#ifndef LEGOLAS_HAS_VULKAN
#  error "Legolas::Vulkan must define LEGOLAS_HAS_VULKAN"
#endif

#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        std::cerr << "\nAssertion failed: (" #cond ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        std::abort(); \
    } \
} while(0)

static bool closeEnough(double a, double b, double relTol) {
    const double scale = std::fabs(a) > 1.0 ? std::fabs(a) : 1.0;
    return std::fabs(a - b) <= relTol * scale;
}

template <class A, class B>
static double maxAbsDiff(const A& a, const B& b) {
    double worst = 0.0;
    for (int i = 0; i < a.size(); ++i) {
        const double d = std::fabs(double(a[i]) - double(b[i]));
        if (d > worst) worst = d;
    }
    return worst;
}

// The policy layer must be usable (and side-effect free) with or without a
// Vulkan driver: this is what allows core code to compile and run everywhere.
static void testBackendPolicy() {
    std::cout << "[Test] Backend policy... " << std::flush;

    TEST_ASSERT(std::string(Legolas::Vulkan::backendName(Legolas::Vulkan::Backend::CPU)) == "CPU");
    TEST_ASSERT(std::string(Legolas::Vulkan::backendName(Legolas::Vulkan::Backend::Vulkan)) == "Vulkan");
    TEST_ASSERT(std::string(Legolas::Vulkan::backendName(Legolas::Vulkan::Backend::Auto)) == "Auto");

    const bool device = Legolas::Vulkan::available();
    if (std::getenv("LEGOLAS_DISABLE_VULKAN") != nullptr) {
        TEST_ASSERT(!device);
    }
    TEST_ASSERT(device == Legolas::Vulkan::Context::instance().available());
    TEST_ASSERT(Legolas::Vulkan::preferredBackend() ==
                (device ? Legolas::Vulkan::Backend::Vulkan : Legolas::Vulkan::Backend::CPU));

    TEST_ASSERT(!Legolas::Vulkan::shouldUseGpu(Legolas::Vulkan::Backend::CPU, 1u << 30));
    TEST_ASSERT(Legolas::Vulkan::shouldUseGpu(Legolas::Vulkan::Backend::Vulkan, 1));
    TEST_ASSERT(Legolas::Vulkan::minGpuElements() > 0);

    std::cout << "PASSED (device: " << (device ? "available" : "none") << ")" << std::endl;
}

// Backend::CPU must be bit-for-bit identical to the core CPU reductions.
static void testCpuPolicy() {
    std::cout << "[Test] CPU policy matches core reductions... " << std::flush;

    const int n = 4096;
    Legolas::Array<float, 1> a(n), b(n);
    for (int i = 0; i < n; ++i) {
        a[i] = std::sin(0.01f * float(i));
        b[i] = std::cos(0.02f * float(i));
    }

    const double normRef = Legolas::squaredNorm(a);
    const double dotRef = Legolas::dot(a, b);

    TEST_ASSERT(Legolas::Vulkan::squaredNorm(a, Legolas::Vulkan::Backend::CPU) == normRef);
    TEST_ASSERT(Legolas::Vulkan::dot(a, b, Legolas::Vulkan::Backend::CPU) == dotRef);

    std::cout << "PASSED" << std::endl;
}

// Forcing the GPU on a device-less machine must fall back to the CPU, not
// fail or return garbage.
static void testForcedVulkanFallback() {
    std::cout << "[Test] Forced Vulkan falls back to CPU... " << std::flush;

    const int n = 8192;
    Legolas::Array<float, 1> a(n), b(n);
    for (int i = 0; i < n; ++i) {
        a[i] = 0.5f + float(i % 37) * 0.25f;
        b[i] = 1.0f + float(i % 53) * 0.125f;
    }

    const double normRef = Legolas::squaredNorm(a);
    const double dotRef = Legolas::dot(a, b);

    const double norm = Legolas::Vulkan::squaredNorm(a, Legolas::Vulkan::Backend::Vulkan);
    const double dot = Legolas::Vulkan::dot(a, b, Legolas::Vulkan::Backend::Vulkan);

    TEST_ASSERT(closeEnough(norm, normRef, 1e-4));
    TEST_ASSERT(closeEnough(dot, dotRef, 1e-4));

    std::cout << "PASSED" << std::endl;
}

// Non-eligible arrays (here: double) must transparently stay on the CPU.
static void testIneligibleArrayType() {
    std::cout << "[Test] Non-float arrays stay on CPU... " << std::flush;

    const int n = 1024;
    Legolas::Array<double, 1> a(n);
    for (int i = 0; i < n; ++i) a[i] = double(i) * 0.5;

    const double ref = Legolas::squaredNorm(a);
    const double value = Legolas::Vulkan::squaredNorm(a, Legolas::Vulkan::Backend::Vulkan);
    TEST_ASSERT(value == ref);

    std::cout << "PASSED" << std::endl;
}

// Lazy expressions have no scalar data pointer: the bridge must fall back to
// the CPU reduction instead of failing to compile.
static void testExpressionFallback() {
    std::cout << "[Test] Lazy expressions stay on CPU... " << std::flush;

    const int n = 2048;
    Legolas::Array<float, 1> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = 0.5f + float(i % 31) * 0.125f;
    }

    const double normRef = Legolas::squaredNorm(a * a);
    const double norm = Legolas::Vulkan::squaredNorm(a * a, Legolas::Vulkan::Backend::Vulkan);

    TEST_ASSERT(norm == normRef);

    std::cout << "PASSED" << std::endl;
}

// The core API must auto-dispatch when the backend is linked: with a device
// and a large enough array it must match the forced-GPU result, otherwise it
// must stay on the CPU.
static void testAutomaticCoreDispatch() {
    std::cout << "[Test] Automatic core dispatch... " << std::flush;

    const size_t n = size_t(1) << 20;  // >= default minGpuElements()
    const int count = static_cast<int>(n);
    Legolas::Array<float, 1> a(count), b(count);
    for (int i = 0; i < count; ++i) {
        a[i] = std::sin(0.001f * float(i));
        b[i] = 1.0f + 0.25f * std::cos(0.002f * float(i));
    }

    const double autoNorm = Legolas::squaredNorm(a);
    const double autoDot = Legolas::dot(a, b);
    const double cpuNorm = Legolas::Vulkan::squaredNorm(a, Legolas::Vulkan::Backend::CPU);
    const double cpuDot = Legolas::Vulkan::dot(a, b, Legolas::Vulkan::Backend::CPU);

    if (Legolas::Vulkan::available()) {
        const double gpuNorm = Legolas::Vulkan::squaredNorm(a, Legolas::Vulkan::Backend::Vulkan);
        const double gpuDot = Legolas::Vulkan::dot(a, b, Legolas::Vulkan::Backend::Vulkan);
        TEST_ASSERT(autoNorm == gpuNorm);
        TEST_ASSERT(autoDot == gpuDot);
        TEST_ASSERT(closeEnough(autoNorm, cpuNorm, 1e-4));
        TEST_ASSERT(closeEnough(autoDot, cpuDot, 1e-4));
    } else {
        TEST_ASSERT(autoNorm == cpuNorm);
        TEST_ASSERT(autoDot == cpuDot);
    }

    // Below the threshold Auto must stay on the CPU (bit-exact match).
    const int small = 1024;
    Legolas::Array<float, 1> s(small);
    for (int i = 0; i < small; ++i) s[i] = 0.25f * float(i % 13);
    TEST_ASSERT(Legolas::squaredNorm(s) ==
                Legolas::Vulkan::squaredNorm(s, Legolas::Vulkan::Backend::CPU));

    std::cout << "PASSED" << std::endl;
}

// Automatic GLSL generation from expression templates + runtime SPIR-V
// compilation. Without a shader compiler (or without a device) evaluate()
// must fall back to the CPU assignment.
static void testExpressionEvaluation() {
    std::cout << "[Test] Expression evaluation (GLSL generated at runtime)... " << std::flush;

    const int n = 4096;
    Legolas::Array<float, 1> a(n), b(n), c(n), out(n), refMul(n), refScaled(n);
    for (int i = 0; i < n; ++i) {
        a[i] = std::sin(0.01f * float(i));
        b[i] = 1.0f + 0.25f * std::cos(0.02f * float(i));
        c[i] = 0.5f * float(i % 7);
    }

    refMul = a * b + c;
    const bool gpuMul = Legolas::Vulkan::evaluate(a * b + c, out,
                                                  Legolas::Vulkan::Backend::Vulkan);
    if (gpuMul) {
        TEST_ASSERT(maxAbsDiff(out, refMul) < 1e-5);
    } else {
        for (int i = 0; i < n; ++i) TEST_ASSERT(out[i] == refMul[i]);
    }

    refScaled = 0.5f * a + b;
    const bool gpuScaled = Legolas::Vulkan::evaluate(0.5f * a + b, out,
                                                     Legolas::Vulkan::Backend::Vulkan);
    if (gpuScaled) {
        TEST_ASSERT(maxAbsDiff(out, refScaled) < 1e-5);
    } else {
        for (int i = 0; i < n; ++i) TEST_ASSERT(out[i] == refScaled[i]);
    }

    // Mixed expression covering MinusArray, ScaledArray and a repeated leaf
    // (a*a must be bound once).
    Legolas::Array<float, 1> refMix(n);
    refMix = a * a - 0.25f * b;
    const bool gpuMix = Legolas::Vulkan::evaluate(a * a - 0.25f * b, out,
                                                  Legolas::Vulkan::Backend::Vulkan);
    if (gpuMix) {
        TEST_ASSERT(maxAbsDiff(out, refMix) < 1e-5);
    } else {
        for (int i = 0; i < n; ++i) TEST_ASSERT(out[i] == refMix[i]);
    }

    // GLSL generation is compiler-independent: check the emitted source.
    {
        Legolas::Vulkan::detail::GlslProgram program;
        Legolas::Vulkan::detail::GlslEmitter<decltype(a * a - 0.25f * b)>::emit(
            a * a - 0.25f * b, program);
        TEST_ASSERT(program.inputs.size() == 2);  // a (deduplicated) and b
        const std::string source = Legolas::Vulkan::detail::buildGlslSource(program);
        TEST_ASSERT(source.find("readonly buffer In0") != std::string::npos);
        TEST_ASSERT(source.find("readonly buffer In1") != std::string::npos);
        TEST_ASSERT(source.find("readonly buffer In2") == std::string::npos);
        TEST_ASSERT(source.find("outv[gid]") != std::string::npos);
        TEST_ASSERT(source.find("params.n") != std::string::npos);
    }

    // Non-eligible (double) expressions must not attempt the GPU path.
    Legolas::Array<double, 1> da(n), dout(n);
    for (int i = 0; i < n; ++i) da[i] = double(i) * 0.25;
    TEST_ASSERT(!Legolas::Vulkan::evaluate(da * da, dout,
                                           Legolas::Vulkan::Backend::Vulkan));

    // Resident device output, when both a device and a compiler are present.
    if (Legolas::Vulkan::available() &&
        Legolas::Vulkan::runtimeShaderCompilerAvailable()) {
        Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();
        Legolas::Vulkan::DeviceArray<float> device(ctx, size_t(n));
        TEST_ASSERT(device.valid());
        TEST_ASSERT(Legolas::Vulkan::evaluate(ctx, a * b + c, device));
        Legolas::Array<float, 1> fromDevice(n);
        TEST_ASSERT(Legolas::Vulkan::toHost(device, fromDevice));
        TEST_ASSERT(maxAbsDiff(fromDevice, refMul) < 1e-5);
    }

    std::cout << (gpuMul ? "PASSED (GPU)" : "PASSED (CPU fallback)") << std::endl;
}

static void testDeviceArrayAndGpuReductions() {
    std::cout << "[Test] DeviceArray bridge and GPU reductions... " << std::flush;

    if (!Legolas::Vulkan::available()) {
        std::cout << "SKIPPED (no Vulkan device)" << std::endl;
        return;
    }

    Legolas::Vulkan::Context& ctx = Legolas::Vulkan::Context::instance();

    const size_t n = size_t(1) << 20;
    std::vector<float> hostA(n), hostB(n);
    for (size_t i = 0; i < n; ++i) {
        hostA[i] = std::sin(0.001f * float(i));
        hostB[i] = 1.0f + 0.25f * std::cos(0.002f * float(i));
    }

    Legolas::Vulkan::DeviceArray<float> devA = Legolas::Vulkan::toDevice(ctx, hostA);
    Legolas::Vulkan::DeviceArray<float> devB = Legolas::Vulkan::toDevice(ctx, hostB);
    TEST_ASSERT(devA.valid());
    TEST_ASSERT(devB.valid());
    TEST_ASSERT(devA.size() == n);

    std::vector<float> roundTrip(n);
    TEST_ASSERT(Legolas::Vulkan::toHost(devA, roundTrip));
    for (size_t i = 0; i < n; ++i) {
        TEST_ASSERT(roundTrip[i] == hostA[i]);
    }

    double gpuNorm = 0.0;
    double gpuDot = 0.0;
    TEST_ASSERT(Legolas::Vulkan::squaredNorm(devA, &gpuNorm));
    TEST_ASSERT(Legolas::Vulkan::dot(devA, devB, &gpuDot));

    const int count = static_cast<int>(n);
    Legolas::Array<float, 1> a(count), b(count);
    for (int i = 0; i < count; ++i) {
        a[i] = hostA[size_t(i)];
        b[i] = hostB[size_t(i)];
    }
    const double cpuNorm = Legolas::squaredNorm(a);
    const double cpuDot = Legolas::dot(a, b);

    TEST_ASSERT(closeEnough(gpuNorm, cpuNorm, 1e-4));
    TEST_ASSERT(closeEnough(gpuDot, cpuDot, 1e-4));

    // Auto policy routes a large float array through the device and the core
    // array bridge must agree with the raw device result.
    const double bridged = Legolas::Vulkan::squaredNorm(a);
    TEST_ASSERT(closeEnough(bridged, cpuNorm, 1e-4));

    // The generic kernel API is usable directly from core code.
    Legolas::Vulkan::Kernel finalKernel(ctx, Legolas::Vulkan::spv::reduce_final,
                                        Legolas::Vulkan::spv::reduce_final_words, 2,
                                        sizeof(uint32_t));
    TEST_ASSERT(finalKernel.valid());

    std::cout << "PASSED" << std::endl;
}

int main() {
    std::cout << "=== Legolas::Vulkan generic backend tests ===" << std::endl;

    testBackendPolicy();
    testCpuPolicy();
    testForcedVulkanFallback();
    testIneligibleArrayType();
    testExpressionFallback();
    testAutomaticCoreDispatch();
    testExpressionEvaluation();
    testDeviceArrayAndGpuReductions();

    std::cout << "All Vulkan backend tests passed." << std::endl;
    return 0;
}
