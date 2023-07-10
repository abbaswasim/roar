// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the 'Software'),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version: 1.0.0

#pragma once

#include "core/settings/rorsettings.hpp"
#include "foundation/rormacros.hpp"
#include "rhi/vulkan/rorvulkan_object.hpp"

namespace cfg
{
// TODO: make a better allocator
static VkAllocationCallbacks *VkAllocator = nullptr;
}        // namespace cfg

namespace rhi
{

// TODO: turn some of these smaller functions into inlines

VkDevice                        vk_create_device(VkPhysicalDevice physical_device, std::vector<VkDeviceQueueCreateInfo> &queues);
VkSwapchainKHR                  vk_create_swapchain(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat &a_format, VkExtent2D a_swapchain_extent);
VkImageView                     vk_create_image_view(VkDevice a_device, VkImage a_image, VkFormat a_format, uint32_t a_mip_levels, VkImageAspectFlags a_aspect_flags,
                                                     VkImageViewType    a_type      = VK_IMAGE_VIEW_TYPE_2D,
                                                     VkComponentSwizzle a_r_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     VkComponentSwizzle a_g_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     VkComponentSwizzle a_b_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     VkComponentSwizzle a_a_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY);
VkImage                         vk_create_image(VkDevice a_device, uint32_t a_width, uint32_t a_height, uint32_t a_depth, VkFormat a_format, uint32_t a_mip_levels, VkImageUsageFlags a_usage,
                                                VkImageType a_image_type = VK_IMAGE_TYPE_2D, VkImageTiling a_tiling = VK_IMAGE_TILING_OPTIMAL, VkImageLayout a_initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
                                                VkSharingMode a_sharing_mode = VK_SHARING_MODE_EXCLUSIVE, VkSampleCountFlagBits a_samples_count = VK_SAMPLE_COUNT_1_BIT, std::vector<uint32_t> a_queue_family_indices = {});
void                            vk_create_image_with_memory(VkDevice a_device, VkImage &a_image, uint32_t a_width, uint32_t a_height, uint32_t a_depth, VkFormat a_format, uint32_t a_mip_levels, VkImageUsageFlags a_usage,
                                                            VkDeviceMemory &a_memory, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties,
                                                            VkImageType a_image_type = VK_IMAGE_TYPE_2D, VkImageTiling a_tiling = VK_IMAGE_TILING_OPTIMAL, VkImageLayout a_initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
                                                            VkSharingMode a_sharing_mode = VK_SHARING_MODE_EXCLUSIVE, VkSampleCountFlagBits a_samples_count = VK_SAMPLE_COUNT_1_BIT, std::vector<uint32_t> a_queue_family_indices = {});
VkSampler                       vk_create_image_sampler(VkDevice a_device, VkSamplerAddressMode a_wrap_u, VkSamplerAddressMode a_wrap_v, VkSamplerAddressMode a_wrap_w,
                                                        VkFilter a_min_filter, VkFilter a_mag_filter, VkSamplerMipmapMode a_mip_filter,
                                                        VkBorderColor a_border_color = VK_BORDER_COLOR_INT_OPAQUE_BLACK, bool a_compare = false, VkCompareOp a_comapre_op = VK_COMPARE_OP_ALWAYS,
                                                        bool a_enable_anisotropy = false, uint32_t a_max_anisotropy = 1.0f, bool a_unormalized_coords = false);
VkPipelineShaderStageCreateInfo vk_create_pipeline_shader_stage(VkShaderStageFlagBits a_shader_stage_flags, const char *a_name, VkShaderModule a_shader_module);
VkShaderModule                  vk_create_shader_module(VkDevice a_device, const std::vector<uint32_t> &a_spirv_code);
VkBuffer                        vk_create_buffer(VkDevice a_device, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, std::vector<uint32_t> a_family_queue_indices = {});
VkDeviceMemory                  vk_allocate_memory(VkDevice a_device, VkDeviceSize a_size, uint32_t a_memory_type_index);
VkMemoryRequirements2           vk_buffer_memory_requirements(VkDevice a_device, VkBuffer a_buffer);
VkMemoryRequirements2           vk_image_memory_requirements(VkDevice a_device, VkImage a_image);
void                            vk_bind_buffer_memory(VkDevice a_device, VkBuffer a_buffer, VkDeviceMemory a_memory);
VkDeviceMemory                  vk_bind_buffer_memory(VkDevice a_device, VkBuffer a_buffer, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties);
void                            vk_create_buffer_with_memory(VkDevice a_device, VkBuffer &a_buffer, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, std::vector<uint32_t> &a_queue_family_indices,
                                                             VkDeviceMemory &a_memory, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties);
uint32_t                        vk_find_memory_type(uint32_t a_type_filter, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties);
uint8_t                        *vk_map_memory(VkDevice a_device, VkDeviceMemory a_memory);
void                            vk_unmap_memory(VkDevice a_device, VkDeviceMemory a_memory);
VkCommandBuffer                 vk_begin_single_use_command_buffer(VkDevice a_device, VkCommandPool a_command_pool);
void                            vk_end_single_use_command_buffer(VkCommandBuffer a_command_buffer, VkQueue a_queue);
void                            vk_end_single_use_command_buffer_and_wait(VkDevice a_device, VkCommandBuffer a_command_buffer, VkQueue a_queue, VkCommandPool a_command_pool);
VkCommandPool                   vk_create_command_pools(VkDevice a_device, uint32_t a_queue_family_index, VkCommandPoolCreateFlags a_flags);        // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
VkCommandBuffer                 vk_allocate_command_buffer(VkDevice a_device, VkCommandPool a_command_pool);
void                            vk_begin_command_buffer(VkCommandBuffer a_command_buffer, VkCommandBufferBeginInfo &a_command_buffer_begin_info);
void                            vk_end_command_buffer(VkCommandBuffer a_command_buffer);
void                            vk_queue_submit(VkQueue a_queue, VkSubmitInfo &a_submit_info, VkFence a_fence);
void                            vk_queue_submit(VkQueue a_queue, std::vector<VkSubmitInfo> &a_submit_info, VkFence a_fence);
void                            vk_queue_submit(VkQueue a_queue, VkCommandBuffer a_command_buffer, VkFence a_fence);
void                            vk_queue_submit(VkQueue a_queue, VkSubmitInfo &a_submit_info, std::vector<VkCommandBuffer> a_command_buffers, VkFence a_fence);
void                            vk_queue_wait_idle(VkQueue a_queue);
void                            vk_transition_image_layout(VkDevice a_device, VkCommandPool a_command_pool, VkQueue a_transfer_queue, VkImage a_image, uint32_t a_mip_levels, VkImageLayout a_old_layout, VkImageLayout a_new_layout);
void                            vk_copy_staging_buffer_to_image(VkDevice a_device, VkQueue transfer_queue, VkCommandPool a_command_pool,
                                                                VkBuffer a_source, VkImage a_destination, std::vector<VkBufferImageCopy> buffer_image_copy_regions);
void                            vk_copy_staging_buffers_to_images(VkDevice a_device, VkQueue transfer_queue, VkCommandPool a_command_pool,
                                                                  std::vector<VkBuffer> &a_source, std::vector<VkImage> &a_destination, std::vector<VkBufferImageCopy> buffer_image_copy_regions);

FORCE_INLINE void vk_destroy_swapchain(VkDevice a_device, VkSwapchainKHR a_swapchain);
FORCE_INLINE void vk_destroy_imageview(VkDevice a_device, VkImageView a_image_view);
FORCE_INLINE void vk_destroy_surface(VkInstance a_instance, VkSurfaceKHR a_surface);
FORCE_INLINE void vk_destroy_device(VkDevice a_device);
FORCE_INLINE void vk_destroy_pipeline(VkDevice a_device, VkPipeline a_pipeline);
FORCE_INLINE void vk_destroy_shader_module(VkDevice a_device, VkShaderModule a_module);
FORCE_INLINE void vk_destroy_buffer(VkDevice a_device, VkBuffer a_buffer);
FORCE_INLINE void vk_destroy_memory(VkDevice a_device, VkDeviceMemory a_memory);
FORCE_INLINE void vk_destroy_command_pools(VkDevice a_device, VkCommandPool a_command_pool);
FORCE_INLINE void vk_destroy_command_buffer(VkDevice a_device, VkCommandBuffer a_command_buffer, VkCommandPool a_command_pool);
FORCE_INLINE void vk_destroy_image(VkDevice a_device, VkImage a_image);
FORCE_INLINE void vk_destroy_image_view(VkDevice a_device, VkImageView a_view);
FORCE_INLINE void vk_destroy_image_sampler(VkDevice a_device, VkSampler a_sampler);

}        // namespace rhi

#include "rhi/vulkan/rorvulkan_utils.hh"
