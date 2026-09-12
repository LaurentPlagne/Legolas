#!/usr/bin/env bash
# Compile the GLSL compute shaders to SPIR-V and embed them as C++ headers.
#
# Requires glslc (Vulkan SDK or distribution package) and Python 3.
# The generated Legolas/Vulkan/spv/*.hxx files are versioned, so end users
# never need a shader compiler.
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
GLSLC="${GLSLC:-glslc}"

for src in "${HERE}"/*.comp; do
    out="${src%.comp}.spv"
    echo "compiling $(basename "${src}")"
    "${GLSLC}" --target-env=vulkan1.1 -O -o "${out}" "${src}"
done

python3 "${HERE}/embed_spirv.py" "${HERE}" "${HERE}/../spv"

rm -f "${HERE}"/*.spv
echo "done"
