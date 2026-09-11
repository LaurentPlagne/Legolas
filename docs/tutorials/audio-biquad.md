# Tutorial 3: Real-Time Audio DSP (IIR Biquad Filter)

In digital audio workstations (DAWs) like Logic Pro, Ableton Live, and ProTools, as well as spatial audio systems (Dolby Atmos, Ambisonics), audio signals are processed through banks of **Infinite Impulse Response (IIR) Biquad Filters**.

Biquads implement equalizers (peaking, shelving, notch), crossovers, and analog synthesizer modeling filters.

The standard Direct Form I difference equation is:

$$y[n] = b_0 x[n] + b_1 x[n-1] + b_2 x[n-2] - a_1 y[n-1] - a_2 y[n-2]$$

where:
- $x[n]$ is the current input sample, and $x[n-1], x[n-2]$ are past inputs.
- $y[n]$ is the current output sample, and $y[n-1], y[n-2]$ are past outputs (recursive feedback).

---

## The Vectorization Challenge in Audio

Because $y[n]$ strictly depends on $y[n-1]$ computed in the previous clock cycle, **a single audio stream cannot be auto-vectorized along time $n$**.

However, a modern professional audio production session rarely processes a single channel:
- Multitrack studio recording: 32 to 128 individual tracks (drums, vocals, guitars, synths).
- Spatial audio: 7.1.4 Dolby Atmos has 12 discrete channels.
- Synthesizers: Polyphonic synths with 16 or 32 concurrent voices.

---

## 2. Legolas DLI Multi-Channel Solution

Rather than trying to vectorize along time $n$, Legolas++ interleaves $P=4$ (or $P=8$) audio tracks into vector registers:

```cpp
#include "Legolas/Array/Array.hxx"
#include "Legolas/Array/Map.hxx"

struct BiquadFilter {
    float b0, b1, b2;
    float a1, a2;

    BiquadFilter(float b0_, float b1_, float b2_, float a1_, float a2_)
        : b0(b0_), b1(b1_), b2(b2_), a1(a1_), a2(a2_) {}

    template <class InA2D, class OutA2D>
    void operator()(int begin, int end, InA2D input, OutA2D output) const {
        using Scalar = typename InA2D::RealType;

        Scalar c_b0(b0), c_b1(b1), c_b2(b2);
        Scalar c_a1(a1), c_a2(a2);

        for (int ch = begin; ch < end; ++ch) {
            auto in_track = input[ch];
            auto out_track = output[ch];
            const int num_samples = in_track.size();

            Scalar x1(0.0f), x2(0.0f);
            Scalar y1(0.0f), y2(0.0f);

            // Time loop runs sequentially, but updates P audio tracks simultaneously!
            for (int n = 0; n < num_samples; ++n) {
                Scalar x0 = in_track[n];
                Scalar y0 = c_b0 * x0 + c_b1 * x1 + c_b2 * x2 - c_a1 * y1 - c_a2 * y2;

                out_track[n] = y0;

                // State shift
                x2 = x1; x1 = x0;
                y2 = y1; y1 = y0;
            }
        }
    }
};
```

---

## 3. Performance Results (Apple M1 Max)

Evaluating on a studio session benchmark:
- 64 audio tracks
- 96 kHz high-resolution audio (960,000 samples per track, 10 seconds of audio)
- 61.44 million samples processed per pass

| Configuration | Time (ms) | Throughput (MSamples/s) | GFlops | Speedup vs Scalar |
| :--- | :--- | :--- | :--- | :--- |
| **Scalar Baseline ($P=1$)** | 154.4 ms | 397.9 MSamples/s | 3.18 GFlops | 1.00x |
| **Legolas NEON SIMD ($P=4$, 1 Core)** | 48.3 ms | 1,273.5 MSamples/s | 10.19 GFlops | **3.20x** |
| **Legolas Multi-Core + NEON ($P=4$, 8 Cores)** | **10.5 ms** | **5,832.2 MSamples/s** | **46.66 GFlops** | **14.66x** |

At **5,832 Megasamples per second**, Legolas++ can filter over **60,000 real-time audio streams at 96 kHz** on a single Apple M1 Max chip.
