#!/usr/bin/env bash
# ==============================================================================
# scripts/run_cloud_bench.sh
# 
# One-shot benchmark runner for Cloud Instances (AWS EC2, Hetzner, Scaleway, GCP)
# Supports:
#   * Intel Xeon Scalable (Ice Lake / Sapphire Rapids) - AVX-512 (P=16)
#   * AMD EPYC (Milan / Genoa / Bergamo) - AVX2 / AVX-512
#   * ARM Neoverse (AWS Graviton 3/4, Ampere Altra) - SVE / NEON
# ==============================================================================

set -euo pipefail

echo "======================================================================"
echo " Legolas++ Cloud Cross-Architecture Benchmark Setup"
echo "======================================================================"

# 1. Detect Environment
ARCH=$(uname -m)
OS=$(uname -s)
echo "[INFO] OS: ${OS} | Architecture: ${ARCH}"

if [ -f /proc/cpuinfo ]; then
    MODEL_NAME=$(grep -m1 "model name" /proc/cpuinfo | cut -d: -f2 | sed 's/^[ \t]*//' || true)
    if [ -z "$MODEL_NAME" ]; then
        MODEL_NAME=$(lscpu | grep "Model name:" | cut -d: -f2 | sed 's/^[ \t]*//' || true)
    fi
    echo "[INFO] CPU: ${MODEL_NAME}"
    echo "[INFO] Flags: $(grep -m1 "flags" /proc/cpuinfo | grep -o -E '(avx512f|avx2|avx|neon|sve|sve2)' | sort -u | tr '\n' ' ')"
fi

# 2. Install Packages (Debian / Ubuntu)
if command -v apt-get &> /dev/null; then
    echo "[INFO] Installing build tools via apt..."
    sudo apt-get update -qq
    sudo apt-get install -y -qq build-essential cmake ninja-build python3 python3-pip
fi

# 3. Create Clean Output Directory
BENCH_DIR="cloud_bench_$(date +%Y%m%d_%H%M%S)"
mkdir -p "${BENCH_DIR}"

echo "[INFO] Building and verifying native zero-dependency Legolas++..."
cmake -B build_bench -DCMAKE_BUILD_TYPE=Release
cmake --build build_bench -j
ctest --test-dir build_bench --output-on-failure

echo "[INFO] Running full Thomas resolution curve..."
python3 tst/MultiThomas/plotPerfModern.py

echo "======================================================================"
echo " Benchmarking Complete!"
echo " Reports generated:"
echo "   * benchmarks_backend_comparison.json"
echo "   * Thomas_comparison.svg / .png"
echo "   * Thomas_speedup.svg / .png"
echo "   * tst/MultiThomas/benchmarks_report.html"
echo "======================================================================"
