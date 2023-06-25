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
#include "rhi/rortypes.hpp"
#include "vulkan/vulkan_core.h"

namespace vk
{
extern VkAllocationCallbacks *VkAllocator;
}

namespace rhi
{
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
		return VkFormat::VK_FORMAT_R16_UINT;        // MTL::IndexTypeUInt16;
	else
		return VkFormat::VK_FORMAT_R32_UINT;
	// return MTL::IndexTypeUInt32;
}

constexpr FORCE_INLINE auto to_vulkan_step_function(rhi::StepFunction a_function)
{
	switch (a_function)
	{
		// clang-format off
	case rhi::StepFunction::vertex:                       return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;//MTL::VertexStepFunctionPerVertex;
	case rhi::StepFunction::constant:                     return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;//MTL::VertexStepFunctionConstant;
	case rhi::StepFunction::instance:                     return VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE;//MTL::VertexStepFunctionPerInstance;
	case rhi::StepFunction::patch:                        return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;//MTL::VertexStepFunctionPerPatch;
	case rhi::StepFunction::patch_control_point:          return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;//MTL::VertexStepFunctionPerPatchControlPoint;
			// clang-format on
	}

	assert(0);
}

constexpr FORCE_INLINE auto to_vulkan_resource_option(rhi::ResourceStorageOption a_mode)
{
	return static_cast<uint32_t>(a_mode);
}

constexpr FORCE_INLINE auto to_vulkan_primitive_topoloy(rhi::PrimitiveTopology a_topology)
{
	return static_cast<VkPrimitiveTopology>(a_topology);
}

constexpr FORCE_INLINE auto to_vulkan_primitive_topoloy_class(rhi::PrimitiveTopology a_topology)
{
	auto topclass = primitve_toplogy_to_class(a_topology);

	return static_cast<VkPrimitiveTopology>(topclass);        // TODO: This is a problem. not sure how to distinguish between class and topology in Vulkan
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

}        // namespace rhi
