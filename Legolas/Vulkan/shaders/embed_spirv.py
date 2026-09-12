#!/usr/bin/env python3
"""Embed SPIR-V binaries as C++ headers (Legolas/Vulkan/spv/*.hxx).

Usage: embed_spirv.py <spv_dir> <out_dir>
"""
import glob
import os
import struct
import sys


def main():
    if len(sys.argv) != 3:
        print(__doc__)
        return 1
    spv_dir, out_dir = sys.argv[1], sys.argv[2]
    os.makedirs(out_dir, exist_ok=True)

    for path in sorted(glob.glob(os.path.join(spv_dir, "*.spv"))):
        name = os.path.splitext(os.path.basename(path))[0]
        data = open(path, "rb").read()
        if len(data) % 4 != 0:
            raise RuntimeError("SPIR-V size not a multiple of 4: " + path)
        words = struct.unpack("<%dI" % (len(data) // 4), data)

        lines = []
        lines.append("#pragma once")
        lines.append("#include <cstdint>")
        lines.append("")
        lines.append("namespace Legolas { namespace Vulkan { namespace spv {")
        lines.append("")
        lines.append("static const uint32_t %s[] = {" % name)
        for i in range(0, len(words), 8):
            chunk = ", ".join("0x%08xu" % w for w in words[i:i + 8])
            lines.append("  " + chunk + ",")
        lines.append("};")
        lines.append("static const uint32_t %s_words = sizeof(%s) / sizeof(%s[0]);"
                     % (name, name, name))
        lines.append("")
        lines.append("}}} // namespace Legolas::Vulkan::spv")
        lines.append("")

        out_path = os.path.join(out_dir, name + ".hxx")
        with open(out_path, "w") as f:
            f.write("\n".join(lines))
        print("embedded %-24s %6d words -> %s" % (name, len(words), out_path))
    return 0


if __name__ == "__main__":
    sys.exit(main())
