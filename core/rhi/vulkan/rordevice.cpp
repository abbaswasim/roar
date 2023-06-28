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

#include "platform/rorglfw_wrapper.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/vulkan/rordevice.hpp"

namespace rhi
{

define_translation_unit_vtable(Instance)
{}

define_translation_unit_vtable(PhysicalDevice)
{}

define_translation_unit_vtable(SwapChain)
{}

define_translation_unit_vtable(DeviceVulkan)
{}

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_generic_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      a_message_severity,
    VkDebugUtilsMessageTypeFlagsEXT             a_message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *a_callback_data,
    void                                       *a_user_data)
{
	// (void) a_message_severity;
	// (void) a_message_type;
	// (void) a_callback_data;
	(void) a_user_data;

	std::string prefix{};

	switch (a_message_type)
	{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
			prefix = "performance";
			break;
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
			prefix = "validation";
			break;
		default:        // VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
			prefix = "general";
	}

	if (a_message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		ror::log_error("Validation layer {} error: {}", prefix, a_callback_data->pMessage);
	else if (a_message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		ror::log_warn("Validation layer {} warning: {}", prefix, a_callback_data->pMessage);
	else if (a_message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)        // includes VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		ror::log_info("Validation layer {} info: {}", prefix, a_callback_data->pMessage);
	else
		ror::log_critical("Validation layer {} critical error: {}", prefix, a_callback_data->pMessage);

	return VK_FALSE;
}

Instance::Instance()
{
	auto &setting = ror::settings();

	// Set debug messenger callback setup required later after instance creation
	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};
	debug_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debug_messenger_create_info.pNext = nullptr;

	debug_messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
	                                              VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
	                                              VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

	debug_messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
	                                          VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
	                                          VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

	debug_messenger_create_info.pfnUserCallback = vk_debug_generic_callback;
	debug_messenger_create_info.pUserData       = nullptr;        // Optional

	VkInstance        instance_handle{VK_NULL_HANDLE};
	VkApplicationInfo app_info = {};

	app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext              = nullptr;
	app_info.pApplicationName   = setting.m_application_name.c_str();
	app_info.applicationVersion = setting.m_application_version;
	app_info.pEngineName        = setting.m_engine_name.c_str();
	app_info.engineVersion      = setting.m_engine_version;
	app_info.apiVersion         = ror::vulkan_api_version();
	// Should this be result of vkEnumerateInstanceVersion

	auto extensions = enumerate_properties<VkInstance, VkExtensionProperties>();
	auto layers     = enumerate_properties<VkInstance, VkLayerProperties>();

	VkInstanceCreateInfo instance_create_info    = {};
	instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pNext                   = &debug_messenger_create_info;        // nullptr;
	instance_create_info.pApplicationInfo        = &app_info;
	instance_create_info.enabledLayerCount       = ror::static_cast_safe<uint32_t>(layers.size());
	instance_create_info.ppEnabledLayerNames     = layers.data();
	instance_create_info.enabledExtensionCount   = ror::static_cast_safe<uint32_t>(extensions.size());
	instance_create_info.ppEnabledExtensionNames = extensions.data();
	instance_create_info.flags                   = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	VkResult result{};
	result = vkCreateInstance(&instance_create_info, cfg::VkAllocator, &instance_handle);
	assert(result == VK_SUCCESS && "Failed to create vulkan instance!");

	this->set_handle(instance_handle);

	result = vkCreateDebugUtilsMessengerEXT(this->get_handle(), &debug_messenger_create_info, cfg::VkAllocator, &m_messenger);
	assert(result == VK_SUCCESS && "Failed to create Debug Utils Messenger!");
}

VkImageView create_image_view(VkDevice a_device, VkImage a_image, VkFormat a_format, VkImageAspectFlags a_aspect_flags, uint32_t a_mip_levels, VkImageViewType a_type,
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

void destroy_imageview(VkDevice a_device, VkImageView a_image_view)
{
	vkDestroyImageView(a_device, a_image_view, cfg::VkAllocator);
}

SwapChain create_swapchain(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat swapchain_format, VkExtent2D a_swapchain_extent)
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

	SwapChain swap;

	VkSwapchainKHR swapchain{nullptr};
	auto           result = vkCreateSwapchainKHR(a_device, &swapchain_create_info, cfg::VkAllocator, &swapchain);
	assert(result == VK_SUCCESS);

	std::vector<VkImage>     swapchain_images;
	std::vector<VkImageView> swapchain_images_views;
	swapchain_images = enumerate_general_property<VkImage, true>(vkGetSwapchainImagesKHR, a_device, swapchain);

	// Creating image views for all swapchain images
	swapchain_images_views.resize(swapchain_images.size());

	for (size_t i = 0; i < swapchain_images.size(); ++i)
		swapchain_images_views[i] = create_image_view(a_device, swapchain_images[i], swapchain_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);

	swap.swapchain(swapchain);
	swap.swapchain_images(std::move(swapchain_images));
	swap.swapchain_images_views(std::move(swapchain_images_views));
	swap.format(swapchain_format);

	return swap;
}

void release_swapchain(VkDevice a_device, SwapChain &a_swapchain)
{
	for (auto &image_view : a_swapchain.swapchain_images_views())
	{
		vkDestroyImageView(a_device, image_view, cfg::VkAllocator);
		image_view = nullptr;
	}

	destroy_swapchain(a_device, a_swapchain.swapchain());

	// Release images, and imageviews and swapchain later
	ror::log_critical("Release swapchain properly, not sure if images needs releasing, find out");
}

void destroy_swapchain(VkDevice a_device, VkSwapchainKHR a_swapchain)
{
	vkDestroySwapchainKHR(a_device, a_swapchain, cfg::VkAllocator);
	a_swapchain = nullptr;
}

void DeviceVulkan::create_surface(void *a_window)
{
	auto status = ror::glfw_create_surface<VkInstance, VkSurfaceKHR>(this->m_instance.get_handle(), this->m_surface, a_window);

	if (status != VK_SUCCESS)
		ror::log_critical("Can't create window surface for vulkan device");
}

void DeviceVulkan::create_device()
{
	// TODO: Select properties/features you need here
	VkPhysicalDevice         physical_device = this->m_gpu.get_handle();
	VkPhysicalDeviceFeatures physical_device_features{};

	vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

	if (ror::sample_rate_shading_enabled())
	{
		assert(physical_device_features.sampleRateShading == VK_TRUE && "Sample Rate Shading not avialable");
	}

	VkDeviceCreateInfo       device_create_info{};
	std::vector<float32_t *> priorities_pointers;
	QueueData                queue_data{};

	auto extensions = enumerate_properties<VkPhysicalDevice, VkExtensionProperties>(physical_device);
	auto layers     = enumerate_properties<VkPhysicalDevice, VkLayerProperties>(physical_device);
	auto queues     = get_queue_indices(physical_device, this->m_surface, priorities_pointers, queue_data);

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

	auto result = vkCreateDevice(physical_device, &device_create_info, cfg::VkAllocator, &this->m_device);
	assert(result == VK_SUCCESS);

	// delete priorities_pointers;
	for (auto priority : priorities_pointers)
		delete priority;
	priorities_pointers.clear();

	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[graphics_index].first].queueFamilyIndex, queue_data.m_indicies[graphics_index].second, &this->m_graphics_queue);
	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[compute_index].first].queueFamilyIndex, queue_data.m_indicies[compute_index].second, &this->m_compute_queue);
	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[transfer_index].first].queueFamilyIndex, queue_data.m_indicies[transfer_index].second, &this->m_transfer_queue);
	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[sparse_index].first].queueFamilyIndex, queue_data.m_indicies[sparse_index].second, &this->m_sparse_queue);
	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[protected_index].first].queueFamilyIndex, queue_data.m_indicies[protected_index].second, &this->m_protected_queue);

	this->m_graphics_queue_index = queues[queue_data.m_indicies[graphics_index].first].queueFamilyIndex;
	this->m_present_queue_index  = this->m_graphics_queue_index;

	// Graphics and Present queues are the same
	this->m_present_queue = this->m_graphics_queue;

	// Set transfer queue index as well
	this->m_transfer_queue_index = queues[queue_data.m_indicies[transfer_index].first].queueFamilyIndex;

	// Set compute queue index as well
	this->m_compute_queue_index = queues[queue_data.m_indicies[compute_index].first].queueFamilyIndex;
}

void DeviceVulkan::destory_surface()
{
	vkDestroySurfaceKHR(this->m_instance.get_handle(), this->m_surface, nullptr);
	this->m_surface = nullptr;
}

void DeviceVulkan::destroy_device()
{
	vkDestroyDevice(this->m_device, cfg::VkAllocator);
	this->m_device = nullptr;
}
}        // namespace rhi
