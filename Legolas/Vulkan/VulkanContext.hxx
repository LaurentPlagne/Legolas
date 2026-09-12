#pragma once

#include "Legolas/Vulkan/VulkanLoader.hxx"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace Legolas {
namespace Vulkan {

// ---------------------------------------------------------------------------
// Context: lazily initialized singleton owning the logical device, the compute
// queue and the command pool. If no Vulkan device is available, available()
// returns false and every helper is a no-op, so callers can fall back to CPU.
// ---------------------------------------------------------------------------
class Context {
public:
  static Context& instance() {
    static Context ctx;
    return ctx;
  }

  bool available() const { return available_; }
  const std::string& deviceName() const { return deviceName_; }
  VkPhysicalDeviceType deviceType() const { return deviceType_; }
  VkInstance vkInstance() const { return instance_; }
  VkPhysicalDevice physicalDevice() const { return physicalDevice_; }
  VkDevice device() const { return device_; }
  VkQueue queue() const { return queue_; }
  const VulkanApi& api() const { return api_; }

  bool isUnifiedMemory() const {
    return deviceType_ == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
           deviceType_ == VK_PHYSICAL_DEVICE_TYPE_CPU;
  }

  uint32_t findMemoryType(uint32_t typeBits, VkMemoryPropertyFlags props) const {
    for (uint32_t i = 0; i < memoryProperties_.memoryTypeCount; ++i) {
      if ((typeBits & (1u << i)) &&
          (memoryProperties_.memoryTypes[i].propertyFlags & props) == props) {
        return i;
      }
    }
    return UINT32_MAX;
  }

  // Find host-visible memory, preferring cached system RAM (fast CPU reads)
  // and avoiding the small device-local PCIe BAR window (write-combined).
  uint32_t findHostMemoryType(uint32_t typeBits, VkMemoryPropertyFlags props) const {
    const VkMemoryPropertyFlags cached = props | VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
    for (uint32_t i = 0; i < memoryProperties_.memoryTypeCount; ++i) {
      const VkMemoryPropertyFlags flags = memoryProperties_.memoryTypes[i].propertyFlags;
      if ((typeBits & (1u << i)) && (flags & cached) == cached) return i;
    }
    for (uint32_t i = 0; i < memoryProperties_.memoryTypeCount; ++i) {
      const VkMemoryPropertyFlags flags = memoryProperties_.memoryTypes[i].propertyFlags;
      if ((typeBits & (1u << i)) && (flags & props) == props &&
          !(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) {
        return i;
      }
    }
    return findMemoryType(typeBits, props);
  }

  VkCommandBuffer beginOneShot() {
    VkCommandBufferAllocateInfo ai{};
    ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    ai.commandPool = commandPool_;
    ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    ai.commandBufferCount = 1;
    VkCommandBuffer cb = VK_NULL_HANDLE;
    api_.vkAllocateCommandBuffers(device_, &ai, &cb);

    VkCommandBufferBeginInfo bi{};
    bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    api_.vkBeginCommandBuffer(cb, &bi);
    return cb;
  }

  void endOneShot(VkCommandBuffer cb) {
    api_.vkEndCommandBuffer(cb);
    VkSubmitInfo si{};
    si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    si.commandBufferCount = 1;
    si.pCommandBuffers = &cb;
    api_.vkQueueSubmit(queue_, 1, &si, VK_NULL_HANDLE);
    api_.vkQueueWaitIdle(queue_);
    api_.vkFreeCommandBuffers(device_, commandPool_, 1, &cb);
  }

  void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize bytes) {
    VkCommandBuffer cb = beginOneShot();
    VkBufferCopy region{};
    region.srcOffset = 0;
    region.dstOffset = 0;
    region.size = bytes;
    api_.vkCmdCopyBuffer(cb, src, dst, 1, &region);
    endOneShot(cb);
  }

  // Reusable host-visible staging buffer. All queue operations are synchronous
  // (submit + wait idle), so the same buffer can be safely reused.
  struct Staging {
    VkBuffer buffer = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
  };

  Staging acquireStaging(VkDeviceSize bytes) {
    if (stagingBuffer_ != VK_NULL_HANDLE && stagingSize_ >= bytes) {
      return Staging{stagingBuffer_, stagingMemory_};
    }
    releaseStaging();
    bool host = false;
    if (!createBuffer(bytes,
                      VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                          VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                      &stagingBuffer_, &stagingMemory_, &host, true)) {
      return Staging{};
    }
    stagingSize_ = bytes;
    return Staging{stagingBuffer_, stagingMemory_};
  }

  void waitIdle() {
    if (device_) api_.vkDeviceWaitIdle(device_);
  }

  // Allocate a buffer object + memory. Prefers device-local storage and falls
  // back to host-visible coherent memory (integrated GPU, software driver).
  // When requireHostVisible is set (staging buffers), host-visible memory is
  // selected directly.
  bool createBuffer(VkDeviceSize bytes, VkBufferUsageFlags usage,
                    VkBuffer* buffer, VkDeviceMemory* memory, bool* hostVisible,
                    bool requireHostVisible = false) {
    VkBufferCreateInfo bi{};
    bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bi.size = bytes;
    bi.usage = usage;
    bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (api_.vkCreateBuffer(device_, &bi, nullptr, buffer) != VK_SUCCESS) {
      return false;
    }

    VkMemoryRequirements req{};
    api_.vkGetBufferMemoryRequirements(device_, *buffer, &req);

    const VkMemoryPropertyFlags hostFlags =
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

    VkMemoryPropertyFlags props = requireHostVisible
                                      ? hostFlags
                                      : VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    uint32_t typeIndex = requireHostVisible
                             ? findHostMemoryType(req.memoryTypeBits, props)
                             : findMemoryType(req.memoryTypeBits, props);
    bool host = requireHostVisible;
    if (typeIndex == UINT32_MAX && !requireHostVisible) {
      props = hostFlags;
      typeIndex = findMemoryType(req.memoryTypeBits, props);
      host = true;
    }
    if (typeIndex == UINT32_MAX) {
      api_.vkDestroyBuffer(device_, *buffer, nullptr);
      *buffer = VK_NULL_HANDLE;
      return false;
    }

    VkMemoryAllocateInfo ai{};
    ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ai.allocationSize = req.size;
    ai.memoryTypeIndex = typeIndex;
    if (api_.vkAllocateMemory(device_, &ai, nullptr, memory) != VK_SUCCESS) {
      api_.vkDestroyBuffer(device_, *buffer, nullptr);
      *buffer = VK_NULL_HANDLE;
      return false;
    }
    if (api_.vkBindBufferMemory(device_, *buffer, *memory, 0) != VK_SUCCESS) {
      api_.vkFreeMemory(device_, *memory, nullptr);
      api_.vkDestroyBuffer(device_, *buffer, nullptr);
      *buffer = VK_NULL_HANDLE;
      *memory = VK_NULL_HANDLE;
      return false;
    }
    *hostVisible = host;
    return true;
  }

private:
  Context() { init(); }
  ~Context() { shutdown(); }
  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;

  void init() {
    if (!api_.loadLibrary()) return;

    VkApplicationInfo app{};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "Legolas++";
    app.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app.pEngineName = "Legolas++";
    app.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ci.pApplicationInfo = &app;
    if (api_.vkCreateInstance(&ci, nullptr, &instance_) != VK_SUCCESS) return;
    if (!api_.loadInstance(instance_)) return;

    uint32_t deviceCount = 0;
    if (api_.vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr) != VK_SUCCESS ||
        deviceCount == 0) {
      return;
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    api_.vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

    int bestScore = -1;
    for (VkPhysicalDevice pd : devices) {
      VkPhysicalDeviceProperties props{};
      api_.vkGetPhysicalDeviceProperties(pd, &props);

      uint32_t queueCount = 0;
      api_.vkGetPhysicalDeviceQueueFamilyProperties(pd, &queueCount, nullptr);
      if (queueCount == 0) continue;
      std::vector<VkQueueFamilyProperties> queues(queueCount);
      api_.vkGetPhysicalDeviceQueueFamilyProperties(pd, &queueCount, queues.data());

      uint32_t queueFamily = UINT32_MAX;
      for (uint32_t i = 0; i < queueCount; ++i) {
        if (queues[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
          queueFamily = i;
          break;
        }
      }
      if (queueFamily == UINT32_MAX) continue;

      int score = 10;
      if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score = 100;
      else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) score = 50;
      else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU) score = 20;

      if (score > bestScore) {
        bestScore = score;
        physicalDevice_ = pd;
        queueFamily_ = queueFamily;
        deviceName_ = props.deviceName;
        deviceType_ = props.deviceType;
      }
    }
    if (physicalDevice_ == VK_NULL_HANDLE) return;

    float priority = 1.0f;
    VkDeviceQueueCreateInfo qci{};
    qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qci.queueFamilyIndex = queueFamily_;
    qci.queueCount = 1;
    qci.pQueuePriorities = &priority;

    VkDeviceCreateInfo dci{};
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &qci;
    if (api_.vkCreateDevice(physicalDevice_, &dci, nullptr, &device_) != VK_SUCCESS) {
      return;
    }

    api_.vkGetDeviceQueue(device_, queueFamily_, 0, &queue_);
    api_.vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memoryProperties_);

    VkCommandPoolCreateInfo cpi{};
    cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cpi.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cpi.queueFamilyIndex = queueFamily_;
    if (api_.vkCreateCommandPool(device_, &cpi, nullptr, &commandPool_) != VK_SUCCESS) {
      return;
    }

    available_ = true;
  }

  void shutdown() {
    if (device_) {
      api_.vkDeviceWaitIdle(device_);
      releaseStaging();
      if (commandPool_) api_.vkDestroyCommandPool(device_, commandPool_, nullptr);
      api_.vkDestroyDevice(device_, nullptr);
    }
    if (instance_ && api_.vkDestroyInstance) {
      api_.vkDestroyInstance(instance_, nullptr);
    }
    device_ = VK_NULL_HANDLE;
    commandPool_ = VK_NULL_HANDLE;
    instance_ = VK_NULL_HANDLE;
  }

  void releaseStaging() {
    if (!device_) return;
    if (stagingBuffer_) api_.vkDestroyBuffer(device_, stagingBuffer_, nullptr);
    if (stagingMemory_) api_.vkFreeMemory(device_, stagingMemory_, nullptr);
    stagingBuffer_ = VK_NULL_HANDLE;
    stagingMemory_ = VK_NULL_HANDLE;
    stagingSize_ = 0;
  }

  VulkanApi api_;
  bool available_ = false;
  VkInstance instance_ = VK_NULL_HANDLE;
  VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;
  VkQueue queue_ = VK_NULL_HANDLE;
  VkCommandPool commandPool_ = VK_NULL_HANDLE;
  VkBuffer stagingBuffer_ = VK_NULL_HANDLE;
  VkDeviceMemory stagingMemory_ = VK_NULL_HANDLE;
  VkDeviceSize stagingSize_ = 0;
  uint32_t queueFamily_ = UINT32_MAX;
  VkPhysicalDeviceMemoryProperties memoryProperties_{};
  std::string deviceName_;
  VkPhysicalDeviceType deviceType_ = VK_PHYSICAL_DEVICE_TYPE_OTHER;
};

// ---------------------------------------------------------------------------
// Buffer: device-local storage with staging-based upload/download. On unified
// memory devices the buffer is host-visible and transfers are plain memcpy.
// ---------------------------------------------------------------------------
class Buffer {
public:
  Buffer() = default;

  Buffer(Context& ctx, VkDeviceSize bytes) : ctx_(&ctx), size_(bytes) { create(); }

  ~Buffer() { destroy(); }

  Buffer(Buffer&& other) noexcept { moveFrom(other); }

  Buffer& operator=(Buffer&& other) noexcept {
    if (this != &other) {
      destroy();
      moveFrom(other);
    }
    return *this;
  }

  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

  bool valid() const { return buffer_ != VK_NULL_HANDLE; }
  VkBuffer handle() const { return buffer_; }
  VkDeviceSize size() const { return size_; }
  bool hostVisible() const { return hostVisible_; }

  void upload(const void* data, VkDeviceSize bytes) {
    if (!valid() || bytes == 0) return;
    if (bytes > size_) bytes = size_;
    if (hostVisible_) {
      void* mapped = nullptr;
      if (ctx_->api().vkMapMemory(ctx_->device(), memory_, 0, bytes, 0, &mapped) !=
              VK_SUCCESS ||
          mapped == nullptr) {
        return;
      }
      std::memcpy(mapped, data, static_cast<size_t>(bytes));
      ctx_->api().vkUnmapMemory(ctx_->device(), memory_);
      return;
    }
    Context::Staging staging = ctx_->acquireStaging(bytes);
    if (staging.buffer == VK_NULL_HANDLE) return;
    void* mapped = nullptr;
    if (ctx_->api().vkMapMemory(ctx_->device(), staging.memory, 0, bytes, 0,
                                &mapped) != VK_SUCCESS ||
        mapped == nullptr) {
      return;
    }
    std::memcpy(mapped, data, static_cast<size_t>(bytes));
    ctx_->api().vkUnmapMemory(ctx_->device(), staging.memory);
    ctx_->copyBuffer(staging.buffer, buffer_, bytes);
  }

  void download(void* data, VkDeviceSize bytes) {
    if (!valid() || bytes == 0) return;
    if (bytes > size_) bytes = size_;
    if (hostVisible_) {
      void* mapped = nullptr;
      if (ctx_->api().vkMapMemory(ctx_->device(), memory_, 0, bytes, 0, &mapped) !=
              VK_SUCCESS ||
          mapped == nullptr) {
        return;
      }
      std::memcpy(data, mapped, static_cast<size_t>(bytes));
      ctx_->api().vkUnmapMemory(ctx_->device(), memory_);
      return;
    }
    Context::Staging staging = ctx_->acquireStaging(bytes);
    if (staging.buffer == VK_NULL_HANDLE) return;
    ctx_->copyBuffer(buffer_, staging.buffer, bytes);
    void* mapped = nullptr;
    if (ctx_->api().vkMapMemory(ctx_->device(), staging.memory, 0, bytes, 0,
                                &mapped) != VK_SUCCESS ||
        mapped == nullptr) {
      return;
    }
    std::memcpy(data, mapped, static_cast<size_t>(bytes));
    ctx_->api().vkUnmapMemory(ctx_->device(), staging.memory);
  }

private:
  void create() {
    if (!ctx_ || !ctx_->available() || size_ == 0) return;
    bool host = false;
    if (!ctx_->createBuffer(size_,
                            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                            &buffer_, &memory_, &host)) {
      return;
    }
    hostVisible_ = host;
  }

  void destroy() {
    if (!ctx_ || !ctx_->available()) return;
    if (buffer_) ctx_->api().vkDestroyBuffer(ctx_->device(), buffer_, nullptr);
    if (memory_) ctx_->api().vkFreeMemory(ctx_->device(), memory_, nullptr);
    buffer_ = VK_NULL_HANDLE;
    memory_ = VK_NULL_HANDLE;
    size_ = 0;
    hostVisible_ = false;
  }

  void moveFrom(Buffer& other) {
    ctx_ = other.ctx_;
    buffer_ = other.buffer_;
    memory_ = other.memory_;
    size_ = other.size_;
    hostVisible_ = other.hostVisible_;
    other.ctx_ = nullptr;
    other.buffer_ = VK_NULL_HANDLE;
    other.memory_ = VK_NULL_HANDLE;
    other.size_ = 0;
    other.hostVisible_ = false;
  }

  Context* ctx_ = nullptr;
  VkBuffer buffer_ = VK_NULL_HANDLE;
  VkDeviceMemory memory_ = VK_NULL_HANDLE;
  VkDeviceSize size_ = 0;
  bool hostVisible_ = false;
};

}} // namespace Legolas::Vulkan
