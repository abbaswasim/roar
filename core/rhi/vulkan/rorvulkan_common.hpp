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

#include "foundation/rormacros.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include <optional>
#include <typeindex>
#include <unordered_map>

namespace vk
{
extern VkAllocationCallbacks *VkAllocator;
}

namespace rhi
{

constexpr FORCE_INLINE const char *vk_result_to_string(VkResult a_result)
{
	// clang-format off
		switch (a_result)
		{
		case VK_SUCCESS:                                             return "VK_SUCCESS";
		case VK_NOT_READY:                                           return "VK_NOT_READY";
		case VK_TIMEOUT:                                             return "VK_TIMEOUT";
		case VK_EVENT_SET:                                           return "VK_EVENT_SET";
		case VK_EVENT_RESET:                                         return "VK_EVENT_RESET";
		case VK_INCOMPLETE:                                          return "VK_INCOMPLETE";
		case VK_ERROR_OUT_OF_HOST_MEMORY:                            return "VK_ERROR_OUT_OF_HOST_MEMORY";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:                          return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
		case VK_ERROR_INITIALIZATION_FAILED:                         return "VK_ERROR_INITIALIZATION_FAILED";
		case VK_ERROR_DEVICE_LOST:                                   return "VK_ERROR_DEVICE_LOST";
		case VK_ERROR_MEMORY_MAP_FAILED:                             return "VK_ERROR_MEMORY_MAP_FAILED";
		case VK_ERROR_LAYER_NOT_PRESENT:                             return "VK_ERROR_LAYER_NOT_PRESENT";
		case VK_ERROR_EXTENSION_NOT_PRESENT:                         return "VK_ERROR_EXTENSION_NOT_PRESENT";
		case VK_ERROR_FEATURE_NOT_PRESENT:                           return "VK_ERROR_FEATURE_NOT_PRESENT";
		case VK_ERROR_INCOMPATIBLE_DRIVER:                           return "VK_ERROR_INCOMPATIBLE_DRIVER";
		case VK_ERROR_TOO_MANY_OBJECTS:                              return "VK_ERROR_TOO_MANY_OBJECTS";
		case VK_ERROR_FORMAT_NOT_SUPPORTED:                          return "VK_ERROR_FORMAT_NOT_SUPPORTED";
		case VK_ERROR_FRAGMENTED_POOL:                               return "VK_ERROR_FRAGMENTED_POOL";
		case VK_ERROR_UNKNOWN:                                       return "VK_ERROR_UNKNOWN";
		case VK_ERROR_OUT_OF_POOL_MEMORY:                            return "VK_ERROR_OUT_OF_POOL_MEMORY";
		case VK_ERROR_INVALID_EXTERNAL_HANDLE:                       return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
		case VK_ERROR_FRAGMENTATION:                                 return "VK_ERROR_FRAGMENTATION";
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:                return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
		case VK_PIPELINE_COMPILE_REQUIRED:                           return "VK_PIPELINE_COMPILE_REQUIRED";
		case VK_ERROR_SURFACE_LOST_KHR:                              return "VK_ERROR_SURFACE_LOST_KHR";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:                      return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
		case VK_SUBOPTIMAL_KHR:                                      return "VK_SUBOPTIMAL_KHR";
		case VK_ERROR_OUT_OF_DATE_KHR:                               return "VK_ERROR_OUT_OF_DATE_KHR";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:                      return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
		case VK_ERROR_VALIDATION_FAILED_EXT:                         return "VK_ERROR_VALIDATION_FAILED_EXT";
		case VK_ERROR_INVALID_SHADER_NV:                             return "VK_ERROR_INVALID_SHADER_NV";
		case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:                 return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
		case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:        return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
		case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:     return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
		case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:        return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
		case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:         return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
		case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:           return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:  return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
		case VK_ERROR_NOT_PERMITTED_KHR:                             return "VK_ERROR_NOT_PERMITTED_KHR";
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:           return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
		case VK_THREAD_IDLE_KHR:                                     return "VK_THREAD_IDLE_KHR";
		case VK_THREAD_DONE_KHR:                                     return "VK_THREAD_DONE_KHR";
		case VK_OPERATION_DEFERRED_KHR:                              return "VK_OPERATION_DEFERRED_KHR";
		case VK_OPERATION_NOT_DEFERRED_KHR:                          return "VK_OPERATION_NOT_DEFERRED_KHR";
		case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:                     return "VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
		case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:                return "VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT";
		case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:              return "VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR";
		case VK_RESULT_MAX_ENUM:                                     return "VK_RESULT_MAX_ENUM";
		// case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:                        return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR";
		// case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:                   return "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR";
		// case VK_ERROR_FRAGMENTATION_EXT:                             return "VK_ERROR_FRAGMENTATION_EXT";
		// case VK_ERROR_NOT_PERMITTED_EXT:                             return "VK_ERROR_NOT_PERMITTED_EXT";
		// case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:                    return "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT";
		// case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR:            return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR";
		// case VK_PIPELINE_COMPILE_REQUIRED_EXT:                       return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
		// case VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT:                 return "VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT";
		}
	// clang-format on
}

// Accepts VkResult and compares with VK_SUCCESSS
#define check_return_status(result, funcall)                                                                 \
	if (ror::get_build() == ror::BuildType::build_debug && result != VK_SUCCESS)                             \
	{                                                                                                        \
		ror::log_critical("Vulkan command {} returned error code {}", funcall, vk_result_to_string(result)); \
		assert(0 && funcall);                                                                                \
	}                                                                                                        \
	(void) 0

constexpr FORCE_INLINE auto to_vulkan_pixelformat(rhi::PixelFormat a_pixelformat)
{
	return static_cast<VkFormat>(a_pixelformat);
}

constexpr FORCE_INLINE auto to_vulkan_vertexformat(rhi::VertexFormat a_vertexformat)
{
	assert(a_vertexformat < rhi::VertexFormat::bool32_1 && "Unsupported vertex format");
	return static_cast<VkFormat>(a_vertexformat);
}

constexpr FORCE_INLINE auto to_vulkan_indexformat(rhi::Format a_format)
{
	assert((a_format == rhi::Format::uint32_1 || a_format == rhi::Format::uint16_1) && "Unsupported vulkan index format");

	if (a_format == rhi::Format::uint16_1)
		return VkIndexType::VK_INDEX_TYPE_UINT16;
	else
		return VkIndexType::VK_INDEX_TYPE_UINT32;
}

constexpr FORCE_INLINE auto to_vulkan_step_function(rhi::StepFunction a_function)
{
	switch (a_function)
	{
			// clang-format off
	case rhi::StepFunction::vertex:                       return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
	case rhi::StepFunction::constant:                     assert(0 && "Don't support rhi::StepFunction::constant in Vulkan"); break;
	case rhi::StepFunction::instance:                     return VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE;
	case rhi::StepFunction::patch:                        assert(0 && "Don't support rhi::StepFunction::patch in Vulkan"); break;
	case rhi::StepFunction::patch_control_point:          assert(0 && "Don't support rhi::StepFunction::patch_control_point in Vulkan"); break;
			// clang-format on
	}

	assert(0 && "Shouldn't reach here");

	return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
}

constexpr FORCE_INLINE VkMemoryPropertyFlags to_vulkan_resource_option(rhi::ResourceStorageOption a_mode)
{
	return static_cast<VkMemoryPropertyFlags>(a_mode);
}

constexpr FORCE_INLINE auto to_vulkan_primitive_topoloy(rhi::PrimitiveTopology a_topology)
{
	return static_cast<VkPrimitiveTopology>(a_topology);
}

constexpr FORCE_INLINE auto to_vulkan_cull_mode(rhi::PrimitiveCullMode a_cull_mode)
{
	return static_cast<VkCullModeFlagBits>(a_cull_mode);
}

constexpr FORCE_INLINE auto to_vulkan_winding(rhi::PrimitiveWinding a_winding)
{
	return static_cast<VkFrontFace>(a_winding);
}

constexpr FORCE_INLINE auto to_vulkan_triangle_fill_mode(rhi::TriangleFillMode a_fill_mode)
{
	return static_cast<VkPolygonMode>(a_fill_mode);
}

constexpr FORCE_INLINE auto to_vulkan_depth_compare_function(rhi::DepthCompareFunction a_compare_function)
{
	return static_cast<VkCompareOp>(a_compare_function);
}

FORCE_INLINE constexpr VkImageViewType to_vulkan_image_view_target(rhi::TextureTarget a_target)
{
	assert(static_cast<uint32_t>(a_target) != VK_IMAGE_VIEW_TYPE_MAX_ENUM && "Don't understand this texture target type in vulkan");
	return static_cast<VkImageViewType>(a_target);
}

FORCE_INLINE constexpr VkImageType to_vulkan_image_target(rhi::TextureTarget a_target)
{
	VkImageViewType view_type = to_vulkan_image_view_target(a_target);

	if (view_type == VkImageViewType::VK_IMAGE_VIEW_TYPE_1D ||
	    view_type == VkImageViewType::VK_IMAGE_VIEW_TYPE_1D_ARRAY)
		return VkImageType::VK_IMAGE_TYPE_1D;
	else if (view_type == VkImageViewType::VK_IMAGE_VIEW_TYPE_2D ||
	         view_type == VkImageViewType::VK_IMAGE_VIEW_TYPE_2D_ARRAY ||
	         view_type == VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE ||
	         view_type == VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE_ARRAY)
		return VkImageType::VK_IMAGE_TYPE_2D;
	else if (view_type == VkImageViewType::VK_IMAGE_VIEW_TYPE_3D)
		return VkImageType::VK_IMAGE_TYPE_3D;

	assert(0 && "Don't understand other image view types yet");
	return VkImageType::VK_IMAGE_TYPE_2D;
}

FORCE_INLINE constexpr VkFilter to_vulkan_texture_filter(rhi::TextureFilter a_filter)
{
	return static_cast<VkFilter>(a_filter);
}

FORCE_INLINE constexpr VkSamplerMipmapMode to_vulkan_texture_mip_filter(rhi::TextureMipFilter a_filter)
{
	assert(a_filter != rhi::TextureMipFilter::not_mipmapped && "Don't call to_vulkan_texture_mip_filter if not mipmapped");
	return static_cast<VkSamplerMipmapMode>(a_filter);
}

FORCE_INLINE constexpr VkBorderColor to_vulkan_texture_border(rhi::TextureBorder a_color)
{
	return static_cast<VkBorderColor>(a_color);
}

FORCE_INLINE constexpr VkSamplerAddressMode to_vulkan_texture_address_mode(rhi::TextureAddressMode a_mode)
{
	assert(rhi::TextureAddressMode::clamp_to_zero != a_mode && "Vulkan doesn't support clamp to zero");

	return static_cast<VkSamplerAddressMode>(a_mode);
}

FORCE_INLINE auto get_surface_format()
{
	return VK_FORMAT_B8G8R8A8_SRGB;
}

FORCE_INLINE auto get_surface_colorspace()
{
	return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
	// return VK_COLOR_SPACE_PASS_THROUGH_EXT;
}

FORCE_INLINE auto get_surface_transform()
{
	// TODO: Fix the hardcode 90 degree rotation
	return ror::window_prerotated() ? VK_SURFACE_TRANSFORM_ROTATE_90_BIT_KHR : VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
}

FORCE_INLINE auto get_surface_composition_mode()
{
	return (ror::window_transparent() ?
	            (ror::window_premultiplied() ?
	                 VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR :
	                 VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR) :
	            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR);
}

FORCE_INLINE auto get_swapchain_usage()
{
	return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
}

FORCE_INLINE VkSampleCountFlagBits get_sample_count(VkPhysicalDeviceProperties a_physical_device_properties)
{
	// Get color and depth sample count flags
	VkSampleCountFlags counts = a_physical_device_properties.limits.framebufferColorSampleCounts &
	                            a_physical_device_properties.limits.framebufferDepthSampleCounts;

	VkSampleCountFlagBits required = static_cast<VkSampleCountFlagBits>(ror::multisample_count());        // FIXME: Dangerous, if vulkan header changes or sample count isn't 2's power

	// Return if required available otherwise go lower until an alternative is available
	do
	{
		if ((counts & static_cast<uint32_t>(required)) == required)
			return required;

		required = static_cast<VkSampleCountFlagBits>(required >> 1);

	} while (required);

	// No choice but to return no MSAA
	return VK_SAMPLE_COUNT_1_BIT;
}

FORCE_INLINE auto get_swapchain_sharing_mode(uint32_t a_queue_family_indices[2])
{
	VkSwapchainCreateInfoKHR create_info{};

	if (a_queue_family_indices[0] != a_queue_family_indices[1])
	{
		create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices   = a_queue_family_indices;
	}
	else
	{
		create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0;              // Optional
		create_info.pQueueFamilyIndices   = nullptr;        // Optional
	}

	return create_info;
}

FORCE_INLINE std::vector<const char *> instance_extensions_requested()
{
	/* Usually available extensions
	  VK_KHR_device_group_creation
	  VK_KHR_external_fence_capabilities
	  VK_KHR_external_memory_capabilities
	  VK_KHR_external_semaphore_capabilities
	  VK_KHR_get_physical_device_properties2
	  VK_KHR_get_surface_capabilities2
	  VK_KHR_surface
	  VK_EXT_debug_report
	  VK_EXT_debug_utils
	  VK_EXT_metal_surface
	  VK_EXT_swapchain_colorspace
	  VK_MVK_macos_surface // MacOS specific
	*/
	return std::vector<const char *>
    {
	    // clang-format off
		VK_KHR_SURFACE_EXTENSION_NAME,                                 // VK_KHR_surface
		VK_KHR_DISPLAY_EXTENSION_NAME,                                 // VK_KHR_display
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,                             // VK_EXT_debug_utils
		VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,        // VK_KHR_get_physical_device_properties2
		VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME,                 // VK_KHR_portability_enumeration
		VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,                  // VK_EXT_extended_dynamic_state
#if defined __APPLE__
		VK_EXT_METAL_SURFACE_EXTENSION_NAME,                           // "VK_EXT_metal_surface"
#elif defined __linux__
		VK_KHR_XCB_SURFACE_EXTENSION_NAME                              // "VK_EXT_xcb_surface??"
#endif
	    // clang-format on
	};
}

FORCE_INLINE std::vector<const char *> instance_layers_requested()
{
	/* Usually available layers
	  VK_LAYER_LUNARG_api_dump
	  VK_LAYER_KHRONOS_validation
	*/
	return std::vector<const char *>{ror::settings().m_vulkan.m_validation ? "VK_LAYER_KHRONOS_validation" : ""};
}

FORCE_INLINE std::vector<const char *> device_extensions_requested()
{
	return std::vector<const char *>{
	    VK_KHR_SWAPCHAIN_EXTENSION_NAME,                // VK_KHR_swapchain
	    VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME        // "VK_KHR_portability_subset"
	};
}

FORCE_INLINE std::vector<const char *> device_layers_requested()
{
	// Device layers are now deprecated, this always return empty. Not backward compatible
	return std::vector<const char *>{};
}

template <class _type, typename std::enable_if<std::is_same<_type, VkExtensionProperties>::value>::type * = nullptr>
FORCE_INLINE std::string get_properties_type_name(_type a_type)
{
	return a_type.extensionName;
}

template <class _type, typename std::enable_if<std::is_same<_type, VkLayerProperties>::value>::type * = nullptr>
FORCE_INLINE std::string get_properties_type_name(_type a_type)
{
	return a_type.layerName;
}

template <class _type>
using properties_type = typename std::conditional<std::is_same<_type, VkExtensionProperties>::value, VkExtensionProperties, VkLayerProperties>::type;

template <class _type, class _property, typename std::enable_if<std::is_same<_type, VkInstance>::value>::type * = nullptr, typename std::enable_if<std::is_same<_property, VkExtensionProperties>::value>::type * = nullptr>
FORCE_INLINE VkResult get_properties_function(const char *a_name, uint32_t &a_count, properties_type<_property> *a_properties, _type a_context)
{
	(void) a_context;

	return vkEnumerateInstanceExtensionProperties(a_name, &a_count, a_properties);
}

template <class _type, class _property, typename std::enable_if<std::is_same<_type, VkInstance>::value>::type * = nullptr, typename std::enable_if<std::is_same<_property, VkLayerProperties>::value>::type * = nullptr>
FORCE_INLINE VkResult get_properties_function(const char *a_name, uint32_t &a_count, properties_type<_property> *a_properties, _type a_context)
{
	(void) a_name;
	(void) a_context;

	return vkEnumerateInstanceLayerProperties(&a_count, a_properties);
}

template <class _type, class _property, typename std::enable_if<std::is_same<_type, VkPhysicalDevice>::value>::type * = nullptr, typename std::enable_if<std::is_same<_property, VkExtensionProperties>::value>::type * = nullptr>
FORCE_INLINE VkResult get_properties_function(const char *a_name, uint32_t &a_count, properties_type<_property> *a_properties, _type a_context)
{
	return vkEnumerateDeviceExtensionProperties(a_context, a_name, &a_count, a_properties);
}

template <class _type, class _property, typename std::enable_if<std::is_same<_type, VkPhysicalDevice>::value>::type * = nullptr, typename std::enable_if<std::is_same<_property, VkLayerProperties>::value>::type * = nullptr>
FORCE_INLINE VkResult get_properties_function(const char *a_name, uint32_t &a_count, properties_type<_property> *a_properties, _type a_context)
{
	(void) a_name;

	return vkEnumerateDeviceLayerProperties(a_context, &a_count, a_properties);
}

template <class _type, class _property>
FORCE_INLINE auto get_properties_requested_list()
{
	if constexpr (std::is_same<_type, VkInstance>::value)
	{
		if constexpr (std::is_same<_property, VkExtensionProperties>::value)
			return instance_extensions_requested();
		else
			return instance_layers_requested();
	}
	else if constexpr (std::is_same<_type, VkPhysicalDevice>::value)
	{
		if constexpr (std::is_same<_property, VkExtensionProperties>::value)
			return device_extensions_requested();
		else
			return device_layers_requested();
	}

	return std::vector<const char *>{};
}

template <class _type>
FORCE_INLINE std::string get_name()
{
	static std::unordered_map<std::type_index, std::string> type_names;

	// This should all be compile time constants
	type_names[std::type_index(typeid(VkInstance))]            = "instance";
	type_names[std::type_index(typeid(VkPhysicalDevice))]      = "physical device";
	type_names[std::type_index(typeid(VkExtensionProperties))] = "extension";
	type_names[std::type_index(typeid(VkLayerProperties))]     = "layer";

	return type_names[std::type_index(typeid(_type))];
}

template <class _type, class _property>
FORCE_INLINE std::string get_properties_requested_erro_message(std::string a_prefix = "!.")
{
	static std::string output{"Failed to enumerate "};

	output.append(get_name<_type>());
	output.append(" ");
	output.append(get_name<_property>());
	output.append(a_prefix);

	return output;
}

template <class _type, class _property>
std::vector<const char *> enumerate_properties(_type a_context = nullptr)
{
	uint32_t properties_count{0};
	if (get_properties_function<_type, _property>(nullptr, properties_count, nullptr, a_context) != VK_SUCCESS)
		throw std::runtime_error(get_properties_requested_erro_message<_type, _property>());

	std::vector<properties_type<_property>> properties{properties_count};
	if (get_properties_function<_type, _property>(nullptr, properties_count, properties.data(), a_context) != VK_SUCCESS)
		throw std::runtime_error(get_properties_requested_erro_message<_type, _property>(" calling it again!."));

	ror::log_info("All available {} {}s:", get_name<_type>(), get_name<_property>());
	for (const auto &property : properties)
	{
		ror::log_info("\t{}", get_properties_type_name(property));
	}

	std::vector<const char *> properties_available;

	auto properties_requested = get_properties_requested_list<_type, _property>();

	for (const auto &property_requested : properties_requested)
	{
		if (std::find_if(properties.begin(),
		                 properties.end(),
		                 [&property_requested](properties_type<_property> &arg) {
			                 return std::strcmp(get_properties_type_name(arg).c_str(), property_requested) == 0;
		                 }) != properties.end())
		{
			properties_available.emplace_back(property_requested);
		}
		else
		{
			ror::log_critical("Requested {} {} not available.", get_name<_property>(), property_requested);
		}
	}

	ror::log_info("Enabling the following {}s:", get_name<_property>());
	for (const auto &property : properties_available)
	{
		ror::log_info("\t{}", property);
	}

	return properties_available;
}

template <class _property_type, bool _returns, typename _function, typename... _rest>
std::vector<_property_type> enumerate_general_property(_function &&a_fptr, _rest &&...a_rest_of_args)
{
	// TODO: Add some indication of function name or where the error comes from

	VkResult                    result{VK_SUCCESS};
	unsigned int                count{0};
	std::vector<_property_type> items;

	do
	{
		if constexpr (_returns)
			result = a_fptr(a_rest_of_args..., &count, nullptr);
		else
			a_fptr(a_rest_of_args..., &count, nullptr);

		assert(result == VK_SUCCESS && "enumerate general failed!");
		assert(count > 0 && "None of the properties required are available");

		items.resize(count, _property_type{});

		if constexpr (_returns)
			result = a_fptr(a_rest_of_args..., &count, items.data());
		else
			a_fptr(a_rest_of_args..., &count, items.data());

	} while (result == VK_INCOMPLETE);

	assert(result == VK_SUCCESS && "enumerate general failed!");
	assert(count > 0 && "None of the properties required are available");

	return items;
}

const std::vector<VkQueueFlags> all_family_flags{VK_QUEUE_GRAPHICS_BIT,
                                                 VK_QUEUE_COMPUTE_BIT,
                                                 VK_QUEUE_TRANSFER_BIT,
                                                 VK_QUEUE_SPARSE_BINDING_BIT,
                                                 VK_QUEUE_PROTECTED_BIT};

struct QueueData
{
	QueueData()
	{
		this->m_indicies.resize(all_family_flags.size());
	}

	std::vector<std::pair<uint32_t, uint32_t>> m_indicies{};
};

// a_others is the exclusion list I don't want in this family
inline auto get_dedicated_queue_family(std::vector<VkQueueFamilyProperties> &a_queue_families, VkQueueFlags a_queue_flag, VkQueueFlags a_others, uint32_t &a_index)
{
	uint32_t index = 0;
	for (auto &queue_family : a_queue_families)
	{
		if (((queue_family.queueFlags & a_queue_flag) == a_queue_flag) &&
		    (queue_family.queueCount > 0) &&
		    !((queue_family.queueFlags & a_others) == a_others))
		{
			a_index = index;
			queue_family.queueCount--;
			return true;
		}
		index++;
	}
	return false;
}

// TODO: Extract out
inline auto get_priority(VkQueueFlags a_flag)
{
	if (a_flag & VK_QUEUE_GRAPHICS_BIT)
		return 0.75f;
	if (a_flag & VK_QUEUE_COMPUTE_BIT)
		return 1.00f;
	if (a_flag & VK_QUEUE_TRANSFER_BIT)
		return 0.50f;
	if (a_flag & VK_QUEUE_SPARSE_BINDING_BIT)
		return 0.20f;
	if (a_flag & VK_QUEUE_PROTECTED_BIT)
		return 0.10f;

	return 0.0f;
}

const uint32_t graphics_index{0};
const uint32_t compute_index{1};
const uint32_t transfer_index{2};
const uint32_t sparse_index{3};
const uint32_t protected_index{4};

inline auto get_queue_indices(VkPhysicalDevice a_physical_device, VkSurfaceKHR a_surface, std::vector<float32_t *> &a_priorities_pointers, QueueData &a_queue_data)
{
	auto queue_families = enumerate_general_property<VkQueueFamilyProperties, false>(vkGetPhysicalDeviceQueueFamilyProperties, a_physical_device);

	// Other tests
	// std::vector<VkQueueFamilyProperties> queue_families{
	//	{VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT,
	//	 16,
	//	 64,
	//	 {1, 1, 1}},
	//	{VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT,
	//	 2,
	//	 64,
	//	 {1, 1, 1}},
	//	{VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_SPARSE_BINDING_BIT,
	//	 8,
	//	 64,
	//	 {1, 1, 1}}};

	// std::vector<VkQueueFamilyProperties> queue_families{
	//	{VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT,
	//	 2,
	//	 0,
	//	 {1, 1, 1}}};

	std::vector<std::pair<bool, uint32_t>> found_indices{};
	found_indices.resize(all_family_flags.size());

	found_indices[graphics_index].first = get_dedicated_queue_family(queue_families, VK_QUEUE_GRAPHICS_BIT, static_cast<uint32_t>(~VK_QUEUE_GRAPHICS_BIT), found_indices[graphics_index].second);
	assert(found_indices[graphics_index].first && "No graphics queue found can't continue!");

	found_indices[compute_index].first = get_dedicated_queue_family(queue_families, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_GRAPHICS_BIT, found_indices[compute_index].second);

	if (!found_indices[compute_index].first)
	{
		found_indices[compute_index].first = get_dedicated_queue_family(queue_families, VK_QUEUE_COMPUTE_BIT, static_cast<uint32_t>(~VK_QUEUE_GRAPHICS_BIT), found_indices[compute_index].second);
		assert(found_indices[compute_index].first && "No compute queue found can't continue!");
	}

	// Look for a queue that has transfer but no compute or graphics
	found_indices[transfer_index].first = get_dedicated_queue_family(queue_families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT, found_indices[transfer_index].second);

	if (!found_indices[transfer_index].first)
	{
		// Look for a queue that has transfer but no compute
		found_indices[transfer_index].first = get_dedicated_queue_family(queue_families, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT, found_indices[transfer_index].second);
		if (!found_indices[transfer_index].first)
		{
			// Get the first one that supports transfer, quite possible the one with Graphics
			found_indices[transfer_index].first = get_dedicated_queue_family(queue_families, VK_QUEUE_TRANSFER_BIT, static_cast<uint32_t>(~VK_QUEUE_TRANSFER_BIT), found_indices[transfer_index].second);
			// If still can't find one just use the graphics queue
			if (!found_indices[transfer_index].first)
				found_indices[transfer_index].second = found_indices[graphics_index].second;
		}
	}

	found_indices[sparse_index].first    = get_dedicated_queue_family(queue_families, VK_QUEUE_SPARSE_BINDING_BIT, static_cast<uint32_t>(~VK_QUEUE_SPARSE_BINDING_BIT), found_indices[sparse_index].second);
	found_indices[protected_index].first = get_dedicated_queue_family(queue_families, VK_QUEUE_PROTECTED_BIT, static_cast<uint32_t>(~VK_QUEUE_PROTECTED_BIT), found_indices[protected_index].second);

	std::vector<VkDeviceQueueCreateInfo> device_queue_create_infos{};
	device_queue_create_infos.reserve(all_family_flags.size());

	VkDeviceQueueCreateInfo device_queue_create_info{};
	device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	device_queue_create_info.pNext = nullptr;
	device_queue_create_info.flags = 0;        // Remember if had to change, then need to use vkGetDeviceQueue2

	// device_queue_create_info.queueFamilyIndex = // Assigned in the loop
	// device_queue_create_info.queueCount = 1;        // Assigned in the loop too
	// device_queue_create_info.pQueuePriorities =  // Assigned in the loop

	std::vector<std::pair<std::optional<uint32_t>, std::vector<float32_t>>> consolidated_families;
	consolidated_families.resize(queue_families.size());

	uint32_t priority_index = 0;
	for (const auto &index : found_indices)
	{
		if (index.first)
		{
			if (!consolidated_families[index.second].first.has_value())
				consolidated_families[index.second].first = index.second;

			assert(consolidated_families[index.second].first == index.second && "Index mismatch for queue family!");
			consolidated_families[index.second].second.push_back(get_priority(all_family_flags[priority_index]));
			a_queue_data.m_indicies[priority_index] = std::make_pair(index.second, consolidated_families[index.second].second.size() - 1);
		}
		priority_index++;
	}

	{
		VkBool32 present_support = false;
		auto     result          = vkGetPhysicalDeviceSurfaceSupportKHR(a_physical_device, a_queue_data.m_indicies[graphics_index].first, a_surface, &present_support);
		check_return_status(result, "vkGetPhysicalDeviceSurfaceSupportKHR");
		assert(present_support && "Graphics queue chosen doesn't support presentation!");
	}
	{
		VkBool32 present_support = false;
		auto     result          = vkGetPhysicalDeviceSurfaceSupportKHR(a_physical_device, a_queue_data.m_indicies[compute_index].first, a_surface, &present_support);
		check_return_status(result, "vkGetPhysicalDeviceSurfaceSupportKHR");
		assert(present_support && "Compute queue chosen doesn't support presentation!");
	}

	for (const auto &queue_family : consolidated_families)
	{
		if (queue_family.first.has_value())
		{
			auto pptr = new float32_t[queue_family.second.size()];
			a_priorities_pointers.push_back(pptr);

			for (size_t i = 0; i < queue_family.second.size(); ++i)
			{
				pptr[i] = queue_family.second[i];
			}

			device_queue_create_info.pQueuePriorities = pptr;
			device_queue_create_info.queueFamilyIndex = queue_family.first.value();
			device_queue_create_info.queueCount       = ror::static_cast_safe<uint32_t>(queue_family.second.size());

			device_queue_create_infos.push_back(device_queue_create_info);
		}
	}

	assert(device_queue_create_infos.size() >= 1);

	return device_queue_create_infos;
}
}        // namespace rhi
