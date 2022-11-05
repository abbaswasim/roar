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
}

constexpr FORCE_INLINE auto to_metal_resource_option(rhi::ResourceStorageOption a_mode)
{
	return static_cast<MTL::ResourceOptions>(a_mode);
}

}        // namespace rhi
