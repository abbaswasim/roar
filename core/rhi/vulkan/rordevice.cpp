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
#include "renderer/rorrenderer.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/vulkan/rordevice.hpp"
#include "rhi/vulkan/rorrenderpass.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include "settings/rorsettings.hpp"
#include <cassert>
#include <regex>

namespace rhi
{

static void set_log_level()
{
	if (ror::settings().m_vulkan.m_verbose_log)
		ror::log_set_level(ror::LogLevel::trace);
	else
		ror::log_set_level(ror::LogLevel::warn);
}

static void reset_log_level()
{
	ror::log_set_level(ror::LogLevel::trace);
}

define_translation_unit_vtable(Instance)
{}

define_translation_unit_vtable(PhysicalDevice)
{}

define_translation_unit_vtable(SwapChain)
{}

define_translation_unit_vtable(DeviceVulkan)
{}

define_translation_unit_vtable(VulkanValidationException)
{}

std::string format_validation_message(const char *a_message)
{
	std::regex object("Object");
	std::regex handle("handle =");
	std::regex type("type =");
	std::regex message("\\| MessageID = ");
	std::regex bar(" \\| ");
	std::regex spec("The Vulkan spec states:");

	std::string objects  = std::regex_replace(a_message, object, "\n\t$&");
	std::string handles  = std::regex_replace(objects, handle, "\n\t$&");
	std::string types    = std::regex_replace(handles, type, "\n\t$&");
	std::string messages = std::regex_replace(types, message, "\n\tMessageID = ");
	std::string bars     = std::regex_replace(messages, bar, "\n\t");
	std::string specs    = std::regex_replace(bars, spec, "\n\t$&\n\t\t");

	return specs;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_generic_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT      a_message_severity,
    VkDebugUtilsMessageTypeFlagsEXT             a_message_type,
    const VkDebugUtilsMessengerCallbackDataEXT *a_callback_data,
    void                                       *a_user_data)
{
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

	std::string formatted_message{format_validation_message(a_callback_data->pMessage)};
	if (a_message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		ror::log_error("Validation layer {} error: \n\t{}", prefix, formatted_message.c_str());
		throw VulkanValidationException{formatted_message.c_str()};
	}
	else if (a_message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		ror::log_warn("Validation layer {} warning: \n\t{}", prefix, formatted_message.c_str());
	else if (a_message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)        // includes VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		ror::log_info("Validation layer {} info: \n\t{}", prefix, formatted_message.c_str());
	else
		ror::log_critical("Validation layer {} critical error: \n\t{}", prefix, formatted_message.c_str());

	return VK_FALSE;
}

Instance::Instance()
{
#if defined(USE_VOLK)
	volkInitialize();
#endif

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

	set_log_level();

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

	// Now lets init all the Instance related functions
#if defined(USE_VOLK)
	volkLoadInstance(instance_handle);
#endif

	result = vkCreateDebugUtilsMessengerEXT(this->get_handle(), &debug_messenger_create_info, cfg::VkAllocator, &m_messenger);
	assert(result == VK_SUCCESS && "Failed to create Debug Utils Messenger!");

	reset_log_level();
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

	auto        major                = VK_VERSION_MAJOR(api_version);
	auto        minor                = VK_VERSION_MINOR(api_version);
	auto        patch                = VK_VERSION_PATCH(api_version);
	const char *physical_device_type = vk_physical_device_type_to_string(a_properties.deviceType);

	ror::log_info("Physical device properties:\nAPI Version:    {}.{}.{}\nDriver version: {}\nVendor ID:      {}\nDevice ID:      {}\nDevice Type:    {}\nDevice Name:    {}\n"
	              "maxImageDimension1D = {}\n"
	              "maxImageDimension2D = {}\n"
	              "maxImageDimension3D = {}\n"
	              "maxImageDimensionCube = {}\n"
	              "maxImageArrayLayers = {}\n"
	              "maxTexelBufferElements = {}\n"
	              "maxUniformBufferRange = {}\n"
	              "maxStorageBufferRange = {}\n"
	              "maxPushConstantsSize = {}\n"
	              "maxMemoryAllocationCount = {}\n"
	              "maxSamplerAllocationCount = {}\n"
	              "bufferImageGranularity = {}\n"
	              "sparseAddressSpaceSize = {}\n"
	              "maxBoundDescriptorSets = {}\n"
	              "maxPerStageDescriptorSamplers = {}\n"
	              "maxPerStageDescriptorUniformBuffers = {}\n"
	              "maxPerStageDescriptorStorageBuffers = {}\n"
	              "maxPerStageDescriptorSampledImages = {}\n"
	              "maxPerStageDescriptorStorageImages = {}\n"
	              "maxPerStageDescriptorInputAttachments = {}\n"
	              "maxPerStageResources = {}\n"
	              "maxDescriptorSetSamplers = {}\n"
	              "maxDescriptorSetUniformBuffers = {}\n"
	              "maxDescriptorSetUniformBuffersDynamic = {}\n"
	              "maxDescriptorSetStorageBuffers = {}\n"
	              "maxDescriptorSetStorageBuffersDynamic = {}\n"
	              "maxDescriptorSetSampledImages = {}\n"
	              "maxDescriptorSetStorageImages = {}\n"
	              "maxDescriptorSetInputAttachments = {}\n"
	              "maxVertexInputAttributes = {}\n"
	              "maxVertexInputBindings = {}\n"
	              "maxVertexInputAttributeOffset = {}\n"
	              "maxVertexInputBindingStride = {}\n"
	              "maxVertexOutputComponents = {}\n"
	              "maxTessellationGenerationLevel = {}\n"
	              "maxTessellationPatchSize = {}\n"
	              "maxTessellationControlPerVertexInputComponents = {}\n"
	              "maxTessellationControlPerVertexOutputComponents = {}\n"
	              "maxTessellationControlPerPatchOutputComponents = {}\n"
	              "maxTessellationControlTotalOutputComponents = {}\n"
	              "maxTessellationEvaluationInputComponents = {}\n"
	              "maxTessellationEvaluationOutputComponents = {}\n"
	              "maxGeometryShaderInvocations = {}\n"
	              "maxGeometryInputComponents = {}\n"
	              "maxGeometryOutputComponents = {}\n"
	              "maxGeometryOutputVertices = {}\n"
	              "maxGeometryTotalOutputComponents = {}\n"
	              "maxFragmentInputComponents = {}\n"
	              "maxFragmentOutputAttachments = {}\n"
	              "maxFragmentDualSrcAttachments = {}\n"
	              "maxFragmentCombinedOutputResources = {}\n"
	              "maxComputeSharedMemorySize = {}\n"
	              "maxComputeWorkGroupCount = ({}, {}, {})\n"
	              "maxComputeWorkGroupInvocations = {}\n"
	              "maxComputeWorkGroupSize = ({}, {}, {})\n"
	              "subPixelPrecisionBits = {}\n"
	              "subTexelPrecisionBits = {}\n"
	              "mipmapPrecisionBits = {}\n"
	              "maxDrawIndexedIndexValue = {}\n"
	              "maxDrawIndirectCount = {}\n"
	              "maxSamplerLodBias = {}\n"
	              "maxSamplerAnisotropy = {}\n"
	              "maxViewports = {}\n"
	              "maxViewportDimensions = ({}, {})\n"
	              "viewportBoundsRange = ({}, {})\n"
	              "viewportSubPixelBits = {}\n"
	              "minMemoryMapAlignment = {}\n"
	              "minTexelBufferOffsetAlignment = {}\n"
	              "minUniformBufferOffsetAlignment = {}\n"
	              "minStorageBufferOffsetAlignment = {}\n"
	              "minTexelOffset = {}\n"
	              "maxTexelOffset = {}\n"
	              "minTexelGatherOffset = {}\n"
	              "maxTexelGatherOffset = {}\n"
	              "minInterpolationOffset = {}\n"
	              "maxInterpolationOffset = {}\n"
	              "subPixelInterpolationOffsetBits = {}\n"
	              "maxFramebufferWidth = {}\n"
	              "maxFramebufferHeight = {}\n"
	              "maxFramebufferLayers = {}\n"
	              "framebufferColorSampleCounts = {}\n"
	              "framebufferDepthSampleCounts = {}\n"
	              "framebufferStencilSampleCounts = {}\n"
	              "framebufferNoAttachmentsSampleCounts = {}\n"
	              "maxColorAttachments = {}\n"
	              "sampledImageColorSampleCounts = {}\n"
	              "sampledImageIntegerSampleCounts = {}\n"
	              "sampledImageDepthSampleCounts = {}\n"
	              "sampledImageStencilSampleCounts = {}\n"
	              "storageImageSampleCounts = {}\n"
	              "maxSampleMaskWords = {}\n"
	              "timestampComputeAndGraphics = {}\n"
	              "timestampPeriod = {}\n"
	              "maxClipDistances = {}\n"
	              "maxCullDistances = {}\n"
	              "maxCombinedClipAndCullDistances = {}\n"
	              "discreteQueuePriorities = {}\n"
	              "pointSizeRange = ({}, {})\n"
	              "lineWidthRange = ({}, {})\n"
	              "pointSizeGranularity = {}\n"
	              "lineWidthGranularity = {}\n"
	              "strictLines = {}\n"
	              "standardSampleLocations = {}\n"
	              "optimalBufferCopyOffsetAlignment = {}\n"
	              "optimalBufferCopyRowPitchAlignment = {}\n"
	              "nonCoherentAtomSize = {}\n",
	              major, minor, patch,
	              a_properties.driverVersion,
	              a_properties.vendorID,
	              a_properties.deviceID,
	              physical_device_type,
	              a_properties.deviceName,
	              a_properties.limits.maxImageDimension1D,
	              a_properties.limits.maxImageDimension2D,
	              a_properties.limits.maxImageDimension3D,
	              a_properties.limits.maxImageDimensionCube,
	              a_properties.limits.maxImageArrayLayers,
	              a_properties.limits.maxTexelBufferElements,
	              a_properties.limits.maxUniformBufferRange,
	              a_properties.limits.maxStorageBufferRange,
	              a_properties.limits.maxPushConstantsSize,
	              a_properties.limits.maxMemoryAllocationCount,
	              a_properties.limits.maxSamplerAllocationCount,
	              a_properties.limits.bufferImageGranularity,
	              a_properties.limits.sparseAddressSpaceSize,
	              a_properties.limits.maxBoundDescriptorSets,
	              a_properties.limits.maxPerStageDescriptorSamplers,
	              a_properties.limits.maxPerStageDescriptorUniformBuffers,
	              a_properties.limits.maxPerStageDescriptorStorageBuffers,
	              a_properties.limits.maxPerStageDescriptorSampledImages,
	              a_properties.limits.maxPerStageDescriptorStorageImages,
	              a_properties.limits.maxPerStageDescriptorInputAttachments,
	              a_properties.limits.maxPerStageResources,
	              a_properties.limits.maxDescriptorSetSamplers,
	              a_properties.limits.maxDescriptorSetUniformBuffers,
	              a_properties.limits.maxDescriptorSetUniformBuffersDynamic,
	              a_properties.limits.maxDescriptorSetStorageBuffers,
	              a_properties.limits.maxDescriptorSetStorageBuffersDynamic,
	              a_properties.limits.maxDescriptorSetSampledImages,
	              a_properties.limits.maxDescriptorSetStorageImages,
	              a_properties.limits.maxDescriptorSetInputAttachments,
	              a_properties.limits.maxVertexInputAttributes,
	              a_properties.limits.maxVertexInputBindings,
	              a_properties.limits.maxVertexInputAttributeOffset,
	              a_properties.limits.maxVertexInputBindingStride,
	              a_properties.limits.maxVertexOutputComponents,
	              a_properties.limits.maxTessellationGenerationLevel,
	              a_properties.limits.maxTessellationPatchSize,
	              a_properties.limits.maxTessellationControlPerVertexInputComponents,
	              a_properties.limits.maxTessellationControlPerVertexOutputComponents,
	              a_properties.limits.maxTessellationControlPerPatchOutputComponents,
	              a_properties.limits.maxTessellationControlTotalOutputComponents,
	              a_properties.limits.maxTessellationEvaluationInputComponents,
	              a_properties.limits.maxTessellationEvaluationOutputComponents,
	              a_properties.limits.maxGeometryShaderInvocations,
	              a_properties.limits.maxGeometryInputComponents,
	              a_properties.limits.maxGeometryOutputComponents,
	              a_properties.limits.maxGeometryOutputVertices,
	              a_properties.limits.maxGeometryTotalOutputComponents,
	              a_properties.limits.maxFragmentInputComponents,
	              a_properties.limits.maxFragmentOutputAttachments,
	              a_properties.limits.maxFragmentDualSrcAttachments,
	              a_properties.limits.maxFragmentCombinedOutputResources,
	              a_properties.limits.maxComputeSharedMemorySize,
	              a_properties.limits.maxComputeWorkGroupCount[0], a_properties.limits.maxComputeWorkGroupCount[1], a_properties.limits.maxComputeWorkGroupCount[2],
	              a_properties.limits.maxComputeWorkGroupInvocations,
	              a_properties.limits.maxComputeWorkGroupSize[0], a_properties.limits.maxComputeWorkGroupSize[1], a_properties.limits.maxComputeWorkGroupSize[2],
	              a_properties.limits.subPixelPrecisionBits,
	              a_properties.limits.subTexelPrecisionBits,
	              a_properties.limits.mipmapPrecisionBits,
	              a_properties.limits.maxDrawIndexedIndexValue,
	              a_properties.limits.maxDrawIndirectCount,
	              a_properties.limits.maxSamplerLodBias,
	              a_properties.limits.maxSamplerAnisotropy,
	              a_properties.limits.maxViewports,
	              a_properties.limits.maxViewportDimensions[0], a_properties.limits.maxViewportDimensions[1],
	              a_properties.limits.viewportBoundsRange[0], a_properties.limits.viewportBoundsRange[1],
	              a_properties.limits.viewportSubPixelBits,
	              a_properties.limits.minMemoryMapAlignment,
	              a_properties.limits.minTexelBufferOffsetAlignment,
	              a_properties.limits.minUniformBufferOffsetAlignment,
	              a_properties.limits.minStorageBufferOffsetAlignment,
	              a_properties.limits.minTexelOffset,
	              a_properties.limits.maxTexelOffset,
	              a_properties.limits.minTexelGatherOffset,
	              a_properties.limits.maxTexelGatherOffset,
	              a_properties.limits.minInterpolationOffset,
	              a_properties.limits.maxInterpolationOffset,
	              a_properties.limits.subPixelInterpolationOffsetBits,
	              a_properties.limits.maxFramebufferWidth,
	              a_properties.limits.maxFramebufferHeight,
	              a_properties.limits.maxFramebufferLayers,
	              a_properties.limits.framebufferColorSampleCounts,
	              a_properties.limits.framebufferDepthSampleCounts,
	              a_properties.limits.framebufferStencilSampleCounts,
	              a_properties.limits.framebufferNoAttachmentsSampleCounts,
	              a_properties.limits.maxColorAttachments,
	              a_properties.limits.sampledImageColorSampleCounts,
	              a_properties.limits.sampledImageIntegerSampleCounts,
	              a_properties.limits.sampledImageDepthSampleCounts,
	              a_properties.limits.sampledImageStencilSampleCounts,
	              a_properties.limits.storageImageSampleCounts,
	              a_properties.limits.maxSampleMaskWords,
	              a_properties.limits.timestampComputeAndGraphics,
	              a_properties.limits.timestampPeriod,
	              a_properties.limits.maxClipDistances,
	              a_properties.limits.maxCullDistances,
	              a_properties.limits.maxCombinedClipAndCullDistances,
	              a_properties.limits.discreteQueuePriorities,
	              a_properties.limits.pointSizeRange[0], a_properties.limits.pointSizeRange[1],
	              a_properties.limits.lineWidthRange[1], a_properties.limits.lineWidthRange[1],
	              a_properties.limits.pointSizeGranularity,
	              a_properties.limits.lineWidthGranularity,
	              a_properties.limits.strictLines,
	              a_properties.limits.standardSampleLocations,
	              a_properties.limits.optimalBufferCopyOffsetAlignment,
	              a_properties.limits.optimalBufferCopyRowPitchAlignment,
	              a_properties.limits.nonCoherentAtomSize);

	// "residencyStandard2DBlockShape = {}\n",
	// "residencyStandard2DMultisampleBlockShape = {}\n",
	// "residencyStandard3DBlockShape = {}\n",
	// "residencyAlignedMipSize = {}\n",
	// "residencyNonResidentStrict = {}\n",
	// a_properties.sparseProperties.residencyStandard2DBlockShape,
	// a_properties.sparseProperties.residencyStandard2DMultisampleBlockShape,
	// a_properties.sparseProperties.residencyStandard3DBlockShape,
	// a_properties.sparseProperties.residencyAlignedMipSize,
	// a_properties.sparseProperties.residencyNonResidentStrict);

	// static_cast<const unsigned char*>(a_properties.pipelineCacheUUID),
	// VkPhysicalDeviceLimits          a_properties.limits;
	// VkPhysicalDeviceSparseProperties   a_properties.sparseProperties;
}

std::string vk_format_to_string(uint32_t format)
{
	switch (format)
	{
			// clang-format off
		case VK_FORMAT_R4G4_UNORM_PACK8: return "VK_FORMAT_R4G4_UNORM_PACK8";
		case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return "VK_FORMAT_R4G4B4A4_UNORM_PACK16";
		case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return "VK_FORMAT_B4G4R4A4_UNORM_PACK16";
		case VK_FORMAT_R5G6B5_UNORM_PACK16: return "VK_FORMAT_R5G6B5_UNORM_PACK16";
		case VK_FORMAT_B5G6R5_UNORM_PACK16: return "VK_FORMAT_B5G6R5_UNORM_PACK16";
		case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return "VK_FORMAT_R5G5B5A1_UNORM_PACK16";
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return "VK_FORMAT_B5G5R5A1_UNORM_PACK16";
		case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return "VK_FORMAT_A1R5G5B5_UNORM_PACK16";
		case VK_FORMAT_R8_UNORM: return "VK_FORMAT_R8_UNORM";
		case VK_FORMAT_R8_SNORM: return "VK_FORMAT_R8_SNORM";
		case VK_FORMAT_R8_USCALED: return "VK_FORMAT_R8_USCALED";
		case VK_FORMAT_R8_SSCALED: return "VK_FORMAT_R8_SSCALED";
		case VK_FORMAT_R8_UINT: return "VK_FORMAT_R8_UINT";
		case VK_FORMAT_R8_SINT: return "VK_FORMAT_R8_SINT";
		case VK_FORMAT_R8_SRGB: return "VK_FORMAT_R8_SRGB";
		case VK_FORMAT_R8G8_UNORM: return "VK_FORMAT_R8G8_UNORM";
		case VK_FORMAT_R8G8_SNORM: return "VK_FORMAT_R8G8_SNORM";
		case VK_FORMAT_R8G8_USCALED: return "VK_FORMAT_R8G8_USCALED";
		case VK_FORMAT_R8G8_SSCALED: return "VK_FORMAT_R8G8_SSCALED";
		case VK_FORMAT_R8G8_UINT: return "VK_FORMAT_R8G8_UINT";
		case VK_FORMAT_R8G8_SINT: return "VK_FORMAT_R8G8_SINT";
		case VK_FORMAT_R8G8_SRGB: return "VK_FORMAT_R8G8_SRGB";
		case VK_FORMAT_R8G8B8_UNORM: return "VK_FORMAT_R8G8B8_UNORM";
		case VK_FORMAT_R8G8B8_SNORM: return "VK_FORMAT_R8G8B8_SNORM";
		case VK_FORMAT_R8G8B8_USCALED: return "VK_FORMAT_R8G8B8_USCALED";
		case VK_FORMAT_R8G8B8_SSCALED: return "VK_FORMAT_R8G8B8_SSCALED";
		case VK_FORMAT_R8G8B8_UINT: return "VK_FORMAT_R8G8B8_UINT";
		case VK_FORMAT_R8G8B8_SINT: return "VK_FORMAT_R8G8B8_SINT";
		case VK_FORMAT_R8G8B8_SRGB: return "VK_FORMAT_R8G8B8_SRGB";
		case VK_FORMAT_B8G8R8_UNORM: return "VK_FORMAT_B8G8R8_UNORM";
		case VK_FORMAT_B8G8R8_SNORM: return "VK_FORMAT_B8G8R8_SNORM";
		case VK_FORMAT_B8G8R8_USCALED: return "VK_FORMAT_B8G8R8_USCALED";
		case VK_FORMAT_B8G8R8_SSCALED: return "VK_FORMAT_B8G8R8_SSCALED";
		case VK_FORMAT_B8G8R8_UINT: return "VK_FORMAT_B8G8R8_UINT";
		case VK_FORMAT_B8G8R8_SINT: return "VK_FORMAT_B8G8R8_SINT";
		case VK_FORMAT_B8G8R8_SRGB: return "VK_FORMAT_B8G8R8_SRGB";
		case VK_FORMAT_R8G8B8A8_UNORM: return "VK_FORMAT_R8G8B8A8_UNORM";
		case VK_FORMAT_R8G8B8A8_SNORM: return "VK_FORMAT_R8G8B8A8_SNORM";
		case VK_FORMAT_R8G8B8A8_USCALED: return "VK_FORMAT_R8G8B8A8_USCALED";
		case VK_FORMAT_R8G8B8A8_SSCALED: return "VK_FORMAT_R8G8B8A8_SSCALED";
		case VK_FORMAT_R8G8B8A8_UINT: return "VK_FORMAT_R8G8B8A8_UINT";
		case VK_FORMAT_R8G8B8A8_SINT: return "VK_FORMAT_R8G8B8A8_SINT";
		case VK_FORMAT_R8G8B8A8_SRGB: return "VK_FORMAT_R8G8B8A8_SRGB";
		case VK_FORMAT_B8G8R8A8_UNORM: return "VK_FORMAT_B8G8R8A8_UNORM";
		case VK_FORMAT_B8G8R8A8_SNORM: return "VK_FORMAT_B8G8R8A8_SNORM";
		case VK_FORMAT_B8G8R8A8_USCALED: return "VK_FORMAT_B8G8R8A8_USCALED";
		case VK_FORMAT_B8G8R8A8_SSCALED: return "VK_FORMAT_B8G8R8A8_SSCALED";
		case VK_FORMAT_B8G8R8A8_UINT: return "VK_FORMAT_B8G8R8A8_UINT";
		case VK_FORMAT_B8G8R8A8_SINT: return "VK_FORMAT_B8G8R8A8_SINT";
		case VK_FORMAT_B8G8R8A8_SRGB: return "VK_FORMAT_B8G8R8A8_SRGB";
		case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return "VK_FORMAT_A8B8G8R8_UNORM_PACK32";
		case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return "VK_FORMAT_A8B8G8R8_SNORM_PACK32";
		case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return "VK_FORMAT_A8B8G8R8_USCALED_PACK32";
		case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32";
		case VK_FORMAT_A8B8G8R8_UINT_PACK32: return "VK_FORMAT_A8B8G8R8_UINT_PACK32";
		case VK_FORMAT_A8B8G8R8_SINT_PACK32: return "VK_FORMAT_A8B8G8R8_SINT_PACK32";
		case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return "VK_FORMAT_A8B8G8R8_SRGB_PACK32";
		case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return "VK_FORMAT_A2R10G10B10_UNORM_PACK32";
		case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return "VK_FORMAT_A2R10G10B10_SNORM_PACK32";
		case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return "VK_FORMAT_A2R10G10B10_USCALED_PACK32";
		case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32";
		case VK_FORMAT_A2R10G10B10_UINT_PACK32: return "VK_FORMAT_A2R10G10B10_UINT_PACK32";
		case VK_FORMAT_A2R10G10B10_SINT_PACK32: return "VK_FORMAT_A2R10G10B10_SINT_PACK32";
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return "VK_FORMAT_A2B10G10R10_UNORM_PACK32";
		case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return "VK_FORMAT_A2B10G10R10_SNORM_PACK32";
		case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return "VK_FORMAT_A2B10G10R10_USCALED_PACK32";
		case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32";
		case VK_FORMAT_A2B10G10R10_UINT_PACK32: return "VK_FORMAT_A2B10G10R10_UINT_PACK32";
		case VK_FORMAT_A2B10G10R10_SINT_PACK32: return "VK_FORMAT_A2B10G10R10_SINT_PACK32";
		case VK_FORMAT_R16_UNORM: return "VK_FORMAT_R16_UNORM";
		case VK_FORMAT_R16_SNORM: return "VK_FORMAT_R16_SNORM";
		case VK_FORMAT_R16_USCALED: return "VK_FORMAT_R16_USCALED";
		case VK_FORMAT_R16_SSCALED: return "VK_FORMAT_R16_SSCALED";
		case VK_FORMAT_R16_UINT: return "VK_FORMAT_R16_UINT";
		case VK_FORMAT_R16_SINT: return "VK_FORMAT_R16_SINT";
		case VK_FORMAT_R16_SFLOAT: return "VK_FORMAT_R16_SFLOAT";
		case VK_FORMAT_R16G16_UNORM: return "VK_FORMAT_R16G16_UNORM";
		case VK_FORMAT_R16G16_SNORM: return "VK_FORMAT_R16G16_SNORM";
		case VK_FORMAT_R16G16_USCALED: return "VK_FORMAT_R16G16_USCALED";
		case VK_FORMAT_R16G16_SSCALED: return "VK_FORMAT_R16G16_SSCALED";
		case VK_FORMAT_R16G16_UINT: return "VK_FORMAT_R16G16_UINT";
		case VK_FORMAT_R16G16_SINT: return "VK_FORMAT_R16G16_SINT";
		case VK_FORMAT_R16G16_SFLOAT: return "VK_FORMAT_R16G16_SFLOAT";
		case VK_FORMAT_R16G16B16_UNORM: return "VK_FORMAT_R16G16B16_UNORM";
		case VK_FORMAT_R16G16B16_SNORM: return "VK_FORMAT_R16G16B16_SNORM";
		case VK_FORMAT_R16G16B16_USCALED: return "VK_FORMAT_R16G16B16_USCALED";
		case VK_FORMAT_R16G16B16_SSCALED: return "VK_FORMAT_R16G16B16_SSCALED";
		case VK_FORMAT_R16G16B16_UINT: return "VK_FORMAT_R16G16B16_UINT";
		case VK_FORMAT_R16G16B16_SINT: return "VK_FORMAT_R16G16B16_SINT";
		case VK_FORMAT_R16G16B16_SFLOAT: return "VK_FORMAT_R16G16B16_SFLOAT";
		case VK_FORMAT_R16G16B16A16_UNORM: return "VK_FORMAT_R16G16B16A16_UNORM";
		case VK_FORMAT_R16G16B16A16_SNORM: return "VK_FORMAT_R16G16B16A16_SNORM";
		case VK_FORMAT_R16G16B16A16_USCALED: return "VK_FORMAT_R16G16B16A16_USCALED";
		case VK_FORMAT_R16G16B16A16_SSCALED: return "VK_FORMAT_R16G16B16A16_SSCALED";
		case VK_FORMAT_R16G16B16A16_UINT: return "VK_FORMAT_R16G16B16A16_UINT";
		case VK_FORMAT_R16G16B16A16_SINT: return "VK_FORMAT_R16G16B16A16_SINT";
		case VK_FORMAT_R16G16B16A16_SFLOAT: return "VK_FORMAT_R16G16B16A16_SFLOAT";
		case VK_FORMAT_R32_UINT: return "VK_FORMAT_R32_UINT";
		case VK_FORMAT_R32_SINT: return "VK_FORMAT_R32_SINT";
		case VK_FORMAT_R32_SFLOAT: return "VK_FORMAT_R32_SFLOAT";
		case VK_FORMAT_R32G32_UINT: return "VK_FORMAT_R32G32_UINT";
		case VK_FORMAT_R32G32_SINT: return "VK_FORMAT_R32G32_SINT";
		case VK_FORMAT_R32G32_SFLOAT: return "VK_FORMAT_R32G32_SFLOAT";
		case VK_FORMAT_R32G32B32_UINT: return "VK_FORMAT_R32G32B32_UINT";
		case VK_FORMAT_R32G32B32_SINT: return "VK_FORMAT_R32G32B32_SINT";
		case VK_FORMAT_R32G32B32_SFLOAT: return "VK_FORMAT_R32G32B32_SFLOAT";
		case VK_FORMAT_R32G32B32A32_UINT: return "VK_FORMAT_R32G32B32A32_UINT";
		case VK_FORMAT_R32G32B32A32_SINT: return "VK_FORMAT_R32G32B32A32_SINT";
		case VK_FORMAT_R32G32B32A32_SFLOAT: return "VK_FORMAT_R32G32B32A32_SFLOAT";
			// clang-format on
	}
	return "";
}

std::string vk_format_property_flag(uint32_t format_flag)
{
	std::string supported_str{};

	if ((format_flag & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT) == VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_STORAGE_IMAGE_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT) == VK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_STORAGE_IMAGE_ATOMIC_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT) == VK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_UNIFORM_TEXEL_BUFFER_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT) == VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT) == VK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_STORAGE_TEXEL_BUFFER_ATOMIC_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT) == VK_FORMAT_FEATURE_VERTEX_BUFFER_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_VERTEX_BUFFER_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT) == VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) == VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_BLIT_SRC_BIT) == VK_FORMAT_FEATURE_BLIT_SRC_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_BLIT_SRC_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_BLIT_DST_BIT) == VK_FORMAT_FEATURE_BLIT_DST_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_BLIT_DST_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_TRANSFER_SRC_BIT) == VK_FORMAT_FEATURE_TRANSFER_SRC_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_TRANSFER_SRC_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_TRANSFER_DST_BIT) == VK_FORMAT_FEATURE_TRANSFER_DST_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_TRANSFER_DST_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT) == VK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_MIDPOINT_CHROMA_SAMPLES_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_LINEAR_FILTER_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_SEPARATE_RECONSTRUCTION_FILTER_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_YCBCR_CONVERSION_CHROMA_RECONSTRUCTION_EXPLICIT_FORCEABLE_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_DISJOINT_BIT) == VK_FORMAT_FEATURE_DISJOINT_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_DISJOINT_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT) == VK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_COSITED_CHROMA_SAMPLES_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT)
		supported_str += "\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_MINMAX_BIT\n";
	if ((format_flag & VK_FORMAT_FEATURE_VIDEO_DECODE_OUTPUT_BIT_KHR) == VK_FORMAT_FEATURE_VIDEO_DECODE_OUTPUT_BIT_KHR)
		supported_str += "\tVK_FORMAT_FEATURE_VIDEO_DECODE_OUTPUT_BIT_KHR\n";
	if ((format_flag & VK_FORMAT_FEATURE_VIDEO_DECODE_DPB_BIT_KHR) == VK_FORMAT_FEATURE_VIDEO_DECODE_DPB_BIT_KHR)
		supported_str += "\tVK_FORMAT_FEATURE_VIDEO_DECODE_DPB_BIT_KHR\n";
	if ((format_flag & VK_FORMAT_FEATURE_ACCELERATION_STRUCTURE_VERTEX_BUFFER_BIT_KHR) == VK_FORMAT_FEATURE_ACCELERATION_STRUCTURE_VERTEX_BUFFER_BIT_KHR)
		supported_str += "\tVK_FORMAT_FEATURE_ACCELERATION_STRUCTURE_VERTEX_BUFFER_BIT_KHR\n";
	if ((format_flag & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT) == VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT)
		supported_str += "\tVK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_CUBIC_BIT_EXT\n";
	if ((format_flag & VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT) == VK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT)
		supported_str += "\tVK_FORMAT_FEATURE_FRAGMENT_DENSITY_MAP_BIT_EXT\n";
	if ((format_flag & VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR) == VK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR)
		supported_str += "\tVK_FORMAT_FEATURE_FRAGMENT_SHADING_RATE_ATTACHMENT_BIT_KHR\n";
	if ((format_flag & VK_FORMAT_FEATURE_VIDEO_ENCODE_INPUT_BIT_KHR) == VK_FORMAT_FEATURE_VIDEO_ENCODE_INPUT_BIT_KHR)
		supported_str += "\tVK_FORMAT_FEATURE_VIDEO_ENCODE_INPUT_BIT_KHR\n";
	if ((format_flag & VK_FORMAT_FEATURE_VIDEO_ENCODE_DPB_BIT_KHR) == VK_FORMAT_FEATURE_VIDEO_ENCODE_DPB_BIT_KHR)
		supported_str += "\tVK_FORMAT_FEATURE_VIDEO_ENCODE_DPB_BIT_KHR";

	return supported_str;
}

static void print_physical_device_formats_properties(VkPhysicalDevice a_physical_device)
{
	std::vector<VkFormat> formats = {
	    VK_FORMAT_R4G4_UNORM_PACK8,
	    VK_FORMAT_R4G4B4A4_UNORM_PACK16,
	    VK_FORMAT_B4G4R4A4_UNORM_PACK16,
	    VK_FORMAT_R5G6B5_UNORM_PACK16,
	    VK_FORMAT_B5G6R5_UNORM_PACK16,
	    VK_FORMAT_R5G5B5A1_UNORM_PACK16,
	    VK_FORMAT_B5G5R5A1_UNORM_PACK16,
	    VK_FORMAT_A1R5G5B5_UNORM_PACK16,
	    VK_FORMAT_R8_UNORM,
	    VK_FORMAT_R8_SNORM,
	    VK_FORMAT_R8_USCALED,
	    VK_FORMAT_R8_SSCALED,
	    VK_FORMAT_R8_UINT,
	    VK_FORMAT_R8_SINT,
	    VK_FORMAT_R8_SRGB,
	    VK_FORMAT_R8G8_UNORM,
	    VK_FORMAT_R8G8_SNORM,
	    VK_FORMAT_R8G8_USCALED,
	    VK_FORMAT_R8G8_SSCALED,
	    VK_FORMAT_R8G8_UINT,
	    VK_FORMAT_R8G8_SINT,
	    VK_FORMAT_R8G8_SRGB,
	    VK_FORMAT_R8G8B8_UNORM,
	    VK_FORMAT_R8G8B8_SNORM,
	    VK_FORMAT_R8G8B8_USCALED,
	    VK_FORMAT_R8G8B8_SSCALED,
	    VK_FORMAT_R8G8B8_UINT,
	    VK_FORMAT_R8G8B8_SINT,
	    VK_FORMAT_R8G8B8_SRGB,
	    VK_FORMAT_B8G8R8_UNORM,
	    VK_FORMAT_B8G8R8_SNORM,
	    VK_FORMAT_B8G8R8_USCALED,
	    VK_FORMAT_B8G8R8_SSCALED,
	    VK_FORMAT_B8G8R8_UINT,
	    VK_FORMAT_B8G8R8_SINT,
	    VK_FORMAT_B8G8R8_SRGB,
	    VK_FORMAT_R8G8B8A8_UNORM,
	    VK_FORMAT_R8G8B8A8_SNORM,
	    VK_FORMAT_R8G8B8A8_USCALED,
	    VK_FORMAT_R8G8B8A8_SSCALED,
	    VK_FORMAT_R8G8B8A8_UINT,
	    VK_FORMAT_R8G8B8A8_SINT,
	    VK_FORMAT_R8G8B8A8_SRGB,
	    VK_FORMAT_B8G8R8A8_UNORM,
	    VK_FORMAT_B8G8R8A8_SNORM,
	    VK_FORMAT_B8G8R8A8_USCALED,
	    VK_FORMAT_B8G8R8A8_SSCALED,
	    VK_FORMAT_B8G8R8A8_UINT,
	    VK_FORMAT_B8G8R8A8_SINT,
	    VK_FORMAT_B8G8R8A8_SRGB,
	    VK_FORMAT_A8B8G8R8_UNORM_PACK32,
	    VK_FORMAT_A8B8G8R8_SNORM_PACK32,
	    VK_FORMAT_A8B8G8R8_USCALED_PACK32,
	    VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
	    VK_FORMAT_A8B8G8R8_UINT_PACK32,
	    VK_FORMAT_A8B8G8R8_SINT_PACK32,
	    VK_FORMAT_A8B8G8R8_SRGB_PACK32,
	    VK_FORMAT_A2R10G10B10_UNORM_PACK32,
	    VK_FORMAT_A2R10G10B10_SNORM_PACK32,
	    VK_FORMAT_A2R10G10B10_USCALED_PACK32,
	    VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
	    VK_FORMAT_A2R10G10B10_UINT_PACK32,
	    VK_FORMAT_A2R10G10B10_SINT_PACK32,
	    VK_FORMAT_A2B10G10R10_UNORM_PACK32,
	    VK_FORMAT_A2B10G10R10_SNORM_PACK32,
	    VK_FORMAT_A2B10G10R10_USCALED_PACK32,
	    VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
	    VK_FORMAT_A2B10G10R10_UINT_PACK32,
	    VK_FORMAT_A2B10G10R10_SINT_PACK32,
	    VK_FORMAT_R16_UNORM,
	    VK_FORMAT_R16_SNORM,
	    VK_FORMAT_R16_USCALED,
	    VK_FORMAT_R16_SSCALED,
	    VK_FORMAT_R16_UINT,
	    VK_FORMAT_R16_SINT,
	    VK_FORMAT_R16_SFLOAT,
	    VK_FORMAT_R16G16_UNORM,
	    VK_FORMAT_R16G16_SNORM,
	    VK_FORMAT_R16G16_USCALED,
	    VK_FORMAT_R16G16_SSCALED,
	    VK_FORMAT_R16G16_UINT,
	    VK_FORMAT_R16G16_SINT,
	    VK_FORMAT_R16G16_SFLOAT,
	    VK_FORMAT_R16G16B16_UNORM,
	    VK_FORMAT_R16G16B16_SNORM,
	    VK_FORMAT_R16G16B16_USCALED,
	    VK_FORMAT_R16G16B16_SSCALED,
	    VK_FORMAT_R16G16B16_UINT,
	    VK_FORMAT_R16G16B16_SINT,
	    VK_FORMAT_R16G16B16_SFLOAT,
	    VK_FORMAT_R16G16B16A16_UNORM,
	    VK_FORMAT_R16G16B16A16_SNORM,
	    VK_FORMAT_R16G16B16A16_USCALED,
	    VK_FORMAT_R16G16B16A16_SSCALED,
	    VK_FORMAT_R16G16B16A16_UINT,
	    VK_FORMAT_R16G16B16A16_SINT,
	    VK_FORMAT_R16G16B16A16_SFLOAT,
	    VK_FORMAT_R32_UINT,
	    VK_FORMAT_R32_SINT,
	    VK_FORMAT_R32_SFLOAT,
	    VK_FORMAT_R32G32_UINT,
	    VK_FORMAT_R32G32_SINT,
	    VK_FORMAT_R32G32_SFLOAT,
	    VK_FORMAT_R32G32B32_UINT,
	    VK_FORMAT_R32G32B32_SINT,
	    VK_FORMAT_R32G32B32_SFLOAT,
	    VK_FORMAT_R32G32B32A32_UINT,
	    VK_FORMAT_R32G32B32A32_SINT,
	    VK_FORMAT_R32G32B32A32_SFLOAT};

	for (auto format : formats)
	{
		VkFormatProperties format_properties;
		vkGetPhysicalDeviceFormatProperties(a_physical_device, format, &format_properties);

		ror::log_info("Physical device format {} properties:\nLinear tiling:\n{}\nOptimal tiling:\n{}\nBuffer feature:\n{}\n",
		              vk_format_to_string(format).c_str(),
		              vk_format_property_flag(format_properties.linearTilingFeatures),
		              vk_format_property_flag(format_properties.optimalTilingFeatures),
		              vk_format_property_flag(format_properties.bufferFeatures));
	}
}

void PhysicalDevice::init(Instance &a_instance)
{
	set_log_level();

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
		ror::log_warn("Couldn't find suitable discrete physical device, falling back to integrated gpu.");
		assert(gpus.size() >= 1);
		physical_device = gpus[0];
	}

	// Now when we have settled down on a physical_device lets get its properties, note it might not be physical_device_properties local variable so don't use that
	vkGetPhysicalDeviceProperties(physical_device, &this->m_physical_device_properties);

	print_physical_device_properties(this->m_physical_device_properties);
	print_physical_device_formats_properties(physical_device);

	this->set_handle(physical_device);

	this->m_memory_properties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PROPERTIES_2;
	this->m_memory_properties.pNext = nullptr;

	vkGetPhysicalDeviceMemoryProperties2(this->get_handle(), &this->m_memory_properties);

	reset_log_level();
}

void SwapChain::setup_framebuffer(VkDevice a_device, rhi::RenderpassVulkan *a_renderpass)
{
	assert(a_renderpass && "Renderpass is nullptr");
	// All the swapchain images should now have their own framebuffers
	for (size_t image_index = 0; image_index < this->m_swapchain_images.size(); ++image_index)
	{
		assert(this->m_framebuffers[image_index] == nullptr && "Swapchain framebuffer already initialised");

		std::vector<VkImageView> attachments{this->m_swapchain_images_views[image_index]};        // TODO:: Add more, need depth and others

		auto pass_render_targets = a_renderpass->render_targets();
		for (auto &render_target : pass_render_targets)
		{
			auto &texture = render_target.m_target_reference.get();
			if (is_pixel_format_depth_format(texture.format()))        // Add depth from the render pass
				attachments.push_back(texture.platform_image_view());
		}

		this->m_framebuffers[image_index] = vk_create_framebuffer(a_device, a_renderpass->platform_renderpass(0), attachments, this->m_extent);
	}
}

void SwapChain::create(VkPhysicalDevice a_physical_device, VkDevice a_device, VkSurfaceKHR a_surface, VkFormat swapchain_format, VkExtent2D a_swapchain_extent)
{
	// Remember vk_create_swapchain might change swapchain_format which is later used here
	this->m_swapchain = vk_create_swapchain(a_physical_device, a_device, a_surface, swapchain_format, a_swapchain_extent);
	this->m_extent    = a_swapchain_extent;

	this->m_swapchain_images = enumerate_general_property<VkImage, true>(vkGetSwapchainImagesKHR, a_device, this->m_swapchain);

	// Creating image views for all swapchain images
	this->m_swapchain_images_views.resize(this->m_swapchain_images.size());

	for (size_t i = 0; i < this->m_swapchain_images.size(); ++i)
		this->m_swapchain_images_views[i] = vk_create_image_view(a_device, this->m_swapchain_images[i], swapchain_format, 1, 1, VK_IMAGE_ASPECT_COLOR_BIT);

	this->m_format = swapchain_format;
	this->m_framebuffers.resize(this->m_swapchain_images.size(), nullptr);        // To be filled in when we know which renderpass they go into
}

void SwapChain::release(VkDevice a_device)
{
	for (auto &image_view : this->swapchain_images_views())
	{
		vkDestroyImageView(a_device, image_view, cfg::VkAllocator);
		image_view = nullptr;
	}

	for (auto &framebuffer : this->swapchain_framebuffers())
	{
		if (framebuffer)
		{
			vk_destroy_framebuffers(a_device, framebuffer);
			framebuffer = nullptr;
		}
	}

	vk_destroy_swapchain(a_device, this->swapchain());
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

	set_log_level();

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

	reset_log_level();
}

void DeviceVulkan::swapchain_setup(ror::Renderer *a_renderer)
{
	assert(a_renderer && "Renderer is nullptr");
	auto render_passes = a_renderer->current_frame_graph();

	for (auto &pass : render_passes)
	{
		auto &render_supasses = pass.subpasses();
		for (auto &subpass : render_supasses)
		{
			if (subpass.technique() == rhi::RenderpassTechnique::fragment)
			{
				// Update dimensions for those render passes that I think rely on framebuffer/window size. These would usually be swapchains
				if (subpass.type() == rhi::RenderpassType::forward_light)        // NOTE: Only working for forward light at the moment
				{
					this->m_swapchain.setup_framebuffer(this->platform_device(), &pass);
					pass.final(true);
					return;
				}
			}
		}
	}
}

}        // namespace rhi
