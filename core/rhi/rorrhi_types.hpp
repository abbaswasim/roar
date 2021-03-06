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

#pragma once

#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "roar.hpp"
#include <unordered_map>

namespace rhi
{
enum class PixelFormat
{
	r8_int8,
	r8_uint8,
	r8_int8_norm,
	r8_uint8_norm,
	a8_uint8_norm,

	r16_int16,
	r16_uint16,
	r16_int16_norm,
	r16_uint16_norm,
	r16_float16,

	r5g6b5_uint16_norm,
	r5g5b5a1_uint16_norm,

	r8g8_int16,
	r8g8_uint16,
	r8g8_int16_norm,
	r8g8_uint16_norm,

	r32_int32,
	r32_uint32,

	r32_float32,

	r16g16_int32,
	r16g16_uint32,
	r16g16_int32_norm,
	r16g16_uint32_norm,

	r16g16_float32,

	r8g8b8a8_int32,
	r8g8b8a8_uint32,
	r8g8b8a8_int32_norm,
	r8g8b8a8_uint32_norm,
	r8g8b8a8_uint32_norm_srgb,

	b8g8r8a8_uint32_norm,
	b8g8r8a8_uint32_norm_srgb,

	r10g10b10a2_uint32,
	r10g10b10a2_uint32_norm,

	r11g11b10_float32,
	r9g9b9e5_float32,
	b10g10r10a2_uint32_norm,

	r32g32_int64,
	r32g32_uint64,
	r32g32_float64,

	r16g16b16a16_int64,
	r16g16b16a16_uint64,
	r16g16b16a16_int64_norm,
	r16g16b16a16_uint64_norm,
	r16g16b16a16_float64,

	r32g32b32a32_int128,
	r32g32b32a32_uint128,
	r32g32b32a32_float128,

	stencil8_int8,
	depth16_uint16_norm,
	depth24_stencil8_uint32_norm,
	depth32_float32,
	depth32_stencil8_float32,        //! Not sure if this is supported anywhere?
};

enum class VertexFormat
{
	int8_1,
	int8_2,
	int8_3,
	int8_4,

	int16_1,
	int16_2,
	int16_3,
	int16_4,

	int32_1,
	int32_2,
	int32_3,
	int32_4,

	half16_1,
	half16_2,
	half16_3,
	half16_4,

	float32_1,
	float32_2,
	float32_3,
	float32_4,
	float32_9,
	float32_16,

	float64_1,
	float64_2,
	float64_3,
	float64_4,

	uint8_1,
	uint8_2,
	uint8_3,
	uint8_4,

	uint16_1,
	uint16_2,
	uint16_3,
	uint16_4,

	uint32_1,
	uint32_2,
	uint32_3,
	uint32_4,

	uint64_1,
	uint64_2,
	uint64_3,
	uint64_4,

	int8_1_norm,
	int8_2_norm,
	int8_3_norm,
	int8_4_norm,

	int16_1_norm,
	int16_2_norm,
	int16_3_norm,
	int16_4_norm,

	uint8_1_norm,
	uint8_2_norm,
	uint8_3_norm,
	uint8_4_norm,

	uint16_1_norm,
	uint16_2_norm,
	uint16_3_norm,
	uint16_4_norm,

	int1010102_norm,
	uint1010102_norm,
	int101111_norm,
	uint101111_norm,
	uint8_4_norm_bgra,

	uint8_custom,
	uint16_custom,
	uint32_custom,
	float32_custom
};

/*
  The following types are not used widely so not supported
  details https://github.com/KhronosGroup/glTF/issues/832
  int32_1_norm,
  int32_2_norm,
  int32_3_norm,
  int32_4_norm,
  uint32_1_norm,
  uint32_2_norm,
  uint32_3_norm,
  uint32_4_norm,
*/

// Defines what's in the buffer
enum class BufferPackSemantic
{
	positions,
	non_positions,
	indices,
	instances
};

enum class StepFunction
{
	constant,
	vertex,
	instance,
	patch,
	patch_control_point
};

enum class PrimitiveTopology
{
	point,
	line,
	lineStrip,
	triangle,
	triangleStrip
};

struct BufferRange
{
	uint32_t m_location;        // Which buffer is returned
	uint64_t m_offset;          // Which offset I can use
	uint64_t m_length;          // How many bytes are available
};

// enum class ShaderSemantic : uint32_t
// {
//	vertex_position        = 1 << 0,
//	vertex_texture_coord_0 = 1 << 1,
//	vertex_texture_coord_1 = 1 << 2,
//	vertex_texture_coord_2 = 1 << 3,
//	vertex_normal          = 1 << 4,
//	vertex_tangent         = 1 << 5,
//	vertex_color           = 1 << 6,
//	vertex_bone_id         = 1 << 7,
//	vertex_weight          = 1 << 8,
//	instance_translation   = 1 << 9,         // Usually x,y,z translation
//	instance_rotation      = 1 << 10,        // Usually a quaternion
//	instance_scale         = 1 << 11,        // Usually a non-uniform scale in x, y and z axis
//	instance_transform     = 1 << 12,        // Usually a matrix4
//	vertex_index           = 1 << 13,
//	mesh_index             = 1 << 14,
//	meshlet_data           = 1 << 15,
//	drawcall_data          = 1 << 16,
//	custom                 = 1 << 17
// };

// All this gross dance is required to not have to define the enum values and strings twice, basically defines the one above
// Position is usually x,y,z translation
// Rotation is usually a quaternion
// Scale is usually a non-uniform scale in x, y and z axis
// Transformation is usually a matrix4
// clang-format off
#define describe_shader_semantics(item)                 \
	item(vertex_position)        item_value(= 0),       \
	item(vertex_texture_coord_0) item_value(= 1 << 0),  \
	item(vertex_texture_coord_1) item_value(= 1 << 1),  \
	item(vertex_texture_coord_2) item_value(= 1 << 2),  \
	item(vertex_normal)          item_value(= 1 << 3),  \
	item(vertex_bent_normal)     item_value(= 1 << 4),  \
	item(vertex_tangent)         item_value(= 1 << 5),  \
	item(vertex_color)           item_value(= 1 << 6),  \
	item(vertex_bone_id)         item_value(= 1 << 7),  \
	item(vertex_weight)          item_value(= 1 << 8),  \
	item(vertex_morph_target)    item_value(= 1 << 9),  \
	item(vertex_morph_weight)    item_value(= 1 << 10), \
	item(instance_translation)   item_value(= 1 << 11), \
	item(instance_rotation)      item_value(= 1 << 12), \
	item(instance_scale)         item_value(= 1 << 13), \
	item(instance_transform)     item_value(= 1 << 14), \
	item(vertex_index)           item_value(= 1 << 15), \
	item(mesh_index)             item_value(= 1 << 16), \
	item(meshlet_data)           item_value(= 1 << 17), \
	item(drawcall_data)          item_value(= 1 << 18), \
	item(custom)                 item_value(= 1 << 19)
// clang-format on
#define item(_enum) _enum
#define item_value(_enum) _enum

enum class ShaderSemantic : uint32_t
{
	describe_shader_semantics(item)
};

#undef item
#undef item_value

uint32_t       pixel_format_to_bytes(PixelFormat a_pixel_format);
uint32_t       vertex_format_to_bytes(VertexFormat a_vertex_format);
uint32_t       vertex_format_to_location(VertexFormat a_vertex_format);
ShaderSemantic get_format_shader_semantic(const std::string &a_format);

}        // namespace rhi
