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

#include "foundation/rorutilities.hpp"
#include "rhi/rortypes.hpp"
#include <cassert>
#include <unordered_map>

namespace rhi
{
/*
constexpr uint32_t pixel_format_to_bits(PixelFormat a_pixel_format)
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
*/

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

/*
constexpr uint32_t vertex_format_to_bytes(VertexFormat a_vertex_format)
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

constexpr uint32_t vertex_format_to_location(VertexFormat a_vertex_format)
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
*/

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
// clang-format on

rhi::PixelFormat string_to_pixel_format(const std::string &a_format)
{
	// clang-format off
	if      (a_format == "a8_uint8_norm"                 ) return rhi::PixelFormat::a8_uint8_norm;
	else if (a_format == "r8_uint8_norm"                 ) return rhi::PixelFormat::r8_uint8_norm;
	else if (a_format == "r8_uint8_norm_srgb"            ) return rhi::PixelFormat::r8_uint8_norm_srgb;
	else if (a_format == "r8_int8_norm"                  ) return rhi::PixelFormat::r8_int8_norm;
	else if (a_format == "r8_uint8"                      ) return rhi::PixelFormat::r8_uint8;
	else if (a_format == "r8_int8"                       ) return rhi::PixelFormat::r8_int8;
	else if (a_format == "r16_uint16_norm"               ) return rhi::PixelFormat::r16_uint16_norm;
	else if (a_format == "r16_int16_norm"                ) return rhi::PixelFormat::r16_int16_norm;
	else if (a_format == "r16_uint16"                    ) return rhi::PixelFormat::r16_uint16;
	else if (a_format == "r16_int16"                     ) return rhi::PixelFormat::r16_int16;
	else if (a_format == "r16_float16"                   ) return rhi::PixelFormat::r16_float16;
	else if (a_format == "r8g8_uint16_norm"              ) return rhi::PixelFormat::r8g8_uint16_norm;
	else if (a_format == "r8g8_uint16_norm_srgb"         ) return rhi::PixelFormat::r8g8_uint16_norm_srgb;
	else if (a_format == "r8g8_int16_norm"               ) return rhi::PixelFormat::r8g8_int16_norm;
	else if (a_format == "r8g8_uint16"                   ) return rhi::PixelFormat::r8g8_uint16;
	else if (a_format == "r8g8_int16"                    ) return rhi::PixelFormat::r8g8_int16;
	else if (a_format == "b5g6r5_uint16_norm"            ) return rhi::PixelFormat::b5g6r5_uint16_norm;
	else if (a_format == "a1b5g5r5_uint16_norm"          ) return rhi::PixelFormat::a1b5g5r5_uint16_norm;
	else if (a_format == "a4b4g4r4_uint16_norm"          ) return rhi::PixelFormat::a4b4g4r4_uint16_norm;
	else if (a_format == "b5g5r5a1_uint16_norm"          ) return rhi::PixelFormat::b5g5r5a1_uint16_norm;
	else if (a_format == "r32_uint32"                    ) return rhi::PixelFormat::r32_uint32;
	else if (a_format == "r32_int32"                     ) return rhi::PixelFormat::r32_int32;
	else if (a_format == "r32_float32"                   ) return rhi::PixelFormat::r32_float32;
	else if (a_format == "r16g16_uint32_norm"            ) return rhi::PixelFormat::r16g16_uint32_norm;
	else if (a_format == "r16g16_int32_norm"             ) return rhi::PixelFormat::r16g16_int32_norm;
	else if (a_format == "r16g16_uint32"                 ) return rhi::PixelFormat::r16g16_uint32;
	else if (a_format == "r16g16_int32"                  ) return rhi::PixelFormat::r16g16_int32;
	else if (a_format == "r16g16_float32"                ) return rhi::PixelFormat::r16g16_float32;
	else if (a_format == "r8g8b8a8_uint32_norm"          ) return rhi::PixelFormat::r8g8b8a8_uint32_norm;
	else if (a_format == "r8g8b8a8_uint32_norm_srgb"     ) return rhi::PixelFormat::r8g8b8a8_uint32_norm_srgb;
	else if (a_format == "r8g8b8a8_int32_norm"           ) return rhi::PixelFormat::r8g8b8a8_int32_norm;
	else if (a_format == "r8g8b8a8_uint32"               ) return rhi::PixelFormat::r8g8b8a8_uint32;
	else if (a_format == "r8g8b8a8_int32"                ) return rhi::PixelFormat::r8g8b8a8_int32;
	else if (a_format == "b8g8r8a8_uint32_norm"          ) return rhi::PixelFormat::b8g8r8a8_uint32_norm;
	else if (a_format == "b8g8r8a8_uint32_norm_srgb"     ) return rhi::PixelFormat::b8g8r8a8_uint32_norm_srgb;
	else if (a_format == "r10g10b10a2_uint32_norm"       ) return rhi::PixelFormat::r10g10b10a2_uint32_norm;
	else if (a_format == "r10g10b10a2_uint32"            ) return rhi::PixelFormat::r10g10b10a2_uint32;
	else if (a_format == "r11g11b10_float32"             ) return rhi::PixelFormat::r11g11b10_float32;
	else if (a_format == "r9g9b9e5_float32"              ) return rhi::PixelFormat::r9g9b9e5_float32;
	else if (a_format == "b10g10r10a2_uint32_norm"       ) return rhi::PixelFormat::b10g10r10a2_uint32_norm;
	else if (a_format == "r32g32_uint64"                 ) return rhi::PixelFormat::r32g32_uint64;
	else if (a_format == "r32g32_int64"                  ) return rhi::PixelFormat::r32g32_int64;
	else if (a_format == "r32g32_float64"                ) return rhi::PixelFormat::r32g32_float64;
	else if (a_format == "r16g16b16a16_uint64_norm"      ) return rhi::PixelFormat::r16g16b16a16_uint64_norm;
	else if (a_format == "r16g16b16a16_int64_norm"       ) return rhi::PixelFormat::r16g16b16a16_int64_norm;
	else if (a_format == "r16g16b16a16_uint64"           ) return rhi::PixelFormat::r16g16b16a16_uint64;
	else if (a_format == "r16g16b16a16_int64"            ) return rhi::PixelFormat::r16g16b16a16_int64;
	else if (a_format == "r16g16b16a16_float64"          ) return rhi::PixelFormat::r16g16b16a16_float64;
	else if (a_format == "r32g32b32a32_uint128"          ) return rhi::PixelFormat::r32g32b32a32_uint128;
	else if (a_format == "r32g32b32a32_int128"           ) return rhi::PixelFormat::r32g32b32a32_int128;
	else if (a_format == "r32g32b32a32_float128"         ) return rhi::PixelFormat::r32g32b32a32_float128;
	else if (a_format == "bc1_rgba"                      ) return rhi::PixelFormat::bc1_rgba;
	else if (a_format == "bc1_rgba_srgb"                 ) return rhi::PixelFormat::bc1_rgba_srgb;
	else if (a_format == "bc2_rgba"                      ) return rhi::PixelFormat::bc2_rgba;
	else if (a_format == "bc2_rgba_srgb"                 ) return rhi::PixelFormat::bc2_rgba_srgb;
	else if (a_format == "bc3_rgba"                      ) return rhi::PixelFormat::bc3_rgba;
	else if (a_format == "bc3_rgba_srgb"                 ) return rhi::PixelFormat::bc3_rgba_srgb;
	else if (a_format == "bc4_r_unsigned_norm"           ) return rhi::PixelFormat::bc4_r_unsigned_norm;
	else if (a_format == "bc4_r_norm"                    ) return rhi::PixelFormat::bc4_r_norm;
	else if (a_format == "bc5_rg_unsigned_norm"          ) return rhi::PixelFormat::bc5_rg_unsigned_norm;
	else if (a_format == "bc5_rg_norm"                   ) return rhi::PixelFormat::bc5_rg_norm;
	else if (a_format == "bc6h_rgb_float"                ) return rhi::PixelFormat::bc6h_rgb_float;
	else if (a_format == "bc6h_rgbu_float"               ) return rhi::PixelFormat::bc6h_rgbu_float;
	else if (a_format == "bc7_rgba_unsigned_norm"        ) return rhi::PixelFormat::bc7_rgba_unsigned_norm;
	else if (a_format == "bc7_rgba_unsinged_norm_srgb"   ) return rhi::PixelFormat::bc7_rgba_unsinged_norm_srgb;
	else if (a_format == "pvrtc_rgb_2bpp"                ) return rhi::PixelFormat::pvrtc_rgb_2bpp;
	else if (a_format == "pvrtc_rgb_2bpp_srgb"           ) return rhi::PixelFormat::pvrtc_rgb_2bpp_srgb;
	else if (a_format == "pvrtc_rgb_4bpp"                ) return rhi::PixelFormat::pvrtc_rgb_4bpp;
	else if (a_format == "pvrtc_rgb_4bpp_srgb"           ) return rhi::PixelFormat::pvrtc_rgb_4bpp_srgb;
	else if (a_format == "pvrtc_rgba_2bpp"               ) return rhi::PixelFormat::pvrtc_rgba_2bpp;
	else if (a_format == "pvrtc_rgba_2bpp_srgb"          ) return rhi::PixelFormat::pvrtc_rgba_2bpp_srgb;
	else if (a_format == "pvrtc_rgba_4bpp"               ) return rhi::PixelFormat::pvrtc_rgba_4bpp;
	else if (a_format == "pvrtc_rgba_4bpp_srgb"          ) return rhi::PixelFormat::pvrtc_rgba_4bpp_srgb;
	else if (a_format == "eac_r11_unsigned_norm"         ) return rhi::PixelFormat::eac_r11_unsigned_norm;
	else if (a_format == "eac_r11_norm"                  ) return rhi::PixelFormat::eac_r11_norm;
	else if (a_format == "eac_r11g11_unsigned_norm"      ) return rhi::PixelFormat::eac_r11g11_unsigned_norm;
	else if (a_format == "eac_r11g11_norm"               ) return rhi::PixelFormat::eac_r11g11_norm;
	else if (a_format == "eac_r8g8b8a8"                  ) return rhi::PixelFormat::eac_r8g8b8a8;
	else if (a_format == "eac_r8g8b8a8_srgb"             ) return rhi::PixelFormat::eac_r8g8b8a8_srgb;
	else if (a_format == "etc2_r8g8b8"                   ) return rhi::PixelFormat::etc2_r8g8b8;
	else if (a_format == "etc2_r8g8b8_srgb"              ) return rhi::PixelFormat::etc2_r8g8b8_srgb;
	else if (a_format == "etc2_r8g8b8a1"                 ) return rhi::PixelFormat::etc2_r8g8b8a1;
	else if (a_format == "etc2_r8g8b8a1_srgb"            ) return rhi::PixelFormat::etc2_r8g8b8a1_srgb;
	else if (a_format == "astc_4x4_srgb"                 ) return rhi::PixelFormat::astc_4x4_srgb;
	else if (a_format == "astc_5x4_srgb"                 ) return rhi::PixelFormat::astc_5x4_srgb;
	else if (a_format == "astc_5x5_srgb"                 ) return rhi::PixelFormat::astc_5x5_srgb;
	else if (a_format == "astc_6x5_srgb"                 ) return rhi::PixelFormat::astc_6x5_srgb;
	else if (a_format == "astc_6x6_srgb"                 ) return rhi::PixelFormat::astc_6x6_srgb;
	else if (a_format == "astc_8x5_srgb"                 ) return rhi::PixelFormat::astc_8x5_srgb;
	else if (a_format == "astc_8x6_srgb"                 ) return rhi::PixelFormat::astc_8x6_srgb;
	else if (a_format == "astc_8x8_srgb"                 ) return rhi::PixelFormat::astc_8x8_srgb;
	else if (a_format == "astc_10x5_srgb"                ) return rhi::PixelFormat::astc_10x5_srgb;
	else if (a_format == "astc_10x6_srgb"                ) return rhi::PixelFormat::astc_10x6_srgb;
	else if (a_format == "astc_10x8_srgb"                ) return rhi::PixelFormat::astc_10x8_srgb;
	else if (a_format == "astc_10x10_srgb"               ) return rhi::PixelFormat::astc_10x10_srgb;
	else if (a_format == "astc_12x10_srgb"               ) return rhi::PixelFormat::astc_12x10_srgb;
	else if (a_format == "astc_12x12_srgb"               ) return rhi::PixelFormat::astc_12x12_srgb;
	else if (a_format == "astc_4x4_ldr"                  ) return rhi::PixelFormat::astc_4x4_ldr;
	else if (a_format == "astc_5x4_ldr"                  ) return rhi::PixelFormat::astc_5x4_ldr;
	else if (a_format == "astc_5x5_ldr"                  ) return rhi::PixelFormat::astc_5x5_ldr;
	else if (a_format == "astc_6x5_ldr"                  ) return rhi::PixelFormat::astc_6x5_ldr;
	else if (a_format == "astc_6x6_ldr"                  ) return rhi::PixelFormat::astc_6x6_ldr;
	else if (a_format == "astc_8x5_ldr"                  ) return rhi::PixelFormat::astc_8x5_ldr;
	else if (a_format == "astc_8x6_ldr"                  ) return rhi::PixelFormat::astc_8x6_ldr;
	else if (a_format == "astc_8x8_ldr"                  ) return rhi::PixelFormat::astc_8x8_ldr;
	else if (a_format == "astc_10x5_ldr"                 ) return rhi::PixelFormat::astc_10x5_ldr;
	else if (a_format == "astc_10x6_ldr"                 ) return rhi::PixelFormat::astc_10x6_ldr;
	else if (a_format == "astc_10x8_ldr"                 ) return rhi::PixelFormat::astc_10x8_ldr;
	else if (a_format == "astc_10x10_ldr"                ) return rhi::PixelFormat::astc_10x10_ldr;
	else if (a_format == "astc_12x10_ldr"                ) return rhi::PixelFormat::astc_12x10_ldr;
	else if (a_format == "astc_12x12_ldr"                ) return rhi::PixelFormat::astc_12x12_ldr;
	else if (a_format == "astc_4x4_hdr"                  ) return rhi::PixelFormat::astc_4x4_hdr;
	else if (a_format == "astc_5x4_hdr"                  ) return rhi::PixelFormat::astc_5x4_hdr;
	else if (a_format == "astc_5x5_hdr"                  ) return rhi::PixelFormat::astc_5x5_hdr;
	else if (a_format == "astc_6x5_hdr"                  ) return rhi::PixelFormat::astc_6x5_hdr;
	else if (a_format == "astc_6x6_hdr"                  ) return rhi::PixelFormat::astc_6x6_hdr;
	else if (a_format == "astc_8x5_hdr"                  ) return rhi::PixelFormat::astc_8x5_hdr;
	else if (a_format == "astc_8x6_hdr"                  ) return rhi::PixelFormat::astc_8x6_hdr;
	else if (a_format == "astc_8x8_hdr"                  ) return rhi::PixelFormat::astc_8x8_hdr;
	else if (a_format == "astc_10x5_hdr"                 ) return rhi::PixelFormat::astc_10x5_hdr;
	else if (a_format == "astc_10x6_hdr"                 ) return rhi::PixelFormat::astc_10x6_hdr;
	else if (a_format == "astc_10x8_hdr"                 ) return rhi::PixelFormat::astc_10x8_hdr;
	else if (a_format == "astc_10x10_hdr"                ) return rhi::PixelFormat::astc_10x10_hdr;
	else if (a_format == "astc_12x10_hdr"                ) return rhi::PixelFormat::astc_12x10_hdr;
	else if (a_format == "astc_12x12_hdr"                ) return rhi::PixelFormat::astc_12x12_hdr;
	else if (a_format == "gbgr422"                       ) return rhi::PixelFormat::gbgr422;
	else if (a_format == "bgrg422"                       ) return rhi::PixelFormat::bgrg422;
	else if (a_format == "depth16_uint16_norm"           ) return rhi::PixelFormat::depth16_uint16_norm;
	else if (a_format == "depth32_float32"               ) return rhi::PixelFormat::depth32_float32;
	else if (a_format == "stencil8_int8"                 ) return rhi::PixelFormat::stencil8_int8;
	else if (a_format == "depth24_norm_stencil8_uint32"  ) return rhi::PixelFormat::depth24_norm_stencil8_uint32;
	else if (a_format == "depth32_float32_stencil8_int8" ) return rhi::PixelFormat::depth32_float32_stencil8_int8;
	else if (a_format == "x32_stencil8"                  ) return rhi::PixelFormat::x32_stencil8;
	else if (a_format == "x24_stencil8"                  ) return rhi::PixelFormat::x24_stencil8;
	else if (a_format == "b10g10r10a10_xr"               ) return rhi::PixelFormat::b10g10r10a10_xr;
	else if (a_format == "b10g10r10a10_xr_srgb"          ) return rhi::PixelFormat::b10g10r10a10_xr_srgb;
	else if (a_format == "b10g10r10_xr"                  ) return rhi::PixelFormat::b10g10r10_xr;
	else if (a_format == "b10g10r10_xr_srgb"             ) return rhi::PixelFormat::b10g10r10_xr_srgb;
	// clang-format on

	assert(0);        // Not a recongnised format
	return rhi::PixelFormat::r8g8b8a8_uint32_norm_srgb;
}

rhi::VertexFormat string_to_vertex_format(const std::string &a_format)
{
	// clang-format off
	if      (a_format == "invalid"              ) return rhi::VertexFormat::invalid;
	else if (a_format == "int8_1"               ) return rhi::VertexFormat::int8_1;
	else if (a_format == "int8_2"               ) return rhi::VertexFormat::int8_2;
	else if (a_format == "int8_3"               ) return rhi::VertexFormat::int8_3;
	else if (a_format == "int8_4"               ) return rhi::VertexFormat::int8_4;
	else if (a_format == "int16_1"              ) return rhi::VertexFormat::int16_1;
	else if (a_format == "int16_2"              ) return rhi::VertexFormat::int16_2;
	else if (a_format == "int16_3"              ) return rhi::VertexFormat::int16_3;
	else if (a_format == "int16_4"              ) return rhi::VertexFormat::int16_4;
	else if (a_format == "int32_1"              ) return rhi::VertexFormat::int32_1;
	else if (a_format == "int32_2"              ) return rhi::VertexFormat::int32_2;
	else if (a_format == "int32_3"              ) return rhi::VertexFormat::int32_3;
	else if (a_format == "int32_4"              ) return rhi::VertexFormat::int32_4;
	else if (a_format == "half16_1"             ) return rhi::VertexFormat::half16_1;
	else if (a_format == "half16_2"             ) return rhi::VertexFormat::half16_2;
	else if (a_format == "half16_3"             ) return rhi::VertexFormat::half16_3;
	else if (a_format == "half16_4"             ) return rhi::VertexFormat::half16_4;
	else if (a_format == "float32_1"            ) return rhi::VertexFormat::float32_1;
	else if (a_format == "float32_2"            ) return rhi::VertexFormat::float32_2;
	else if (a_format == "float32_3"            ) return rhi::VertexFormat::float32_3;
	else if (a_format == "float32_4"            ) return rhi::VertexFormat::float32_4;
	else if (a_format == "uint8_1"              ) return rhi::VertexFormat::uint8_1;
	else if (a_format == "uint8_2"              ) return rhi::VertexFormat::uint8_2;
	else if (a_format == "uint8_3"              ) return rhi::VertexFormat::uint8_3;
	else if (a_format == "uint8_4"              ) return rhi::VertexFormat::uint8_4;
	else if (a_format == "uint16_1"             ) return rhi::VertexFormat::uint16_1;
	else if (a_format == "uint16_2"             ) return rhi::VertexFormat::uint16_2;
	else if (a_format == "uint16_3"             ) return rhi::VertexFormat::uint16_3;
	else if (a_format == "uint16_4"             ) return rhi::VertexFormat::uint16_4;
	else if (a_format == "uint32_1"             ) return rhi::VertexFormat::uint32_1;
	else if (a_format == "uint32_2"             ) return rhi::VertexFormat::uint32_2;
	else if (a_format == "uint32_3"             ) return rhi::VertexFormat::uint32_3;
	else if (a_format == "uint32_4"             ) return rhi::VertexFormat::uint32_4;
	else if (a_format == "int8_1_norm"          ) return rhi::VertexFormat::int8_1_norm;
	else if (a_format == "int8_2_norm"          ) return rhi::VertexFormat::int8_2_norm;
	else if (a_format == "int8_3_norm"          ) return rhi::VertexFormat::int8_3_norm;
	else if (a_format == "int8_4_norm"          ) return rhi::VertexFormat::int8_4_norm;
	else if (a_format == "int16_1_norm"         ) return rhi::VertexFormat::int16_1_norm;
	else if (a_format == "int16_2_norm"         ) return rhi::VertexFormat::int16_2_norm;
	else if (a_format == "int16_3_norm"         ) return rhi::VertexFormat::int16_3_norm;
	else if (a_format == "int16_4_norm"         ) return rhi::VertexFormat::int16_4_norm;
	else if (a_format == "uint8_1_norm"         ) return rhi::VertexFormat::uint8_1_norm;
	else if (a_format == "uint8_2_norm"         ) return rhi::VertexFormat::uint8_2_norm;
	else if (a_format == "uint8_3_norm"         ) return rhi::VertexFormat::uint8_3_norm;
	else if (a_format == "uint8_4_norm"         ) return rhi::VertexFormat::uint8_4_norm;
	else if (a_format == "uint16_1_norm"        ) return rhi::VertexFormat::uint16_1_norm;
	else if (a_format == "uint16_2_norm"        ) return rhi::VertexFormat::uint16_2_norm;
	else if (a_format == "uint16_3_norm"        ) return rhi::VertexFormat::uint16_3_norm;
	else if (a_format == "uint16_4_norm"        ) return rhi::VertexFormat::uint16_4_norm;
	else if (a_format == "int1010102_norm"      ) return rhi::VertexFormat::int1010102_norm;
	else if (a_format == "uint1010102_norm"     ) return rhi::VertexFormat::uint1010102_norm;
	else if (a_format == "uint8_4_norm_bgra"    ) return rhi::VertexFormat::uint8_4_norm_bgra;
	else if (a_format == "bool32_1"             ) return rhi::VertexFormat::bool32_1;
	else if (a_format == "bool32_2"             ) return rhi::VertexFormat::bool32_2;
	else if (a_format == "bool32_3"             ) return rhi::VertexFormat::bool32_3;
	else if (a_format == "bool32_4"             ) return rhi::VertexFormat::bool32_4;
	else if (a_format == "uint64_1"             ) return rhi::VertexFormat::uint64_1;
	else if (a_format == "uint64_2"             ) return rhi::VertexFormat::uint64_2;
	else if (a_format == "uint64_3"             ) return rhi::VertexFormat::uint64_3;
	else if (a_format == "uint64_4"             ) return rhi::VertexFormat::uint64_4;
	else if (a_format == "float64_1"            ) return rhi::VertexFormat::float64_1;
	else if (a_format == "float64_2"            ) return rhi::VertexFormat::float64_2;
	else if (a_format == "float64_3"            ) return rhi::VertexFormat::float64_3;
	else if (a_format == "float64_4"            ) return rhi::VertexFormat::float64_4;
	else if (a_format == "float32_2x2"          ) return rhi::VertexFormat::float32_2x2;
	else if (a_format == "float32_2x3"          ) return rhi::VertexFormat::float32_2x3;
	else if (a_format == "float32_2x4"          ) return rhi::VertexFormat::float32_2x4;
	else if (a_format == "float32_3x2"          ) return rhi::VertexFormat::float32_3x2;
	else if (a_format == "float32_3x3"          ) return rhi::VertexFormat::float32_3x3;
	else if (a_format == "float32_3x4"          ) return rhi::VertexFormat::float32_3x4;
	else if (a_format == "float32_4x2"          ) return rhi::VertexFormat::float32_4x2;
	else if (a_format == "float32_4x3"          ) return rhi::VertexFormat::float32_4x3;
	else if (a_format == "float32_4x4"          ) return rhi::VertexFormat::float32_4x4;
	else if (a_format == "float64_2x2"          ) return rhi::VertexFormat::float64_2x2;
	else if (a_format == "float64_2x3"          ) return rhi::VertexFormat::float64_2x3;
	else if (a_format == "float64_2x4"          ) return rhi::VertexFormat::float64_2x4;
	else if (a_format == "float64_3x2"          ) return rhi::VertexFormat::float64_3x2;
	else if (a_format == "float64_3x3"          ) return rhi::VertexFormat::float64_3x3;
	else if (a_format == "float64_3x4"          ) return rhi::VertexFormat::float64_3x4;
	else if (a_format == "float64_4x2"          ) return rhi::VertexFormat::float64_4x2;
	else if (a_format == "float64_4x3"          ) return rhi::VertexFormat::float64_4x3;
	else if (a_format == "float64_4x4"          ) return rhi::VertexFormat::float64_4x4;
	else if (a_format == "uint8_custom"         ) return rhi::VertexFormat::uint8_custom;
	else if (a_format == "uint16_custom"        ) return rhi::VertexFormat::uint16_custom;
	else if (a_format == "uint32_custom"        ) return rhi::VertexFormat::uint32_custom;
	else if (a_format == "float32_custom"       ) return rhi::VertexFormat::float32_custom;
	else if (a_format == "int101111_norm"       ) return rhi::VertexFormat::int101111_norm;
	else if (a_format == "uint101111_norm"      ) return rhi::VertexFormat::uint101111_norm;
	else if (a_format == "struct_1"             ) return rhi::VertexFormat::struct_1;
	else if (a_format == "struct_0"             ) return rhi::VertexFormat::struct_0;
	// clang-format on

	assert(0);        // Not a recongnised format
	return rhi::VertexFormat::float32_4;
}

rhi::PrimitiveWinding string_to_primitive_winding(const std::string &a_winding)
{
	auto winding{rhi::PrimitiveWinding::counter_clockwise};

	if (a_winding == "clockwise")
		winding = rhi::PrimitiveWinding::clockwise;
	else
	{
		assert(a_winding == "counter_clockwise");
	}

	return winding;
}

rhi::PrimitiveCullMode string_to_cull_mode(const std::string &a_cull_mode)
{
	auto cull_mode{rhi::PrimitiveCullMode::back};

	if (a_cull_mode == "front")
		cull_mode = rhi::PrimitiveCullMode::front;
	else if (a_cull_mode == "none")
		cull_mode = rhi::PrimitiveCullMode::none;
	else
	{
		assert(a_cull_mode == "back" && "Invalid cull_mode");
	}

	return cull_mode;
}

rhi::ShaderType string_to_shader_type(const std::string &a_extension)
{
	auto type{rhi::ShaderType::vertex};
	if (a_extension == ".vert")
		type = rhi::ShaderType::vertex;
	else if (a_extension == ".frag")
		type = rhi::ShaderType::fragment;
	else if (a_extension == ".comp")
		type = rhi::ShaderType::compute;
	else if (a_extension == ".kern")
		type = rhi::ShaderType::compute;
	else if (a_extension == ".mesh")
		type = rhi::ShaderType::mesh;
	else
		assert(0 && "Implement more types");

	return type;
}

const std::string shader_type_to_string(rhi::ShaderType a_type)
{
	// clang-format off
	switch (a_type)
	{
		case rhi::ShaderType::mesh:               return "mesh";
		case rhi::ShaderType::task:               return "task";
		case rhi::ShaderType::vertex:             return "vertex";
		case rhi::ShaderType::compute:            return "compute";
		case rhi::ShaderType::fragment:           return "fragment";
		case rhi::ShaderType::ray_miss:           return "ray_miss";
		case rhi::ShaderType::ray_any_hit:        return "ray_any_hit";
		case rhi::ShaderType::ray_closest_hit:    return "ray_closest_hit";
		case rhi::ShaderType::ray_intersection:   return "ray_intersection";
		case rhi::ShaderType::ray_generation:     return "ray_generation";
		case rhi::ShaderType::none:               return "none";
		case rhi::ShaderType::tile:               return "tile";

	}
	// clang-format on

	assert(0 && "Implement more types");

	return "";
}

rhi::ShaderStage string_to_shader_stage(const std::string &a_stage)
{
	auto stage{rhi::ShaderStage::vertex};
	if (a_stage == "vertex")
		stage = rhi::ShaderStage::vertex;
	else if (a_stage == "fragment")
		stage = rhi::ShaderStage::fragment;
	else if (a_stage == "compute")
		stage = rhi::ShaderStage::compute;
	else if (a_stage == "vertex_fragment")
		stage = rhi::ShaderStage::vertex_fragment;
	else if (a_stage == "compute_vertex")
		stage = rhi::ShaderStage::compute_vertex;
	else if (a_stage == "compute_fragment")
		stage = rhi::ShaderStage::compute_fragment;
	else if (a_stage == "compute_vertex_fragment")
		stage = rhi::ShaderStage::compute_vertex_fragment;
	else
		assert(0 && "Implement more types");

	return stage;
}

bool is_pixel_format_depth_format(const rhi::PixelFormat a_format)
{
	if (a_format == PixelFormat::depth16_uint16_norm ||
	    a_format == PixelFormat::depth24_norm_stencil8_uint32 ||
	    a_format == PixelFormat::depth32_float32_stencil8_int8 ||
	    a_format == PixelFormat::depth32_float32 ||
	    a_format == PixelFormat::x32_stencil8 ||
	    a_format == PixelFormat::x24_stencil8)
		return true;

	return false;
}

std::string renderpass_type_to_string(const rhi::RenderpassType &a_type)
{
	// clang-format off
	switch (a_type)
	{
		case rhi::RenderpassType::lut: 					return "lut";
		case rhi::RenderpassType::main: 				return "main";
		case rhi::RenderpassType::depth: 				return "depth";
		case rhi::RenderpassType::shadow: 				return "shadow";
		case rhi::RenderpassType::light_bin: 			return "light_bin";
		case rhi::RenderpassType::reflection: 			return "reflection";
		case rhi::RenderpassType::refraction: 			return "refraction";
		case rhi::RenderpassType::pre_process: 		    return "pre_process";
		case rhi::RenderpassType::post_process: 		return "post_process";
		case rhi::RenderpassType::tone_mapping: 		return "tone_mapping";
		case rhi::RenderpassType::forward_light: 		return "forward_light";
		case rhi::RenderpassType::node_transform: 		return "node_transform";
		case rhi::RenderpassType::deferred_gbuffer: 	return "deferred_gbuffer";
		case rhi::RenderpassType::reflection_probes: 	return "reflection_probes";
		case rhi::RenderpassType::image_based_light: 	return "image_based_light";
		case rhi::RenderpassType::ambient_occlusion: 	return "ambient_occlusion";
		case rhi::RenderpassType::skeletal_transform: 	return "skeletal_transform";
	    case rhi::RenderpassType::deferred_clustered:   return "deferred_clustered";
	}
	// clang-format on

	assert(0);
	return "";
}

}        // namespace rhi
