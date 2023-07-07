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

#include "foundation/rorutilities.hpp"
#include "resources/rorresource.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"

namespace rhi
{

VkImageView vk_create_image_view(VkDevice a_device, VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspect_flags, uint32_t a_mip_levels, VkImageViewType a_type,
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

VkDevice vk_create_device(VkPhysicalDevice physical_device, std::vector<VkDeviceQueueCreateInfo> &queues)
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

VkBuffer vk_create_buffer(VkDevice a_device, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, std::vector<uint32_t> &a_queue_family_indices)
{
	VkBufferCreateInfo buffer_info{};

	buffer_info.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buffer_info.pNext                 = nullptr;
	buffer_info.flags                 = 0;
	buffer_info.size                  = a_size;                                                                // example: 1024 * 1024 * 2;        // 2kb
	buffer_info.usage                 = a_usage;                                                               // example: VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
	buffer_info.sharingMode           = a_sharing_mode;                                                        // example: VK_SHARING_MODE_EXCLUSIVE; has performance implications, not all resources are shared either
	buffer_info.queueFamilyIndexCount = ror::static_cast_safe<uint32_t>(a_queue_family_indices.size());        // Size of queue familes buffer will support
	buffer_info.pQueueFamilyIndices   = a_queue_family_indices.data();                                         // example: {GRAPHICS ,COMPUTE} in a vector

	VkBuffer buffer;
	VkResult result = vkCreateBuffer(a_device, &buffer_info, cfg::VkAllocator, &buffer);
	check_return_status(result, "VkCreateBuffer");

	assert(buffer && "Couldn't allocate buffer, probably out of memory");

	return buffer;
}

VkDeviceMemory vk_allocate_memory(VkDevice a_device, VkDeviceSize a_size, uint32_t a_memory_type_index)
{
	VkMemoryAllocateInfo allocation_info{};
	allocation_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocation_info.allocationSize  = a_size;                     // a_buffer_mem_req.size;
	allocation_info.memoryTypeIndex = a_memory_type_index;        // find_memory_type(a_buffer_mem_req.memoryTypeBits, a_properties);
	allocation_info.pNext           = nullptr;

	VkDeviceMemory buffer_memory;
	VkResult       result = vkAllocateMemory(a_device, &allocation_info, cfg::VkAllocator, &buffer_memory);
	check_return_status(result, "vkAllocateMemory");
	assert(buffer_memory && "Memory allocation failed");

	return buffer_memory;
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
	auto result = vkBindBufferMemory(a_device, a_buffer, a_memory, 0);
	check_return_status(result, "vkBindBufferMemory");
}

VkDeviceMemory vk_bind_buffer_memory(VkDevice a_device, VkBuffer a_buffer, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties)
{
	VkMemoryRequirements2 buffer_mem_req    = vk_buffer_memory_requirements(a_device, a_buffer);
	auto                  memory_type_index = vk_find_memory_type(buffer_mem_req.memoryRequirements.memoryTypeBits, a_memory_properties, a_properties);
	auto                  memory            = vk_allocate_memory(a_device, buffer_mem_req.memoryRequirements.size, memory_type_index);

	vk_bind_buffer_memory(a_device, a_buffer, memory);

	return memory;
}

void vk_create_buffer_with_memory(VkDevice a_device, VkBuffer &a_buffer, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, std::vector<uint32_t> &a_queue_family_indices,
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

uint32_t vk_find_memory_type(uint32_t a_type_filter, VkPhysicalDeviceMemoryProperties a_memory_properties, VkMemoryPropertyFlags a_properties)
{
	for (uint32_t i = 0; i < a_memory_properties.memoryTypeCount; i++)
		if ((a_type_filter & (1 << i)) && ((a_memory_properties.memoryTypes[i].propertyFlags & a_properties) == a_properties))
			return i;

	throw std::runtime_error("Failed to find suitable memory type!");
}

uint8_t *vk_map_memory(VkDevice a_device, VkDeviceMemory a_memory)
{
	void *data{nullptr};
	auto  result = vkMapMemory(a_device, a_memory, 0, VK_WHOLE_SIZE, 0, &data);
	check_return_status(result, "vkMapMemory");

	return static_cast<uint8_t *>(data);
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

VkCommandBuffer vk_begin_command_buffer(VkCommandBuffer a_command_buffer, VkCommandBufferBeginInfo &a_command_buffer_begin_info)
{
	auto result = vkBeginCommandBuffer(a_command_buffer, &a_command_buffer_begin_info);
	check_return_status(result, "vkBeginCommandBuffer");
}

VkCommandBuffer vk_end_command_buffer(VkCommandBuffer a_command_buffer)
{
	auto result = vkEndCommandBuffer(a_command_buffer);
	check_return_status(result, "vkEndCommandBuffer");
}

VkCommandBuffer vk_begin_single_use_cmd_buffer(VkDevice a_device, VkCommandPool a_command_pool)
{
	VkCommandBuffer staging_command_buffer = vk_allocate_command_buffer(a_device, a_command_pool);

	VkCommandBufferBeginInfo command_buffer_begin_info{};
	command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vk_begin_command_buffer(staging_command_buffer, command_buffer_begin_info);

	return staging_command_buffer;
}

void vk_queue_submit(VkQueue a_queue, VkSubmitInfo &a_submit_info, VkFence a_fence)
{
	auto result = vkQueueSubmit(a_queue, 1, &a_submit_info, a_fence);
	check_return_status(result, "vkQueueSubmit");
}

void vk_queue_submit(VkQueue a_queue, std::vector<VkSubmitInfo> &a_submit_info, VkFence a_fence)
{
	auto result = vkQueueSubmit(a_queue, ror::static_cast_safe<uint32_t>(a_submit_info.size()), a_submit_info.data(), a_fence);
	check_return_status(result, "vkQueueSubmit");
}

void vk_queue_submit(VkQueue a_queue, VkCommandBuffer a_command_buffer, VkFence a_fence)
{
	VkSubmitInfo submit_info{};
	submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers    = &a_command_buffer;

	vk_queue_submit(a_queue, submit_info, a_fence);
}

void vk_queue_submit(VkQueue a_queue, VkSubmitInfo &a_submit_info, std::vector<VkCommandBuffer> a_command_buffers, VkFence a_fence)
{
	a_submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	a_submit_info.commandBufferCount = ror::static_cast_safe<uint32_t>(a_command_buffers.size());
	a_submit_info.pCommandBuffers    = a_command_buffers.data();

	vk_queue_submit(a_queue, a_submit_info, a_fence);
}

void vk_queue_wait_idle(VkQueue a_queue)
{
	auto result = vkQueueWaitIdle(a_queue);
	check_return_status(result, "vkQueueWaitIdle");
}

void vk_end_single_use_command_buffer(VkCommandBuffer a_command_buffer, VkQueue a_queue)
{
	vk_end_command_buffer(a_command_buffer);
	vk_queue_submit(a_queue, a_command_buffer, VK_NULL_HANDLE);
}

void vk_end_single_use_command_buffer_and_wait(VkDevice a_device, VkCommandBuffer a_command_buffer, VkQueue a_transfer_queue, VkCommandPool a_command_pool)
{
	vk_end_single_use_command_buffer(a_command_buffer, a_transfer_queue);
	vk_queue_wait_idle(a_transfer_queue);
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

}        // namespace rhi
