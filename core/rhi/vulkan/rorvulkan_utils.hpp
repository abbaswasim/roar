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

VkDevice                        vk_create_device(VkPhysicalDevice physical_device, std::vector<VkDeviceQueueCreateInfo> &queues);
VkSwapchainKHR                  vk_create_swapchain(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat &a_format, VkExtent2D a_swapchain_extent);
VkImageView                     vk_create_image_view(VkDevice a_device, VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspect_flags, uint32_t a_mip_levels,
                                                     VkImageViewType    a_type      = VK_IMAGE_VIEW_TYPE_2D,
                                                     VkComponentSwizzle a_r_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     VkComponentSwizzle a_g_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     VkComponentSwizzle a_b_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY,
                                                     VkComponentSwizzle a_a_swizzle = VK_COMPONENT_SWIZZLE_IDENTITY);
VkPipelineShaderStageCreateInfo vk_create_pipeline_shader_stage(VkShaderStageFlagBits a_shader_stage_flags, const char *a_name, VkShaderModule a_shader_module);
VkShaderModule vk_create_shader_module(VkDevice a_device, const std::vector<uint32_t> &a_spirv_code);

FORCE_INLINE void vk_destroy_swapchain(VkDevice a_device, VkSwapchainKHR a_swapchain);
FORCE_INLINE void vk_destroy_imageview(VkDevice a_device, VkImageView a_image_view);
FORCE_INLINE void vk_destroy_surface(VkInstance a_instance, VkSurfaceKHR a_surface);
FORCE_INLINE void vk_destroy_device(VkDevice a_device);
FORCE_INLINE void vk_destroy_pipeline(VkDevice a_device, VkPipeline a_pipeline);
FORCE_INLINE void vk_destroy_shader_module(VkDevice a_device, VkShaderModule a_module);

}        // namespace rhi

#include "rhi/vulkan/rorvulkan_utils.hh"
