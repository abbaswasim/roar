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

#include "rhi/rortexture.hpp"

#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace rhi
{

void read_texture_from_memory(const uint8_t *a_data, size_t a_data_size, rhi::TextureImage &a_texture)
{
	int32_t w = 0, h = 0, bpp = 0;
	auto   *new_data = stbi_load_from_memory(a_data, ror::static_cast_safe<int32_t>(a_data_size), &w, &h, &bpp, 0);        // Final argument = 0 means get real bpp

	a_texture.push_empty_mip();
	a_texture.format(rhi::PixelFormat::r8g8b8a8_uint32_norm_srgb);        // TODO: How do I read this via STB or gltf?
	a_texture.reset(new_data, static_cast<uint64_t>(w * h * bpp));        // a_texture now owns the new_data pointer returned by stbi
	a_texture.width(static_cast<uint32_t>(w));
	a_texture.height(static_cast<uint32_t>(h));
	a_texture.depth(1u);
	a_texture.bytes_per_pixel(static_cast<uint32_t>(bpp));
	a_texture.name("stbi_memory");
}

}        // namespace rhi
