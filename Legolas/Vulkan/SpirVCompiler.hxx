#pragma once

// Runtime GLSL -> SPIR-V compilation for the expression-template backend.
//
// The committed kernels (Legolas/Vulkan/spv/*.hxx) never need a shader
// compiler, but expressions are only known at runtime. This helper locates an
// external compiler (glslc or glslangValidator) and compiles generated GLSL
// on demand, caching the resulting SPIR-V in memory for the process lifetime.
//
//   - LEGOLAS_VULKAN_GLSLC overrides the compiler path.
//   - When no compiler is found, available() is false and the caller falls
//     back to the CPU.
//   - Runtime compilation is POSIX-only; Windows users can point
//     LEGOLAS_VULKAN_GLSLC at a compiler but process spawning is not wired
//     there, so available() stays false.

#include <cstdint>
#include <cstdlib>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#if !defined(_WIN32)
#  include <cerrno>
#  include <cstdio>
#  include <sys/wait.h>
#  include <unistd.h>
#endif

namespace Legolas {
namespace Vulkan {
namespace detail {

class SpirVCompiler {
public:
  static bool available() { return !compilerPath().empty(); }

  static const std::string& compilerPath() {
    static const std::string path = findCompiler();
    return path;
  }

  // Compile GLSL compute source to SPIR-V words. Thread-safe; successful
  // results are cached by source text.
  static bool compile(const std::string& glsl, std::vector<uint32_t>& spirv) {
    if (!available()) return false;

    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    static std::unordered_map<std::string, std::vector<uint32_t>> cache;
    const auto cached = cache.find(glsl);
    if (cached != cache.end()) {
      spirv = cached->second;
      return !spirv.empty();
    }

    if (!compileProcess(glsl, spirv) || spirv.empty()) return false;
    cache.emplace(glsl, spirv);
    return true;
  }

private:
  static bool isGlslangValidator(const std::string& path) {
    return path.find("glslangValidator") != std::string::npos ||
           path.find("glslang") != std::string::npos;
  }

  static bool findInPath(const char* name) {
#if defined(_WIN32)
    (void)name;
    return false;
#else
    const char* path = std::getenv("PATH");
    if (path == nullptr) return false;
    const std::string paths(path);
    size_t begin = 0;
    while (begin <= paths.size()) {
      const size_t end = paths.find(':', begin);
      const std::string dir =
          paths.substr(begin, end == std::string::npos ? std::string::npos
                                                       : end - begin);
      if (!dir.empty() && ::access((dir + "/" + name).c_str(), X_OK) == 0) {
        return true;
      }
      if (end == std::string::npos) break;
      begin = end + 1;
    }
    return false;
#endif
  }

  static std::string findCompiler() {
    const char* env = std::getenv("LEGOLAS_VULKAN_GLSLC");
    if (env != nullptr && *env != '\0') return std::string(env);
    if (findInPath("glslc")) return "glslc";
    if (findInPath("glslangValidator")) return "glslangValidator";
    return std::string();
  }

#if !defined(_WIN32)
  static bool runProcess(const std::vector<std::string>& argv) {
    std::vector<char*> args;
    args.reserve(argv.size() + 1);
    for (const std::string& arg : argv) {
      args.push_back(const_cast<char*>(arg.c_str()));
    }
    args.push_back(nullptr);

    const pid_t pid = ::fork();
    if (pid < 0) return false;
    if (pid == 0) {
      if (std::getenv("LEGOLAS_VULKAN_DEBUG") == nullptr) {
        if (std::freopen("/dev/null", "w", stdout) == nullptr) ::_exit(127);
        if (std::freopen("/dev/null", "w", stderr) == nullptr) ::_exit(127);
      }
      ::execvp(args[0], args.data());
      ::_exit(127);
    }

    int status = 0;
    while (::waitpid(pid, &status, 0) < 0) {
      if (errno != EINTR) return false;
    }
    return WIFEXITED(status) && WEXITSTATUS(status) == 0;
  }

  static bool writeFile(const std::string& path, const std::string& data) {
    std::FILE* file = std::fopen(path.c_str(), "wb");
    if (file == nullptr) return false;
    const size_t written =
        std::fwrite(data.data(), 1, data.size(), file);
    std::fclose(file);
    return written == data.size();
  }

  static bool readWords(const std::string& path, std::vector<uint32_t>& words) {
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (file == nullptr) return false;
    if (std::fseek(file, 0, SEEK_END) != 0) {
      std::fclose(file);
      return false;
    }
    const long size = std::ftell(file);
    std::fseek(file, 0, SEEK_SET);
    if (size <= 0 || size % 4 != 0) {
      std::fclose(file);
      return false;
    }
    words.resize(static_cast<size_t>(size) / 4);
    const size_t read = std::fread(words.data(), 4, words.size(), file);
    std::fclose(file);
    return read == words.size();
  }

  static bool makeTemp(std::string& path) {
    char buffer[] = "/tmp/legolas_spirv_XXXXXX";
    const int fd = ::mkstemp(buffer);
    if (fd < 0) return false;
    ::close(fd);
    path = buffer;
    return true;
  }

  static bool compileProcess(const std::string& glsl,
                             std::vector<uint32_t>& spirv) {
    std::string sourcePath;
    std::string binaryPath;
    if (!makeTemp(sourcePath) || !makeTemp(binaryPath)) {
      if (!sourcePath.empty()) ::unlink(sourcePath.c_str());
      if (!binaryPath.empty()) ::unlink(binaryPath.c_str());
      return false;
    }
    // mkstemp does not add the extension expected by the compilers.
    const std::string compPath = sourcePath + ".comp";
    const std::string spvPath = binaryPath + ".spv";
    std::rename(sourcePath.c_str(), compPath.c_str());
    std::rename(binaryPath.c_str(), spvPath.c_str());

    const std::string& compiler = compilerPath();
    std::vector<std::string> argv;
    argv.push_back(compiler);
    if (isGlslangValidator(compiler)) {
      argv.push_back("-V");
      argv.push_back("--target-env");
      argv.push_back("vulkan1.1");
      argv.push_back("-o");
      argv.push_back(spvPath);
      argv.push_back(compPath);
    } else {
      argv.push_back("--target-env=vulkan1.1");
      argv.push_back("-O");
      argv.push_back("-o");
      argv.push_back(spvPath);
      argv.push_back(compPath);
    }

    bool ok = writeFile(compPath, glsl) && runProcess(argv) &&
              readWords(spvPath, spirv);

    ::unlink(compPath.c_str());
    ::unlink(spvPath.c_str());
    return ok;
  }
#else
  static bool compileProcess(const std::string&, std::vector<uint32_t>&) {
    return false;
  }
#endif
};

} // namespace detail
}} // namespace Legolas::Vulkan
