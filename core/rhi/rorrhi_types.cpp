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

#include "rhi/rorrhi_types.hpp"

namespace rhi
{
uint32_t pixel_format_to_bytes(PixelFormat a_pixel_format)
{
	switch (a_pixel_format)
	{
		case PixelFormat::r8_int8:
		case PixelFormat::r8_uint8:
		case PixelFormat::r8_int8_norm:
		case PixelFormat::r8_uint8_norm:
		case PixelFormat::a8_uint8_norm:
		case PixelFormat::stencil8_int8:
			return 8;
		case PixelFormat::r16_int16:
		case PixelFormat::r16_uint16:
		case PixelFormat::r16_int16_norm:
		case PixelFormat::r16_uint16_norm:
		case PixelFormat::r16_float16:
		case PixelFormat::r5g6b5_uint16_norm:
		case PixelFormat::r5g5b5a1_uint16_norm:
		case PixelFormat::r8g8_int16:
		case PixelFormat::r8g8_uint16:
		case PixelFormat::r8g8_int16_norm:
		case PixelFormat::r8g8_uint16_norm:
		case PixelFormat::depth16_uint16_norm:
			return 16;
		case PixelFormat::r32_int32:
		case PixelFormat::r32_uint32:
		case PixelFormat::r32_float32:
		case PixelFormat::r16g16_int32:
		case PixelFormat::r16g16_uint32:
		case PixelFormat::r16g16_int32_norm:
		case PixelFormat::r16g16_uint32_norm:
		case PixelFormat::r16g16_float32:
		case PixelFormat::r8g8b8a8_int32:
		case PixelFormat::r8g8b8a8_uint32:
		case PixelFormat::r8g8b8a8_int32_norm:
		case PixelFormat::r8g8b8a8_uint32_norm:
		case PixelFormat::r8g8b8a8_uint32_norm_srgb:
		case PixelFormat::b8g8r8a8_uint32_norm:
		case PixelFormat::b8g8r8a8_uint32_norm_srgb:
		case PixelFormat::r10g10b10a2_uint32:
		case PixelFormat::r10g10b10a2_uint32_norm:
		case PixelFormat::r11g11b10_float32:
		case PixelFormat::r9g9b9e5_float32:
		case PixelFormat::b10g10r10a2_uint32_norm:
		case PixelFormat::depth24_stencil8_uint32_norm:
		case PixelFormat::depth32_float32:
		case PixelFormat::depth32_stencil8_float32:
			return 32;
		case PixelFormat::r32g32_int64:
		case PixelFormat::r32g32_uint64:
		case PixelFormat::r32g32_float64:
		case PixelFormat::r16g16b16a16_int64:
		case PixelFormat::r16g16b16a16_uint64:
		case PixelFormat::r16g16b16a16_int64_norm:
		case PixelFormat::r16g16b16a16_uint64_norm:
		case PixelFormat::r16g16b16a16_float64:
			return 64;
		case PixelFormat::r32g32b32a32_int128:
		case PixelFormat::r32g32b32a32_uint128:
		case PixelFormat::r32g32b32a32_float128:
			return 128;
	}

	return 0;
}

uint32_t vertex_format_to_bytes(VertexFormat a_vertex_format)
{
	switch (a_vertex_format)
	{
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
		case VertexFormat::int32_2:
		case VertexFormat::uint32_2:
		case VertexFormat::uint64_1:
		case VertexFormat::half16_4:
		case VertexFormat::float32_2:
		case VertexFormat::float64_1:
			return 8;
		case VertexFormat::int32_3:
		case VertexFormat::uint32_3:
		case VertexFormat::float32_3:
			return 12;
		case VertexFormat::int32_4:
		case VertexFormat::uint32_4:
		case VertexFormat::uint64_2:
		case VertexFormat::float32_4:
		case VertexFormat::float64_2:
			return 16;
		case VertexFormat::float64_3:
		case VertexFormat::uint64_3:
			return 24;
		case VertexFormat::float64_4:
		case VertexFormat::uint64_4:
			return 32;
		case VertexFormat::float32_9:
			return 36;
		case VertexFormat::float32_16:
			return 64;
	}

	return 0;
}

uint32_t vertex_format_to_location(VertexFormat a_vertex_format)
{
	switch (a_vertex_format)
	{
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
		case VertexFormat::uint16_custom:          // To be handled differently
		case VertexFormat::uint32_custom:         // To be handled differently
		case VertexFormat::float32_custom:        // To be handled differently
			return 1;
		case VertexFormat::uint64_1:
		case VertexFormat::uint64_2:
		case VertexFormat::float64_1:
		case VertexFormat::float64_2:
		case VertexFormat::uint64_3:
		case VertexFormat::uint64_4:
		case VertexFormat::float64_3:
		case VertexFormat::float64_4:
			return 1;        // TODO: This is only true if vertex attribute is used in Vulkan Vertex shader
		case VertexFormat::float32_9:
			return 3;        // TODO: This is a guess, confirm
		case VertexFormat::float32_16:
			return 4;        // TODO: This is a guess, confirm
	}

	return 0;
}

// clang-format off
ShaderSemantic get_format_shader_semantic(const std::string &a_format)
{

#define item(_string){#_string, ShaderSemantic::_string}
#define item_value(_enum)

static std::unordered_map<std::string, ShaderSemantic> string_to_shader_semantic
{
	describe_shader_semantics(item)
};

#undef item
#undef item_value
	// clang-format on

	return string_to_shader_semantic[a_format];
}

}        // namespace rhi
