#pragma once

#include "Legolas/Vulkan/VulkanContext.hxx"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace Legolas {
namespace Vulkan {

// ---------------------------------------------------------------------------
// Kernel: a compute pipeline with `bindingCount` storage-buffer bindings
// (binding i -> buffer i) and an optional push-constant block.
//
// dispatch() records `repeat` back-to-back invocations inside a single
// command buffer (with the required memory barriers) and submits once, which
// keeps submission overhead negligible for small grids and mirrors the inner
// loop of the CPU benchmarks.
// ---------------------------------------------------------------------------
class Kernel {
public:
  Kernel() = default;

  Kernel(Context& ctx, const uint32_t* spv, size_t spvWordCount,
         uint32_t bindingCount, uint32_t pushConstantBytes = 0)
      : ctx_(&ctx), bindingCount_(bindingCount) {
    create(spv, spvWordCount, pushConstantBytes);
  }

  ~Kernel() { destroy(); }

  Kernel(Kernel&& other) noexcept { moveFrom(other); }

  Kernel& operator=(Kernel&& other) noexcept {
    if (this != &other) {
      destroy();
      moveFrom(other);
    }
    return *this;
  }

  Kernel(const Kernel&) = delete;
  Kernel& operator=(const Kernel&) = delete;

  bool valid() const { return pipeline_ != VK_NULL_HANDLE; }

  void dispatch(const Buffer* const* buffers, uint32_t bufferCount,
                uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ,
                const void* pushData = nullptr, uint32_t pushBytes = 0,
                uint32_t repeat = 1) {
    if (!valid() || bufferCount > bindingCount_) return;

    std::vector<VkDescriptorBufferInfo> bufferInfos(bufferCount);
    std::vector<VkWriteDescriptorSet> writes(bufferCount);
    for (uint32_t i = 0; i < bufferCount; ++i) {
      bufferInfos[i].buffer = buffers[i] ? buffers[i]->handle() : VK_NULL_HANDLE;
      bufferInfos[i].offset = 0;
      bufferInfos[i].range = VK_WHOLE_SIZE;

      writes[i] = VkWriteDescriptorSet{};
      writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      writes[i].dstSet = descriptorSet_;
      writes[i].dstBinding = i;
      writes[i].descriptorCount = 1;
      writes[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
      writes[i].pBufferInfo = &bufferInfos[i];
    }
    ctx_->api().vkUpdateDescriptorSets(ctx_->device(), bufferCount, writes.data(),
                                       0, nullptr);

    VkCommandBuffer cb = ctx_->beginOneShot();
    ctx_->api().vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline_);
    ctx_->api().vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_COMPUTE,
                                        pipelineLayout_, 0, 1, &descriptorSet_,
                                        0, nullptr);
    if (pushData && pushBytes > 0) {
      ctx_->api().vkCmdPushConstants(cb, pipelineLayout_,
                                     VK_SHADER_STAGE_COMPUTE_BIT, 0, pushBytes,
                                     pushData);
    }

    std::vector<VkBufferMemoryBarrier> barriers(bufferCount);
    for (uint32_t i = 0; i < bufferCount; ++i) {
      barriers[i] = VkBufferMemoryBarrier{};
      barriers[i].sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
      barriers[i].srcAccessMask =
          VK_ACCESS_TRANSFER_WRITE_BIT | VK_ACCESS_SHADER_WRITE_BIT;
      barriers[i].dstAccessMask = VK_ACCESS_SHADER_READ_BIT |
                                  VK_ACCESS_SHADER_WRITE_BIT |
                                  VK_ACCESS_TRANSFER_READ_BIT;
      barriers[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barriers[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
      barriers[i].buffer = buffers[i] ? buffers[i]->handle() : VK_NULL_HANDLE;
      barriers[i].offset = 0;
      barriers[i].size = VK_WHOLE_SIZE;
    }

    for (uint32_t r = 0; r < repeat; ++r) {
      if (bufferCount > 0) {
        ctx_->api().vkCmdPipelineBarrier(
            cb,
            VK_PIPELINE_STAGE_TRANSFER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_TRANSFER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            0, 0, nullptr, bufferCount, barriers.data(), 0, nullptr);
      }
      ctx_->api().vkCmdDispatch(cb, groupCountX, groupCountY, groupCountZ);
    }
    ctx_->endOneShot(cb);
  }

private:
  void create(const uint32_t* spv, size_t spvWordCount, uint32_t pushConstantBytes) {
    if (!ctx_ || !ctx_->available()) return;

    VkShaderModuleCreateInfo smi{};
    smi.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    smi.codeSize = spvWordCount * sizeof(uint32_t);
    smi.pCode = spv;
    if (ctx_->api().vkCreateShaderModule(ctx_->device(), &smi, nullptr, &module_) !=
        VK_SUCCESS) {
      return;
    }

    std::vector<VkDescriptorSetLayoutBinding> bindings(bindingCount_);
    for (uint32_t i = 0; i < bindingCount_; ++i) {
      bindings[i] = VkDescriptorSetLayoutBinding{};
      bindings[i].binding = i;
      bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
      bindings[i].descriptorCount = 1;
      bindings[i].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    }

    VkDescriptorSetLayoutCreateInfo dli{};
    dli.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    dli.bindingCount = bindingCount_;
    dli.pBindings = bindings.empty() ? nullptr : bindings.data();
    if (ctx_->api().vkCreateDescriptorSetLayout(ctx_->device(), &dli, nullptr,
                                                &setLayout_) != VK_SUCCESS) {
      return;
    }

    VkPushConstantRange range{};
    range.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    range.offset = 0;
    range.size = pushConstantBytes;

    VkPipelineLayoutCreateInfo pli{};
    pli.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pli.setLayoutCount = 1;
    pli.pSetLayouts = &setLayout_;
    pli.pushConstantRangeCount = pushConstantBytes > 0 ? 1 : 0;
    pli.pPushConstantRanges = pushConstantBytes > 0 ? &range : nullptr;
    if (ctx_->api().vkCreatePipelineLayout(ctx_->device(), &pli, nullptr,
                                           &pipelineLayout_) != VK_SUCCESS) {
      return;
    }

    VkComputePipelineCreateInfo cpci{};
    cpci.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    cpci.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    cpci.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    cpci.stage.module = module_;
    cpci.stage.pName = "main";
    cpci.layout = pipelineLayout_;
    if (ctx_->api().vkCreateComputePipelines(ctx_->device(), VK_NULL_HANDLE, 1,
                                             &cpci, nullptr,
                                             &pipeline_) != VK_SUCCESS) {
      return;
    }

    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSize.descriptorCount = bindingCount_ > 0 ? bindingCount_ : 1;

    VkDescriptorPoolCreateInfo dpi{};
    dpi.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    dpi.maxSets = 1;
    dpi.poolSizeCount = 1;
    dpi.pPoolSizes = &poolSize;
    if (ctx_->api().vkCreateDescriptorPool(ctx_->device(), &dpi, nullptr,
                                           &pool_) != VK_SUCCESS) {
      return;
    }

    VkDescriptorSetAllocateInfo dsai{};
    dsai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    dsai.descriptorPool = pool_;
    dsai.descriptorSetCount = 1;
    dsai.pSetLayouts = &setLayout_;
    ctx_->api().vkAllocateDescriptorSets(ctx_->device(), &dsai, &descriptorSet_);
  }

  void destroy() {
    if (!ctx_ || !ctx_->available()) return;
    Context& ctx = *ctx_;
    if (ctx.device()) {
      if (pool_) ctx.api().vkDestroyDescriptorPool(ctx.device(), pool_, nullptr);
      if (pipeline_) ctx.api().vkDestroyPipeline(ctx.device(), pipeline_, nullptr);
      if (pipelineLayout_)
        ctx.api().vkDestroyPipelineLayout(ctx.device(), pipelineLayout_, nullptr);
      if (setLayout_)
        ctx.api().vkDestroyDescriptorSetLayout(ctx.device(), setLayout_, nullptr);
      if (module_) ctx.api().vkDestroyShaderModule(ctx.device(), module_, nullptr);
    }
    module_ = VK_NULL_HANDLE;
    setLayout_ = VK_NULL_HANDLE;
    pipelineLayout_ = VK_NULL_HANDLE;
    pipeline_ = VK_NULL_HANDLE;
    pool_ = VK_NULL_HANDLE;
    descriptorSet_ = VK_NULL_HANDLE;
  }

  void moveFrom(Kernel& other) {
    ctx_ = other.ctx_;
    module_ = other.module_;
    setLayout_ = other.setLayout_;
    pipelineLayout_ = other.pipelineLayout_;
    pipeline_ = other.pipeline_;
    pool_ = other.pool_;
    descriptorSet_ = other.descriptorSet_;
    bindingCount_ = other.bindingCount_;
    other.ctx_ = nullptr;
    other.module_ = VK_NULL_HANDLE;
    other.setLayout_ = VK_NULL_HANDLE;
    other.pipelineLayout_ = VK_NULL_HANDLE;
    other.pipeline_ = VK_NULL_HANDLE;
    other.pool_ = VK_NULL_HANDLE;
    other.descriptorSet_ = VK_NULL_HANDLE;
    other.bindingCount_ = 0;
  }

  Context* ctx_ = nullptr;
  VkShaderModule module_ = VK_NULL_HANDLE;
  VkDescriptorSetLayout setLayout_ = VK_NULL_HANDLE;
  VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;
  VkPipeline pipeline_ = VK_NULL_HANDLE;
  VkDescriptorPool pool_ = VK_NULL_HANDLE;
  VkDescriptorSet descriptorSet_ = VK_NULL_HANDLE;
  uint32_t bindingCount_ = 0;
};

}} // namespace Legolas::Vulkan
