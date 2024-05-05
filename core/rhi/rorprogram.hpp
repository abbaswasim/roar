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

#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortexture.hpp"
#include <unordered_map>
#include <variant>
#include <vector>

namespace rhi
{
// This is a horrible hack on top of Vulkan Descriptor sets

// clang-format off
using descriptor_variant     = std::variant<const rhi::ShaderBuffer *,
											const rhi::TextureImage *,
											std::pair<const rhi::TextureImage *,
													  const rhi::TextureSampler *>>;

using descriptor_data_pair   = std::pair<                               // Pair of Data and Binding
                                  descriptor_variant,                   // Data
                                  uint32_t>;                            // Binding
using descriptor_data_vector = std::vector<descriptor_data_pair>;       // List of pairs of shader_buffer/combined_image_samplers with their binding

using descriptor_update_type = std::unordered_map<uint32_t,             // This is set id, for each set data will be provided
	                                              descriptor_data_vector>;

// clang-format on
}        // namespace rhi

#if defined(ROR_RENDER_TYPE_VULKAN)
#	include "rhi/vulkan/rorprogram.hpp"
#elif defined(ROR_RENDER_TYPE_METAL)
#	include "rhi/metal/rorprogram.hpp"
#else
#	error "Unsupported buffer platform"
#endif

namespace rhi
{}        // namespace rhi

#include "rhi/rorprogram.hh"
