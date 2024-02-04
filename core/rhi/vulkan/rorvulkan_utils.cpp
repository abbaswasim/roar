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

#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "resources/rorresource.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <cstddef>
#include <filesystem>
#include <mutex>
#include <vector>

#define FENCE_TIMEOUT 100000000000

namespace rhi
{

VkImageView vk_create_image_view(VkDevice a_device, VkImage a_image, VkFormat a_format, uint32_t a_mip_levels, VkImageAspectFlags a_aspect_flags, VkImageViewType a_type,
                                 VkComponentSwizzle a_r_swizzle, VkComponentSwizzle a_g_swizzle, VkComponentSwizzle a_b_swizzle, VkComponentSwizzle a_a_swizzle)
{
	VkImageViewCreateInfo image_view_create_info = {};
	image_view_create_info.sType                 = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	image_view_create_info.pNext                 = nullptr;
	image_view_create_info.flags                 = 0;
	image_view_create_info.image                 = a_image;
	image_view_create_info.viewType              = a_type;        // could be any of VK_IMAGE_VIEW_TYPE_2D; VK_IMAGE_VIEW_TYPE_1D; etc
	image_view_create_info.format                = a_format;

	image_view_create_info.components.r = a_r_swizzle;        // default as VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_create_info.components.g = a_g_swizzle;        // default as VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_create_info.components.b = a_b_swizzle;        // default as VK_COMPONENT_SWIZZLE_IDENTITY;
	image_view_create_info.components.a = a_a_swizzle;        // default as VK_COMPONENT_SWIZZLE_IDENTITY;

	image_view_create_info.subresourceRange.aspectMask     = a_aspect_flags;        // Things like VK_IMAGE_ASPECT_COLOR_BIT;
	image_view_create_info.subresourceRange.baseMipLevel   = 0;
	image_view_create_info.subresourceRange.levelCount     = a_mip_levels;
	image_view_create_info.subresourceRange.baseArrayLayer = 0;
	image_view_create_info.subresourceRange.layerCount     = 1;

	VkImageView image_view;
	VkResult    result = vkCreateImageView(a_device, &image_view_create_info, nullptr, &image_view);
	check_return_status(result, "vkCreateImageView");

	return image_view;
}

VkImage vk_create_image(VkDevice a_device, uint32_t a_width, uint32_t a_height, uint32_t a_depth, VkFormat a_format, uint32_t a_mip_levels, VkImageUsageFlags a_usage, VkImageType a_image_type,
                        VkImageTiling a_tiling, VkImageLayout a_initial_layout, VkSharingMode a_sharing_mode, VkSampleCountFlagBits a_samples_count, const std::vector<uint32_t> &&a_queue_family_indices)
{
	VkImageCreateInfo image_info{};
	image_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.pNext         = nullptr;
	image_info.flags         = 0;                   // Optional
	image_info.imageType     = a_image_type;        // VK_IMAGE_TYPE_2D;
	image_info.format        = a_format;
	image_info.extent.width  = a_width;
	image_info.extent.height = a_height;
	image_info.extent.depth  = a_depth;
	image_info.mipLevels     = a_mip_levels;
	image_info.arrayLayers   = 1;                      // Do I need to expose this?
	image_info.samples       = a_samples_count;        // VK_SAMPLE_COUNT_1_BIT;
	image_info.tiling        = a_tiling;               // VK_IMAGE_TILING_OPTIMAL
	image_info.usage         = a_usage;
	image_info.sharingMode   = a_sharing_mode;          // VK_SHARING_MODE_EXCLUSIVE;
	image_info.initialLayout = a_initial_layout;        // VK_IMAGE_LAYOUT_UNDEFINED;

	if (a_sharing_mode == VK_SHARING_MODE_CONCURRENT)
	{
		assert(a_queue_family_indices.size() > 0 && "Not enough queue families provided for concurent usage image");
		image_info.queueFamilyIndexCount = ror::static_cast_safe<uint32_t>(a_queue_family_indices.size());
		image_info.pQueueFamilyIndices   = a_queue_family_indices.data();
	}

	VkImage  image;
	VkResult result = vkCreateImage(a_device, &image_info, cfg::VkAllocator, &image);
	check_return_status(result, "vkCreateImage");

	return image;
}

VkSampler vk_create_image_sampler(VkDevice a_device, VkSamplerAddressMode a_wrap_u, VkSamplerAddressMode a_wrap_v, VkSamplerAddressMode a_wrap_w,
                                  VkFilter a_min_filter, VkFilter a_mag_filter, VkSamplerMipmapMode a_mip_filter,
                                  VkBorderColor a_border_color, bool a_compare, VkCompareOp a_comapre_op,
                                  bool a_enable_anisotropy, uint32_t a_max_anisotropy, bool a_unormalized_coords)
{
	VkSamplerCreateInfo sampler_info{};

	sampler_info.sType     = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_info.magFilter = a_mag_filter;        // VK_FILTER_LINEAR;
	sampler_info.minFilter = a_min_filter;        // VK_FILTER_LINEAR;

	sampler_info.addressModeU = a_wrap_u;        // VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.addressModeV = a_wrap_v;        // VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler_info.addressModeW = a_wrap_w;        // VK_SAMPLER_ADDRESS_MODE_REPEAT;

	sampler_info.anisotropyEnable = a_enable_anisotropy;
	sampler_info.maxAnisotropy    = static_cast<float32_t>(a_max_anisotropy);

	sampler_info.borderColor             = a_border_color;              // VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler_info.unnormalizedCoordinates = a_unormalized_coords;        // VK_FALSE means coordinates are in range 0.0 - 1.0 otherwise 0.0 to image size

	sampler_info.compareEnable = a_compare;           // These are used for depth textures
	sampler_info.compareOp     = a_comapre_op;        // These are used for depth textures
	sampler_info.mipmapMode    = a_mip_filter;        // VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler_info.mipLodBias    = 0.0f;
	sampler_info.minLod        = 0.0f;
	sampler_info.maxLod        = VK_LOD_CLAMP_NONE;        // a_mip_levels;

	VkSampler texture_sampler{};
	VkResult  result = vkCreateSampler(a_device, &sampler_info, nullptr, &texture_sampler);
	check_return_status(result, "vkCreateSampler");

	return texture_sampler;
}

VkImageMemoryBarrier2 vk_create_image_barrier(VkImage a_image, VkPipelineStageFlags2 a_src_stage_mask, VkAccessFlags2 a_src_access_mask, VkImageLayout a_old_layout, VkImageLayout a_new_layout,
                                              VkPipelineStageFlags2 a_dst_stage_mask, VkAccessFlags2 a_dst_access_mask, VkImageAspectFlags a_aspect_mask, uint32_t a_base_mip_level, uint32_t a_mip_level_count)
{
	VkImageMemoryBarrier2 image_barrier{};

	image_barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
	image_barrier.pNext                           = nullptr;
	image_barrier.srcStageMask                    = a_src_stage_mask;
	image_barrier.srcAccessMask                   = a_src_access_mask;
	image_barrier.dstStageMask                    = a_dst_stage_mask;
	image_barrier.dstAccessMask                   = a_dst_access_mask;
	image_barrier.oldLayout                       = a_old_layout;
	image_barrier.newLayout                       = a_new_layout;
	image_barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	image_barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	image_barrier.image                           = a_image;
	image_barrier.subresourceRange.aspectMask     = a_aspect_mask;
	image_barrier.subresourceRange.baseMipLevel   = a_base_mip_level;
	image_barrier.subresourceRange.levelCount     = a_mip_level_count;
	image_barrier.subresourceRange.baseArrayLayer = 0;
	image_barrier.subresourceRange.layerCount     = VK_REMAINING_ARRAY_LAYERS;

	return image_barrier;

	// NOTE: To use an image barrier2 you will do something like the following

	// VkImageMemoryBarrier2 image_barrier2{};
	// VkBufferMemoryBarrier2 buffer_barrier2{};
	// VkDependencyFlags dependency_flags{VK_DEPENDENCY_BY_REGION_BIT};

	// VkDependencyInfo dependency_info{};
	// dependency_info.sType                    = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
	// dependency_info.pNext                    = nullptr;
	// dependency_info.dependencyFlags          = dependency_flags;
	// dependency_info.bufferMemoryBarrierCount = 1;
	// dependency_info.pBufferMemoryBarriers    = &buffer_barrier2;
	// dependency_info.imageMemoryBarrierCount  = 1;
	// dependency_info.pImageMemoryBarriers     = &image_barrier2;

	// vkCmdPipelineBarrier2(command_buffer, &dependency_info);
}

VkBufferMemoryBarrier2 vk_create_buffer_barrier(VkBuffer a_buffer, VkPipelineStageFlags2 a_src_stage_mask, VkAccessFlags2 a_src_access_mask, VkPipelineStageFlags2 a_dst_stage_mask, VkAccessFlags2 a_dst_access_mask)
{
	VkBufferMemoryBarrier2 buffer_barrier{};

	buffer_barrier.sType               = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER_2;
	buffer_barrier.pNext               = nullptr;
	buffer_barrier.srcStageMask        = a_src_stage_mask;
	buffer_barrier.srcAccessMask       = a_src_access_mask;
	buffer_barrier.dstStageMask        = a_dst_stage_mask;
	buffer_barrier.dstAccessMask       = a_dst_access_mask;
	buffer_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	buffer_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	buffer_barrier.buffer              = a_buffer;
	buffer_barrier.offset              = 0;
	buffer_barrier.size                = VK_WHOLE_SIZE;

	return buffer_barrier;
}

void vk_transition_image_layout(const VkDevice a_device, VkCommandPool a_command_pool, VkQueue a_transfer_queue, VkImage a_image, uint32_t a_mip_levels, VkImageLayout a_old_layout, VkImageLayout a_new_layout, std::mutex *a_mutex)
{
	VkCommandBuffer command_buffer = vk_begin_single_use_command_buffer(a_device, a_command_pool);

	VkPipelineStageFlags source_stage{};
	VkPipelineStageFlags destination_stage{};

	// TODO: Port me to vk_create_image_barrier
	VkImageMemoryBarrier barrier{};
	barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout                       = a_old_layout;
	barrier.newLayout                       = a_new_layout;
	barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
	barrier.image                           = a_image;
	barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel   = 0;
	barrier.subresourceRange.levelCount     = a_mip_levels;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount     = 1;
	barrier.srcAccessMask                   = 0;        // TODO:
	barrier.dstAccessMask                   = 0;        // TODO:

	if (a_old_layout == VK_IMAGE_LAYOUT_UNDEFINED && a_new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		source_stage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destination_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (a_old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && a_new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		source_stage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(command_buffer, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

	vk_end_single_use_command_buffer_and_wait(a_device, command_buffer, a_transfer_queue, a_command_pool, a_mutex);
}

void vk_copy_staging_buffer_to_image(VkDevice a_device, VkQueue transfer_queue, VkCommandPool a_command_pool,
                                     VkBuffer a_source, VkImage a_destination, const std::vector<VkBufferImageCopy> &buffer_image_copy_regions, std::mutex *a_mutex)
{
	VkCommandBuffer staging_command_buffer = vk_begin_single_use_command_buffer(a_device, a_command_pool);

	vkCmdCopyBufferToImage(staging_command_buffer, a_source, a_destination, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ror::static_cast_safe<uint32_t>(buffer_image_copy_regions.size()), buffer_image_copy_regions.data());

	vk_end_single_use_command_buffer_and_wait(a_device, staging_command_buffer, transfer_queue, a_command_pool, a_mutex);
}

// This is taking std::vectors instead of VkBuffer and VkImage so that we only use single cmdbuffer
void vk_copy_staging_buffers_to_images(VkDevice a_device, VkQueue transfer_queue, VkCommandPool a_command_pool,
                                       std::vector<VkBuffer> &a_source, const std::vector<VkImage> &a_destination, const std::vector<VkBufferImageCopy> &buffer_image_copy_regions, std::mutex *a_mutex)
{
	VkCommandBuffer staging_command_buffer = vk_begin_single_use_command_buffer(a_device, a_command_pool);

	if (a_source.size() != a_destination.size())
		ror::log_critical("Copying from different size a_source to a_destination, something won't be copied correctly");

	// Could this be done in one go?, i.e. remove this loop?
	// TODO: Find out why I can have many sources and destinations but single set of copy_regions
	for (size_t i = 0; i < a_source.size(); ++i)
		vkCmdCopyBufferToImage(staging_command_buffer, a_source[i], a_destination[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, ror::static_cast_safe<uint32_t>(buffer_image_copy_regions.size()), buffer_image_copy_regions.data());

	vk_end_single_use_command_buffer_and_wait(a_device, staging_command_buffer, transfer_queue, a_command_pool, a_mutex);
}

VkSwapchainKHR vk_create_swapchain(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat &swapchain_format, VkExtent2D a_swapchain_extent)
{
	VkSurfaceCapabilitiesKHR capabilities;
	auto                     result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(a_physical_device, a_surface, &capabilities);
	check_return_status(result, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");

	assert(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max());

	if (capabilities.currentExtent.width == 0xFFFFFFFF && capabilities.currentExtent.height == 0xFFFFFFFF)
		a_swapchain_extent = capabilities.currentExtent;
	else
	{
		VkExtent2D actualExtent{a_swapchain_extent};
		a_swapchain_extent.width  = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		a_swapchain_extent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	}

	uint32_t image_count = ror::number_of_buffers();
	if (capabilities.maxImageCount > 0 && image_count > capabilities.maxImageCount)
	{
		image_count = capabilities.maxImageCount;
	}
	assert(image_count >= capabilities.minImageCount && image_count <= capabilities.maxImageCount && "Min image count for swapchain is bigger than requested!\n");

	auto surface_formats = enumerate_general_property<VkSurfaceFormatKHR, true>(vkGetPhysicalDeviceSurfaceFormatsKHR, a_physical_device, a_surface);

	// Choose format
	VkSurfaceFormatKHR surface_format;
	bool               surface_found = false;
	for (const auto &available_format : surface_formats)
	{
		if (available_format.format == get_surface_format() &&
		    available_format.colorSpace == get_surface_colorspace())
		{
			surface_format = available_format;
			surface_found  = true;
			break;
		}
	}

	if (!surface_found)
	{
		if (surface_formats.size() == 1 && surface_formats[0].format == VK_FORMAT_UNDEFINED)        // Special case which means all formats are supported
		{
			surface_format.format     = get_surface_format();
			surface_format.colorSpace = get_surface_colorspace();
			surface_found             = true;
		}
		else
		{
			surface_format = surface_formats[0];        // Get the first one otherwise
			surface_found  = true;
			ror::log_error("Requested surface format and color space not available, chosing the first one!\n");
		}
	}

	assert(surface_found);

	swapchain_format = surface_format.format;

	auto present_modes = enumerate_general_property<VkPresentModeKHR, true>(vkGetPhysicalDeviceSurfacePresentModesKHR, a_physical_device, a_surface);

	// Start with the only available present mode and change if requested
	VkPresentModeKHR present_mode{VK_PRESENT_MODE_FIFO_KHR};

	if (ror::vsync())
	{
		present_mode = VK_PRESENT_MODE_MAX_ENUM_KHR;
		VkPresentModeKHR present_mode_required{VK_PRESENT_MODE_IMMEDIATE_KHR};

		for (const auto &available_present_mode : present_modes)
		{
			if (available_present_mode == present_mode_required)
			{
				present_mode = available_present_mode;
				break;
			}
		}

		assert(present_mode != VK_PRESENT_MODE_MAX_ENUM_KHR);
	}

	uint32_t queue_family_indices[]{0, 0};        // TODO: Get graphics and present queue indices
	auto     sci = get_swapchain_sharing_mode(queue_family_indices);

	VkSwapchainCreateInfoKHR swapchain_create_info = {};
	swapchain_create_info.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info.pNext                    = nullptr;
	swapchain_create_info.flags                    = 0;
	swapchain_create_info.surface                  = a_surface;
	swapchain_create_info.minImageCount            = image_count;
	swapchain_create_info.imageFormat              = surface_format.format;
	swapchain_create_info.imageColorSpace          = surface_format.colorSpace;
	swapchain_create_info.imageExtent              = a_swapchain_extent;
	swapchain_create_info.imageArrayLayers         = 1;
	swapchain_create_info.imageUsage               = get_swapchain_usage();
	swapchain_create_info.imageSharingMode         = sci.imageSharingMode;
	swapchain_create_info.queueFamilyIndexCount    = sci.queueFamilyIndexCount;
	swapchain_create_info.pQueueFamilyIndices      = sci.pQueueFamilyIndices;
	swapchain_create_info.preTransform             = get_surface_transform();
	swapchain_create_info.compositeAlpha           = get_surface_composition_mode();
	swapchain_create_info.presentMode              = present_mode;
	swapchain_create_info.clipped                  = VK_TRUE;
	swapchain_create_info.oldSwapchain             = VK_NULL_HANDLE;

	VkSwapchainKHR swapchain{nullptr};
	result = vkCreateSwapchainKHR(a_device, &swapchain_create_info, cfg::VkAllocator, &swapchain);
	check_return_status(result, "vkCreateSwapchainKHR");

	return swapchain;
}

VkDevice vk_create_device(VkPhysicalDevice physical_device, const std::vector<VkDeviceQueueCreateInfo> &queues)
{
	// TODO: Select properties/features you need here
	VkPhysicalDeviceFeatures physical_device_features{};

	vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

	if (ror::sample_rate_shading_enabled())
	{
		assert(physical_device_features.sampleRateShading == VK_TRUE && "Sample Rate Shading not avialable");
	}

	VkDeviceCreateInfo device_create_info{};
	auto               extensions = enumerate_properties<VkPhysicalDevice, VkExtensionProperties>(physical_device);
	auto               layers     = enumerate_properties<VkPhysicalDevice, VkLayerProperties>(physical_device);

	device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.pNext                   = nullptr;
	device_create_info.flags                   = 0;
	device_create_info.queueCreateInfoCount    = ror::static_cast_safe<uint32_t>(queues.size());
	device_create_info.pQueueCreateInfos       = queues.data();
	device_create_info.enabledLayerCount       = ror::static_cast_safe<uint32_t>(layers.size());
	device_create_info.ppEnabledLayerNames     = layers.data();
	device_create_info.enabledExtensionCount   = ror::static_cast_safe<uint32_t>(extensions.size());
	device_create_info.ppEnabledExtensionNames = extensions.data();
	device_create_info.pEnabledFeatures        = &physical_device_features;        // TODO: Shouldn't use this, just use what you need not everything available
	// device_create_info.pEnabledFeatures = nullptr;

	VkDevice device;
	auto     result = vkCreateDevice(physical_device, &device_create_info, cfg::VkAllocator, &device);
	check_return_status(result, "vkCreateDevice");

	return device;
}

VkViewport vk_create_viewport(float32_t a_x, float32_t a_y, float32_t a_width, float32_t a_height, float32_t a_min_depth, float32_t a_max_depth)
{
	VkViewport viewport = {};

	viewport.x        = a_x;
	viewport.y        = a_y;
	viewport.width    = a_width;
	viewport.height   = a_height;
	viewport.minDepth = a_min_depth;
	viewport.maxDepth = a_max_depth;

	return viewport;
}

VkRect2D vk_create_scissor(int32_t a_x, int32_t a_y, uint32_t a_width, uint32_t a_height)
{
	VkRect2D scissor = {};

	scissor.offset = {a_x, a_y};
	scissor.extent = {a_width, a_height};

	return scissor;
}

VkPipelineViewportStateCreateInfo vk_create_viewport_state(const VkViewport &a_viewport, const VkRect2D &a_scissor)
{
	VkPipelineViewportStateCreateInfo pipeline_viewport_state_info = {};

	pipeline_viewport_state_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	pipeline_viewport_state_info.pNext         = nullptr;
	pipeline_viewport_state_info.flags         = 0;
	pipeline_viewport_state_info.viewportCount = 1;
	pipeline_viewport_state_info.pViewports    = &a_viewport;
	pipeline_viewport_state_info.scissorCount  = 1;
	pipeline_viewport_state_info.pScissors     = &a_scissor;

	return pipeline_viewport_state_info;
}

VkPipelineRasterizationStateCreateInfo vk_create_pipeline_rasterisation_state(VkPolygonMode a_polygon_mode /*= VK_POLYGON_MODE_FILL*/, VkCullModeFlags a_cullmode /* = VK_CULL_MODE_BACK_BIT */,
                                                                              VkFrontFace a_winding /*= VK_FRONT_FACE_COUNTER_CLOCKWISE*/, float32_t a_line_width /*= 1.0f*/)
{
	VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_info = {};

	pipeline_rasterization_state_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	pipeline_rasterization_state_info.pNext                   = nullptr;
	pipeline_rasterization_state_info.flags                   = 0;
	pipeline_rasterization_state_info.depthClampEnable        = VK_FALSE;
	pipeline_rasterization_state_info.rasterizerDiscardEnable = VK_FALSE;
	pipeline_rasterization_state_info.polygonMode             = a_polygon_mode;        // VK_POLYGON_MODE_FILL;
	pipeline_rasterization_state_info.lineWidth               = a_line_width;          // 1.0f;
	pipeline_rasterization_state_info.cullMode                = a_cullmode;            // VK_CULL_MODE_BACK_BIT;
	pipeline_rasterization_state_info.frontFace               = a_winding;             // VK_FRONT_FACE_COUNTER_CLOCKWISE;

	pipeline_rasterization_state_info.depthBiasEnable         = VK_FALSE;        // Shadow mapping related
	pipeline_rasterization_state_info.depthBiasConstantFactor = 0.0f;            // Optional
	pipeline_rasterization_state_info.depthBiasClamp          = 0.0f;            // Optional
	pipeline_rasterization_state_info.depthBiasSlopeFactor    = 0.0f;            // Optional

	return pipeline_rasterization_state_info;
}

VkPipelineMultisampleStateCreateInfo vk_create_pipeline_multisample_state(VkSampleCountFlagBits a_samples, bool a_alpha_to_coverage, bool a_alpha_to_one)
{
	VkPipelineMultisampleStateCreateInfo pipeline_multisampling_state_info = {};

	pipeline_multisampling_state_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	pipeline_multisampling_state_info.pNext                 = nullptr;
	pipeline_multisampling_state_info.flags                 = 0;
	pipeline_multisampling_state_info.rasterizationSamples  = a_samples;
	pipeline_multisampling_state_info.sampleShadingEnable   = VK_FALSE;                   // (get_sample_rate_shading_enabled() ? VK_TRUE : VK_FALSE);
	pipeline_multisampling_state_info.minSampleShading      = 1.0f;                       // (get_sample_rate_shading_enabled() ? get_sample_rate_shading() : 1.0f);
	pipeline_multisampling_state_info.pSampleMask           = nullptr;                    // Optional
	pipeline_multisampling_state_info.alphaToCoverageEnable = a_alpha_to_coverage;        // VK_FALSE;
	pipeline_multisampling_state_info.alphaToOneEnable      = a_alpha_to_one;             // VK_FALSE;

	return pipeline_multisampling_state_info;
}

VkPipelineShaderStageCreateInfo vk_create_pipeline_shader_stage(VkShaderStageFlagBits a_shader_stage_flags, const char *a_name, VkShaderModule a_shader_module)
{
	VkPipelineShaderStageCreateInfo shader_stage_info = {};
	shader_stage_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader_stage_info.pNext                           = nullptr;
	shader_stage_info.flags                           = 0;
	shader_stage_info.stage                           = a_shader_stage_flags;
	shader_stage_info.module                          = a_shader_module;
	shader_stage_info.pName                           = a_name;
	shader_stage_info.pSpecializationInfo             = nullptr;

	return shader_stage_info;
}

VkPipelineVertexInputStateCreateInfo vk_create_pipeline_vertex_input_state(const std::vector<VkVertexInputAttributeDescription> &a_vertex_attribute_descriptions, const std::vector<VkVertexInputBindingDescription> &a_vertex_attribute_bindings)
{
	VkPipelineVertexInputStateCreateInfo pipeline_vertex_input_state_info = {};

	pipeline_vertex_input_state_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pipeline_vertex_input_state_info.pNext                           = nullptr;
	pipeline_vertex_input_state_info.flags                           = 0;
	pipeline_vertex_input_state_info.vertexBindingDescriptionCount   = static_cast<uint32_t>(a_vertex_attribute_bindings.size());
	pipeline_vertex_input_state_info.pVertexBindingDescriptions      = a_vertex_attribute_bindings.data();
	pipeline_vertex_input_state_info.vertexAttributeDescriptionCount = static_cast<uint32_t>(a_vertex_attribute_descriptions.size());
	pipeline_vertex_input_state_info.pVertexAttributeDescriptions    = a_vertex_attribute_descriptions.data();

	return pipeline_vertex_input_state_info;
}

VkPipelineInputAssemblyStateCreateInfo vk_create_pipeline_input_assembly_state(VkPrimitiveTopology a_topology, bool a_primitive_restart)
{
	VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_info = {};
	pipeline_input_assembly_info.sType                                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	pipeline_input_assembly_info.pNext                                  = nullptr;
	pipeline_input_assembly_info.topology                               = a_topology;                 // VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipeline_input_assembly_info.primitiveRestartEnable                 = a_primitive_restart;        // VK_FALSE;

	return pipeline_input_assembly_info;
}

VkShaderModule vk_create_shader_module(VkDevice a_device, const std::vector<uint32_t> &a_spirv_code)
{
	VkShaderModuleCreateInfo shader_module_info = {};
	shader_module_info.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shader_module_info.pNext                    = nullptr;
	shader_module_info.flags                    = 0;
	shader_module_info.codeSize                 = a_spirv_code.size() * sizeof(uint32_t);        // a_spirv_code is std::vector<uint32_t> so its size in machine units is * 4
	shader_module_info.pCode                    = reinterpret_cast<const uint32_t *>(a_spirv_code.data());

	VkShaderModule shader_module;
	VkResult       result = vkCreateShaderModule(a_device, &shader_module_info, cfg::VkAllocator, &shader_module);
	check_return_status(result, "vkCreateShaderModule");

	return shader_module;
}

VkBuffer vk_create_buffer(VkDevice a_device, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, const std::vector<uint32_t> &a_queue_family_indices)
{
	VkBufferCreateInfo buffer_info{};

	buffer_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.pNext       = nullptr;
	buffer_info.flags       = 0;
	buffer_info.size        = a_size;                // example: 1024 * 1024 * 2;        // 2kb
	buffer_info.usage       = a_usage;               // example: VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
	buffer_info.sharingMode = a_sharing_mode;        // example: VK_SHARING_MODE_EXCLUSIVE; has performance implications, not all resources are shared either

	if (a_sharing_mode == VK_SHARING_MODE_CONCURRENT)
	{
		assert(a_queue_family_indices.size() > 0 && "Not enough queue families provided for concurent usage buffer");
		buffer_info.queueFamilyIndexCount = ror::static_cast_safe<uint32_t>(a_queue_family_indices.size());        // Size of queue familes buffer will support
		buffer_info.pQueueFamilyIndices   = a_queue_family_indices.data();                                         // example: {GRAPHICS ,COMPUTE} in a vector
	}

	VkBuffer buffer;
	VkResult result = vkCreateBuffer(a_device, &buffer_info, cfg::VkAllocator, &buffer);
	check_return_status(result, "VkCreateBuffer");

	assert(buffer && "Couldn't allocate buffer, probably out of memory");

	return buffer;
}

VkDeviceMemory vk_allocate_memory(VkDevice a_device, VkDeviceSize a_size, uint32_t a_memory_type_index)
{
	// FIXME: Don't forget suballocations because this won't work for anything more than 4096 on most GPUS
	VkMemoryAllocateInfo allocation_info{};
	allocation_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocation_info.allocationSize  = a_size;                     // a_buffer_mem_req.size;
	allocation_info.memoryTypeIndex = a_memory_type_index;        // find_memory_type(a_buffer_mem_req.memoryTypeBits, a_properties);
	allocation_info.pNext           = nullptr;

	VkDeviceMemory memory;
	VkResult       result = vkAllocateMemory(a_device, &allocation_info, cfg::VkAllocator, &memory);
	check_return_status(result, "vkAllocateMemory");
	assert(memory && "Memory allocation failed");

	return memory;
}

VkMemoryRequirements2 vk_buffer_memory_requirements(VkDevice a_device, VkBuffer a_buffer)
{
	VkMemoryRequirements2 buffer_mem_req{};
	buffer_mem_req.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
	buffer_mem_req.pNext = nullptr;

	VkBufferMemoryRequirementsInfo2 buffer_mem_info{};
	buffer_mem_info.sType  = VK_STRUCTURE_TYPE_BUFFER_MEMORY_REQUIREMENTS_INFO_2;
	buffer_mem_info.pNext  = nullptr;
	buffer_mem_info.buffer = a_buffer;

	vkGetBufferMemoryRequirements2(a_device, &buffer_mem_info, &buffer_mem_req);

	return buffer_mem_req;
}

VkMemoryRequirements2 vk_image_memory_requirements(VkDevice a_device, VkImage a_image)
{
	VkMemoryRequirements2 image_mem_req{};
	image_mem_req.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
	image_mem_req.pNext = nullptr;

	VkImageMemoryRequirementsInfo2 image_mem_info{};
	image_mem_info.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2;
	image_mem_info.pNext = nullptr;
	image_mem_info.image = a_image;

	vkGetImageMemoryRequirements2(a_device, &image_mem_info, &image_mem_req);

	return image_mem_req;
}

void vk_bind_buffer_memory(VkDevice a_device, VkBuffer a_buffer, VkDeviceMemory a_memory)
{
	auto result = vkBindBufferMemory(a_device, a_buffer, a_memory, 0);        // 0 is memory offset
	check_return_status(result, "vkBindBufferMemory");
}

void vk_bind_image_memory(VkDevice a_device, VkImage a_image, VkDeviceMemory a_memory)
{
	auto result = vkBindImageMemory(a_device, a_image, a_memory, 0);        // 0 is memory offset
	check_return_status(result, "vkBindImageMemory");
}

VkDeviceMemory vk_bind_buffer_memory(VkDevice a_device, VkBuffer a_buffer, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties)
{
	VkMemoryRequirements2 buffer_mem_req    = vk_buffer_memory_requirements(a_device, a_buffer);
	auto                  memory_type_index = vk_find_memory_type(buffer_mem_req.memoryRequirements.memoryTypeBits, a_memory_properties, a_properties);
	auto                  memory            = vk_allocate_memory(a_device, buffer_mem_req.memoryRequirements.size, memory_type_index);

	vk_bind_buffer_memory(a_device, a_buffer, memory);

	return memory;
}

VkDeviceMemory vk_bind_image_memory(VkDevice a_device, VkImage a_image, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties)
{
	VkMemoryRequirements2 image_mem_req     = vk_image_memory_requirements(a_device, a_image);
	auto                  memory_type_index = vk_find_memory_type(image_mem_req.memoryRequirements.memoryTypeBits, a_memory_properties, a_properties);
	auto                  memory            = vk_allocate_memory(a_device, image_mem_req.memoryRequirements.size, memory_type_index);

	vk_bind_image_memory(a_device, a_image, memory);

	return memory;
}

void vk_create_buffer_with_memory(VkDevice a_device, VkBuffer &a_buffer, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, const std::vector<uint32_t> &a_queue_family_indices,
                                  VkDeviceMemory &a_memory, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties)
{
	assert(a_buffer == nullptr && "Recreating buffer");
	assert(a_memory == nullptr && "Recreating memory");

	a_buffer                                = vk_create_buffer(a_device, a_size, a_usage, a_sharing_mode, a_queue_family_indices);
	VkMemoryRequirements2 buffer_mem_req    = vk_buffer_memory_requirements(a_device, a_buffer);
	auto                  memory_type_index = vk_find_memory_type(buffer_mem_req.memoryRequirements.memoryTypeBits, a_memory_properties, a_properties);
	a_memory                                = vk_allocate_memory(a_device, buffer_mem_req.memoryRequirements.size, memory_type_index);

	vk_bind_buffer_memory(a_device, a_buffer, a_memory);
}

void vk_create_image_with_memory(VkDevice a_device, VkImage &a_image, uint32_t a_width, uint32_t a_height, uint32_t a_depth, VkFormat a_format, uint32_t a_mip_levels, VkImageUsageFlags a_usage,
                                 VkDeviceMemory &a_memory, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties,
                                 VkImageType a_image_type, VkImageTiling a_tiling, VkImageLayout a_initial_layout, VkSharingMode a_sharing_mode, VkSampleCountFlagBits a_samples_count,
                                 const std::vector<uint32_t> &&a_queue_family_indices)
{
	assert(a_image == nullptr && "Recreating image");
	assert(a_memory == nullptr && "Recreating memory");

	a_image                                 = vk_create_image(a_device, a_width, a_height, a_depth, a_format,
	                                                          a_mip_levels, a_usage, a_image_type, a_tiling,
	                                                          a_initial_layout, a_sharing_mode, a_samples_count, std::move(a_queue_family_indices));
	VkMemoryRequirements2 image_mem_req     = vk_image_memory_requirements(a_device, a_image);
	auto                  memory_type_index = vk_find_memory_type(image_mem_req.memoryRequirements.memoryTypeBits, a_memory_properties, a_properties);
	a_memory                                = vk_allocate_memory(a_device, image_mem_req.memoryRequirements.size, memory_type_index);

	vk_bind_image_memory(a_device, a_image, a_memory);
}

uint32_t vk_find_memory_type(uint32_t a_type_filter, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties)
{
	for (uint32_t i = 0; i < a_memory_properties.memoryTypeCount; i++)
		if ((a_type_filter & (1 << i)) && ((a_memory_properties.memoryTypes[i].propertyFlags & a_properties) == a_properties))
			return i;

	throw std::runtime_error("Failed to find suitable memory type!");
}

uint8_t *vk_map_memory(VkDevice a_device, VkDeviceMemory a_memory, std::uintptr_t a_offset, std::uintptr_t a_length)
{
	void *data{nullptr};
	auto  result = vkMapMemory(a_device, a_memory, a_offset, a_length, 0, &data);
	check_return_status(result, "vkMapMemory");

	return static_cast<uint8_t *>(data);
}

uint8_t *vk_map_memory(VkDevice a_device, VkDeviceMemory a_memory)
{
	return vk_map_memory(a_device, a_memory, 0, VK_WHOLE_SIZE);
}

void vk_unmap_memory(VkDevice a_device, VkDeviceMemory a_memory)
{
	vkUnmapMemory(a_device, a_memory);
}

VkCommandBuffer vk_allocate_command_buffer(VkDevice a_device, VkCommandPool a_command_pool)
{
	VkCommandBufferAllocateInfo command_buffer_allocate_info{};
	command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	command_buffer_allocate_info.pNext              = nullptr;
	command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	command_buffer_allocate_info.commandPool        = a_command_pool;
	command_buffer_allocate_info.commandBufferCount = 1;

	VkCommandBuffer staging_command_buffer;
	VkResult        result = vkAllocateCommandBuffers(a_device, &command_buffer_allocate_info, &staging_command_buffer);
	check_return_status(result, "vkAllocateCommandBuffers");

	return staging_command_buffer;
}

VkDescriptorSet vk_allocate_descriptor_set(VkDevice a_device, VkDescriptorSetLayout a_layout, VkDescriptorPool a_pool, uint32_t a_count, VkResult *a_result)
{
	VkDescriptorSetAllocateInfo descriptor_set_allocate_info{};
	descriptor_set_allocate_info.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptor_set_allocate_info.pNext              = nullptr;
	descriptor_set_allocate_info.descriptorPool     = a_pool;
	descriptor_set_allocate_info.descriptorSetCount = a_count;
	descriptor_set_allocate_info.pSetLayouts        = &a_layout;

	VkDescriptorSet descriptor_set{};
	VkResult        result = vkAllocateDescriptorSets(a_device, &descriptor_set_allocate_info, &descriptor_set);
	if (a_result)
		*a_result = result;
	else
	{
		check_return_status(result, "vkAllocateDescriptorSet");
	}

	return descriptor_set;
}

VkDescriptorSet vk_allocate_descriptor_set(VkDevice a_device, VkDescriptorSetLayout a_layout, VkDescriptorPool a_pool, VkResult *a_result)
{
	return vk_allocate_descriptor_set(a_device, a_layout, a_pool, 1, a_result);
}

VkDescriptorSetLayoutBinding vk_create_descriptor_set_layout_binding(uint32_t a_binding, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags)
{
	VkDescriptorSetLayoutBinding descriptor_set_layout_binding{};
	descriptor_set_layout_binding.binding            = a_binding;
	descriptor_set_layout_binding.descriptorType     = a_type;
	descriptor_set_layout_binding.descriptorCount    = 1;
	descriptor_set_layout_binding.stageFlags         = a_stage_flags;
	descriptor_set_layout_binding.pImmutableSamplers = nullptr;

	return descriptor_set_layout_binding;
}

VkDescriptorSetLayoutCreateInfo vk_create_descriptor_set_layout_info(const std::vector<VkDescriptorSetLayoutBinding> &a_bindings)
{
	VkDescriptorSetLayoutCreateInfo descriptor_set_layout_createinfo{};
	descriptor_set_layout_createinfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptor_set_layout_createinfo.pNext        = nullptr;
	descriptor_set_layout_createinfo.flags        = 0;
	descriptor_set_layout_createinfo.bindingCount = static_cast<uint32_t>(a_bindings.size());
	descriptor_set_layout_createinfo.pBindings    = a_bindings.data();

	return descriptor_set_layout_createinfo;
}

VkDescriptorSetLayout vk_create_descriptor_set_layout(VkDevice a_device, const std::vector<VkDescriptorSetLayoutBinding> &a_bindings)
{
	VkDescriptorSetLayoutCreateInfo descriptor_set_layout_createinfo = vk_create_descriptor_set_layout_info(a_bindings);
	VkDescriptorSetLayout           layout;

	auto result = vkCreateDescriptorSetLayout(a_device, &descriptor_set_layout_createinfo, cfg::VkAllocator, &layout);
	check_return_status(result, "vkCreateDescriptorSetLayout");

	return layout;
}

VkDescriptorSetLayout vk_create_descriptor_set_layout(VkDevice a_device, const VkDescriptorSetLayoutCreateInfo &a_descriptor_set_layout_createinfo)
{
	VkDescriptorSetLayout layout;

	auto result = vkCreateDescriptorSetLayout(a_device, &a_descriptor_set_layout_createinfo, cfg::VkAllocator, &layout);
	check_return_status(result, "vkCreateDescriptorSetLayout");

	return layout;
}

VkWriteDescriptorSet vk_create_write_descriptor_set(VkDescriptorSet a_descriptor_set, uint32_t a_binding, VkDescriptorType a_type, VkDescriptorImageInfo *a_image_info, VkDescriptorBufferInfo *a_buffer_info)
{
	VkWriteDescriptorSet write_descriptor_set{};
	write_descriptor_set.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write_descriptor_set.pNext            = nullptr;
	write_descriptor_set.dstSet           = a_descriptor_set;
	write_descriptor_set.dstBinding       = a_binding;
	write_descriptor_set.dstArrayElement  = 0;
	write_descriptor_set.descriptorCount  = 1;
	write_descriptor_set.descriptorType   = a_type;
	write_descriptor_set.pImageInfo       = a_image_info;
	write_descriptor_set.pBufferInfo      = a_buffer_info;
	write_descriptor_set.pTexelBufferView = nullptr;

	return write_descriptor_set;
}

void vk_begin_command_buffer(VkCommandBuffer a_command_buffer, VkCommandBufferUsageFlags a_flags)
{
	VkCommandBufferBeginInfo command_buffer_begin_info{};
	command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags            = a_flags;        // VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT, VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
	command_buffer_begin_info.pNext            = nullptr;
	command_buffer_begin_info.pInheritanceInfo = nullptr;

	auto result = vkBeginCommandBuffer(a_command_buffer, &command_buffer_begin_info);
	check_return_status(result, "vkBeginCommandBuffer");
}

void vk_begin_command_buffer(VkCommandBuffer a_command_buffer, VkCommandBufferBeginInfo &a_command_buffer_begin_info)
{
	auto result = vkBeginCommandBuffer(a_command_buffer, &a_command_buffer_begin_info);
	check_return_status(result, "vkBeginCommandBuffer");
}

void vk_end_command_buffer(VkCommandBuffer a_command_buffer)
{
	auto result = vkEndCommandBuffer(a_command_buffer);
	check_return_status(result, "vkEndCommandBuffer");
}

VkCommandBuffer vk_begin_single_use_command_buffer(VkDevice a_device, VkCommandPool a_command_pool)
{
	VkCommandBuffer staging_command_buffer = vk_allocate_command_buffer(a_device, a_command_pool);

	VkCommandBufferBeginInfo command_buffer_begin_info{};
	command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags            = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	command_buffer_begin_info.pNext            = nullptr;
	command_buffer_begin_info.pInheritanceInfo = nullptr;

	vk_begin_command_buffer(staging_command_buffer, command_buffer_begin_info);

	return staging_command_buffer;
}

// Usage is like
// auto fence = vk_create_fence(device);
// auto result = vkResetFences(device, 1, &fence);
// result = vkQueueWaitIdle(a_queue);
// check_return_status(result, "vkQueueWaitIdle");
// vkWaitForFences(device, 1, &fence, VK_TRUE, FENCE_TIMEOUT);
VkFence vk_create_fence(VkDevice a_device, VkFenceCreateFlags a_flags)
{
	(void) FENCE_TIMEOUT;

	VkFenceCreateInfo fence_create_info{};

	fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_create_info.pNext = nullptr;
	fence_create_info.flags = a_flags;        // not VK_FENCE_CREATE_SIGNALED_BIT to start with

	VkFence fence;
	auto    result = vkCreateFence(a_device, &fence_create_info, cfg::VkAllocator, &fence);
	check_return_status(result, "vkCreateFence");

	return fence;
}

// https://quick-bench.com/q/0pML2GpVI8eC_J3aZAdK4kDJeHY tests whether having a mutex in single threaded app matters
// The answer is yes. Try to avoid a lock when you don't need it in single threaded app or otherwise with  an if
void vk_queue_submit(VkQueue a_queue, uint32_t a_submit_info_count, const VkSubmitInfo *a_submit_info, VkFence a_fence, std::mutex *a_mutex)
{
	VkResult result{VK_SUCCESS};
	if (!a_mutex)
	{
		result = vkQueueSubmit(a_queue, a_submit_info_count, a_submit_info, a_fence);
		check_return_status(result, "vkQueueSubmit");
	}
	else
	{
		std::scoped_lock<std::mutex> lock{*a_mutex};

		result = vkQueueSubmit(a_queue, a_submit_info_count, a_submit_info, a_fence);
		check_return_status(result, "vkQueueSubmit");
	}
}

void vk_queue_submit(VkQueue a_queue, const std::vector<VkSubmitInfo> &a_submit_info, VkFence a_fence, std::mutex *a_mutex)
{
	vk_queue_submit(a_queue, ror::static_cast_safe<uint32_t>(a_submit_info.size()), a_submit_info.data(), a_fence, a_mutex);
}

void vk_queue_submit(VkQueue a_queue, VkCommandBuffer a_command_buffer, VkFence a_fence, std::mutex *a_mutex)
{
	VkSubmitInfo submit_info{};
	submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers    = &a_command_buffer;

	vk_queue_submit(a_queue, 1, &submit_info, a_fence, a_mutex);
}

void vk_queue_submit(VkQueue a_queue, VkSubmitInfo &a_submit_info, const std::vector<VkCommandBuffer> &a_command_buffers, VkFence a_fence, std::mutex *a_mutex)
{
	VkSubmitInfo submit_info{a_submit_info};
	submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = ror::static_cast_safe<uint32_t>(a_command_buffers.size());
	submit_info.pCommandBuffers    = a_command_buffers.data();

	vk_queue_submit(a_queue, 1, &submit_info, a_fence, a_mutex);
}

void vk_queue_wait_idle(VkQueue a_queue, std::mutex *a_mutex)
{
	if (!a_mutex)
	{
		auto result = vkQueueWaitIdle(a_queue);
		check_return_status(result, "vkQueueWaitIdle");
	}
	else
	{
		std::scoped_lock<std::mutex> lock{*a_mutex};

		auto result = vkQueueWaitIdle(a_queue);
		check_return_status(result, "vkQueueWaitIdle");
	}
}

void vk_end_single_use_command_buffer(VkCommandBuffer a_command_buffer, VkQueue a_queue, std::mutex *a_mutex)
{
	vk_end_command_buffer(a_command_buffer);
	vk_queue_submit(a_queue, a_command_buffer, VK_NULL_HANDLE, a_mutex);
}

void vk_end_single_use_command_buffer_and_wait(VkDevice a_device, VkCommandBuffer a_command_buffer, VkQueue a_transfer_queue, VkCommandPool a_command_pool, std::mutex *a_mutex)
{
	vk_end_single_use_command_buffer(a_command_buffer, a_transfer_queue, a_mutex);
	vk_queue_wait_idle(a_transfer_queue, a_mutex);
	vk_destroy_command_buffer(a_device, a_command_buffer, a_command_pool);
}

VkCommandPool vk_create_command_pools(VkDevice a_device, uint32_t a_queue_family_index, VkCommandPoolCreateFlags a_flags)
{
	VkCommandPoolCreateInfo command_pool_info = {};
	command_pool_info.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	command_pool_info.pNext                   = nullptr;
	command_pool_info.flags                   = a_flags;        // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	command_pool_info.queueFamilyIndex        = a_queue_family_index;

	VkCommandPool command_pool;
	VkResult      result = vkCreateCommandPool(a_device, &command_pool_info, cfg::VkAllocator, &command_pool);
	check_return_status(result, "vkCreateCommandPool");

	return command_pool;
}

VkPipelineDepthStencilStateCreateInfo vk_create_depth_stencil_state(bool a_depth_test, bool a_depth_write, VkCompareOp a_depth_compare,
                                                                    bool a_stencil_test, VkStencilOpState a_stencil_front, VkStencilOpState a_stencil_back,
                                                                    bool a_depth_bound_test, float32_t a_min_depth_bound, float32_t a_max_depth_bound)
{
	VkPipelineDepthStencilStateCreateInfo pipeline_depth_stencil_info = {};

	pipeline_depth_stencil_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	pipeline_depth_stencil_info.pNext                 = nullptr;
	pipeline_depth_stencil_info.flags                 = 0;
	pipeline_depth_stencil_info.depthTestEnable       = a_depth_test;              // VK_TRUE;
	pipeline_depth_stencil_info.depthWriteEnable      = a_depth_write;             // VK_TRUE;
	pipeline_depth_stencil_info.depthCompareOp        = a_depth_compare;           // VK_COMPARE_OP_LESS_OR_EQUAL;
	pipeline_depth_stencil_info.depthBoundsTestEnable = a_depth_bound_test;        // VK_FALSE;
	pipeline_depth_stencil_info.stencilTestEnable     = a_stencil_test;            // VK_FALSE;
	pipeline_depth_stencil_info.front                 = a_stencil_front;           // VkStencilOpState{};
	pipeline_depth_stencil_info.back                  = a_stencil_back;            // VkStencilOpState{};
	pipeline_depth_stencil_info.minDepthBounds        = a_min_depth_bound;         // 0.0f;
	pipeline_depth_stencil_info.maxDepthBounds        = a_max_depth_bound;         // 1.0f;

	return pipeline_depth_stencil_info;
}

VkPipelineColorBlendAttachmentState vk_create_color_blend_attachment_state(bool a_blend_enable, bool a_premultiplied_alpha)
{
	VkPipelineColorBlendAttachmentState pipeline_color_blend_attachment_info = {};

	pipeline_color_blend_attachment_info.blendEnable         = a_blend_enable;
	pipeline_color_blend_attachment_info.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	pipeline_color_blend_attachment_info.dstColorBlendFactor = (a_premultiplied_alpha ? VK_BLEND_FACTOR_ONE : VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);
	pipeline_color_blend_attachment_info.colorBlendOp        = VK_BLEND_OP_ADD;
	pipeline_color_blend_attachment_info.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	pipeline_color_blend_attachment_info.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	pipeline_color_blend_attachment_info.alphaBlendOp        = VK_BLEND_OP_ADD;
	pipeline_color_blend_attachment_info.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	return pipeline_color_blend_attachment_info;
}

VkPipelineColorBlendStateCreateInfo vk_create_color_blend_state(const VkPipelineColorBlendAttachmentState &a_pipeline_color_blend_attachment_state)
{
	VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_info = {};

	pipeline_color_blend_state_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	pipeline_color_blend_state_info.pNext             = nullptr;
	pipeline_color_blend_state_info.flags             = 0;
	pipeline_color_blend_state_info.logicOpEnable     = VK_FALSE;
	pipeline_color_blend_state_info.logicOp           = VK_LOGIC_OP_COPY;
	pipeline_color_blend_state_info.attachmentCount   = 1;
	pipeline_color_blend_state_info.pAttachments      = &a_pipeline_color_blend_attachment_state;
	pipeline_color_blend_state_info.blendConstants[0] = 0.0f;
	pipeline_color_blend_state_info.blendConstants[1] = 0.0f;
	pipeline_color_blend_state_info.blendConstants[2] = 0.0f;
	pipeline_color_blend_state_info.blendConstants[3] = 0.0f;

	return pipeline_color_blend_state_info;
}

VkPipelineDynamicStateCreateInfo vk_create_dynamic_state(const std::vector<VkDynamicState> &a_dynamic_state)
{
	VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_info = {};

	pipeline_dynamic_state_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	pipeline_dynamic_state_info.pNext             = nullptr;
	pipeline_dynamic_state_info.flags             = 0;
	pipeline_dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(a_dynamic_state.size());
	pipeline_dynamic_state_info.pDynamicStates    = a_dynamic_state.data();

	return pipeline_dynamic_state_info;
}

VkPipelineLayoutCreateInfo vk_create_pipeline_layout_state(const std::vector<VkDescriptorSetLayout> &a_descriptors)
{
	VkPipelineLayoutCreateInfo pipeline_layout_info = {};

	pipeline_layout_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipeline_layout_info.pNext                  = nullptr;
	pipeline_layout_info.flags                  = 0;
	pipeline_layout_info.setLayoutCount         = static_cast<uint32_t>(a_descriptors.size());
	pipeline_layout_info.pSetLayouts            = a_descriptors.data();
	pipeline_layout_info.pushConstantRangeCount = 0;              // Optional
	pipeline_layout_info.pPushConstantRanges    = nullptr;        // Optional

	return pipeline_layout_info;
}

VkPipelineLayout vk_create_pipeline_layout(const VkDevice a_device, const VkPipelineLayoutCreateInfo &a_pipeline_layout_info)
{
	VkPipelineLayout pipeline_layout;

	VkResult result = vkCreatePipelineLayout(a_device, &a_pipeline_layout_info, cfg::VkAllocator, &pipeline_layout);
	check_return_status(result, "vkCreatePipelineLayout");

	return pipeline_layout;
}

VkPipelineCache vk_create_pipeline_cache(const VkDevice a_device)
{
	VkPipelineCacheCreateInfo pipeline_create_info;

	pipeline_create_info.sType           = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	pipeline_create_info.pNext           = nullptr;
	pipeline_create_info.flags           = 0;        // Could be VK_PIPELINE_CACHE_CREATE_EXTERNALLY_SYNCHRONIZED_BIT but let the driver work it out
	pipeline_create_info.initialDataSize = 0;
	pipeline_create_info.pInitialData    = nullptr;        // TODO: Read from disk

	VkPipelineCache cache;

	vkCreatePipelineCache(a_device, &pipeline_create_info, cfg::VkAllocator, &cache);

	return cache;
}

VkGraphicsPipelineCreateInfo vk_create_graphics_pipeline_state(const std::vector<VkPipelineShaderStageCreateInfo> &a_shader_stages,
                                                               VkPipelineVertexInputStateCreateInfo               &a_pipeline_vertex_input_state_info,
                                                               VkPipelineInputAssemblyStateCreateInfo             &a_pipeline_input_assembly_state_info,
                                                               VkPipelineViewportStateCreateInfo                  &a_pipeline_viewport_state_info,
                                                               VkPipelineRasterizationStateCreateInfo             &a_pipeline_rasterisation_state_info,
                                                               VkPipelineMultisampleStateCreateInfo               &a_pipeline_multisample_state_info,
                                                               VkPipelineDepthStencilStateCreateInfo              &a_pipeline_depth_stencil_state_info,
                                                               VkPipelineColorBlendStateCreateInfo                &a_pipeline_color_blend_state_info,
                                                               VkPipelineDynamicStateCreateInfo                   &a_pipeline_dynamic_state_info,
                                                               VkPipelineLayout                                   &a_pipeline_layout,
                                                               VkRenderPass                                       &a_render_pass,
                                                               uint32_t                                            a_render_subpass)
{
	VkGraphicsPipelineCreateInfo graphics_pipeline_create_info = {};

	graphics_pipeline_create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphics_pipeline_create_info.pNext               = nullptr;
	graphics_pipeline_create_info.flags               = 0;
	graphics_pipeline_create_info.stageCount          = static_cast<uint32_t>(a_shader_stages.size());
	graphics_pipeline_create_info.pStages             = a_shader_stages.data();
	graphics_pipeline_create_info.pVertexInputState   = &a_pipeline_vertex_input_state_info;
	graphics_pipeline_create_info.pInputAssemblyState = &a_pipeline_input_assembly_state_info;
	graphics_pipeline_create_info.pTessellationState  = nullptr;
	graphics_pipeline_create_info.pViewportState      = &a_pipeline_viewport_state_info;
	graphics_pipeline_create_info.pRasterizationState = &a_pipeline_rasterisation_state_info;
	graphics_pipeline_create_info.pMultisampleState   = &a_pipeline_multisample_state_info;
	graphics_pipeline_create_info.pDepthStencilState  = &a_pipeline_depth_stencil_state_info;
	graphics_pipeline_create_info.pColorBlendState    = &a_pipeline_color_blend_state_info;
	graphics_pipeline_create_info.pDynamicState       = &a_pipeline_dynamic_state_info;
	graphics_pipeline_create_info.layout              = a_pipeline_layout;
	graphics_pipeline_create_info.renderPass          = a_render_pass;
	graphics_pipeline_create_info.subpass             = a_render_subpass;
	graphics_pipeline_create_info.basePipelineHandle  = VK_NULL_HANDLE;
	graphics_pipeline_create_info.basePipelineIndex   = -1;

	return graphics_pipeline_create_info;
}

VkPipeline vk_create_graphics_pipeline(const VkDevice a_device, VkPipelineCache &a_pipeline_cache,
                                       const std::vector<VkPipelineShaderStageCreateInfo> &a_shader_stages,
                                       VkPipelineVertexInputStateCreateInfo               &a_pipeline_vertex_input_state_info,
                                       VkPipelineInputAssemblyStateCreateInfo             &a_pipeline_input_assembly_state_info,
                                       VkPipelineViewportStateCreateInfo                  &a_pipeline_viewport_state_info,
                                       VkPipelineRasterizationStateCreateInfo             &a_pipeline_rasterisation_state_info,
                                       VkPipelineMultisampleStateCreateInfo               &a_pipeline_multisample_state_info,
                                       VkPipelineDepthStencilStateCreateInfo              &a_pipeline_depth_stencil_state_info,
                                       VkPipelineColorBlendStateCreateInfo                &a_pipeline_color_blend_state_info,
                                       VkPipelineDynamicStateCreateInfo                   &a_pipeline_dynamic_state_info,
                                       VkPipelineLayout                                   &a_pipeline_layout,
                                       VkRenderPass                                       &a_render_pass,
                                       uint32_t                                            a_render_subpass)
{
	auto graphics_pipeline_create_info = vk_create_graphics_pipeline_state(a_shader_stages, a_pipeline_vertex_input_state_info, a_pipeline_input_assembly_state_info,
	                                                                       a_pipeline_viewport_state_info, a_pipeline_rasterisation_state_info, a_pipeline_multisample_state_info, a_pipeline_depth_stencil_state_info,
	                                                                       a_pipeline_color_blend_state_info, a_pipeline_dynamic_state_info, a_pipeline_layout, a_render_pass, a_render_subpass);

	VkPipeline graphics_pipeline{nullptr};

	VkResult result = vkCreateGraphicsPipelines(a_device, a_pipeline_cache, 1, &graphics_pipeline_create_info, cfg::VkAllocator, &graphics_pipeline);
	// check_return_status(result, "vkCreateGraphicsPipeline"); // NOTE: Not doing this because dont' want to assert here

	if (ror::get_build() == ror::BuildType::build_debug && result != VK_SUCCESS)
		ror::log_critical("Vulkan render program creation failed with error: {}", vk_result_to_string(result));

	return graphics_pipeline;
}

VkPipeline vk_create_graphics_pipeline(const VkDevice a_device, VkPipelineCache &a_pipeline_cache, const VkGraphicsPipelineCreateInfo &a_graphics_pipeline_create_info)
{
	VkPipeline graphics_pipeline{};

	VkResult result = vkCreateGraphicsPipelines(a_device, a_pipeline_cache, 1, &a_graphics_pipeline_create_info, cfg::VkAllocator, &graphics_pipeline);
	check_return_status(result, "vkCreateGraphicsPipeline");

	return graphics_pipeline;
}

VkRenderPass vk_create_render_pass(VkDevice a_device, const std::vector<VkAttachmentDescription> &&a_attachments, const std::vector<VkSubpassDescription> &a_subpasses, const std::vector<VkSubpassDependency> &a_dependencies)
{
	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.pNext                  = nullptr;
	render_pass_info.flags                  = 0;
	render_pass_info.attachmentCount        = ror::static_cast_safe<uint32_t>(a_attachments.size());
	render_pass_info.pAttachments           = a_attachments.data();
	render_pass_info.subpassCount           = ror::static_cast_safe<uint32_t>(a_subpasses.size());
	render_pass_info.pSubpasses             = a_subpasses.data();
	render_pass_info.dependencyCount        = ror::static_cast_safe<uint32_t>(a_dependencies.size());
	render_pass_info.pDependencies          = a_dependencies.data();

	VkRenderPass render_pass{};
	VkResult     result = vkCreateRenderPass(a_device, &render_pass_info, cfg::VkAllocator, &render_pass);
	check_return_status(result, "vkCreateRenderPass");

	return render_pass;
}

VkRenderPass vk_create_render_pass(VkDevice a_device, const VkAttachmentDescription &a_attachments, const VkSubpassDescription &a_subpasses, const VkSubpassDependency &a_dependencies)
{
	VkRenderPassCreateInfo render_pass_info = {};
	render_pass_info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	render_pass_info.pNext                  = nullptr;
	render_pass_info.flags                  = 0;
	render_pass_info.attachmentCount        = 1;
	render_pass_info.pAttachments           = &a_attachments;
	render_pass_info.subpassCount           = 1;
	render_pass_info.pSubpasses             = &a_subpasses;
	render_pass_info.dependencyCount        = 1;
	render_pass_info.pDependencies          = &a_dependencies;

	VkRenderPass render_pass{};
	VkResult     result = vkCreateRenderPass(a_device, &render_pass_info, cfg::VkAllocator, &render_pass);
	check_return_status(result, "vkCreateRenderPass");

	return render_pass;
}

VkAttachmentDescription vk_create_attachment_description(VkFormat a_format, VkSampleCountFlagBits a_samples,
                                                         VkAttachmentLoadOp  a_load_op,
                                                         VkAttachmentStoreOp a_store_op,
                                                         VkAttachmentLoadOp  a_stencil_load_op,
                                                         VkAttachmentStoreOp a_stencil_store_op,
                                                         VkImageLayout       a_initial_layout,
                                                         VkImageLayout       a_final_layout)
{
	VkAttachmentDescription attachment_description = {};

	attachment_description.flags          = 0;
	attachment_description.format         = a_format;
	attachment_description.samples        = a_samples;                 // VK_SAMPLE_COUNT_1_BIT, VK_SAMPLE_COUNT_2_BIT, VK_SAMPLE_COUNT_4_BIT, VK_SAMPLE_COUNT_8_BIT, VK_SAMPLE_COUNT_16_BIT, 32 and 64 etc
	attachment_description.loadOp         = a_load_op;                 // VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment_description.storeOp        = a_store_op;                // VK_ATTACHMENT_STORE_OP_STORE;
	attachment_description.stencilLoadOp  = a_stencil_load_op;         // VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment_description.stencilStoreOp = a_stencil_store_op;        // VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment_description.initialLayout  = a_initial_layout;          // VK_IMAGE_LAYOUT_UNDEFINED;
	attachment_description.finalLayout    = a_final_layout;            // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;        // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	return attachment_description;
}

VkAttachmentReference vk_create_attachment_reference(uint32_t a_attachment_index, VkImageLayout a_image_layout)
{
	VkAttachmentReference attachment_reference = {};

	attachment_reference.attachment = a_attachment_index;
	attachment_reference.layout     = a_image_layout;        // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	return attachment_reference;
}

VkSubpassDescription vk_create_subpass_description(VkPipelineBindPoint   a_pipeline_bind_point,
                                                   VkAttachmentReference a_color_attachment_reference, VkAttachmentReference a_depth_attachment_reference, VkAttachmentReference a_resolve_attachment_reference)
{
	VkSubpassDescription subpass_description = {};

	subpass_description.flags                   = 0;
	subpass_description.pipelineBindPoint       = a_pipeline_bind_point;        // VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.inputAttachmentCount    = 0;
	subpass_description.pInputAttachments       = nullptr;
	subpass_description.colorAttachmentCount    = 1;
	subpass_description.pColorAttachments       = &a_color_attachment_reference;
	subpass_description.pResolveAttachments     = &a_resolve_attachment_reference;
	subpass_description.pDepthStencilAttachment = &a_depth_attachment_reference;
	subpass_description.preserveAttachmentCount = 0;
	subpass_description.pPreserveAttachments    = nullptr;

	return subpass_description;
}

VkSubpassDescription vk_create_subpass_description(VkPipelineBindPoint a_pipeline_bind_point, VkAttachmentReference a_input_attachment,
                                                   VkAttachmentReference a_color_attachment_reference, VkAttachmentReference a_depth_attachment_reference, VkAttachmentReference a_resolve_attachment_reference)
{
	VkSubpassDescription subpass_description = {};

	subpass_description.flags                   = 0;
	subpass_description.pipelineBindPoint       = a_pipeline_bind_point;        // VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.inputAttachmentCount    = 1;
	subpass_description.pInputAttachments       = &a_input_attachment;
	subpass_description.colorAttachmentCount    = 1;
	subpass_description.pColorAttachments       = &a_color_attachment_reference;
	subpass_description.pResolveAttachments     = &a_resolve_attachment_reference;
	subpass_description.pDepthStencilAttachment = &a_depth_attachment_reference;
	subpass_description.preserveAttachmentCount = 0;
	subpass_description.pPreserveAttachments    = nullptr;

	return subpass_description;
}

VkSubpassDescription vk_create_subpass_description(VkPipelineBindPoint a_pipeline_bind_point, const std::vector<VkAttachmentReference> &a_input_attachments,
                                                   VkAttachmentReference a_color_attachment_reference, VkAttachmentReference a_depth_attachment_reference, VkAttachmentReference a_resolve_attachment_reference)
{
	VkSubpassDescription subpass_description = {};

	subpass_description.flags                   = 0;
	subpass_description.pipelineBindPoint       = a_pipeline_bind_point;        // VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.inputAttachmentCount    = ror::static_cast_safe<uint32_t>(a_input_attachments.size());
	subpass_description.pInputAttachments       = a_input_attachments.data();
	subpass_description.colorAttachmentCount    = 1;
	subpass_description.pColorAttachments       = &a_color_attachment_reference;
	subpass_description.pResolveAttachments     = &a_resolve_attachment_reference;
	subpass_description.pDepthStencilAttachment = &a_depth_attachment_reference;
	subpass_description.preserveAttachmentCount = 0;
	subpass_description.pPreserveAttachments    = nullptr;

	return subpass_description;
}

VkSubpassDescription vk_create_subpass_description(VkPipelineBindPoint a_pipeline_bind_point, const std::vector<VkAttachmentReference> &a_input_attachments, const std::vector<VkAttachmentReference> &a_color_attachment_reference,
                                                   const VkAttachmentReference *a_depth_attachment_reference, const VkAttachmentReference *a_resolve_attachment_reference)
{
	VkSubpassDescription subpass_description = {};

	subpass_description.flags                   = 0;
	subpass_description.pipelineBindPoint       = a_pipeline_bind_point;        // VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass_description.inputAttachmentCount    = ror::static_cast_safe<uint32_t>(a_input_attachments.size());
	subpass_description.pInputAttachments       = a_input_attachments.data();
	subpass_description.colorAttachmentCount    = ror::static_cast_safe<uint32_t>(a_color_attachment_reference.size());
	subpass_description.pColorAttachments       = a_color_attachment_reference.data();
	subpass_description.pResolveAttachments     = a_resolve_attachment_reference;
	subpass_description.pDepthStencilAttachment = a_depth_attachment_reference;
	subpass_description.preserveAttachmentCount = 0;
	subpass_description.pPreserveAttachments    = nullptr;

	return subpass_description;
}

VkSubpassDependency vk_create_subpass_dependency(uint32_t a_src_subpass, uint32_t a_dst_subpass, VkPipelineStageFlags a_src_stage_mask, VkPipelineStageFlags a_dst_stage_mask,
                                                 VkAccessFlags a_src_access_mask, VkAccessFlags a_dst_access_mask, VkDependencyFlags a_dependency_flags)        // , int32_t a_view_offset)
{
	// For details of how this works read https://www.reddit.com/r/vulkan/comments/s80reu/subpass_dependencies_what_are_those_and_why_do_i/

	VkSubpassDependency subpass_dependency = {};

	// subpass_dependency.sType           = VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2;
	// subpass_dependency.pNext           = nullptr;
	subpass_dependency.srcSubpass      = a_src_subpass;            // VK_SUBPASS_EXTERNAL means finish all subpasses from previous renderpasses before continue
	subpass_dependency.dstSubpass      = a_dst_subpass;            // index of the current subpass for which this dependency exists
	subpass_dependency.srcStageMask    = a_src_stage_mask;         // All stages we ask Vulkan to finish before moving to dstSubpass like VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	subpass_dependency.dstStageMask    = a_dst_stage_mask;         // All the stages in dstSubpass not allowed to start until after the stages in srcStageMask have completed within srcSubpass
	subpass_dependency.srcAccessMask   = a_src_access_mask;        // Bitmask of all the Vulkan memory access types used by srcSubpass
	subpass_dependency.dstAccessMask   = a_dst_access_mask;        // Bitmask of all the Vulkan memory access types we're going to use in dstSubpass like VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	subpass_dependency.dependencyFlags = a_dependency_flags;
	// subpass_dependency.viewOffset      = a_view_offset;

	return subpass_dependency;
}

VkFramebuffer vk_create_framebuffer(VkDevice a_device, VkRenderPass a_renderpass, const std::vector<VkImageView> &a_attachments, VkExtent2D a_dimensions, uint32_t a_layers)
{
	VkFramebufferCreateInfo framebuffer_info = {};

	framebuffer_info.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebuffer_info.pNext           = nullptr;
	framebuffer_info.flags           = 0;
	framebuffer_info.renderPass      = a_renderpass;
	framebuffer_info.attachmentCount = static_cast<uint32_t>(a_attachments.size());
	framebuffer_info.pAttachments    = a_attachments.data();
	framebuffer_info.width           = a_dimensions.width;
	framebuffer_info.height          = a_dimensions.height;
	framebuffer_info.layers          = a_layers;

	VkFramebuffer framebuffer{nullptr};
	VkResult      result = vkCreateFramebuffer(a_device, &framebuffer_info, cfg::VkAllocator, &framebuffer);
	check_return_status(result, "vkCreateFramebuffer");

	return framebuffer;
}

}        // namespace rhi
