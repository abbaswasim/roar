// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2021
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

#include "rhi/rortypes.hpp"
#include <cassert>
#include <unordered_map>

namespace rhi
{
uint32_t pixel_format_to_bits(PixelFormat a_pixel_format)
{
	switch (a_pixel_format)
	{
		case PixelFormat::invalid:
		case PixelFormat::a8_uint8_norm:
		case PixelFormat::r8_uint8_norm:
		case PixelFormat::r8_uint8_norm_srgb:
		case PixelFormat::r8_int8_norm:
		case PixelFormat::r8_uint8:
		case PixelFormat::r8_int8:
		case PixelFormat::stencil8_int8:
			return 8;
		case PixelFormat::depth16_uint16_norm:
		case PixelFormat::r16_uint16_norm:
		case PixelFormat::r16_int16_norm:
		case PixelFormat::r16_uint16:
		case PixelFormat::r16_int16:
		case PixelFormat::r16_float16:
		case PixelFormat::r8g8_uint16_norm:
		case PixelFormat::r8g8_uint16_norm_srgb:
		case PixelFormat::r8g8_int16_norm:
		case PixelFormat::r8g8_uint16:
		case PixelFormat::r8g8_int16:
		case PixelFormat::b5g6r5_uint16_norm:
		case PixelFormat::a1b5g5r5_uint16_norm:
		case PixelFormat::a4b4g4r4_uint16_norm:
		case PixelFormat::b5g5r5a1_uint16_norm:
			return 16;
		case PixelFormat::x24_stencil8:
		case PixelFormat::depth32_float32:
		case PixelFormat::depth24_norm_stencil8_uint32:
		case PixelFormat::r32_uint32:
		case PixelFormat::r32_int32:
		case PixelFormat::r32_float32:
		case PixelFormat::r16g16_uint32_norm:
		case PixelFormat::r16g16_int32_norm:
		case PixelFormat::r16g16_uint32:
		case PixelFormat::r16g16_int32:
		case PixelFormat::r16g16_float32:
		case PixelFormat::r8g8b8a8_uint32_norm:
		case PixelFormat::r8g8b8a8_uint32_norm_srgb:
		case PixelFormat::r8g8b8a8_int32_norm:
		case PixelFormat::r8g8b8a8_uint32:
		case PixelFormat::r8g8b8a8_int32:
		case PixelFormat::b8g8r8a8_uint32_norm:
		case PixelFormat::b8g8r8a8_uint32_norm_srgb:
		case PixelFormat::r10g10b10a2_uint32_norm:
		case PixelFormat::r10g10b10a2_uint32:
		case PixelFormat::r11g11b10_float32:
		case PixelFormat::r9g9b9e5_float32:
		case PixelFormat::b10g10r10a2_uint32_norm:
			return 32;
		case PixelFormat::r32g32_uint64:
		case PixelFormat::r32g32_int64:
		case PixelFormat::r32g32_float64:
		case PixelFormat::r16g16b16a16_uint64_norm:
		case PixelFormat::r16g16b16a16_int64_norm:
		case PixelFormat::r16g16b16a16_uint64:
		case PixelFormat::r16g16b16a16_int64:
		case PixelFormat::r16g16b16a16_float64:
			return 64;
		case PixelFormat::r32g32b32a32_uint128:
		case PixelFormat::r32g32b32a32_int128:
		case PixelFormat::r32g32b32a32_float128:
			return 128;
		case PixelFormat::bc1_rgba:
		case PixelFormat::bc1_rgba_srgb:
		case PixelFormat::bc2_rgba:
		case PixelFormat::bc2_rgba_srgb:
		case PixelFormat::bc3_rgba:
		case PixelFormat::bc3_rgba_srgb:
		case PixelFormat::bc4_r_unsigned_norm:
		case PixelFormat::bc4_r_norm:
		case PixelFormat::bc5_rg_unsigned_norm:
		case PixelFormat::bc5_rg_norm:
		case PixelFormat::bc6h_rgb_float:
		case PixelFormat::bc6h_rgbu_float:
		case PixelFormat::bc7_rgba_unsigned_norm:
		case PixelFormat::bc7_rgba_unsinged_norm_srgb:

		case PixelFormat::pvrtc_rgb_2bpp:
		case PixelFormat::pvrtc_rgb_2bpp_srgb:
		case PixelFormat::pvrtc_rgb_4bpp:
		case PixelFormat::pvrtc_rgb_4bpp_srgb:
		case PixelFormat::pvrtc_rgba_2bpp:
		case PixelFormat::pvrtc_rgba_2bpp_srgb:
		case PixelFormat::pvrtc_rgba_4bpp:
		case PixelFormat::pvrtc_rgba_4bpp_srgb:

		case PixelFormat::eac_r11_unsigned_norm:
		case PixelFormat::eac_r11_norm:
		case PixelFormat::eac_r11g11_unsigned_norm:
		case PixelFormat::eac_r11g11_norm:
		case PixelFormat::eac_r8g8b8a8:
		case PixelFormat::eac_r8g8b8a8_srgb:

		case PixelFormat::etc2_r8g8b8:
		case PixelFormat::etc2_r8g8b8_srgb:
		case PixelFormat::etc2_r8g8b8a1:
		case PixelFormat::etc2_r8g8b8a1_srgb:
			assert(0);
			return 128;
		case PixelFormat::astc_4x4_srgb:
		case PixelFormat::astc_5x4_srgb:
		case PixelFormat::astc_5x5_srgb:
		case PixelFormat::astc_6x5_srgb:
		case PixelFormat::astc_6x6_srgb:
		case PixelFormat::astc_8x5_srgb:
		case PixelFormat::astc_8x6_srgb:
		case PixelFormat::astc_8x8_srgb:
		case PixelFormat::astc_10x5_srgb:
		case PixelFormat::astc_10x6_srgb:
		case PixelFormat::astc_10x8_srgb:
		case PixelFormat::astc_10x10_srgb:
		case PixelFormat::astc_12x10_srgb:
		case PixelFormat::astc_12x12_srgb:
		case PixelFormat::astc_4x4_ldr:
		case PixelFormat::astc_5x4_ldr:
		case PixelFormat::astc_5x5_ldr:
		case PixelFormat::astc_6x5_ldr:
		case PixelFormat::astc_6x6_ldr:
		case PixelFormat::astc_8x5_ldr:
		case PixelFormat::astc_8x6_ldr:
		case PixelFormat::astc_8x8_ldr:
		case PixelFormat::astc_10x5_ldr:
		case PixelFormat::astc_10x6_ldr:
		case PixelFormat::astc_10x8_ldr:
		case PixelFormat::astc_10x10_ldr:
		case PixelFormat::astc_12x10_ldr:
		case PixelFormat::astc_12x12_ldr:
		case PixelFormat::astc_4x4_hdr:
		case PixelFormat::astc_5x4_hdr:
		case PixelFormat::astc_5x5_hdr:
		case PixelFormat::astc_6x5_hdr:
		case PixelFormat::astc_6x6_hdr:
		case PixelFormat::astc_8x5_hdr:
		case PixelFormat::astc_8x6_hdr:
		case PixelFormat::astc_8x8_hdr:
		case PixelFormat::astc_10x5_hdr:
		case PixelFormat::astc_10x6_hdr:
		case PixelFormat::astc_10x8_hdr:
		case PixelFormat::astc_10x10_hdr:
		case PixelFormat::astc_12x10_hdr:
		case PixelFormat::astc_12x12_hdr:
			return 128;
		case PixelFormat::gbgr422:
		case PixelFormat::bgrg422:
		case PixelFormat::b10g10r10a10_xr:
		case PixelFormat::b10g10r10a10_xr_srgb:
		case PixelFormat::b10g10r10_xr:
		case PixelFormat::b10g10r10_xr_srgb:
			assert(0);
			return 128;
		case PixelFormat::x32_stencil8:
		case PixelFormat::depth32_float32_stencil8_int8:
			return 40;
	}

	return 0;
}

/*
Advice on how to make sure glsl and c++ play well along

Follow these rules:
  Stop using vec3. This includes 3-column matrices as well.
  Only use mat4 matrices.
  In C++, set the alignas of each vec2 type to 8 and each vec4 type to 16. Also, set mat4's alignment to 16.
  In C++, for any struct you intend to use in a UBO/SSBO, alignas it to 16.
  For std140 layouts, never use arrays of anything other than structs or vec4-equivalent types.
  In C++, for any array member of a struct, alignas it to 16.

*/

uint32_t vertex_format_to_bytes(VertexFormat a_vertex_format)
{
	switch (a_vertex_format)
	{
		case VertexFormat::invalid:
			return 0;
		case VertexFormat::struct_1:
			return 0;
		case VertexFormat::int8_1:
		case VertexFormat::int8_1_norm:
		case VertexFormat::uint8_1:
		case VertexFormat::uint8_1_norm:
		case VertexFormat::uint8_custom:
			return 1;
		case VertexFormat::int8_2:
		case VertexFormat::int8_2_norm:
		case VertexFormat::uint8_2:
		case VertexFormat::uint8_2_norm:
		case VertexFormat::int16_1:
		case VertexFormat::int16_1_norm:
		case VertexFormat::uint16_1:
		case VertexFormat::uint16_1_norm:
		case VertexFormat::half16_1:
		case VertexFormat::uint16_custom:
			return 2;
		case VertexFormat::int8_3:
		case VertexFormat::int8_3_norm:
		case VertexFormat::uint8_3:
		case VertexFormat::uint8_3_norm:
			return 3;
		case VertexFormat::bool32_1:
		case VertexFormat::int8_4:
		case VertexFormat::int8_4_norm:
		case VertexFormat::uint8_4:
		case VertexFormat::uint8_4_norm:
		case VertexFormat::int16_2:
		case VertexFormat::int16_2_norm:
		case VertexFormat::uint16_2:
		case VertexFormat::uint16_2_norm:
		case VertexFormat::int32_1:
		case VertexFormat::uint32_1:
		case VertexFormat::half16_2:
		case VertexFormat::float32_1:
		case VertexFormat::int1010102_norm:
		case VertexFormat::uint1010102_norm:
		case VertexFormat::int101111_norm:
		case VertexFormat::uint101111_norm:
		case VertexFormat::uint8_4_norm_bgra:
		case VertexFormat::uint32_custom:
		case VertexFormat::float32_custom:
			return 4;
		case VertexFormat::int16_3:
		case VertexFormat::int16_3_norm:
		case VertexFormat::uint16_3:
		case VertexFormat::uint16_3_norm:
		case VertexFormat::half16_3:
			return 6;
		case VertexFormat::int16_4:
		case VertexFormat::int16_4_norm:
		case VertexFormat::uint16_4:
		case VertexFormat::uint16_4_norm:
		case VertexFormat::bool32_2:
		case VertexFormat::int32_2:
		case VertexFormat::uint32_2:
		case VertexFormat::uint64_1:
		case VertexFormat::half16_4:
		case VertexFormat::float32_2:
		case VertexFormat::float64_1:
			return 8;
		case VertexFormat::bool32_3:
		case VertexFormat::int32_3:
		case VertexFormat::uint32_3:
		case VertexFormat::float32_3:
			return 12;
		case VertexFormat::bool32_4:
		case VertexFormat::int32_4:
		case VertexFormat::uint32_4:
		case VertexFormat::uint64_2:
		case VertexFormat::float32_4:
		case VertexFormat::float64_2:
		case VertexFormat::float32_2x2:
			return 16;
		case VertexFormat::float64_3:
		case VertexFormat::uint64_3:
		case VertexFormat::float32_2x3:
		case VertexFormat::float32_3x2:
			return 24;
		case VertexFormat::float64_4:
		case VertexFormat::uint64_4:
		case VertexFormat::float32_2x4:
		case VertexFormat::float32_4x2:
		case VertexFormat::float64_2x2:
			return 32;
		case VertexFormat::float32_3x3:
			return 36;
		case VertexFormat::float32_3x4:
		case VertexFormat::float32_4x3:
		case VertexFormat::float64_2x3:
		case VertexFormat::float64_3x2:
			return 48;
		case VertexFormat::float32_4x4:
		case VertexFormat::float64_2x4:
		case VertexFormat::float64_4x2:
			return 64;
		case VertexFormat::float64_3x3:
			return 72;
		case VertexFormat::float64_3x4:
		case VertexFormat::float64_4x3:
			return 96;
		case VertexFormat::float64_4x4:
			return 128;
	}

	return 0;
}

uint32_t vertex_format_to_location(VertexFormat a_vertex_format)
{
	switch (a_vertex_format)
	{
		case VertexFormat::invalid:
			return 0;
		case VertexFormat::struct_1:
		case VertexFormat::bool32_1:
		case VertexFormat::bool32_2:
		case VertexFormat::bool32_3:
		case VertexFormat::bool32_4:
		case VertexFormat::int8_1:
		case VertexFormat::int8_2:
		case VertexFormat::int8_3:
		case VertexFormat::int8_4:
		case VertexFormat::int16_1:
		case VertexFormat::int16_2:
		case VertexFormat::int16_3:
		case VertexFormat::int16_4:
		case VertexFormat::int32_1:
		case VertexFormat::int32_2:
		case VertexFormat::int32_3:
		case VertexFormat::int32_4:
		case VertexFormat::half16_1:
		case VertexFormat::half16_2:
		case VertexFormat::half16_3:
		case VertexFormat::half16_4:
		case VertexFormat::float32_1:
		case VertexFormat::float32_2:
		case VertexFormat::float32_3:
		case VertexFormat::float32_4:
		case VertexFormat::uint8_1:
		case VertexFormat::uint8_2:
		case VertexFormat::uint8_3:
		case VertexFormat::uint8_4:
		case VertexFormat::uint16_1:
		case VertexFormat::uint16_2:
		case VertexFormat::uint16_3:
		case VertexFormat::uint16_4:
		case VertexFormat::uint32_1:
		case VertexFormat::uint32_2:
		case VertexFormat::uint32_3:
		case VertexFormat::uint32_4:
		case VertexFormat::int8_1_norm:
		case VertexFormat::int8_2_norm:
		case VertexFormat::int8_3_norm:
		case VertexFormat::int8_4_norm:
		case VertexFormat::int16_1_norm:
		case VertexFormat::int16_2_norm:
		case VertexFormat::int16_3_norm:
		case VertexFormat::int16_4_norm:
		case VertexFormat::uint8_1_norm:
		case VertexFormat::uint8_2_norm:
		case VertexFormat::uint8_3_norm:
		case VertexFormat::uint8_4_norm:
		case VertexFormat::uint16_1_norm:
		case VertexFormat::uint16_2_norm:
		case VertexFormat::uint16_3_norm:
		case VertexFormat::uint16_4_norm:
		case VertexFormat::int1010102_norm:
		case VertexFormat::uint1010102_norm:
		case VertexFormat::int101111_norm:
		case VertexFormat::uint101111_norm:
		case VertexFormat::uint8_4_norm_bgra:
		case VertexFormat::uint8_custom:          // To be handled differently
		case VertexFormat::uint16_custom:         // To be handled differently
		case VertexFormat::uint32_custom:         // To be handled differently
		case VertexFormat::float32_custom:        // To be handled differently
		case VertexFormat::uint64_1:
		case VertexFormat::uint64_2:
		case VertexFormat::float64_1:
		case VertexFormat::float64_2:
			return 1;        // TODO: This is only true if vertex attribute is used in Vulkan Vertex shader
		case VertexFormat::uint64_3:
		case VertexFormat::uint64_4:
		case VertexFormat::float64_3:
		case VertexFormat::float64_4:
		case VertexFormat::float32_2x2:
		case VertexFormat::float32_2x3:
		case VertexFormat::float32_2x4:
			return 2;
		case VertexFormat::float32_3x3:
		case VertexFormat::float32_3x2:
		case VertexFormat::float32_3x4:
			return 3;
		case VertexFormat::float32_4x2:
		case VertexFormat::float32_4x3:
		case VertexFormat::float32_4x4:
			return 4;
		case VertexFormat::float64_2x2:
		case VertexFormat::float64_2x3:
		case VertexFormat::float64_3x2:
		case VertexFormat::float64_2x4:
		case VertexFormat::float64_4x2:
		case VertexFormat::float64_3x3:
		case VertexFormat::float64_3x4:
		case VertexFormat::float64_4x3:
		case VertexFormat::float64_4x4:
			return 8;        // TODO: This is a guess, confirm
	}

	return 0;
}

// clang-format off
BufferSemantic get_format_semantic(const std::string &a_format)
{
#define item(_string){#_string, BufferSemantic::_string}
#define item_value(_enum)

	static std::unordered_map<std::string, BufferSemantic> string_to_shader_semantic
	{
		describe_buffer_semantics(item)
	};

#undef item
#undef item_value

	return string_to_shader_semantic[a_format];
}

std::string get_format_semantic(const BufferSemantic &a_semantic)
{
#define item(_string){BufferSemantic::_string, #_string}
#define item_value(_enum)

	static std::unordered_map<BufferSemantic, std::string> semantic_to_string
	{
		describe_buffer_semantics(item)
	};

#undef item
#undef item_value

	return semantic_to_string[a_semantic];
}

constexpr bool has_semantic(uint64_t a_type, BufferSemantic a_semantic)
{
	return ((a_type & ror::enum_to_type_cast(a_semantic)) == ror::enum_to_type_cast(a_semantic));
}

// clang-format on
}        // namespace rhi
