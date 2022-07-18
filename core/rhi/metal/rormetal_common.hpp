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
#include "rhi/crtp_interfaces/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <Metal/MTLTexture.hpp>

// NOTE: Don't include any Metal headers here like <Metal/MTLDevice.hpp> etc
// All metal headers needs to be included in the rormetal_common.cpp file so we
// Generate implementations only once. If these are included here they gets compiled
// without implementation of the functions and then we get linker errors

namespace mtl
{
constexpr FORCE_INLINE MTL::PixelFormat to_metal_pixelformat(rhi::PixelFormat a_pixelformat)
{
	return static_cast<MTL::PixelFormat>(a_pixelformat);
}

constexpr FORCE_INLINE MTL::TextureType to_metal_texture_target(rhi::TextureTarget a_target)
{
	switch (a_target)
	{
		case rhi::TextureTarget::texture_1D:
			return MTL::TextureType::TextureType1D;
		case rhi::TextureTarget::texture_1D_array:
			return MTL::TextureType::TextureType1DArray;
		case rhi::TextureTarget::texture_2D:
			return MTL::TextureType::TextureType2D;
		case rhi::TextureTarget::texture_2D_array:
			return MTL::TextureType::TextureType2DArray;
		case rhi::TextureTarget::texture_2D_MS:
			return MTL::TextureType::TextureType2DMultisample;
		case rhi::TextureTarget::texture_cube:
			return MTL::TextureType::TextureTypeCube;
		case rhi::TextureTarget::texture_cube_array:
			return MTL::TextureType::TextureTypeCubeArray;
		case rhi::TextureTarget::texture_3D:
			return MTL::TextureType::TextureType3D;
		case rhi::TextureTarget::texture_2D_MS_array:
			return MTL::TextureType::TextureType2DMultisampleArray;
	}
}
}        // namespace mtl
