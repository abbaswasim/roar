// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "rhi/crtp_interfaces/rortexture.hpp"

namespace rhi
{

uint32_t calculate_texture_mip_levels(uint32_t a_width, uint32_t a_height, uint32_t a_depth)
{
	return static_cast<uint32_t>(std::floor(std::log2(std::max(std::max(a_width, a_height), a_depth))) + 1u);
}

size_t calculate_texture_size(uint32_t a_width, uint32_t a_height, uint32_t a_depth, rhi::PixelFormat a_format, bool a_mipmapped, bool a_cubemap, bool a_array)
{
	uint32_t level_count{a_mipmapped ? calculate_texture_mip_levels(a_width, a_height, a_depth) : 1};
	uint32_t size{0};

	if (a_cubemap && !a_array)
	{
		assert(a_depth == 1 && "3D cubemap invalid texture");
	}

	if (a_depth != 1 && !a_array)
	{
		assert(!a_cubemap && "3D cubemap invalid texture");
	}

	uint32_t depth{a_array ? 1 : a_depth};

	for (size_t level = 0; level < level_count; ++level)
	{
		uint32_t mip_width  = std::max(1u, a_width >> level);
		uint32_t mip_height = std::max(1u, a_height >> level);
		uint32_t mip_depth  = std::max(1u, depth >> level);

		size += mip_width * mip_height * mip_depth;
	}

	if (a_cubemap)
		size *= 6;

	if (a_array)
		size *= a_depth;

	return size * pixel_format_to_bytes(a_format);
}

}        // namespace rhi
