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
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <cassert>

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

	auto api_version = VK_API_VERSION_1_0;
	// vkEnumerateInstanceVersion can't be nullptr because its vusym function but the real function that it calls could be null
	// in which case we are on 1.0 but its not supported
	vkEnumerateInstanceVersion(&api_version);

	auto major = VK_VERSION_MAJOR(api_version);
	auto minor = VK_VERSION_MINOR(api_version);
	auto patch = VK_VERSION_PATCH(api_version);

	auto requestd_major = setting.m_vulkan.m_version_major;
	auto requestd_minor = setting.m_vulkan.m_version_minor;

	ror::log_info("Highest vulkan version available : {}.{}.{} and requested version : {}.{}.0", major, minor, patch, requestd_major, requestd_minor);

	assert(major >= requestd_major && minor >= requestd_minor && "Requested vulkan version not available");

	VkInstance        instance_handle{VK_NULL_HANDLE};
	VkApplicationInfo app_info = {};

	app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext              = nullptr;
	app_info.pApplicationName   = setting.m_application_name.c_str();
	app_info.applicationVersion = setting.m_application_version;
	app_info.pEngineName        = setting.m_engine_name.c_str();
	app_info.engineVersion      = setting.m_engine_version;
	app_info.apiVersion         = ror::vulkan_api_version();

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

	auto result = vkCreateInstance(&instance_create_info, cfg::VkAllocator, &instance_handle);
	check_return_status(result, "vkCreateInstance");

	this->set_handle(instance_handle);

	result = vkCreateDebugUtilsMessengerEXT(this->get_handle(), &debug_messenger_create_info, cfg::VkAllocator, &m_messenger);
	assert(result == VK_SUCCESS && "Failed to create Debug Utils Messenger!");
}

static const char *vk_physical_device_type_to_string(VkPhysicalDeviceType a_type)
{
	// clang-format off
	switch (a_type)
	{
	case VK_PHYSICAL_DEVICE_TYPE_OTHER:          return "VK_PHYSICAL_DEVICE_TYPE_OTHER";
	case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
	case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:   return "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
	case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:    return "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
	case VK_PHYSICAL_DEVICE_TYPE_CPU:            return "VK_PHYSICAL_DEVICE_TYPE_CPU";
	case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:       return "VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM";
	}
	// clang-format on

	return "UNKNOWN_DEVICE";
}

static void print_physical_device_properties(const VkPhysicalDeviceProperties &a_properties)
{
	auto api_version = a_properties.apiVersion;

	auto major = VK_VERSION_MAJOR(api_version);
	auto minor = VK_VERSION_MINOR(api_version);
	auto patch = VK_VERSION_PATCH(api_version);

	ror::log_info("Physical device properties:\nAPI Version:    {}.{}.{}\nDriver version: {}\nVendor ID:      {}\nDevice ID:      {}\nDevice Type:    {}\nDevice Name:    {}",
	              major, minor, patch,
	              a_properties.driverVersion,
	              a_properties.vendorID,
	              a_properties.deviceID,
	              vk_physical_device_type_to_string(a_properties.deviceType),
	              a_properties.deviceName);

	// static_cast<const unsigned char*>(a_properties.pipelineCacheUUID),
	// VkPhysicalDeviceLimits          a_properties.limits;
	// VkPhysicalDeviceSparseProperties   a_properties.sparseProperties;
}

void PhysicalDevice::init(Instance &a_instance)
{
	auto gpus = enumerate_general_property<VkPhysicalDevice, true>(vkEnumeratePhysicalDevices, a_instance.get_handle());

	VkPhysicalDevice           physical_device{VK_NULL_HANDLE};
	VkPhysicalDeviceProperties physical_device_properties{};        //! Physical device properties cache

	for (auto gpu : gpus)
	{
		vkGetPhysicalDeviceProperties(gpu, &physical_device_properties);

		if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			physical_device = gpu;
			break;
		}
	}

	if (physical_device == nullptr)
	{
		ror::log_critical("Couldn't find suitable discrete physical device, falling back to integrated gpu.");
		assert(gpus.size() > 1);
		physical_device = gpus[0];
	}

	// Now when we have settled down on a physical_device lets get its properties, note it might not be physical_device_properties local variable so don't use that
	vkGetPhysicalDeviceProperties(physical_device, &this->m_physical_device_properties);
	print_physical_device_properties(this->m_physical_device_properties);

	this->set_handle(physical_device);

	this->m_memory_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
	this->m_memory_properties.pNext = nullptr;

	vkGetPhysicalDeviceMemoryProperties2(this->get_handle(), &this->m_memory_properties);
}

void SwapChain::create(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat swapchain_format, VkExtent2D a_swapchain_extent)
{
	// Remember vk_create_swapchain might change swapchain_format which is later used here
	this->m_swapchain = vk_create_swapchain(a_physical_device, a_device, a_surface, swapchain_format, a_swapchain_extent);

	this->m_swapchain_images = enumerate_general_property<VkImage, true>(vkGetSwapchainImagesKHR, a_device, this->m_swapchain);

	// Creating image views for all swapchain images
	this->m_swapchain_images_views.resize(this->m_swapchain_images.size());

	for (size_t i = 0; i < this->m_swapchain_images.size(); ++i)
		this->m_swapchain_images_views[i] = vk_create_image_view(a_device, this->m_swapchain_images[i], swapchain_format, 1, VK_IMAGE_ASPECT_COLOR_BIT);

	this->m_format = swapchain_format;
}

void SwapChain::release(VkDevice a_device)
{
	for (auto &image_view : this->swapchain_images_views())
	{
		vkDestroyImageView(a_device, image_view, cfg::VkAllocator);
		image_view = nullptr;
	}

	vk_destroy_swapchain(a_device, this->swapchain());

	// Release images, and imageviews and swapchain later
	ror::log_critical("Release swapchain properly, not sure if images needs releasing, find out");
}

void DeviceVulkan::create_surface(void *a_window)
{
	// TODO: Add other types (Android non-glfw)
	auto status = ror::glfw_create_surface<VkInstance, VkSurfaceKHR>(this->m_instance.get_handle(), this->m_surface, a_window);

	if (status != VK_SUCCESS)
		ror::log_critical("Can't create window surface for vulkan device");
}

void DeviceVulkan::create_device()
{
	// TODO: Select properties/features you need here
	std::vector<float32_t *> priorities_pointers;
	QueueData                queue_data{};
	auto                     queues = get_queue_indices(this->m_gpu.get_handle(), this->m_surface, priorities_pointers, queue_data);

	this->m_device = vk_create_device(this->m_gpu.get_handle(), queues);

	// delete priorities_pointers after device is created
	for (auto priority : priorities_pointers)
		delete priority;
	priorities_pointers.clear();

	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[graphics_index].first].queueFamilyIndex, queue_data.m_indicies[graphics_index].second, &this->m_graphics_queue.m_queue);
	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[compute_index].first].queueFamilyIndex, queue_data.m_indicies[compute_index].second, &this->m_compute_queue.m_queue);
	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[transfer_index].first].queueFamilyIndex, queue_data.m_indicies[transfer_index].second, &this->m_transfer_queue.m_queue);
	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[sparse_index].first].queueFamilyIndex, queue_data.m_indicies[sparse_index].second, &this->m_sparse_queue.m_queue);
	vkGetDeviceQueue(this->m_device, queues[queue_data.m_indicies[protected_index].first].queueFamilyIndex, queue_data.m_indicies[protected_index].second, &this->m_protected_queue.m_queue);

	this->m_graphics_queue.m_index = queues[queue_data.m_indicies[graphics_index].first].queueFamilyIndex;
	this->m_present_queue.m_index  = this->m_graphics_queue.m_index;

	// Graphics and Present queues are the same
	this->m_present_queue.m_queue = this->m_graphics_queue.m_queue;

	// Set transfer queue index as well
	this->m_transfer_queue.m_index = queues[queue_data.m_indicies[transfer_index].first].queueFamilyIndex;

	// Set compute queue index as well
	this->m_compute_queue.m_index = queues[queue_data.m_indicies[compute_index].first].queueFamilyIndex;

	// Set sparse queue index as well
	this->m_sparse_queue.m_index = queues[queue_data.m_indicies[sparse_index].first].queueFamilyIndex;

	// Set protected queue index as well
	this->m_protected_queue.m_index = queues[queue_data.m_indicies[protected_index].first].queueFamilyIndex;

	this->m_pipeline_cache = vk_create_pipeline_cache(this->m_device);
}

}        // namespace rhi
