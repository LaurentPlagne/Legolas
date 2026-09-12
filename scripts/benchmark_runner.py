#!/usr/bin/env python3
"""
scripts/benchmark_runner.py

Automated benchmark collector & aggregator for Legolas++.
Runs benchmarks, parses output, generates structured JSON and Markdown summaries.
"""

import os
import sys
import json
import re
import platform
import subprocess
from datetime import datetime, timezone

def get_system_info():
    info = {
        "system": platform.system(),
        "machine": platform.machine(),
        "processor": platform.processor() or "Unknown",
        "timestamp": datetime.now(timezone.utc).isoformat()
    }
    if platform.system() == "Darwin":
        try:
            model = subprocess.check_output(["sysctl", "-n", "machdep.cpu.brand_string"], text=True).strip()
            info["cpu_model"] = model
            cores = subprocess.check_output(["sysctl", "-n", "hw.physicalcpu"], text=True).strip()
            info["cores"] = int(cores)
        except Exception:
            pass
    elif platform.system() == "Linux":
        try:
            with open("/proc/cpuinfo") as f:
                for line in f:
                    if "model name" in line:
                        info["cpu_model"] = line.split(":", 1)[1].strip()
                        break
            with open("/proc/cpuinfo") as f:
                flags = set()
                for line in f:
                    if "flags" in line:
                        for flag in ["avx512f", "avx2", "avx", "neon", "sve", "fma"]:
                            if flag in line:
                                flags.add(flag)
                info["cpu_flags"] = sorted(list(flags))
        except Exception:
            pass
    return info

def run_command(cmd, cwd=None):
    print(f"[*] Running: {' '.join(cmd)}")
    result = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    return result.returncode, result.stdout

def benchmark_thomas(build_dir):
    thomas_bin = os.path.join(build_dir, "tst", "MultiThomas", "MultiThomas")
    if not os.path.exists(thomas_bin):
        return None
    ret, out = run_command([thomas_bin, "64"])
    metrics = {}
    for line in out.splitlines():
        m = re.match(r"(Thomas_[A-Za-z0-9_]+):\s+([0-9.]+)\s+GFlops", line.strip())
        if m:
            metrics[m.group(1)] = float(m.group(2))
    return metrics

def benchmark_video(build_dir):
    video_bin = os.path.join(build_dir, "examples", "VideoPipeline")
    if not os.path.exists(video_bin):
        return None
    ret, out = run_command([video_bin])
    metrics = {}
    for line in out.splitlines():
        if "Throughput:" in line:
            m = re.search(r"Throughput:\s+([0-9.]+)\s+FPS", line)
            if m:
                val = float(m.group(1))
                if "scalar_fps" not in metrics:
                    metrics["scalar_fps"] = val
                elif "simd_fps" not in metrics:
                    metrics["simd_fps"] = val
                else:
                    metrics["parallel_fps"] = val
        if "Speedup:" in line:
            m = re.search(r"Speedup:\s+([0-9.]+)x", line)
            if m:
                val = float(m.group(1))
                if "simd_speedup" not in metrics:
                    metrics["simd_speedup"] = val
                else:
                    metrics["parallel_speedup"] = val
    return metrics

def benchmark_video_metal(build_dir):
    metal_bin = os.path.join(build_dir, "examples", "VideoPipelineMetal")
    if not os.path.exists(metal_bin):
        return None
    ret, out = run_command([metal_bin])
    metrics = {}
    for line in out.splitlines():
        if "Throughput:" in line:
            m = re.search(r"Throughput:\s+([0-9.]+)\s+FPS", line)
            if m:
                metrics["metal_fps"] = float(m.group(1))
        if "Speedup:" in line:
            m = re.search(r"Speedup:\s+([0-9.]+)x", line)
            if m:
                metrics["metal_speedup"] = float(m.group(1))
    return metrics

def benchmark_audio(build_dir):
    audio_bin = os.path.join(build_dir, "examples", "AudioBiquad")
    if not os.path.exists(audio_bin):
        return None
    ret, out = run_command([audio_bin])
    metrics = {}
    for line in out.splitlines():
        if "Throughput:" in line:
            m = re.search(r"Throughput:\s+([0-9.]+)\s+MSamples/s", line)
            if m:
                val = float(m.group(1))
                if "scalar_msamples" not in metrics:
                    metrics["scalar_msamples"] = val
                elif "simd_msamples" not in metrics:
                    metrics["simd_msamples"] = val
                else:
                    metrics["parallel_msamples"] = val
        if "Speedup:" in line:
            m = re.search(r"Speedup:\s+([0-9.]+)x", line)
            if m:
                val = float(m.group(1))
                if "simd_speedup" not in metrics:
                    metrics["simd_speedup"] = val
                else:
                    metrics["parallel_speedup"] = val
    return metrics

def benchmark_conv(build_dir):
    conv_bin = os.path.join(build_dir, "examples", "DepthwiseConv")
    if not os.path.exists(conv_bin):
        return None
    ret, out = run_command([conv_bin])
    metrics = {}
    for line in out.splitlines():
        if "Speed:" in line:
            m = re.search(r"Speed:\s+([0-9.]+)\s+GFlops", line)
            if m:
                val = float(m.group(1))
                if "scalar_gflops" not in metrics:
                    metrics["scalar_gflops"] = val
                elif "simd_gflops" not in metrics:
                    metrics["simd_gflops"] = val
                else:
                    metrics["parallel_gflops"] = val
        if "Speedup:" in line:
            m = re.search(r"Speedup:\s+([0-9.]+)x", line)
            if m:
                val = float(m.group(1))
                if "parallel_speedup" not in metrics:
                    metrics["parallel_speedup"] = val
    return metrics

def benchmark_options(build_dir):
    opt_bin = os.path.join(build_dir, "examples", "OptionPricing")
    if not os.path.exists(opt_bin):
        return None
    ret, out = run_command([opt_bin])
    metrics = {}
    for line in out.splitlines():
        if "Options/sec:" in line or "options/s" in line.lower():
            m = re.search(r"([0-9.,]+)\s+options?/s", line, re.IGNORECASE)
            if m:
                val = float(m.group(1).replace(",", ""))
                if "scalar_ops" not in metrics:
                    metrics["scalar_ops"] = val
                else:
                    metrics["parallel_ops"] = val
    return metrics

def benchmark_vulkan(build_dir):
    vk_bin = os.path.join(build_dir, "examples", "VulkanBench")
    if not os.path.exists(vk_bin):
        return None
    ret, out = run_command([vk_bin, "--skip-heavy"])
    metrics = {}
    for line in out.splitlines():
        if "Vulkan GPU" in line and "ms" in line:
            parts = line.split()
            # Extract kernel times or speedups if present
            metrics["raw_output"] = True
    return metrics

def generate_markdown(data):
    host = data.get("host", {})
    benches = data.get("benchmarks", {})
    cpu_str = host.get("cpu_model", f"{host.get('system')} {host.get('machine')}")
    
    lines = []
    lines.append(f"### ⚡ Legolas++ Benchmark Summary: {cpu_str}")
    lines.append(f"*Platform: {host.get('system')} {host.get('machine')} | Date: {host.get('timestamp')}*\n")
    lines.append("| Workload / Application | Baseline (Scalar) | Legolas SIMD | Legolas Parallel | Peak Speedup |")
    lines.append("| :--- | :---: | :---: | :---: | :---: |")

    # MultiThomas
    th = benches.get("MultiThomas", {})
    if th:
        p1 = th.get("Thomas_P1_Seq", 0.0)
        p_simd = th.get("Thomas_P8_Seq", th.get("Thomas_P4_Seq", 0.0))
        p_par = th.get("Thomas_P8_Par", 0.0)
        spd = f"{(p_par / p1):.2f}×" if p1 > 0 else "-"
        lines.append(f"| **MultiThomas** ($N_x=64$) | {p1:.2f} GFlops | {p_simd:.2f} GFlops | **{p_par:.2f} GFlops** | **{spd}** |")

    # VideoPipeline
    vp = benches.get("VideoPipeline", {})
    if vp:
        s_fps = vp.get("scalar_fps", 0.0)
        v_simd = vp.get("simd_fps", 0.0)
        v_par = vp.get("parallel_fps", 0.0)
        spd = f"{vp.get('parallel_speedup', 1.0):.2f}×"
        lines.append(f"| **VideoPipeline** (32×720p HD) | {s_fps:.1f} FPS | {v_simd:.1f} FPS | **{v_par:.1f} FPS** | **{spd}** |")

    # VideoPipelineMetal
    vpm = benches.get("VideoPipelineMetal", {})
    if vpm:
        m_fps = vpm.get("metal_fps", 0.0)
        m_spd = f"{vpm.get('metal_speedup', 1.0):.2f}×"
        lines.append(f"| **VideoPipeline Metal GPU** | — | — | **{m_fps:.1f} FPS** | **{m_spd}** |")

    # AudioBiquad
    ab = benches.get("AudioBiquad", {})
    if ab:
        s_ms = ab.get("scalar_msamples", 0.0)
        a_simd = ab.get("simd_msamples", 0.0)
        a_par = ab.get("parallel_msamples", 0.0)
        spd = f"{ab.get('parallel_speedup', 1.0):.2f}×"
        lines.append(f"| **AudioBiquad** (64 Tracks) | {s_ms:.1f} MS/s | {a_simd:.1f} MS/s | **{a_par:.1f} MS/s** | **{spd}** |")

    # DepthwiseConv
    dc = benches.get("DepthwiseConv", {})
    if dc:
        s_gf = dc.get("scalar_gflops", 0.0)
        c_simd = dc.get("simd_gflops", 0.0)
        c_par = dc.get("parallel_gflops", 0.0)
        spd = f"{dc.get('parallel_speedup', 1.0):.2f}×"
        lines.append(f"| **DepthwiseConv** (128ch MobileNet) | {s_gf:.2f} GFlops | {c_simd:.2f} GFlops | **{c_par:.2f} GFlops** | **{spd}** |")

    # OptionPricing
    op = benches.get("OptionPricing", {})
    if op:
        s_op = op.get("scalar_ops", 0.0)
        p_op = op.get("parallel_ops", 0.0)
        spd = f"{(p_op / s_op):.2f}×" if s_op > 0 else "-"
        lines.append(f"| **OptionPricing** (16k options) | {s_op:,.0f} opt/s | — | **{p_op:,.0f} opt/s** | **{spd}** |")

    # Roofline Model analysis
    lines.append("\n#### 📐 Roofline Efficiency & Hardware Bottleneck Analysis\n")
    lines.append("| Workload | Arithmetic Intensity ($I$) | Achieved Throughput | Physical Ceiling | Hardware Regime |")
    lines.append("| :--- | :---: | :---: | :--- | :--- |")
    if th:
        p_par = th.get("Thomas_P8_Par", 0.0)
        lines.append(f"| **MultiThomas** ($N_x=64$) | 0.65 FLOP/Byte | {p_par:.1f} GFlops | Memory Bandwidth Bound | DRAM Streaming |")
    if vp:
        fps = vp.get("parallel_fps", 0.0)
        bw_gb = (fps * 0.9216 * 12) / 1e3
        lines.append(f"| **VideoPipeline** (32×720p) | 1.50 FLOP/Byte | {bw_gb:.1f} GB/s ({fps:,.0f} FPS) | Memory Bus Saturation | Streaming Memory-Bound |")
    if ab:
        msamp = ab.get("parallel_msamples", 0.0)
        lines.append(f"| **AudioBiquad** (64ch) | 1.00 FLOP/Byte | {msamp:,.0f} MSamples/s | FMA Vector Pipe Saturation | L1/L2 Cache Resident |")
    if dc:
        lines.append(f"| **DepthwiseConv** (128ch) | 2.25 FLOP/Byte | {dc.get('parallel_gflops', 0.0):.1f} GFlops | Knee Point (L2/Compute) | Cache-Compute Balanced |")
    if op:
        lines.append(f"| **OptionPricing** (16k) | ~18.0 FLOP/Byte | {op.get('parallel_ops', 0.0):,.0f} opt/s | FMA Execution Line-Rate | L3 Cache Compute-Bound |")

    return "\n".join(lines)

def aggregate_reports(json_files, out_md):
    lines = []
    lines.append("## 🏆 Legolas++ Multi-Architecture Automated Benchmark Report\n")
    lines.append("| Workload | Metric | " + " | ".join([f"**Host {i+1}**" for i in range(len(json_files))]) + " |")
    lines.append("| :--- | :--- | " + " | ".join([":---:" for _ in json_files]) + " |")

    datasets = []
    for jf in json_files:
        with open(jf) as f:
            datasets.append(json.load(f))

    # CPU headers
    host_labels = []
    for d in datasets:
        h = d.get("host", {})
        label = h.get("cpu_model", f"{h.get('system')} {h.get('machine')}")
        host_labels.append(label)
    lines.append("| **Hardware Target** | CPU / Arch | " + " | ".join(host_labels) + " |")

    # MultiThomas
    th_vals = []
    for d in datasets:
        val = d.get("benchmarks", {}).get("MultiThomas", {}).get("Thomas_P8_Par", 0.0)
        th_vals.append(f"**{val:.1f} GFlops**" if val > 0 else "—")
    lines.append("| **MultiThomas** ($N_x=64$) | Throughput | " + " | ".join(th_vals) + " |")

    # VideoPipeline CPU
    vp_vals = []
    for d in datasets:
        val = d.get("benchmarks", {}).get("VideoPipeline", {}).get("parallel_fps", 0.0)
        vp_vals.append(f"**{val:,.0f} FPS**" if val > 0 else "—")
    lines.append("| **VideoPipeline (CPU)** | Throughput | " + " | ".join(vp_vals) + " |")

    # VideoPipeline Metal GPU
    metal_vals = []
    for d in datasets:
        val = d.get("benchmarks", {}).get("VideoPipelineMetal", {}).get("metal_fps", 0.0)
        metal_vals.append(f"🔥 **{val:,.0f} FPS**" if val > 0 else "—")
    lines.append("| **VideoPipeline (Metal GPU)** | Throughput | " + " | ".join(metal_vals) + " |")

    # AudioBiquad
    ab_vals = []
    for d in datasets:
        val = d.get("benchmarks", {}).get("AudioBiquad", {}).get("parallel_msamples", 0.0)
        ab_vals.append(f"**{val:,.0f} MS/s**" if val > 0 else "—")
    lines.append("| **AudioBiquad (64ch)** | Throughput | " + " | ".join(ab_vals) + " |")

    # DepthwiseConv
    dc_vals = []
    for d in datasets:
        val = d.get("benchmarks", {}).get("DepthwiseConv", {}).get("parallel_gflops", 0.0)
        dc_vals.append(f"**{val:.1f} GFlops**" if val > 0 else "—")
    lines.append("| **DepthwiseConv (128ch)** | Throughput | " + " | ".join(dc_vals) + " |")

    # OptionPricing
    op_vals = []
    for d in datasets:
        val = d.get("benchmarks", {}).get("OptionPricing", {}).get("parallel_ops", 0.0)
        op_vals.append(f"**{val:,.0f} opt/s**" if val > 0 else "—")
    lines.append("| **OptionPricing** (16k options) | Throughput | " + " | ".join(op_vals) + " |")

    content = "\n".join(lines)
    if out_md:
        with open(out_md, "w") as f:
            f.write(content)
        print(f"[✓] Saved aggregated report to: {out_md}")
    return content

def update_doc_file(doc_path, table_md):
    if not os.path.exists(doc_path):
        print(f"[!] Warning: doc file {doc_path} does not exist.")
        return
    with open(doc_path, "r") as f:
        doc_content = f.read()

    start_tag = "<!-- BEGIN_BENCHMARK_RESULTS -->"
    end_tag = "<!-- END_BENCHMARK_RESULTS -->"

    if start_tag in doc_content and end_tag in doc_content:
        # Strip header from table_md if needed, keep clean table
        lines = [line for line in table_md.splitlines() if line.startswith("|")]
        clean_table = "\n".join(lines)
        pattern = re.compile(f"{re.escape(start_tag)}.*?{re.escape(end_tag)}", re.DOTALL)
        new_block = f"{start_tag}\n{clean_table}\n{end_tag}"
        updated_doc = pattern.sub(new_block, doc_content)
        with open(doc_path, "w") as f:
            f.write(updated_doc)
        print(f"[✓] Successfully updated benchmark table in: {doc_path}")
    else:
        print(f"[!] Warning: Tags {start_tag} / {end_tag} not found in {doc_path}")

def main():
    if "--aggregate" in sys.argv:
        idx = sys.argv.index("--aggregate")
        files = []
        out_md = None
        update_doc = None
        i = idx + 1
        while i < len(sys.argv):
            if sys.argv[i] == "--markdown" and i + 1 < len(sys.argv):
                out_md = sys.argv[i + 1]
                i += 2
            elif sys.argv[i] == "--update-doc" and i + 1 < len(sys.argv):
                update_doc = sys.argv[i + 1]
                i += 2
            else:
                files.append(sys.argv[i])
                i += 1
        summary = aggregate_reports(files, out_md)
        if update_doc:
            update_doc_file(update_doc, summary)
        print("\n" + summary)
        return

    build_dir = sys.argv[1] if len(sys.argv) > 1 else "build"
    out_json = sys.argv[2] if len(sys.argv) > 2 else "benchmark_summary.json"
    out_md = sys.argv[3] if len(sys.argv) > 3 else None

    data = {
        "host": get_system_info(),
        "benchmarks": {}
    }

    print("[*] Benchmarking MultiThomas...")
    thomas = benchmark_thomas(build_dir)
    if thomas: data["benchmarks"]["MultiThomas"] = thomas

    print("[*] Benchmarking VideoPipeline...")
    video = benchmark_video(build_dir)
    if video: data["benchmarks"]["VideoPipeline"] = video

    if platform.system() == "Darwin":
        print("[*] Benchmarking VideoPipelineMetal...")
        metal = benchmark_video_metal(build_dir)
        if metal: data["benchmarks"]["VideoPipelineMetal"] = metal

    print("[*] Benchmarking AudioBiquad...")
    audio = benchmark_audio(build_dir)
    if audio: data["benchmarks"]["AudioBiquad"] = audio

    print("[*] Benchmarking DepthwiseConv...")
    conv = benchmark_conv(build_dir)
    if conv: data["benchmarks"]["DepthwiseConv"] = conv

    print("[*] Benchmarking OptionPricing...")
    opts = benchmark_options(build_dir)
    if opts: data["benchmarks"]["OptionPricing"] = opts

    with open(out_json, "w") as f:
        json.dump(data, f, indent=2)
    print(f"[✓] Saved benchmark data to: {out_json}")

    md_table = generate_markdown(data)
    print("\n" + md_table)
    if out_md:
        with open(out_md, "w") as f:
            f.write(md_table)
        print(f"[✓] Saved markdown table to: {out_md}")

if __name__ == "__main__":
    main()
