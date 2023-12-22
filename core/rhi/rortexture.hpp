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

// #include "foundation/rortypes.hpp"
#include "foundation/rormacros.hpp"
#include "rhi/rorrhi_macros.hpp"

#if defined(ROR_RENDER_TYPE_VULKAN)
#	include "rhi/vulkan/rortexture.hpp"
#elif defined(ROR_RENDER_TYPE_METAL)
#	include "rhi/metal/rortexture.hpp"
#else
#	error "Unsupported buffer platform"
#endif

namespace rhi
{

void              fill_texture_from_memory(uint8_t *a_data, uint32_t a_width, uint32_t a_height, uint32_t a_bytes_per_pixel, rhi::TextureImage &a_texture, bool a_is_hdr = false, const std::string &a_name = "texture_from_memory");
void              read_texture_from_memory(const uint8_t *a_data, size_t a_data_size, rhi::TextureImage &a_texture, bool a_is_hdr = false, const std::string &a_name = "stbi_memory");
FORCE_INLINE void read_texture_from_resource(ror::Resource &a_texture_resource, TextureImage &a_texture, bool a_is_hdr = false);

/**
 * @brief      Generic image loader that will invoke the right decoder based on extension from absolute path
 * @details    Read a texture via absolute file name via resource loading mechanism.
 *             Internally it might use stb or basisu or ktx to load different types of texture images
 * @param      a_absolute_file_name As the name suggests, an absolute file name of the texture to load
 * @return     TextureImage that contains all the mipmaps of the texture and all pixel data
 */
template <TextureTarget _target>
FORCE_INLINE TextureImage read_texture_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);

// FORCE_INLINE TextureImage read_texture_1d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);
FORCE_INLINE TextureImage read_texture_2d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);
// FORCE_INLINE TextureImage read_texture_3d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);
// FORCE_INLINE TextureImage read_texture_cube_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);

}        // namespace rhi

#include "rortexture.hh"
