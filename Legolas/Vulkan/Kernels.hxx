#pragma once

// Legolas++ Vulkan workload kernels (benchmark suite).
//
// This header is intentionally separate from Vulkan.hxx: the generic backend
// (Context / Buffer / Kernel / DeviceArray / reductions) has no knowledge of
// these shaders, so core code can depend on it without pulling in the
// benchmark blobs. Applications embedding their own SPIR-V can do the same.
//
// The blobs are generated once and committed (Legolas/Vulkan/spv/*.hxx), so
// end users never need a shader compiler. Regeneration is opt-in via
// Legolas/Vulkan/shaders/build_shaders.sh.

#include "Legolas/Vulkan/spv/biquad_block_apply.hxx"
#include "Legolas/Vulkan/spv/biquad_block_prefix.hxx"
#include "Legolas/Vulkan/spv/biquad_block_states.hxx"
#include "Legolas/Vulkan/spv/depthwise_conv.hxx"
#include "Legolas/Vulkan/spv/option_pricing.hxx"
#include "Legolas/Vulkan/spv/thomas.hxx"
#include "Legolas/Vulkan/spv/video_pipeline.hxx"
