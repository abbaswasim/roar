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
#include <mutex>

namespace cfg
{
// TODO: make a better allocator
static VkAllocationCallbacks *VkAllocator = nullptr;
}        // namespace cfg

namespace rhi
{

// TODO: turn some of these smaller functions into inlines

VkDevice                               vk_create_device(VkPhysicalDevice physical_device, const std::vector<VkDeviceQueueCreateInfo> &queues);
VkSwapchainKHR                         vk_create_swapchain(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat &a_format, VkExtent2D a_swapchain_extent);
VkImageView                            vk_create_image_view(VkDevice a_device, VkImage a_image, VkFormat a_format, uint32_t a_mip_levels, VkImageAspectFlags a_aspect_flags,
                                                            VkImageViewType    a_type      = VK_IMAGE_VIEW_TYPE_2D,
                                                            VkComponentSwizzle a_r_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                            VkComponentSwizzle a_g_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                            VkComponentSwizzle a_b_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                            VkComponentSwizzle a_a_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY);
VkImage                                vk_create_image(VkDevice a_device, uint32_t a_width, uint32_t a_height, uint32_t a_depth, VkFormat a_format, uint32_t a_mip_levels, VkImageUsageFlags a_usage,
                                                       VkImageType a_image_type = VK_IMAGE_TYPE_2D, VkImageTiling a_tiling = VK_IMAGE_TILING_OPTIMAL, VkImageLayout a_initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
                                                       VkSharingMode a_sharing_mode = VK_SHARING_MODE_EXCLUSIVE, VkSampleCountFlagBits a_samples_count = VK_SAMPLE_COUNT_1_BIT, const std::vector<uint32_t> &&a_queue_family_indices = {});
void                                   vk_create_image_with_memory(VkDevice a_device, VkImage &a_image, uint32_t a_width, uint32_t a_height, uint32_t a_depth, VkFormat a_format, uint32_t a_mip_levels, VkImageUsageFlags a_usage,
                                                                   VkDeviceMemory &a_memory, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties,
                                                                   VkImageType a_image_type = VK_IMAGE_TYPE_2D, VkImageTiling a_tiling = VK_IMAGE_TILING_OPTIMAL, VkImageLayout a_initial_layout = VK_IMAGE_LAYOUT_UNDEFINED,
                                                                   VkSharingMode a_sharing_mode = VK_SHARING_MODE_EXCLUSIVE, VkSampleCountFlagBits a_samples_count = VK_SAMPLE_COUNT_1_BIT, const std::vector<uint32_t> &&a_queue_family_indices = {});
VkSampler                              vk_create_image_sampler(VkDevice a_device, VkSamplerAddressMode a_wrap_u, VkSamplerAddressMode a_wrap_v, VkSamplerAddressMode a_wrap_w,
                                                               VkFilter a_min_filter, VkFilter a_mag_filter, VkSamplerMipmapMode a_mip_filter,
                                                               VkBorderColor a_border_color = VK_BORDER_COLOR_INT_OPAQUE_BLACK, bool a_compare = false, VkCompareOp a_comapre_op = VK_COMPARE_OP_ALWAYS,
                                                               bool a_enable_anisotropy = false, uint32_t a_max_anisotropy = 1.0f, bool a_unormalized_coords = false);
VkPipelineDepthStencilStateCreateInfo  vk_create_depth_stencil_state(bool a_depth_test, bool a_depth_write, VkCompareOp a_depth_compare,
                                                                     bool a_stencil_test, VkStencilOpState a_stencil_front = {}, VkStencilOpState a_stencil_back = {},
                                                                     bool a_depth_bound_test = false, float32_t a_min_depth_bound = 0.0f, float32_t a_max_depth_bound = 1.0f);
VkPipelineColorBlendAttachmentState    vk_create_color_blend_attachment_state(bool a_blend_enable = true, bool a_premultiplied_alpha = false);
VkPipelineColorBlendStateCreateInfo    vk_create_color_blend_state(const VkPipelineColorBlendAttachmentState &a_pipeline_color_blend_attachment_state);
VkPipelineDynamicStateCreateInfo       vk_create_dynamic_state(const std::vector<VkDynamicState> &a_dynamic_state);
VkPipelineLayoutCreateInfo             vk_create_pipeline_layout_state(const std::vector<VkDescriptorSetLayout> &a_descriptors_layouts);
VkPipelineLayout                       vk_create_pipeline_layout(const VkDevice a_device, const VkPipelineLayoutCreateInfo &a_pipeline_layout_info);
VkPipelineCache                        vk_create_pipeline_cache(const VkDevice a_device);
VkViewport                             vk_create_viewport(float32_t a_x, float32_t a_y, float32_t a_width, float32_t a_height, float32_t a_min_depth, float32_t a_max_depth);
VkRect2D                               vk_create_scissor(int32_t a_x, int32_t a_y, uint32_t a_width, uint32_t a_height);
VkPipelineViewportStateCreateInfo      vk_create_viewport_state(const VkViewport &a_viewport, const VkRect2D &a_scissor);
VkPipelineRasterizationStateCreateInfo vk_create_pipeline_rasterisation_state(VkPolygonMode a_polygon_mode = VK_POLYGON_MODE_FILL, VkCullModeFlags a_cullmode = VK_CULL_MODE_BACK_BIT,
                                                                              VkFrontFace a_winding = VK_FRONT_FACE_COUNTER_CLOCKWISE, float32_t a_line_width = 1.0f);
VkPipelineMultisampleStateCreateInfo   vk_create_pipeline_multisample_state(VkSampleCountFlagBits a_samples, bool a_alpha_to_coverage = VK_FALSE, bool a_alpha_to_one = VK_FALSE);
VkPipelineShaderStageCreateInfo        vk_create_pipeline_shader_stage(VkShaderStageFlagBits a_shader_stage_flags, const char *a_name, VkShaderModule a_shader_module);
VkPipelineVertexInputStateCreateInfo   vk_create_pipeline_vertex_input_state(const std::vector<VkVertexInputAttributeDescription> &a_vertex_attribute_descriptions, const std::vector<VkVertexInputBindingDescription> &a_vertex_attribute_bindings);
VkPipelineInputAssemblyStateCreateInfo vk_create_pipeline_input_assembly_state(VkPrimitiveTopology a_topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, bool a_primitive_restart = VK_FALSE);
VkShaderModule                         vk_create_shader_module(VkDevice a_device, const std::vector<uint32_t> &a_spirv_code);
VkBuffer                               vk_create_buffer(VkDevice a_device, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, const std::vector<uint32_t> &a_family_queue_indices = {});
VkDeviceMemory                         vk_allocate_memory(VkDevice a_device, VkDeviceSize a_size, uint32_t a_memory_type_index);
VkMemoryRequirements2                  vk_buffer_memory_requirements(VkDevice a_device, VkBuffer a_buffer);
VkMemoryRequirements2                  vk_image_memory_requirements(VkDevice a_device, VkImage a_image);
void                                   vk_bind_buffer_memory(VkDevice a_device, VkBuffer a_buffer, VkDeviceMemory a_memory);
VkDeviceMemory                         vk_bind_buffer_memory(VkDevice a_device, VkBuffer a_buffer, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties);
void                                   vk_create_buffer_with_memory(VkDevice a_device, VkBuffer &a_buffer, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, const std::vector<uint32_t> &a_queue_family_indices,
                                                                    VkDeviceMemory &a_memory, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties);
uint32_t                               vk_find_memory_type(uint32_t a_type_filter, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties);
uint8_t                               *vk_map_memory(VkDevice a_device, VkDeviceMemory a_memory, std::uintptr_t a_offset, std::uintptr_t a_length);
uint8_t                               *vk_map_memory(VkDevice a_device, VkDeviceMemory a_memory);
void                                   vk_unmap_memory(VkDevice a_device, VkDeviceMemory a_memory);
VkCommandBuffer                        vk_begin_single_use_command_buffer(VkDevice a_device, VkCommandPool a_command_pool);
void                                   vk_end_single_use_command_buffer(VkCommandBuffer a_command_buffer, VkQueue a_queue, const std::mutex *a_mutex);
void                                   vk_end_single_use_command_buffer_and_wait(VkDevice a_device, VkCommandBuffer a_command_buffer, VkQueue a_queue, VkCommandPool a_command_pool, std::mutex *a_mutex);
VkCommandPool                          vk_create_command_pools(VkDevice a_device, uint32_t a_queue_family_index, VkCommandPoolCreateFlags a_flags);        // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
VkCommandBuffer                        vk_allocate_command_buffer(VkDevice a_device, VkCommandPool a_command_pool);
VkDescriptorSet                        vk_allocate_descriptor_set(VkDevice a_device, VkDescriptorSetLayout a_layout, VkDescriptorPool a_pool, uint32_t a_count, VkResult *a_result);
VkDescriptorSet                        vk_allocate_descriptor_set(VkDevice a_device, VkDescriptorSetLayout a_layout, VkDescriptorPool a_pool, VkResult *a_result);
VkDescriptorSetLayoutBinding           vk_create_descriptor_set_layout_binding(uint32_t a_binding, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags);
VkDescriptorSetLayoutCreateInfo        vk_create_descriptor_set_layout_info(const std::vector<VkDescriptorSetLayoutBinding> &a_bindings);
VkDescriptorSetLayout                  vk_create_descriptor_set_layout(VkDevice a_device, const std::vector<VkDescriptorSetLayoutBinding> &a_bindings);
VkDescriptorSetLayout                  vk_create_descriptor_set_layout(VkDevice a_device, const VkDescriptorSetLayoutCreateInfo &descriptor_set_layout_createinfo);
VkDescriptorBufferInfo                 vk_create_descriptor_buffer_info(VkBuffer a_buffer);
VkDescriptorImageInfo                  vk_create_descriptor_image_info(VkImageView a_view, VkSampler a_sampler, VkImageLayout a_layout);
VkWriteDescriptorSet                   vk_create_write_descriptor_set(VkDescriptorSet a_descriptor_set, uint32_t a_binding, VkDescriptorType a_type, VkDescriptorImageInfo *a_image_info, VkDescriptorBufferInfo *a_buffer_info);
void                                   vk_begin_command_buffer(VkCommandBuffer a_command_buffer, VkCommandBufferUsageFlags a_flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
void                                   vk_begin_command_buffer(VkCommandBuffer a_command_buffer, VkCommandBufferBeginInfo &a_command_buffer_begin_info);
void                                   vk_end_command_buffer(VkCommandBuffer a_command_buffer);
void                                   vk_queue_submit(VkQueue a_queue, uint32_t a_submit_info_count, const VkSubmitInfo *a_submit_info, VkFence a_fence, std::mutex *a_mutex);
void                                   vk_queue_submit(VkQueue a_queue, const std::vector<VkSubmitInfo> &a_submit_info, VkFence a_fence, std::mutex *a_mutex);
void                                   vk_queue_submit(VkQueue a_queue, VkCommandBuffer a_command_buffer, VkFence a_fence, std::mutex *a_mutex);
void                                   vk_queue_submit(VkQueue a_queue, VkSubmitInfo &a_submit_info, const std::vector<VkCommandBuffer> &a_command_buffers, VkFence a_fence, std::mutex *a_mutex);
void                                   vk_queue_wait_idle(VkQueue a_queue, std::mutex *a_mutex);
void                                   vk_transition_image_layout(const VkDevice a_device, VkCommandPool a_command_pool, VkQueue a_transfer_queue, VkImage a_image, uint32_t a_mip_levels, VkImageLayout a_old_layout, VkImageLayout a_new_layout, std::mutex *a_mutex);
void                                   vk_copy_staging_buffer_to_image(VkDevice a_device, VkQueue transfer_queue, VkCommandPool a_command_pool,
                                                                       VkBuffer a_source, VkImage a_destination, const std::vector<VkBufferImageCopy> &buffer_image_copy_regions, std::mutex *a_mutex);
void                                   vk_copy_staging_buffers_to_images(VkDevice a_device, VkQueue transfer_queue, VkCommandPool a_command_pool,
                                                                         const std::vector<VkBuffer> &a_source, const std::vector<VkImage> &a_destination, const std::vector<VkBufferImageCopy> &buffer_image_copy_regions, std::mutex *a_mutex);
VkRenderPass                           vk_create_render_pass(VkDevice a_device, const VkAttachmentDescription &a_attachments, const VkSubpassDescription &a_subpasses, const VkSubpassDependency &a_dependencies);
VkRenderPass                           vk_create_render_pass(VkDevice a_device, const std::vector<VkAttachmentDescription> &&a_attachments, const std::vector<VkSubpassDescription> &a_subpasses, const std::vector<VkSubpassDependency> &a_dependencies);
VkAttachmentDescription                vk_create_attachment_description(VkFormat a_format, VkSampleCountFlagBits a_samples = VK_SAMPLE_COUNT_4_BIT,
                                                                        VkAttachmentLoadOp  a_load_op          = VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                        VkAttachmentStoreOp a_store_op         = VK_ATTACHMENT_STORE_OP_STORE,
                                                                        VkAttachmentLoadOp  a_stencil_load_op  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                        VkAttachmentStoreOp a_stencil_store_op = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                        VkImageLayout       a_initial_layout   = VK_IMAGE_LAYOUT_UNDEFINED,
                                                                        VkImageLayout       a_final_layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
VkComputePipelineCreateInfo            vk_create_compute_pipeline_state(VkPipelineShaderStageCreateInfo &a_shader_stages, VkPipelineLayout &a_pipeline_layout);
VkGraphicsPipelineCreateInfo           vk_create_graphics_pipeline_state(const std::vector<VkPipelineShaderStageCreateInfo> &a_shader_stages,
                                                                         VkPipelineVertexInputStateCreateInfo &a_pipeline_vertex_input_state_info, VkPipelineInputAssemblyStateCreateInfo &a_pipeline_input_assembly_state_info,
                                                                         VkPipelineViewportStateCreateInfo &a_pipeline_viewport_state_info, VkPipelineRasterizationStateCreateInfo &a_pipeline_rasterisation_state_info,
                                                                         VkPipelineMultisampleStateCreateInfo &a_pipeline_multisample_state_info, VkPipelineDepthStencilStateCreateInfo &a_pipeline_depth_stencil_state_info,
                                                                         VkPipelineColorBlendStateCreateInfo &a_pipeline_color_blend_state_info, VkPipelineDynamicStateCreateInfo &a_pipeline_dynamic_state_info,
                                                                         VkPipelineLayout &a_pipeline_layout, VkRenderPass &a_render_pass, uint32_t a_render_subpass);
VkPipeline                             vk_create_compute_pipeline(const VkDevice                   a_device,
                                                                  VkPipelineCache                 &a_pipeline_cache,
                                                                  VkPipelineLayout                &a_pipeline_layout,
                                                                  VkPipelineShaderStageCreateInfo &a_shader_stage);
VkPipeline                             vk_create_graphics_pipeline(const VkDevice a_device, VkPipelineCache &a_pipeline_cache,
                                                                   const std::vector<VkPipelineShaderStageCreateInfo> &a_shader_stages, VkPipelineVertexInputStateCreateInfo &a_pipeline_vertex_input_state_info,
                                                                   VkPipelineInputAssemblyStateCreateInfo &a_pipeline_input_assembly_state_info, VkPipelineViewportStateCreateInfo &a_pipeline_viewport_state_info,
                                                                   VkPipelineRasterizationStateCreateInfo &a_pipeline_rasterisation_state_info, VkPipelineMultisampleStateCreateInfo &a_pipeline_multisample_state_info,
                                                                   VkPipelineDepthStencilStateCreateInfo &a_pipeline_depth_stencil_state_info, VkPipelineColorBlendStateCreateInfo &a_pipeline_color_blend_state_info,
                                                                   VkPipelineDynamicStateCreateInfo &a_pipeline_dynamic_state_info, VkPipelineLayout &a_pipeline_layout,
                                                                   VkRenderPass &a_render_pass, uint32_t a_render_subpass);
VkPipeline                             vk_create_graphics_pipeline(const VkDevice a_device, VkPipelineCache &a_pipeline_cache, const VkGraphicsPipelineCreateInfo &a_graphics_pipeline_create_info);
VkAttachmentReference                  vk_create_attachment_reference(uint32_t a_attachment_index, VkImageLayout a_image_layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
VkSubpassDescription                   vk_create_subpass_description(VkPipelineBindPoint   a_pipeline_bind_point,
                                                                     VkAttachmentReference a_color_attachment_reference, VkAttachmentReference a_depth_attachment_reference, VkAttachmentReference a_resolve_attachment_reference);
VkSubpassDescription                   vk_create_subpass_description(VkPipelineBindPoint a_pipeline_bind_point, VkAttachmentReference a_input_attachment,
                                                                     VkAttachmentReference a_color_attachment_reference, VkAttachmentReference a_depth_attachment_reference, VkAttachmentReference a_resolve_attachment_reference);
VkSubpassDescription                   vk_create_subpass_description(VkPipelineBindPoint a_pipeline_bind_point, const std::vector<VkAttachmentReference> &a_input_attachments,
                                                                     VkAttachmentReference a_color_attachment_reference, VkAttachmentReference a_depth_attachment_reference, VkAttachmentReference a_resolve_attachment_reference);
VkSubpassDescription                   vk_create_subpass_description(VkPipelineBindPoint a_pipeline_bind_point, const std::vector<VkAttachmentReference> &a_input_attachments, const std::vector<VkAttachmentReference> &a_color_attachment_reference,
                                                                     const VkAttachmentReference *a_depth_attachment_reference, const VkAttachmentReference *a_resolve_attachment_reference);
VkSubpassDependency                    vk_create_subpass_dependency(uint32_t a_src_subpass, uint32_t a_dst_subpass, VkPipelineStageFlags a_src_stage_mask, VkPipelineStageFlags a_dst_stage_mask,
                                                                    VkAccessFlags a_src_access_mask, VkAccessFlags a_dst_access_mask, VkDependencyFlags a_dependency_flags = 0);        //, int32_t a_view_offset = 0);
VkImageMemoryBarrier2                  vk_create_image_barrier(VkImage a_image, VkPipelineStageFlags2 a_src_stage_mask, VkAccessFlags2 a_src_access_mask, VkImageLayout a_old_layout, VkImageLayout a_new_layout,
                                                               VkPipelineStageFlags2 a_dst_stage_mask, VkAccessFlags2 a_dst_access_mask, VkImageAspectFlags a_aspect_mask, uint32_t a_base_mip_level, uint32_t a_mip_level_count);
VkBufferMemoryBarrier2                 vk_create_buffer_barrier(VkBuffer a_buffer, VkPipelineStageFlags2 a_src_stage_mask, VkAccessFlags2 a_src_access_mask, VkPipelineStageFlags2 a_dst_stage_mask, VkAccessFlags2 a_dst_access_mask);
VkFramebuffer                          vk_create_framebuffer(VkDevice a_device, VkRenderPass a_renderpass, const std::vector<VkImageView> &a_attachments, VkExtent2D a_dimensions, uint32_t a_layers = 1);
VkFence                                vk_create_fence(VkDevice a_device, VkFenceCreateFlags a_flags = 0);

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
FORCE_INLINE void vk_destroy_render_pass(VkDevice a_device);
FORCE_INLINE void vk_destroy_fence(VkDevice a_device, VkFence a_fence);
FORCE_INLINE void vk_destroy_framebuffers(VkDevice a_device, VkFramebuffer a_framebuffer);

}        // namespace rhi

#include "rhi/vulkan/rorvulkan_utils.hh"
