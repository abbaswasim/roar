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
#include <cassert>

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

	// TODO: Add more astc and other compressed types
	astc_4x4_srgb_block,
	bc7_srgb_block
};

enum class VertexFormat : uint32_t
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

// Generic name when semantics are not clear
using Format = VertexFormat;

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

struct BufferRange
{
	uint32_t m_location;        // Which buffer is returned
	uint64_t m_offset;          // Which offset I can use
	uint64_t m_length;          // How many bytes are available
};

// If new types are required make sure to change in rormodel.hpp
enum class PrimitiveTopology
{
	points,
	lines,
	lines_loop,
	lines_strip,
	triangles,
	triangles_strip,
	triangles_fan,
};

enum class TextureFilter
{
	nearest,
	linear,
};

enum class TextureAddressMode
{
	clamp_to_edge,
	mirrored_repeat,
	repeat
};

enum class BlendMode
{
	opaque,
	mask,
	blend
};

enum class MaterialModel
{
	lit,
	eye,
	hair,
	cloth,
	water,
	unlit,
	// twosided,           // This is handled in the material itself
	clearcoat,
	subsurface
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
//	...
//  ...
// };

// All this gross dance is required to not have to define the enum values and strings twice, basically defines the one above
// Position is usually x,y,z translation
// Rotation is usually a quaternion
// Scale is usually a non-uniform scale in x, y and z axis
// Transformation is usually a matrix4
// clang-format off
#define describe_buffer_semantics(item)                                 \
	item(vertex_position)                 item_value(= 0),              \
	item(vertex_texture_coord_0)          item_value(= 1ul << 0),       \
	item(vertex_texture_coord_1)          item_value(= 1ul << 1),       \
	item(vertex_texture_coord_2)          item_value(= 1ul << 2),       \
	item(vertex_normal)                   item_value(= 1ul << 3),       \
	item(vertex_bent_normal)              item_value(= 1ul << 4),       \
	item(vertex_tangent)                  item_value(= 1ul << 5),       \
	item(vertex_color_0)                  item_value(= 1ul << 6),       \
	item(vertex_color_1)                  item_value(= 1ul << 7),       \
	item(vertex_bone_id_0)                item_value(= 1ul << 8),       \
	item(vertex_bone_id_1)                item_value(= 1ul << 9),       \
	item(vertex_weight_0)                 item_value(= 1ul << 10),      \
	item(vertex_weight_1)                 item_value(= 1ul << 11),      \
	item(vertex_morph_target)             item_value(= 1ul << 12),      \
	item(vertex_morph_weight)             item_value(= 1ul << 13),      \
	item(vertex_index)                    item_value(= 1ul << 14),      \
	item(instance_translation)            item_value(= 1ul << 15),      \
	item(instance_rotation)               item_value(= 1ul << 16),      \
	item(instance_scale)                  item_value(= 1ul << 17),      \
	item(instance_transform)              item_value(= 1ul << 18),      \
	item(mesh_index)                      item_value(= 1ul << 19),      \
	item(meshlet_data)                    item_value(= 1ul << 20),      \
	item(drawcall_data)                   item_value(= 1ul << 21),      \
	item(texture_image_data)              item_value(= 1ul << 22),      \
	item(texture_sampler_data)            item_value(= 1ul << 23),      \
	item(texture_data)                    item_value(= 1ul << 24),      \
	item(material_data)                   item_value(= 1ul << 25),      \
	item(mesh_data)                       item_value(= 1ul << 26),      \
	item(skin_data)                       item_value(= 1ul << 27),      \
	item(node_data)                       item_value(= 1ul << 28),      \
	item(node_suplementory_data)          item_value(= 1ul << 29),      \
	item(animation_sampler_data)          item_value(= 1ul << 30),      \
	item(animation_channel_data)          item_value(= 1ul << 31),      \
	item(animation_input_data)            item_value(= 1ul << 32),      \
	item(animation_output_data)           item_value(= 1ul << 33),      \
	item(animation_data)                  item_value(= 1ul << 34),      \
	item(bounding_box_data)               item_value(= 1ul << 35),      \
	item(index_data)                      item_value(= 1ul << 36),      \
	item(joint_index_data)                item_value(= 1ul << 37),      \
	item(instance_trs)                    item_value(= 1ul << 38),      \
	item(gltf_bin_buffer)                 item_value(= 1ul << 39),      \
	item(custom)                          item_value(= 1ul << 63)
// clang-format on
#define item(_enum) _enum
#define item_value(_enum) _enum

enum class BufferSemantic : uint64_t
{
	describe_buffer_semantics(item)
};

#undef item
#undef item_value

#define item(_enum) _enum
#define item_value(_enum)

enum class AttributeIndex : uint32_t
{
	describe_buffer_semantics(item)
};

#undef item
#undef item_value

uint32_t       pixel_format_to_bytes(PixelFormat a_pixel_format);
uint32_t       vertex_format_to_bytes(VertexFormat a_vertex_format);
uint32_t       vertex_format_to_location(VertexFormat a_vertex_format);
BufferSemantic get_format_shader_semantic(const std::string &a_format);

const auto format_to_bytes = vertex_format_to_bytes;

// Specialisation of these are defined in different places
template <typename _semantic_type>
constexpr rhi::BufferSemantic get_format_shader_semantic()
{
	assert(0 && "Can't use generic version of this function, define specialisation first");
	return rhi::BufferSemantic::custom;
}

#define define_type_to_shader_semantics(type) \
	template <>                               \
	constexpr rhi::BufferSemantic get_format_shader_semantic<type>()
/*
// To use the above for an Animation type do
define_type_to_shader_semantics(Animation)
{
	return BufferSemantic::animation_data;
}
*/
}        // namespace rhi
