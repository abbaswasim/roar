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
	assert(result == VK_SUCCESS && "VKImageView creation failed");

	return image_view;
}

VkSwapchainKHR vk_create_swapchain(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat &swapchain_format, VkExtent2D a_swapchain_extent)
{
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(a_physical_device, a_surface, &capabilities);
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
	auto           result = vkCreateSwapchainKHR(a_device, &swapchain_create_info, cfg::VkAllocator, &swapchain);
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

}        // namespace rhi
