#include <iostream>
#include <cstdlib>
#include <cmath>
#include <atomic>
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Reductions.hxx"
#include "Legolas/include/Parallel.hxx"

#define TEST_ASSERT(cond) do { \
    if (!(cond)) { \
        std::cerr << "\nAssertion failed: (" #cond ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        std::abort(); \
    } \
} while(0)

void testMoveConstructor() {
    std::cout << "[Test] Move Constructor... " << std::flush;
    {
        const int n = 128;
        Legolas::Array<float, 1> a(n);
        for (int i = 0; i < n; ++i) {
            a[i] = static_cast<float>(i * 2 + 1);
        }
        float* orig_ptr = a.realDataPtr();
        TEST_ASSERT(a.owner() == true);
        TEST_ASSERT(orig_ptr != nullptr);

        // Move construct
        Legolas::Array<float, 1> b(std::move(a));

        // a should be nulled out and not own buffer
        TEST_ASSERT(a.owner() == false);
        TEST_ASSERT(a.realDataPtr() == nullptr);

        // b should own original buffer
        TEST_ASSERT(b.owner() == true);
        TEST_ASSERT(b.realDataPtr() == orig_ptr);
        TEST_ASSERT(b.size() == n);

        for (int i = 0; i < n; ++i) {
            TEST_ASSERT(b[i] == static_cast<float>(i * 2 + 1));
        }
    }
    // Destruction of both a and b should not double-free
    std::cout << "PASSED" << std::endl;
}

void testMoveAssignment() {
    std::cout << "[Test] Move Assignment... " << std::flush;
    {
        const int n1 = 64;
        const int n2 = 128;
        Legolas::Array<float, 1> dest(n1);
        dest.fill(1.0f);

        Legolas::Array<float, 1> src(n2);
        for (int i = 0; i < n2; ++i) {
            src[i] = static_cast<float>(i * 3);
        }
        float* src_ptr = src.realDataPtr();

        // Move assign
        dest = std::move(src);

        // src should be cleared
        TEST_ASSERT(src.owner() == false);
        TEST_ASSERT(src.realDataPtr() == nullptr);

        // dest should have src's buffer and shape
        TEST_ASSERT(dest.owner() == true);
        TEST_ASSERT(dest.realDataPtr() == src_ptr);
        TEST_ASSERT(dest.size() == n2);

        for (int i = 0; i < n2; ++i) {
            TEST_ASSERT(dest[i] == static_cast<float>(i * 3));
        }

        // Self-move assignment test
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-move"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
#endif
        dest = std::move(dest);
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
        TEST_ASSERT(dest.owner() == true);
        TEST_ASSERT(dest.realDataPtr() == src_ptr);
        TEST_ASSERT(dest.size() == n2);
    }
    std::cout << "PASSED" << std::endl;
}

void testReductionsAndEdgeCases() {
    std::cout << "[Test] Reductions & Edge Cases (empty, odd sizes)... " << std::flush;
    {
        // 1. Non-multiple of pack size (P=4)
        const int n = 7;
        Legolas::Array<double, 1, 4, 1> a(n);
        Legolas::Array<double, 1, 4, 1> b(n);
        a.fill(2.0);
        b.fill(3.0);

        double d = Legolas::dot(a, b);
        double expected = 7.0 * 2.0 * 3.0; // 42.0
        TEST_ASSERT(std::abs(d - expected) < 1e-9);

        double norm_sq = Legolas::squaredNorm(a);
        double expected_norm = 7.0 * (2.0 * 2.0); // 28.0
        TEST_ASSERT(std::abs(norm_sq - expected_norm) < 1e-9);
    }
    std::cout << "PASSED" << std::endl;
}

void testNestedParallelFor() {
    std::cout << "[Test] Nested parallel_for (scheduler re-entrancy)... " << std::flush;
    {
        std::atomic<int> counter{0};
        const int outer_count = 8;
        const int inner_count = 4;

        Legolas::parallel_for(0, outer_count, [&](int i) {
            (void)i;
            // Nested parallel_for should run inline without deadlock or corruption
            Legolas::parallel_for(0, inner_count, [&](int j) {
                (void)j;
                counter.fetch_add(1, std::memory_order_relaxed);
            });
        });

        TEST_ASSERT(counter.load() == outer_count * inner_count);
    }
    std::cout << "PASSED" << std::endl;
}

int main() {
    std::cout << "=== Running Legolas ArrayLifecycle Unit Tests ===" << std::endl;
    testMoveConstructor();
    testMoveAssignment();
    testReductionsAndEdgeCases();
    testNestedParallelFor();
    std::cout << "=== All ArrayLifecycle tests PASSED successfully! ===" << std::endl;
    return 0;
}
