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

#include <Metal/MTLDepthStencil.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <Metal/MTLRenderPipeline.hpp>
#include <Metal/MTLResource.hpp>
#include <Metal/MTLSampler.hpp>
#include <Metal/MTLStageInputOutputDescriptor.hpp>
#include <Metal/MTLTexture.hpp>
#include <Metal/MTLVertexDescriptor.hpp>

// NOTE: Don't include any Metal headers here like <Metal/MTLDevice.hpp> etc
// All metal headers needs to be included in the rormetal_common.cpp file so we
// Generate implementations only once. If these are included here they gets compiled
// without implementation of the functions and then we get linker errors

namespace rhi
{
constexpr FORCE_INLINE auto to_metal_pixelformat(rhi::PixelFormat a_pixelformat)
{
	return static_cast<MTL::PixelFormat>(a_pixelformat);
}

constexpr FORCE_INLINE auto to_metal_vertexformat(rhi::VertexFormat a_vertexformat)
{
	assert(a_vertexformat < rhi::VertexFormat::bool32_1 && "Unsupported vertex format");
	return static_cast<MTL::VertexFormat>(a_vertexformat);
}

constexpr FORCE_INLINE auto to_metal_indexformat(rhi::Format a_format)
{
	assert((a_format == rhi::Format::uint32_1 || a_format == rhi::Format::uint16_1) && "Unsupported metal index format");

	if (a_format == rhi::Format::uint16_1)
		return MTL::IndexTypeUInt16;
	else
		return MTL::IndexTypeUInt32;
}

constexpr FORCE_INLINE auto to_metal_step_function(rhi::StepFunction a_function)
{
	switch (a_function)
	{
	// clang-format off
		case rhi::StepFunction::vertex:                       return MTL::VertexStepFunctionPerVertex;
	    case rhi::StepFunction::constant:                     return MTL::VertexStepFunctionConstant;
    	case rhi::StepFunction::instance:                     return MTL::VertexStepFunctionPerInstance;
    	case rhi::StepFunction::patch:                        return MTL::VertexStepFunctionPerPatch;
    	case rhi::StepFunction::patch_control_point:          return MTL::VertexStepFunctionPerPatchControlPoint;
	// clang-format on
	}

	assert(0);
}

constexpr FORCE_INLINE auto to_metal_resource_option(rhi::ResourceStorageOption a_mode)
{
	return static_cast<MTL::ResourceOptions>(a_mode);
}

constexpr FORCE_INLINE auto to_metal_primitive_topoloy(rhi::PrimitiveTopology a_topology)
{
	return static_cast<MTL::PrimitiveType>(a_topology);
}

constexpr FORCE_INLINE auto to_metal_primitive_topoloy_class(rhi::PrimitiveTopology a_topology)
{
	auto topclass = primitve_toplogy_to_class(a_topology);

	return static_cast<MTL::PrimitiveTopologyClass>(topclass);
}

constexpr FORCE_INLINE auto to_metal_cull_mode(rhi::PrimitiveCullMode a_cull_mode)
{
	assert(a_cull_mode != rhi::PrimitiveCullMode::front_back && "Metal does not support front_back cull mode");
	return static_cast<MTL::CullMode>(a_cull_mode);
}

constexpr FORCE_INLINE auto to_metal_winding(rhi::PrimitiveWinding a_winding)
{
	return static_cast<MTL::Winding>(a_winding);
}

constexpr FORCE_INLINE auto to_metal_triangle_fill_mode(rhi::TriangleFillMode a_fill_mode)
{
	return static_cast<MTL::TriangleFillMode>(a_fill_mode);
}

constexpr FORCE_INLINE auto to_metal_depth_compare_function(rhi::DepthCompareFunction a_compare_function)
{
	return static_cast<MTL::CompareFunction>(a_compare_function);
}

}        // namespace rhi
