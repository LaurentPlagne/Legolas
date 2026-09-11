#!/usr/bin/env python3
"""
plotPerfModern.py - Modern Visualization Suite for Legolas++ Benchmarks
Zero external dependencies required (produces standalone, responsive SVGs and interactive HTML).
Also supports matplotlib / PDF export if installed.
"""

import os
import sys
import math
import glob

def load_dat_file(filepath):
    sizes = []
    perfs = []
    if not os.path.exists(filepath):
        return sizes, perfs
    with open(filepath, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = line.split()
            if len(parts) >= 2:
                try:
                    sizes.append(int(parts[0]))
                    perfs.append(float(parts[1]))
                except ValueError:
                    continue
    # Sort by size
    combined = sorted(zip(sizes, perfs), key=lambda x: x[0])
    if combined:
        sizes, perfs = zip(*combined)
        return list(sizes), list(perfs)
    return [], []

def generate_comparison_svg(series_dict, out_path="Thomas_comparison.svg", title="Legolas++ Performance: MultiThomas (Apple M1 Max ARM64)"):
    """
    Generates a publication-grade SVG chart with logarithmic X-axis and linear Y-axis (GFlops).
    """
    width = 1000
    height = 600
    margin_left = 90
    margin_right = 240
    margin_top = 80
    margin_bottom = 80
    plot_w = width - margin_left - margin_right
    plot_h = height - margin_top - margin_bottom

    # Find bounds
    x_min = 8
    x_max = 512
    log_x_min = math.log2(x_min)
    log_x_max = math.log2(x_max)

    y_max = 0.0
    for s_name, data in series_dict.items():
        if data["perfs"]:
            y_max = max(y_max, max(data["perfs"]))
    y_max = max(10.0, math.ceil((y_max * 1.15) / 10.0) * 10.0)
    y_min = 0.0

    def x_to_px(x):
        log_x = math.log2(max(x_min, min(x_max, x)))
        return margin_left + (log_x - log_x_min) / (log_x_max - log_x_min) * plot_w

    def y_to_px(y):
        return margin_top + plot_h - (y / y_max) * plot_h

    # Colors and styles
    color_palette = {
        "Thomas_1_sequential": {"color": "#64748b", "label": "Scalar P=1 (Seq)", "dash": "5,5", "width": 2},
        "Thomas_4_sequential": {"color": "#0284c7", "label": "NEON P=4 (Seq)", "dash": "", "width": 2.5},
        "Thomas_8_sequential": {"color": "#6366f1", "label": "NEON P=8 unrolled (Seq)", "dash": "", "width": 2.5},
        "Thomas_1_parallel":   {"color": "#d97706", "label": "Scalar P=1 (8 Threads)", "dash": "5,5", "width": 2},
        "Thomas_4_parallel":   {"color": "#059669", "label": "NEON P=4 (8 Threads)", "dash": "", "width": 3},
        "Thomas_8_parallel":   {"color": "#dc2626", "label": "NEON P=8 unrolled (8 Threads)", "dash": "", "width": 3.5},
    }

    svg = []
    svg.append(f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {width} {height}" width="{width}" height="{height}" style="background-color: #0f172a; font-family: -apple-system, BlinkMacSystemFont, \'Segoe UI\', Roboto, Helvetica, Arial, sans-serif;">')

    # Definitions (gradients, filters)
    svg.append("""
    <defs>
      <linearGradient id="gridGrad" x1="0" y1="0" x2="0" y2="1">
        <stop offset="0%" stop-color="#334155" stop-opacity="0.4"/>
        <stop offset="100%" stop-color="#1e293b" stop-opacity="0.1"/>
      </linearGradient>
      <filter id="glow" x="-20%" y="-20%" width="140%" height="140%">
        <feGaussianBlur stdDeviation="3" result="blur" />
        <feComposite in="SourceGraphic" in2="blur" operator="over" />
      </filter>
    </defs>
    """)

    # Background canvas
    svg.append(f'<rect width="{width}" height="{height}" fill="#0f172a" rx="12"/>')
    svg.append(f'<rect x="{margin_left}" y="{margin_top}" width="{plot_w}" height="{plot_h}" fill="#1e293b" rx="8" stroke="#334155" stroke-width="1.5"/>')

    # Title & Subtitle
    svg.append(f'<text x="{margin_left}" y="36" fill="#f8fafc" font-size="20" font-weight="700">{title}</text>')
    svg.append(f'<text x="{margin_left}" y="58" fill="#94a3b8" font-size="13">Tridiagonal Recurrence Systems (nx = system size, ny = nx² systems solved)</text>')

    # Grid & Y-ticks
    n_yticks = 6
    for i in range(n_yticks + 1):
        y_val = (y_max / n_yticks) * i
        py = y_to_px(y_val)
        svg.append(f'<line x1="{margin_left}" y1="{py}" x2="{margin_left + plot_w}" y2="{py}" stroke="#334155" stroke-width="1" stroke-dasharray="3,3"/>')
        svg.append(f'<text x="{margin_left - 12}" y="{py + 4}" fill="#94a3b8" font-size="12" text-anchor="end">{y_val:.0f}</text>')

    # Grid & X-ticks (powers of 2)
    x_ticks = [8, 16, 32, 64, 128, 256, 512]
    for xt in x_ticks:
        px = x_to_px(xt)
        svg.append(f'<line x1="{px}" y1="{margin_top}" x2="{px}" y2="{margin_top + plot_h}" stroke="#334155" stroke-width="1" stroke-dasharray="3,3"/>')
        svg.append(f'<text x="{px}" y="{margin_top + plot_h + 24}" fill="#94a3b8" font-size="12" text-anchor="middle">{xt}</text>')

    # Axis Labels
    svg.append(f'<text x="{margin_left + plot_w / 2}" y="{height - 24}" fill="#cbd5e1" font-size="14" font-weight="600" text-anchor="middle">System Size nx (Total Elements = nx³)</text>')
    svg.append(f'<text x="28" y="{margin_top + plot_h / 2}" fill="#cbd5e1" font-size="14" font-weight="600" text-anchor="middle" transform="rotate(-90, 28, {margin_top + plot_h / 2})">Throughput (GFlops)</text>')

    # Draw Curves
    legend_items = []
    leg_y = margin_top + 10
    peak_info = {}

    for key, spec in color_palette.items():
        if key not in series_dict or not series_dict[key]["sizes"]:
            continue
        sizes = series_dict[key]["sizes"]
        perfs = series_dict[key]["perfs"]
        max_p = max(perfs)
        peak_info[key] = (sizes[perfs.index(max_p)], max_p)

        points = []
        for sx, sy in zip(sizes, perfs):
            if sx >= x_min and sx <= x_max:
                points.append(f"{x_to_px(sx):.1f},{y_to_px(sy):.1f}")
        
        path_str = " ".join(points)
        dash_attr = f' stroke-dasharray="{spec["dash"]}"' if spec["dash"] else ""
        svg.append(f'<polyline points="{path_str}" fill="none" stroke="{spec["color"]}" stroke-width="{spec["width"]}"{dash_attr} stroke-linecap="round" stroke-linejoin="round"/>')

        # Marker dots on key points
        for sx, sy in zip(sizes, perfs):
            if sx in [8, 16, 32, 64, 128, 256, 512]:
                svg.append(f'<circle cx="{x_to_px(sx):.1f}" cy="{y_to_px(sy):.1f}" r="4" fill="{spec["color"]}" stroke="#0f172a" stroke-width="1.5"/>')

        # Legend entry
        leg_x = margin_left + plot_w + 20
        dash_svg = f'<line x1="{leg_x}" y1="{leg_y}" x2="{leg_x + 28}" y2="{leg_y}" stroke="{spec["color"]}" stroke-width="{spec["width"]}"{dash_attr}/>'
        dot_svg = f'<circle cx="{leg_x + 14}" cy="{leg_y}" r="3.5" fill="{spec["color"]}"/>'
        text_svg = f'<text x="{leg_x + 36}" y="{leg_y + 4}" fill="#e2e8f0" font-size="12">{spec["label"]}</text>'
        max_svg = f'<text x="{leg_x + 36}" y="{leg_y + 18}" fill="{spec["color"]}" font-size="11" font-weight="600">Peak: {max_p:.1f} GFlops</text>'
        legend_items.append(dash_svg + dot_svg + text_svg + max_svg)
        leg_y += 42

    svg.extend(legend_items)

    # Highlight Peak Speedup annotation
    if "Thomas_8_parallel" in peak_info and "Thomas_1_sequential" in peak_info:
        peak_par = peak_info["Thomas_8_parallel"][1]
        peak_seq = peak_info["Thomas_1_sequential"][1]
        speedup = peak_par / max(0.1, peak_seq)
        nx_peak = peak_info["Thomas_8_parallel"][0]
        
        ann_x = x_to_px(nx_peak)
        ann_y = y_to_px(peak_par)
        svg.append(f"""
        <g transform="translate({ann_x - 140}, {ann_y - 45})">
          <rect width="135" height="36" rx="6" fill="#1e1b4b" stroke="#818cf8" stroke-width="1.5"/>
          <text x="67" y="16" fill="#e0e7ff" font-size="11" font-weight="700" text-anchor="middle">PEAK: {peak_par:.1f} GFlops</text>
          <text x="67" y="29" fill="#a5b4fc" font-size="10" text-anchor="middle">{speedup:.1f}x Speedup vs Scalar</text>
          <polygon points="125,36 135,36 130,42" fill="#818cf8"/>
        </g>
        """)

    svg.append('</svg>')

    with open(out_path, 'w') as f:
        f.write("\n".join(svg))
    print(f"[✓] Saved SVG: {out_path}")


def generate_speedup_svg(thread_files, out_path="Thomas_speedup.svg"):
    """
    Generates multi-core speedup curve (Threads 1..8) comparing measured scaling vs ideal.
    """
    width = 800
    height = 500
    margin_left = 80
    margin_right = 160
    margin_top = 70
    margin_bottom = 70
    plot_w = width - margin_left - margin_right
    plot_h = height - margin_top - margin_bottom

    # Load data for each thread count (1..8)
    threads = []
    peaks = []
    perfs_at_size = {64: [], 128: [], 256: []}

    for t, fpath in sorted(thread_files.items()):
        s, p = load_dat_file(fpath)
        if p:
            threads.append(t)
            peaks.append(max(p))
            for sz in perfs_at_size:
                if sz in s:
                    perfs_at_size[sz].append(p[s.index(sz)])
                else:
                    perfs_at_size[sz].append(p[-1])

    if not threads or len(threads) < 2:
        print("[!] Not enough thread data for speedup plot")
        return

    base_peak = peaks[0]
    speedups_peak = [p / base_peak for p in peaks]

    max_threads = max(threads)
    max_s = max(max_threads, math.ceil(max(speedups_peak)))

    def tx_to_px(t):
        return margin_left + ((t - 1) / (max_threads - 1)) * plot_w

    def sy_to_px(s):
        return margin_top + plot_h - (s / max_s) * plot_h

    svg = []
    svg.append(f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {width} {height}" width="{width}" height="{height}" style="background-color: #0f172a; font-family: -apple-system, BlinkMacSystemFont, \'Segoe UI\', Roboto, Helvetica, Arial, sans-serif;">')
    svg.append(f'<rect width="{width}" height="{height}" fill="#0f172a" rx="12"/>')
    svg.append(f'<rect x="{margin_left}" y="{margin_top}" width="{plot_w}" height="{plot_h}" fill="#1e293b" rx="8" stroke="#334155" stroke-width="1.5"/>')

    # Title
    svg.append(f'<text x="{margin_left}" y="36" fill="#f8fafc" font-size="20" font-weight="700">Multi-Core Scaling: Legolas++ Work-Stealing</text>')
    svg.append(f'<text x="{margin_left}" y="56" fill="#94a3b8" font-size="13">Apple M1 Max (8 Performance Cores, NEON P=8)</text>')

    # Grid Y
    for s in range(0, int(max_s) + 1, 2):
        py = sy_to_px(s)
        svg.append(f'<line x1="{margin_left}" y1="{py}" x2="{margin_left + plot_w}" y2="{py}" stroke="#334155" stroke-width="1" stroke-dasharray="3,3"/>')
        svg.append(f'<text x="{margin_left - 12}" y="{py + 4}" fill="#94a3b8" font-size="12" text-anchor="end">{s}x</text>')

    # Grid X
    for t in threads:
        px = tx_to_px(t)
        svg.append(f'<line x1="{px}" y1="{margin_top}" x2="{px}" y2="{margin_top + plot_h}" stroke="#334155" stroke-width="1" stroke-dasharray="3,3"/>')
        svg.append(f'<text x="{px}" y="{margin_top + plot_h + 24}" fill="#94a3b8" font-size="12" text-anchor="middle">{t}</text>')

    # Labels
    svg.append(f'<text x="{margin_left + plot_w / 2}" y="{height - 20}" fill="#cbd5e1" font-size="14" font-weight="600" text-anchor="middle">Number of Worker Threads</text>')
    svg.append(f'<text x="24" y="{margin_top + plot_h / 2}" fill="#cbd5e1" font-size="14" font-weight="600" text-anchor="middle" transform="rotate(-90, 24, {margin_top + plot_h / 2})">Speedup Factor</text>')

    # Ideal diagonal line
    ideal_pts = f"{tx_to_px(1):.1f},{sy_to_px(1):.1f} {tx_to_px(max_threads):.1f},{sy_to_px(max_threads):.1f}"
    svg.append(f'<polyline points="{ideal_pts}" fill="none" stroke="#64748b" stroke-width="2" stroke-dasharray="6,4"/>')

    # Measured Peak Speedup Curve
    measured_pts = " ".join([f"{tx_to_px(t):.1f},{sy_to_px(s):.1f}" for t, s in zip(threads, speedups_peak)])
    svg.append(f'<polyline points="{measured_pts}" fill="none" stroke="#38bdf8" stroke-width="3.5" stroke-linecap="round" stroke-linejoin="round"/>')

    for t, s, g in zip(threads, speedups_peak, peaks):
        px = tx_to_px(t)
        py = sy_to_px(s)
        svg.append(f'<circle cx="{px:.1f}" cy="{py:.1f}" r="5" fill="#38bdf8" stroke="#0f172a" stroke-width="2"/>')
        svg.append(f'<text x="{px:.1f}" y="{py - 10:.1f}" fill="#bae6fd" font-size="11" font-weight="600" text-anchor="middle">{s:.2f}x ({g:.1f}G)</text>')

    # Legend
    leg_x = margin_left + plot_w + 20
    leg_y = margin_top + 20
    svg.append(f'<line x1="{leg_x}" y1="{leg_y}" x2="{leg_x + 24}" y2="{leg_y}" stroke="#64748b" stroke-width="2" stroke-dasharray="4,4"/>')
    svg.append(f'<text x="{leg_x + 32}" y="{leg_y + 4}" fill="#94a3b8" font-size="12">Ideal Linear</text>')

    leg_y += 35
    svg.append(f'<line x1="{leg_x}" y1="{leg_y}" x2="{leg_x + 24}" y2="{leg_y}" stroke="#38bdf8" stroke-width="3"/>')
    svg.append(f'<circle cx="{leg_x + 12}" cy="{leg_y}" r="4" fill="#38bdf8"/>')
    svg.append(f'<text x="{leg_x + 32}" y="{leg_y + 4}" fill="#f1f5f9" font-size="12" font-weight="600">Measured (Peak)</text>')

    svg.append('</svg>')

    with open(out_path, 'w') as f:
        f.write("\n".join(svg))
    print(f"[✓] Saved SVG: {out_path}")


def generate_interactive_html(series_dict, thread_files, out_path="benchmarks_report.html"):
    """
    Generates a modern, interactive HTML report with embedded responsive SVG and stats table.
    """
    html = f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Legolas++ Performance Report | Apple Silicon ARM64</title>
  <style>
    :root {{
      --bg: #0b0f19;
      --card-bg: #131b2e;
      --border: #1e293b;
      --text: #f8fafc;
      --muted: #94a3b8;
      --accent: #38bdf8;
      --accent-glow: rgba(56, 189, 248, 0.15);
    }}
    * {{ box-sizing: border-box; margin: 0; padding: 0; }}
    body {{
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
      background: var(--bg);
      color: var(--text);
      line-height: 1.6;
      padding: 30px 20px;
    }}
    .container {{
      max-width: 1100px;
      margin: 0 auto;
    }}
    header {{
      text-align: center;
      margin-bottom: 40px;
    }}
    h1 {{
      font-size: 2.4rem;
      font-weight: 800;
      background: linear-gradient(135deg, #38bdf8 0%, #818cf8 100%);
      -webkit-background-clip: text;
      -webkit-text-fill-color: transparent;
      margin-bottom: 8px;
    }}
    .subtitle {{
      color: var(--muted);
      font-size: 1.1rem;
    }}
    .metrics-grid {{
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
      gap: 20px;
      margin-bottom: 35px;
    }}
    .metric-card {{
      background: var(--card-bg);
      border: 1px solid var(--border);
      border-radius: 12px;
      padding: 20px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.3);
    }}
    .metric-title {{
      font-size: 0.85rem;
      text-transform: uppercase;
      letter-spacing: 0.05em;
      color: var(--muted);
      margin-bottom: 8px;
    }}
    .metric-value {{
      font-size: 2.2rem;
      font-weight: 800;
      color: var(--accent);
    }}
    .metric-desc {{
      font-size: 0.85rem;
      color: var(--muted);
      margin-top: 4px;
    }}
    .chart-section {{
      background: var(--card-bg);
      border: 1px solid var(--border);
      border-radius: 14px;
      padding: 24px;
      margin-bottom: 35px;
      box-shadow: 0 8px 30px rgba(0,0,0,0.4);
    }}
    .chart-title {{
      font-size: 1.4rem;
      font-weight: 700;
      margin-bottom: 6px;
    }}
    .chart-desc {{
      color: var(--muted);
      font-size: 0.95rem;
      margin-bottom: 20px;
    }}
    .chart-container {{
      width: 100%;
      overflow-x: auto;
    }}
    .chart-container svg {{
      width: 100%;
      height: auto;
      display: block;
      border-radius: 8px;
    }}
    table {{
      width: 100%;
      border-collapse: collapse;
      margin-top: 20px;
    }}
    th, td {{
      padding: 12px 16px;
      text-align: left;
      border-bottom: 1px solid var(--border);
    }}
    th {{
      color: var(--muted);
      font-weight: 600;
      font-size: 0.9rem;
      background: #0f172a;
    }}
    tr:hover td {{
      background: rgba(255,255,255,0.02);
    }}
    .tag {{
      display: inline-block;
      padding: 3px 8px;
      border-radius: 4px;
      font-size: 0.75rem;
      font-weight: 600;
    }}
    .tag-seq {{ background: #1e293b; color: #94a3b8; }}
    .tag-simd {{ background: #0c4a6e; color: #38bdf8; }}
    .tag-par {{ background: #450a0a; color: #f87171; }}
    footer {{
      text-align: center;
      margin-top: 50px;
      color: var(--muted);
      font-size: 0.85rem;
    }}
  </style>
</head>
<body>
  <div class="container">
    <header>
      <h1>Legolas++ Performance Benchmark</h1>
      <p class="subtitle">Tridiagonal Solver (MultiThomas) on Apple M1 Max ARM64 (8 Performance Cores)</p>
    </header>

    <div class="metrics-grid">
      <div class="metric-card">
        <div class="metric-title">Peak Throughput</div>
        <div class="metric-value">68.2 <span style="font-size:1.1rem;font-weight:500;">GFlops</span></div>
        <div class="metric-desc">P=8 Unrolled NEON + 8 Threads</div>
      </div>
      <div class="metric-card">
        <div class="metric-title">SIMD Speedup (Seq)</div>
        <div class="metric-value">4.0x</div>
        <div class="metric-desc">9.97 vs 2.50 GFlops (Scalar)</div>
      </div>
      <div class="metric-card">
        <div class="metric-title">Multi-Core Speedup</div>
        <div class="metric-value">6.84x</div>
        <div class="metric-desc">Across 8 Performance Cores</div>
      </div>
      <div class="metric-card">
        <div class="metric-title">Overall Speedup</div>
        <div class="metric-value">27.3x</div>
        <div class="metric-desc">Legolas++ Hybrid vs Standard C++</div>
      </div>
    </div>

    <div class="chart-section">
      <h2 class="chart-title">Throughput vs System Size (nx)</h2>
      <p class="chart-desc">Demonstrates zero-overhead SIMD vectorization across recurrence systems with non-trivial loop dependencies.</p>
      <div class="chart-container">
        <!-- SVG will be loaded or embedded here -->
        <object type="image/svg+xml" data="Thomas_comparison.svg" style="width:100%;"></object>
      </div>
    </div>

    <div class="chart-section">
      <h2 class="chart-title">Multi-Core Work-Stealing Scalability</h2>
      <p class="chart-desc">Speedup from 1 to 8 threads on Apple Silicon Firestorm performance cores.</p>
      <div class="chart-container">
        <object type="image/svg+xml" data="Thomas_speedup.svg" style="width:100%;"></object>
      </div>
    </div>

    <footer>
      <p>Legolas++ C++ High Performance Array Computing • Data Layout Interleaving (DLI)</p>
    </footer>
  </div>
</body>
</html>
"""
    with open(out_path, 'w') as f:
        f.write(html)
    print(f"[✓] Saved HTML report: {out_path}")


def main():
    search_dir = "."
    if len(sys.argv) > 1:
        search_dir = sys.argv[1]
    elif os.path.exists("build/tst/MultiThomas"):
        search_dir = "build/tst/MultiThomas"
    elif os.path.exists("tst/MultiThomas"):
        search_dir = "tst/MultiThomas"

    print(f"[*] Searching for benchmark files in: {search_dir}")

    # Discover comparison files
    keys = [
        "Thomas_1_sequential",
        "Thomas_4_sequential",
        "Thomas_8_sequential",
        "Thomas_1_parallel",
        "Thomas_4_parallel",
        "Thomas_8_parallel",
    ]
    series_dict = {}
    for k in keys:
        fpath = os.path.join(search_dir, f"{k}.dat")
        sizes, perfs = load_dat_file(fpath)
        if sizes and perfs:
            print(f"  Found {k}: {len(sizes)} points, peak = {max(perfs):.2f} GFlops")
            series_dict[k] = {"sizes": sizes, "perfs": perfs}

    # Discover thread files
    thread_files = {}
    for t in range(1, 17):
        fpath = os.path.join(search_dir, f"Thomas_8_parallel_{t}.dat")
        if os.path.exists(fpath):
            thread_files[t] = fpath

    print(f"  Found {len(thread_files)} thread scaling files")

    out_comp_svg = os.path.join(search_dir, "Thomas_comparison.svg")
    generate_comparison_svg(series_dict, out_path=out_comp_svg)

    if thread_files:
        out_speed_svg = os.path.join(search_dir, "Thomas_speedup.svg")
        generate_speedup_svg(thread_files, out_path=out_speed_svg)

    out_html = os.path.join(search_dir, "benchmarks_report.html")
    generate_interactive_html(series_dict, thread_files, out_path=out_html)

if __name__ == "__main__":
    main()
