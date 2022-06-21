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

#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rormaterial.hpp"
#include "graphics/rormodel.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortypes.hpp"
#include "shader_system/rorshader_system.hpp"
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace rhi
{

std::string format_to_glsl_type(VertexFormat a_format)
{
	// bool
	// int
	// uint
	// float
	// double

	// bvecn:
	// ivecn:
	// uvecn:
	// vecn:
	// dvecn:

	// mat3x3
	// mat3x4
	// mat4x4

	// clang-format off
	switch (a_format)
	{
		case VertexFormat::struct_1:			return " ";
		case VertexFormat::bool32_1:			return " bool";
		case VertexFormat::bool32_2:			return " bvec2";
		case VertexFormat::bool32_3:			return " bvec3";
		case VertexFormat::bool32_4:			return " bvec4";
		case VertexFormat::int8_1:				return " int";
		case VertexFormat::int8_2:				return " ivec2";
		case VertexFormat::int8_3:				return " ivec3";
		case VertexFormat::int8_4:				return " ivec4";
		case VertexFormat::int16_1:				return " int";
		case VertexFormat::int16_2:				return " ivec2";
		case VertexFormat::int16_3:				return " ivec3";
		case VertexFormat::int16_4:				return " ivec4";
		case VertexFormat::int32_1:				return " int";
		case VertexFormat::int32_2:				return " ivec2";
		case VertexFormat::int32_3:				return " ivec3";
		case VertexFormat::int32_4:				return " ivec4";
		case VertexFormat::half16_1:			return " int";
		case VertexFormat::half16_2:			return " ivec2";
		case VertexFormat::half16_3:			return " ivec3";
		case VertexFormat::half16_4:			return " ivec4";

		case VertexFormat::float32_1:			return " float";
		case VertexFormat::float32_2:			return " vec2";
		case VertexFormat::float32_3:			return " vec3";
		case VertexFormat::float32_4:			return " vec4";
		case VertexFormat::float32_2x2:			return " mat2x2";
		case VertexFormat::float32_2x3:			return " mat2x3";
		case VertexFormat::float32_2x4:			return " mat2x4";
		case VertexFormat::float32_3x2:			return " mat3x2";
		case VertexFormat::float32_3x3:			return " mat3x3";
		case VertexFormat::float32_3x4:			return " mat3x4";
		case VertexFormat::float32_4x2:			return " mat4x2";
		case VertexFormat::float32_4x3:			return " mat4x3";
		case VertexFormat::float32_4x4:			return " mat4x4";
		case VertexFormat::float64_2x2:			return " dmat2x2";
		case VertexFormat::float64_2x3:			return " dmat2x3";
		case VertexFormat::float64_2x4:			return " dmat2x4";
		case VertexFormat::float64_3x2:			return " dmat3x2";
		case VertexFormat::float64_3x3:			return " dmat3x3";
		case VertexFormat::float64_3x4:			return " dmat3x4";
		case VertexFormat::float64_4x2:			return " dmat4x2";
		case VertexFormat::float64_4x3:			return " dmat4x3";
		case VertexFormat::float64_4x4:			return " dmat4x4";
		case VertexFormat::float64_1:			return " double";
		case VertexFormat::float64_2:			return " dvec2";
		case VertexFormat::float64_3:			return " dvec3";
		case VertexFormat::float64_4:			return " dvec4";

		case VertexFormat::uint8_1:				return " uint";
		case VertexFormat::uint8_2:				return " uvec2";
		case VertexFormat::uint8_3:				return " uvec3";
		case VertexFormat::uint8_4:				return " uvec4";
		case VertexFormat::uint16_1:			return " uint";
		case VertexFormat::uint16_2:			return " uvec2";
		case VertexFormat::uint16_3:			return " uvec3";
		case VertexFormat::uint16_4:			return " uvec4";
		case VertexFormat::uint32_1:			return " uint";
		case VertexFormat::uint32_2:			return " uvec2";
		case VertexFormat::uint32_3:			return " uvec3";
		case VertexFormat::uint32_4:			return " uvec4";
		case VertexFormat::uint64_1:			return " double";
		case VertexFormat::uint64_2:			return " dvec2";
		case VertexFormat::uint64_3:			return " dvec3";
		case VertexFormat::uint64_4:			return " dvec4";

		case VertexFormat::int8_1_norm:			return " int";
		case VertexFormat::int8_2_norm:			return " ivec2";
		case VertexFormat::int8_3_norm:			return " ivec3";
		case VertexFormat::int8_4_norm:			return " ivec4";
		case VertexFormat::int16_1_norm:		return " int";
		case VertexFormat::int16_2_norm:		return " ivec2";
		case VertexFormat::int16_3_norm:		return " ivec3";
		case VertexFormat::int16_4_norm:		return " ivec4";
		case VertexFormat::uint8_1_norm:		return " uint";
		case VertexFormat::uint8_2_norm:		return " uvec2";
		case VertexFormat::uint8_3_norm:		return " uvec3";
		case VertexFormat::uint8_4_norm:		return " uvec4";
		case VertexFormat::uint16_1_norm:		return " uint";
		case VertexFormat::uint16_2_norm:		return " uvec2";
		case VertexFormat::uint16_3_norm:		return " uvec3";
		case VertexFormat::uint16_4_norm:		return " uvec4";
		case VertexFormat::int1010102_norm:		return " int";
		case VertexFormat::uint1010102_norm:	return " uint";
		case VertexFormat::int101111_norm:		return " int";
		case VertexFormat::uint101111_norm:		return " uint";
		case VertexFormat::uint8_4_norm_bgra:	return " uint";
		case VertexFormat::uint8_custom:		return " uint";
		case VertexFormat::uint16_custom:		return " uint";
		case VertexFormat::uint32_custom:		return " uint";
		case VertexFormat::float32_custom:		return " float";
	}
	// clang-format on

	return "";
}

std::string attribute_format(VertexFormat a_format)
{
	// bool
	// int
	// uint
	// float
	// double

	// bvecn:
	// ivecn:
	// uvecn:
	// vecn:
	// dvecn:

	// mat3x3
	// mat3x4
	// mat4x4

	// TODO: Support 16bit_storage_input_output like https://github.com/KhronosGroup/Vulkan-Samples/tree/master/samples/performance/16bit_storage_input_output
	// TODO: Support mesh quantisation https://github.com/KhronosGroup/glTF/tree/master/extensions/2.0/Khronos/KHR_mesh_quantization
	// clang-format off
	switch (a_format)
	{
		case VertexFormat::struct_1:			return " ";
		case VertexFormat::bool32_1:			return " bool";
		case VertexFormat::bool32_2:			return " bvec2";
		case VertexFormat::bool32_3:			return " bvec3";
		case VertexFormat::bool32_4:			return " bvec4";
		case VertexFormat::int8_1:				return " int";
		case VertexFormat::int8_2:				return " ivec2";
		case VertexFormat::int8_3:				return " ivec3";
		case VertexFormat::int8_4:				return " ivec4";
		case VertexFormat::int16_1:				return " int";
		case VertexFormat::int16_2:				return " ivec2";
		case VertexFormat::int16_3:				return " ivec3";
		case VertexFormat::int16_4:				return " ivec4";
		case VertexFormat::int32_1:				return " int";
		case VertexFormat::int32_2:				return " ivec2";
		case VertexFormat::int32_3:				return " ivec3";
		case VertexFormat::int32_4:				return " ivec4";
		case VertexFormat::half16_1:			return " int";
		case VertexFormat::half16_2:			return " ivec2";
		case VertexFormat::half16_3:			return " ivec3";
		case VertexFormat::half16_4:			return " ivec4";

		case VertexFormat::float32_1:			return " float";
		case VertexFormat::float32_2:			return " vec2";
		case VertexFormat::float32_3:			return " vec3";
		case VertexFormat::float32_4:			return " vec4";
		case VertexFormat::uint8_1:				return " uint";
		case VertexFormat::uint8_2:				return " uvec2";
		case VertexFormat::uint8_3:				return " uvec3";
		case VertexFormat::uint8_4:				return " uvec4";
		case VertexFormat::uint16_1:			return " uint";
		case VertexFormat::uint16_2:			return " uvec2";
		case VertexFormat::uint16_3:			return " uvec3";
		case VertexFormat::uint16_4:			return " uvec4";
		case VertexFormat::uint32_1:			return " uint";
		case VertexFormat::uint32_2:			return " uvec2";
		case VertexFormat::uint32_3:			return " uvec3";
		case VertexFormat::uint32_4:			return " uvec4";

		case VertexFormat::int8_1_norm:			return " int";
		case VertexFormat::int8_2_norm:			return " ivec2";
		case VertexFormat::int8_3_norm:			return " ivec3";
		case VertexFormat::int8_4_norm:			return " ivec4";
		case VertexFormat::int16_1_norm:		return " int";
		case VertexFormat::int16_2_norm:		return " ivec2";
		case VertexFormat::int16_3_norm:		return " ivec3";
		case VertexFormat::int16_4_norm:		return " ivec4";
		case VertexFormat::uint8_1_norm:		return " uint";
		case VertexFormat::uint8_2_norm:		return " uvec2";
		case VertexFormat::uint8_3_norm:		return " uvec3";
		case VertexFormat::uint8_4_norm:		return " uvec4";
		case VertexFormat::uint16_1_norm:		return " uint";
		case VertexFormat::uint16_2_norm:		return " uvec2";
		case VertexFormat::uint16_3_norm:		return " uvec3";
		case VertexFormat::uint16_4_norm:		return " uvec4";

		case VertexFormat::uint8_custom:		return " uint";
		case VertexFormat::uint16_custom:		return " uint";
		case VertexFormat::uint32_custom:		return " uint";
		case VertexFormat::float32_custom:		return " float";

		case VertexFormat::float32_2x2:
		case VertexFormat::float32_2x3:
		case VertexFormat::float32_2x4:
		case VertexFormat::float32_3x2:
		case VertexFormat::float32_3x3:
		case VertexFormat::float32_3x4:
		case VertexFormat::float32_4x2:
		case VertexFormat::float32_4x3:
		case VertexFormat::float32_4x4:
		case VertexFormat::float64_2x2:
		case VertexFormat::float64_2x3:
		case VertexFormat::float64_2x4:
		case VertexFormat::float64_3x2:
		case VertexFormat::float64_3x3:
		case VertexFormat::float64_3x4:
		case VertexFormat::float64_4x2:
		case VertexFormat::float64_4x3:
		case VertexFormat::float64_4x4:
		case VertexFormat::float64_1:
		case VertexFormat::float64_2:
		case VertexFormat::float64_3:
		case VertexFormat::float64_4:

		case VertexFormat::uint64_1:
		case VertexFormat::uint64_2:
		case VertexFormat::uint64_3:
		case VertexFormat::uint64_4:

		case VertexFormat::int1010102_norm:
		case VertexFormat::uint1010102_norm:
		case VertexFormat::int101111_norm:
		case VertexFormat::uint101111_norm:
		case VertexFormat::uint8_4_norm_bgra:
			ror::log_critical("Format {} not supported but requested", a_format);
			assert(0 && "Format not supported");
	}
	// clang-format on

	return "";
}

template <typename _type>
void replace_next_at(_type a_value, std::string &a_result)
{
	auto index = a_result.find_first_of('@');
	assert(index != std::string::npos);
	std::string replacement{};
	if constexpr (std::is_arithmetic_v<_type>)
		replacement = std::to_string(a_value);
	else
		replacement = a_value;

	a_result.replace(index, 1, replacement);
}

template <typename _type>
void replace_all_ats(uint32_t a_ats_count, _type a_at_replace, std::string &a_result)
{
	for (uint32_t j = 0; j < a_ats_count; ++j)
		replace_next_at(a_at_replace, a_result);
}

void append_count_times(uint32_t a_count, uint32_t a_ats_count,
						std::string &a_result, const std::string &a_body)
{
	for (uint32_t i = 0; i < a_count; ++i)
	{
		auto str{a_body};
		replace_all_ats(a_ats_count, i, str);
		a_result.append(str);
	}
}

const std::string vs_morph_common_str = R"coms(
const uint weights_count = @;

layout(set = @, binding = @) uniform morph_weight
{
	float morph_weights[weights_count];
} in_morph_weights;

)coms";

std::string vs_morph_common(size_t a_weights_count, uint32_t a_morph_set, uint32_t a_morph_binding)
{
	auto str{vs_morph_common_str};

	replace_next_at(a_weights_count, str);
	replace_next_at(a_morph_set, str);
	replace_next_at(a_morph_binding, str);

	return str;
}

const std::string vs_morph_stuff_header_str{"\nvoid morph_@(inout @ @)\n{\n"};
const std::string vs_morph_stuff_line_str{"\t@ += in_morph_weights.morph_weights[@] * in_target_@@;\n"};

std::string vs_morph_attribute_common(uint32_t a_count, const std::string &a_name, const std::string &a_type)
{
	auto str{vs_morph_stuff_header_str};

	replace_next_at(a_name, str);
	replace_next_at(a_type, str);
	replace_next_at(a_name, str);

	for (uint32_t i = 0; i < a_count; ++i)
	{
		auto line{vs_morph_stuff_line_str};
		replace_next_at(a_name, line);
		replace_next_at(i, line);
		replace_next_at(a_name, line);
		replace_next_at(i, line);

		str.append(line);
	}

	str.append("\n}\n");
	return str;
}

// TODO: These should be conditional
// There should be per_view vs per_renderable uniforms, view requires many more matrices while renderable only needs MVP, Normal matrices
// TODO: Separate this by view and renderable
const std::string per_view_common_str = R"com(
layout(std140, set = @, binding = @) uniform per_view_uniform
{
	mat4 mvp_mat4;
	mat4 model_mat4;
	mat4 view_mat4;
	mat4 projection_mat4;
	mat4 view_projection_mat4;
	mat4 inverse_projection_mat4;
	mat4 inverse_view_projection_mat4;
	mat3 normal_mat4;
	vec3 camera_position;
} in_per_view_uniforms;
)com";

std::string per_view_common(uint32_t a_set, uint32_t a_binding)
{
	auto str{per_view_common_str};

	replace_next_at(a_set, str);
	replace_next_at(a_binding, str);

	return str;
}

const std::string per_frame_common_str = R"com(
layout(std140, set = @, binding = @) uniform per_frame_uniform
{
	bool force_opaque;
	bool debug_all;
	bool debug_normals;
	bool debug_positions;
	bool debug_tangents;
	bool debug_base_color;
	bool debug_brdf_visibility;
	bool debug_brdf_occlusion;
	bool debug_brdf_fresnel;
} in_per_frame_uniforms;
)com";

std::string per_frame_common(uint32_t a_set, uint32_t a_binding)
{
	auto str{per_frame_common_str};

	replace_next_at(a_set, str);
	replace_next_at(a_binding, str);

	return str;
}

const std::string vs_skin_common_str = R"scom(
const uint joints_count = @;

struct trs_transform
{
	vec4 rotation;
	vec3 translation;
	vec3 scale;
@

layout(std140, set = @, binding = @) uniform joint_transform
{
	trs_transform joint_transforms[joints_count];
} in_joint_transforms;
)scom";

const std::string vs_skin_common_normal_str = R"scomn(
// Source filament shaders from https://google.github.io/filament/Filament.html
vec3 skin_normal(vec3 normal, uint index)
{
	vec4 rotation      = in_joint_transforms.joint_transforms[index].rotation;
	vec3 scale_inverse = in_joint_transforms.joint_transforms[index].scale_inverse;

	normal *= scale_inverse;                                                                       // apply the inverse of the non-uniform scales
	normal += 2.0 * cross(rotation.xyz, cross(rotation.xyz, normal) + rotation.w * normal);        // apply the rigid transform (valid only for unit quaternions)

	return normal;
}
)scomn";

const std::string vs_skin_common_position_str = R"scomp(
vec3 skin_position(vec3 vertex, uint index)
{
	vec4 rotation    = in_joint_transforms.joint_transforms[index].rotation;
	vec3 translation = in_joint_transforms.joint_transforms[index].translation;
	vec3 scale       = in_joint_transforms.joint_transforms[index].scale;

	vertex *= scale;                                                                               // apply the non-uniform scales
	vertex += 2.0 * cross(rotation.xyz, cross(rotation.xyz, vertex) + rotation.w * vertex);        // apply the rigid transform (valid only for unit quaternions)
	vertex += translation;                                                                         // apply the translation

	return vertex;
}
)scomp";

std::string vs_skin_common(uint32_t a_joints_count, uint32_t a_joint_set, uint32_t a_joint_binding, bool a_has_normal)
{
	auto       str{vs_skin_common_str};
	const auto scale_inverse{"\tvec3 scale_inverse;\n};"};

	replace_next_at(a_joints_count, str);

	if (a_has_normal)
		replace_next_at(scale_inverse, str);
	else
		replace_next_at("};", str);

	replace_next_at(a_joint_set, str);
	replace_next_at(a_joint_binding, str);

	if (a_has_normal)
		str.append(vs_skin_common_normal_str);

	str.append(vs_skin_common_position_str);

	return str;
}

const std::string vs_skin_normal_header_str{"\nvoid skin_normal(inout vec3 normal)\n{"};
const std::string vs_skin_normal_str = R"ncom(
	normal = skin_normal(normal, in_vertex_bone_id_@.x) * in_vertex_weight_@.x +
			 skin_normal(normal, in_vertex_bone_id_@.y) * in_vertex_weight_@.y +
			 skin_normal(normal, in_vertex_bone_id_@.z) * in_vertex_weight_@.z +
			 skin_normal(normal, in_vertex_bone_id_@.w) * in_vertex_weight_@.w;
)ncom";

std::string vs_skin_normal(uint32_t a_joints_weights_count)
{
	auto result{vs_skin_normal_header_str};
	append_count_times(a_joints_weights_count, 8, result, vs_skin_normal_str);
	result.append("}\n");

	return result;
}

const std::string vs_skin_position_header_str{"\nvoid skin_position(inout vec3 position) \n{"};
const std::string vs_skin_position_str = R"pcom(
	position = skin_position(position, in_vertex_bone_id_@.x) * in_vertex_weight_@.x +
			   skin_position(position, in_vertex_bone_id_@.y) * in_vertex_weight_@.y +
			   skin_position(position, in_vertex_bone_id_@.z) * in_vertex_weight_@.z +
			   skin_position(position, in_vertex_bone_id_@.w) * in_vertex_weight_@.w;
)pcom";

std::string vs_skin_position(uint32_t a_joints_weights_count)
{
	auto result{vs_skin_position_header_str};
	append_count_times(a_joints_weights_count, 8, result, vs_skin_position_str);
	result.append("}\n");

	return result;
}

// TODO: Create un-projected, perspective correct world position
// vec3 wp = model * view * vertex_position;
// wp = wp.xyz / vertex_position.w; // or something like that
const std::string vs_set_position_str = R"spos(
void world_transform_position(inout vec4 vertex_position)
{
	// vertex_position = model * view * projection * vertex_position;
	vertex_position = in_per_view_uniforms.mvp_mat4 * vertex_position;
}

void set_position()
{
	vec4 vertex_position = in_vertex_position;

	morph_vertex_position(vertex_position);
	skin_position(vertex_position.xyz);
	world_transform_position(vertex_position);

	out_vertex_position = vertex_position;

	gl_Position = vertex_position;
}
)spos";

const std::string vs_set_normal_str = R"snor(
void world_transform_normal(inout vec3 vertex_normal)
{
	vertex_normal = in_per_view_uniforms.normal_mat4 * vertex_normal;
}

void set_normal()
{
	vec3 vertex_normal = in_vertex_normal;

	morph_vertex_normal(vertex_normal);
	skin_normal(vertex_normal);
	world_transform_normal(vertex_normal);

	out_vertex_normal = vertex_normal;
}
)snor";

const std::string vs_set_tangent_str = R"stan(
void set_tangent()
{
	vec4 vertex_tangent = in_vertex_tangent;

	morph_vertex_tangent(vertex_tangent);
	skin_position(vertex_tangent.xyz);
	world_transform_position(vertex_tangent);

	out_vertex_tangent = vertex_tangent;
}
)stan";

const std::string vs_set_texture_coordinate_str = R"suv0(
void set_texture_coordinate_@()
{
	vec2 vertex_texture_coordinate = in_vertex_texture_coord_@;

	morph_vertex_texture_coord_@(vertex_texture_coordinate);
	out_vertex_texture_coord_@ = vertex_texture_coordinate;
}
)suv0";

std::string vs_set_texture_coordinate(uint32_t a_uvs_count)
{
	assert(a_uvs_count < 4 && "Too many UV sets provided, only support upto 3");

	std::string result{};
	append_count_times(a_uvs_count, 4, result, vs_set_texture_coordinate_str);
	return result;
}

const std::string vs_set_color_str = R"scol0(
void set_color_@()
{
	vec4 vertex_color = in_vertex_color_@;

	morph_vertex_color_@(vertex_color);
	out_vertex_color_@ = vertex_color;
}
)scol0";

std::string vs_set_color(uint32_t a_colors_count)
{
	assert(a_colors_count < 3 && "Too many color sets provided, only support upto 2");

	std::string result{};
	append_count_times(a_colors_count, 4, result, vs_set_color_str);
	return result;
}

const std::string vs_main_str = R"vsm(
void main()
{
)vsm";

std::string vs_set_main(bool a_has_normal, bool a_has_tangent, bool a_has_bent_normal, uint32_t a_uv_count, uint32_t a_color_count)
{
	auto result{vs_main_str};

	if (a_has_normal)
		result.append("\tset_normal();\n");

	if (a_has_tangent)
		result.append("\tset_tangent();\n");

	if (a_has_bent_normal)
		result.append("\tset_bent_normal();\n");

	if (a_uv_count > 0)
	{
		std::string uvs_patch{};
		append_count_times(a_uv_count, 1, uvs_patch, "\tset_texture_coordinate_@();\n");
		result.append(uvs_patch);
	}

	if (a_color_count > 0)
	{
		std::string color_patch{};
		append_count_times(a_color_count, 1, color_patch, "\tset_color_@();\n");
		result.append(color_patch);
	}

	result.append("\tset_position();\n}\n");

	return result;
}

// This is to be used for calculating standard output formats for each attribute so I can remove lots of combinations and have a standard interface into fragment shaders
// This is basically an override on whatever data is coming in
static const std::unordered_map<rhi::BufferSemantic, rhi::VertexFormat> attrib_in_out_format{
	{rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_4},
	{rhi::BufferSemantic::vertex_normal, rhi::VertexFormat::float32_3},
	{rhi::BufferSemantic::vertex_tangent, rhi::VertexFormat::float32_4},
	{rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2},
	{rhi::BufferSemantic::vertex_texture_coord_1, rhi::VertexFormat::float32_2},
	{rhi::BufferSemantic::vertex_texture_coord_2, rhi::VertexFormat::float32_2},
	{rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::float32_4},
	{rhi::BufferSemantic::vertex_color_1, rhi::VertexFormat::float32_4}};

std::string vertex_shader_input_output(const VertexDescriptor &a_vertex_descriptor, uint32_t a_location_offset, uint32_t a_target_offset, const std::string &a_prefix, bool a_output)
{
	std::string result{};
	auto       &attributes = a_vertex_descriptor.attributes();

	const std::string layout{"layout (location = "};
	const std::string in{" in"};
	const std::string out{" out"};
	const std::string precision{" highp"};        // TODO: Need to abstract out precision

	if (!(a_prefix != "" && a_output))        // Don't write out anything to output if we are traversing morph targets
	{
		for (auto &attrib : attributes)
		{
			const auto semantic      = attrib.semantics();
			auto       in_out_format = attrib.format();

			const auto out_format_iter = attrib_in_out_format.find(semantic);

			if (out_format_iter != attrib_in_out_format.end())
				in_out_format = out_format_iter->second;

			if (semantic < rhi::BufferSemantic::vertex_index)        // Only need vertex attributes that are bellow vertex_index, FIXME: what happens to custom ones?
			{
				auto location          = attrib.location();
				auto in_out_format_str = attribute_format(in_out_format);
				auto semantic_str      = get_format_semantic(semantic);

				std::string first_half{layout + std::to_string(location + a_location_offset) + ")"};
				std::string middle_half{precision + in_out_format_str};
				std::string second_half{(a_prefix != "" ? "_" + a_prefix : "") + "_" + semantic_str + (a_prefix != "" ? std::to_string(a_target_offset) : "") + ";\n"};

				std::string line{first_half + in + middle_half + in + second_half};

				result.append(line);

				// TODO: Add support for outputting normal + tangent or TBN
				if (semantic < rhi::BufferSemantic::vertex_bone_id_0)        // Only output vertex attributes that are bellow vertex_bone_id_0, FIXME: Add support for custom ones
					result.append(a_output ? std::string{first_half + out + middle_half + out + second_half} : "");
			}
		}
	}

	return result;
}

// TODO: Use https://github.com/aras-p/glsl-optimizer on the output for further optimising the GLSL
std::string generate_primitive_vertex_shader(const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_primitive_index)
{
	const auto &mesh                     = a_model.meshes()[a_mesh_index];
	auto       &vertex_descriptor        = mesh.m_attribute_vertex_descriptors[a_primitive_index];
	auto       &vertex_target_descriptor = mesh.m_morph_targets_vertex_descriptors[a_primitive_index];
	auto        type                     = vertex_descriptor.type();
	auto        has_bone_id_0            = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_0)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_0);
	auto        has_bone_id_1            = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_1)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_bone_id_1);
	auto        has_normal               = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal);
	auto        has_tangent              = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_tangent)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_tangent);
	auto        has_uv_0                 = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0);
	auto        has_uv_1                 = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_1)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_1);
	auto        has_uv_2                 = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_2)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_2);
	auto        has_color_0              = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_0)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_0);
	auto        has_color_1              = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_1)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_1);

	std::unordered_map<rhi::BufferSemantic, std::pair<uint32_t, bool>> targets_count{
		{rhi::BufferSemantic::vertex_position, {0, true}},
		{rhi::BufferSemantic::vertex_normal, {0, has_normal}},
		{rhi::BufferSemantic::vertex_tangent, {0, has_tangent}},
		{rhi::BufferSemantic::vertex_texture_coord_0, {0, has_uv_0}},
		{rhi::BufferSemantic::vertex_texture_coord_1, {0, has_uv_1}},
		{rhi::BufferSemantic::vertex_texture_coord_2, {0, has_uv_2}},
		{rhi::BufferSemantic::vertex_color_0, {0, has_color_0}},
		{rhi::BufferSemantic::vertex_color_1, {0, has_color_1}}};

	std::string result{"#version 450\n\nprecision highp float;\nprecision highp int;\n"};        // TODO: abstract out version

	// Write out vertex shader input output
	result.append(rhi::vertex_shader_input_output(vertex_descriptor, 0, 0, "", true));

	uint32_t location_offset = 0;

	if (!vertex_descriptor.attributes().empty())
	{
		const auto &att      = vertex_descriptor.attributes().back();
		auto        location = att.location();
		location_offset      = location;        // Its +1 then the last attribute in the list because of vertex_index not considered, if mesh don't have index data, this will conflict if not incremented
		if (att.semantics() != rhi::BufferSemantic::vertex_index)
			location_offset++;
	}

	uint32_t j = 0;
	for (const auto &v : vertex_target_descriptor)
	{
		result.append(rhi::vertex_shader_input_output(v, location_offset, j++, "target"));

		if (!v.attributes().empty())
		{
			auto location = v.attributes().back().location();
			location_offset += location + 1;
		}

		auto &attributes = v.attributes();
		for (auto &attrib : attributes)
		{
			const auto semantic = attrib.semantics();
			targets_count[semantic].first++;
		}
	}

	// Write out common methods
	result.append(per_view_common(0, 0));        // TODO: Abstract out set and binding

	// Only add morph common if mesh has weights and has morph targets
	if (mesh.m_morph_weights.size() > 0 && vertex_target_descriptor.size() > 0)
		result.append(vs_morph_common(mesh.m_morph_weights.size(), 0, 1));        // TODO: Abstract out set and binding

	// Write out common morph target functions
	for (auto &tc : targets_count)
	{
		const auto out_format_iter = attrib_in_out_format.find(tc.first);

		assert(out_format_iter != attrib_in_out_format.end() && "Can't find supported target attribute");

		auto in_out_format     = out_format_iter->second;
		auto in_out_format_str = attribute_format(in_out_format);
		auto semantic_str      = get_format_semantic(tc.first);

		if (tc.second.second > 0)
			result.append(vs_morph_attribute_common(tc.second.first, semantic_str, in_out_format_str));
	}

	if (has_bone_id_0)
	{
		assert(mesh.m_skin_index > -1 && "Skin isn't valid in the mesh");
		const auto &skin = a_model.skins()[static_cast<size_t>(mesh.m_skin_index)];
		assert(skin.m_joints.size() > 0 && "No joints available in this skin");

		result.append(vs_skin_common(static_cast<uint32_t>(skin.m_joints.size()), 2, 0, has_normal));        // TODO: Abstract out set and binding
	}

	// vs_skin_normal and vs_skin_position needs to be unconditional from bas_bone_id_0, because these are referenced in set_postion and set_normal
	auto joint_sets{0u};
	if (has_bone_id_0)
		joint_sets++;

	if (has_bone_id_1)
		joint_sets++;

	if (has_normal)
		result.append(vs_skin_normal(joint_sets));        // Max 2 joint/weights sets

	result.append(vs_skin_position(joint_sets));        // Max 2 joint/weights sets

	result.append(vs_set_position_str);        // Add set_position

	if (has_normal)
		result.append(vs_set_normal_str);        // Add set_normal

	if (has_tangent)
		result.append(vs_set_tangent_str);        // Add set_tangent

	auto uv_sets{0u};
	if (has_uv_0)
		uv_sets++;

	if (has_uv_1)
		uv_sets++;

	if (has_uv_2)
		uv_sets++;

	if (uv_sets > 0)
		result.append(vs_set_texture_coordinate(uv_sets));        // Max 3 UV sets count

	auto color_sets{0u};
	if (has_color_0)
		color_sets++;

	if (has_color_1)
		color_sets++;

	if (color_sets > 0)
		result.append(vs_set_color(color_sets));        // Max 2 color sets count

	result.append(vs_set_main(has_normal, has_tangent, false, uv_sets, color_sets));        // Add main as well

	return result;
}

// Vertex shader methods finish here, now starts fragment shader methods

// TODO: Work out the vec3 padding issues in this UBO
const std::string fs_directional_light_common_str = R"com(
const uint directional_lights_count = @;

struct DirectionalLight
{
	vec3  color;
	vec3  direction;
	float intensity;
};

layout(std140, set = @, binding = @) uniform directional_light_uniform
{
	DirectionalLight lights[directional_lights_count];
} in_directional_light_uniforms;
)com";

const std::string fs_point_light_common_str = R"com(
const uint point_lights_count = @;

struct PointLight
{
	vec3  color;
	vec3  position;
	float intensity;
};

layout(std140, set = @, binding = @) uniform point_light_uniform
{
	PointLight lights[point_lights_count];
} in_point_light_uniforms;
)com";

const std::string fs_spot_light_common_str = R"com(
const uint spot_lights_count = @;

struct SpotLight
{
	vec3  color;
	vec3  position;
	vec3  direction;
	float intensity;
	float inner_angle;
	float outer_angle;
};

layout(std140, set = @, binding = @) uniform spot_light_uniform
{
	SpotLight lights[spot_lights_count];
} in_spot_light_uniforms;
)com";

std::string fs_light_common(uint32_t a_lights_count, uint32_t a_set, uint32_t a_binding, std::string light_code)
{
	replace_next_at(a_lights_count, light_code);
	replace_next_at(a_set, light_code);
	replace_next_at(a_binding, light_code);

	return light_code;
}

std::string fragment_shader_input_output(const VertexDescriptor &a_vertex_descriptor)
{
	std::string result{};

	auto &attributes = a_vertex_descriptor.attributes();

	const std::string layout{"layout (location = "};
	const std::string in{" in"};
	const std::string precision{" highp"};

	for (auto &attrib : attributes)
	{
		auto semantic = attrib.semantics();
		if (semantic < rhi::BufferSemantic::vertex_bone_id_0)        // Only need vertex attributes that are bellow vertex_bone_id_0, FIXME: what happens to custom ones?
		{
			auto       location        = attrib.location();
			auto       in_out_format   = attrib.format();
			const auto out_format_iter = attrib_in_out_format.find(semantic);

			if (out_format_iter != attrib_in_out_format.end())
				in_out_format = out_format_iter->second;

			auto format_str   = attribute_format(in_out_format);
			auto semantic_str = get_format_semantic(semantic);

			std::string first_half{layout + std::to_string(location) + ")"};
			std::string middle_half{precision + format_str};
			std::string second_half{"_" + semantic_str + ";\n"};

			std::string line{first_half + in + middle_half + in + second_half};
			result.append(line);
		}
	}

	// layout (location = 0) out vec4 out_color; is defined in main.frag.glsl

	return result;
}

template <typename _factor_type>
constexpr const char *factor_cast()
{
	return "";
}

template <>
constexpr const char *factor_cast<ror::Color4f>()
{
	return "vec4";
}

template <>
constexpr const char *factor_cast<ror::Color3f>()
{
	return "vec3";
}

template <>
constexpr const char *factor_cast<ror::Color2f>()
{
	return "vec2";
}

template <>
constexpr const char *factor_cast<float32_t>()
{
	return "float";
}

/*
The following will make something like

vec2 get_base_color_uvs()
{
	return in_vertex_texture_coord_0;
}

vec4 get_base_color()
{
	vec2 base_color_uv = get_base_color_uvs();
	base_color_uv = (base_color_uv_transform * base_color_uv).xy;
	base_color = texture(base_color_sampler, base_color_uv.xy) * vec4(base_color_factor, 1.0);
}

if called like texture_lookup(mat_comp, "base_color");
*/

// This will always create a vec2/3/4/float get_{a_comp_name}() function for valid component because at least a factor is always there
template <typename _factor_type>
std::string texture_lookup(const ror::Material::Component<_factor_type> &a_mat_comp, const std::string &a_comp_name, const std::string &a_retrun_type = "vec4", const std::string &a_swizzle = "")
{
	if (a_mat_comp.m_type != ror::Material::ComponentType::none)
	{
		std::string factor_type{factor_cast<decltype(a_mat_comp.m_factor)>()};
		std::string output_coords{};
		std::string output_lookup{"\n" + a_retrun_type + " get_" + a_comp_name + "()\n{\n\t"};
		std::string factor_cast_str{factor_type + "(in_factors." + a_comp_name + "_factor)"};

		if (a_mat_comp.m_type == ror::Material::ComponentType::factor)
			output_lookup.append("return " + factor_cast_str);
		else        // So we are either factor_texture or texture
		{
			output_coords.append({"\nvec2 get_" + a_comp_name + "_uvs()\n{\n\t"});
			assert((a_mat_comp.m_uv_map == 0 || a_mat_comp.m_uv_map == 1 || a_mat_comp.m_uv_map == 2) && "Unsupported uv map");
			std::string coords({"in_vertex_texture_coord_" + std::to_string(a_mat_comp.m_uv_map)});

			// Second work out if it has uv transform or not
			if (a_mat_comp.m_has_transform)
			{
				output_coords.append({"vec2 " + a_comp_name + "_uv = " + coords + ";\n\t"});
				// Encode the matrix into the shader
				// TODO: Fix this if the matrix is animated in some way
				output_coords.append("const mat3 " + a_comp_name + "_uv_transform  = mat3(");

				for (uint32_t i = 0; i < 8; ++i)
					output_coords.append(std::to_string(a_mat_comp.m_transform.m_values[i]) + ",");

				output_coords.append(std::to_string(a_mat_comp.m_transform.m_values[8]) + ");\n\t");

				output_coords.append(a_comp_name + "_uv = (" + a_comp_name + "_uv_transform * vec3(" + a_comp_name + "_uv, 1.0)).xy;\n\t");
				output_coords.append("return " + a_comp_name + "_uv;\n}\n");
			}
			else
				output_coords.append("return " + coords + ";\n}\n");

			output_lookup.append({"vec2 " + a_comp_name + "_uv = get_" + a_comp_name + "_uvs();\n\t"});

			// Third check if we need to apply _factor and what's the type
			if (a_comp_name == "normal" || a_comp_name == "bent_normal")
				output_lookup.append("return (normalize(texture(" + a_comp_name + "_sampler, " + a_comp_name + "_uv)" + a_swizzle + ") * 2.0 - 1.0)");
			else
				output_lookup.append("return texture(" + a_comp_name + "_sampler, " + a_comp_name + "_uv)" + a_swizzle);

			if (a_mat_comp.m_type == ror::Material::ComponentType::factor_texture)
			{
				output_lookup.append(" * ");
				if (a_comp_name == "normal" || a_comp_name == "bent_normal")
				{
					auto tmp{"in_factors." + a_comp_name + "_factor"};
					output_lookup.append("vec3(" + tmp + ", " + tmp + ", 1.0)");
				}
				else
					output_lookup.append(factor_cast_str);
			}
		}

		output_lookup.append(";\n}\n");

		return output_coords + output_lookup;
	}

	return "";
}

// Calls texture_lookup for all material components if it has textures
std::string texture_lookups(const ror::Material &a_material)
{
	std::string output{""};

	output.append(texture_lookup(a_material.m_base_color, "base_color"));
	output.append(texture_lookup(a_material.m_diffuse_color, "diffuse_color"));
	output.append(texture_lookup(a_material.m_specular_glossyness, "specular_glossyness"));
	output.append(texture_lookup(a_material.m_emissive, "emissive"));
	output.append(texture_lookup(a_material.m_anisotropy, "anisotrophy"));
	output.append(texture_lookup(a_material.m_transmission, "transmission", "vec2"));
	output.append(texture_lookup(a_material.m_sheen_color, "sheen_color", "vec3"));
	output.append(texture_lookup(a_material.m_sheen_roughness, "sheen_roughness"));
	output.append(texture_lookup(a_material.m_clearcoat_normal, "clearcoat_normal"));
	output.append(texture_lookup(a_material.m_clearcoat, "clearcoat"));
	output.append(texture_lookup(a_material.m_clearcoat_roughness, "clearcoat_roughness"));
	output.append(texture_lookup(a_material.m_metallic, "metallic", "float", ".x"));          // Red component of MRO[H] texture
	output.append(texture_lookup(a_material.m_roughness, "roughness", "float", ".y"));        // Green component of MRO[H] texture
	output.append(texture_lookup(a_material.m_occlusion, "occlusion", "float", ".z"));        // Blue component of MRO[H] texture
	output.append(texture_lookup(a_material.m_normal, "normal", "vec3", ".xyz"));
	// output.append(texture_lookup(a_material.m_tangent, "tangent", "vec3", ".xyz"));
	output.append(texture_lookup(a_material.m_bent_normal, "bent_normal", "vec3", ".xyz"));
	output.append(texture_lookup(a_material.m_height, "height", "float", ".w"));        // Alpha component of MRO[H] texture
	output.append(texture_lookup(a_material.m_anisotropy, "anisotrophy", "vec4"));
	output.append(texture_lookup(a_material.m_opacity, "opacity", "float", ".x"));

	// If we have a normal map, lets add code to generate tangent frame in the shader
	if (a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture ||
		a_material.m_normal.m_type == ror::Material::ComponentType::texture)
	{
		// Read tbn.frag.glsl resource and create a string_view
		auto            &tbn_resource = ror::load_resource("shaders/tbn.frag.glsl", ror::ResourceSemantic::shaders);
		std::string_view tbn_code{reinterpret_cast<const char *>(tbn_resource.data().data()), tbn_resource.data().size()};
		output.append(tbn_code);
	}
	else        // Otherwise add a simple normal map overloaded getter
	{
		output.append("vec3 get_normal(const in vec3 N, const in vec3 V)\n{\n\treturn N;\n}\n");
	}

	output.append("\nfloat get_reflectance()\n{\n\treturn in_factors.reflectance_factor;\n}\n");
	output.append("\nvec3 get_sheen_color()\n{\n\treturn sqrt(get_base_color().rgb);\n}\n");

	return output;
}

std::string material_samplers(const ror::Material &a_material)
{
	std::string       output{"\n"};
	const std::string set_binding{"layout(set = 0, binding = "};           // TODO: Abstract out the set and binding for all of the below
	const std::string type_precision{") uniform highp sampler2D "};        // TODO: Abstract out precision
	uint32_t          binding = 0;

#define output_append(x) output.append(set_binding + std::to_string(binding++) + type_precision + x)

	if (a_material.m_base_color.m_texture.m_handle != -1)
		output_append("base_color_sampler;\n");
	if (a_material.m_diffuse_color.m_texture.m_handle != -1)
		output_append("diffuse_color_sampler;\n");
	if (a_material.m_specular_glossyness.m_texture.m_handle != -1)
		output_append("specular_glossyness_sampler;\n");
	if (a_material.m_emissive.m_texture.m_handle != -1)
		output_append("emissive_sampler;\n");
	if (a_material.m_anisotropy.m_texture.m_handle != -1)
		output_append("anisotrophy_sampler;\n");
	if (a_material.m_transmission.m_texture.m_handle != -1)
		output_append("transmission_sampler;\n");
	if (a_material.m_sheen_color.m_texture.m_handle != -1)
		output_append("sheen_color_sampler;\n");
	if (a_material.m_sheen_roughness.m_texture.m_handle != -1)
		output_append("sheen_roughness_sampler;\n");
	if (a_material.m_clearcoat_normal.m_texture.m_handle != -1)
		output_append("clearcoat_normal_sampler;\n");
	if (a_material.m_clearcoat.m_texture.m_handle != -1)
		output_append("clearcoat_sampler;\n");
	if (a_material.m_clearcoat_roughness.m_texture.m_handle != -1)
		output_append("clearcoat_roughness_sampler;\n");
	if (a_material.m_metallic.m_texture.m_handle != -1)
		output_append("metallic_sampler;\n");
	if (a_material.m_roughness.m_texture.m_handle != -1)
		output_append("roughness_sampler;\n");
	if (a_material.m_occlusion.m_texture.m_handle != -1)
		output_append("occlusion_sampler;\n");
	if (a_material.m_normal.m_texture.m_handle != -1)
		output_append("normal_sampler;\n");
	if (a_material.m_bent_normal.m_texture.m_handle != -1)
		output_append("bent_normal_sampler;\n");
	if (a_material.m_height.m_texture.m_handle != -1)
		output_append("height_sampler;\n");
	if (a_material.m_anisotropy.m_texture.m_handle != -1)
		output_append("anisotrophy_sampler;\n");
	if (a_material.m_opacity.m_texture.m_handle != -1)
		output_append("opacity_sampler;\n");

	return output;
}

std::string material_factors_ubo(const ror::Material &a_material, rhi::ShaderBuffer &a_shader_buffer)
{
	std::string result{"\nlayout(std140, set = 1, binding = 0) uniform factors\n{\n\t"};        // TODO: Abstract out the set and binding
	std::string output{};

#define create_component(name, format, count, var)      \
	{                                                   \
		output.append(var);                             \
		a_shader_buffer.add_entry(name, format, count); \
	}

	if (a_material.m_base_color.m_type == ror::Material::ComponentType::factor || a_material.m_base_color.m_type == ror::Material::ComponentType::factor_texture)
		create_component("base_color_factor", rhi::Format::float32_4, 1, "vec4  base_color_factor;\n\t")
	if (a_material.m_diffuse_color.m_type == ror::Material::ComponentType::factor || a_material.m_diffuse_color.m_type == ror::Material::ComponentType::factor_texture)
		create_component("diffuse_color_factor", rhi::Format::float32_4, 1, "vec4  diffuse_color_factor;\n\t")
	if (a_material.m_specular_glossyness.m_type == ror::Material::ComponentType::factor || a_material.m_specular_glossyness.m_type == ror::Material::ComponentType::factor_texture)
		create_component("specular_glossyness_factor", rhi::Format::float32_4, 1, "vec4  specular_glossyness_factor;\n\t")
	if (a_material.m_emissive.m_type == ror::Material::ComponentType::factor || a_material.m_emissive.m_type == ror::Material::ComponentType::factor_texture)
		create_component("emissive_factor", rhi::Format::float32_4, 1, "vec4  emissive_factor;\n\t")
	if (a_material.m_anisotropy.m_type == ror::Material::ComponentType::factor || a_material.m_anisotropy.m_type == ror::Material::ComponentType::factor_texture)
		create_component("anisotrophy_factor", rhi::Format::float32_4, 1, "vec4  anisotrophy_factor;\n\t")
	if (a_material.m_transmission.m_type == ror::Material::ComponentType::factor || a_material.m_transmission.m_type == ror::Material::ComponentType::factor_texture)
		create_component("transmission_factor", rhi::Format::float32_2, 1, "vec2  transmission_factor;\n\t")
	if (a_material.m_sheen_color.m_type == ror::Material::ComponentType::factor || a_material.m_sheen_color.m_type == ror::Material::ComponentType::factor_texture)
		create_component("sheen_color_factor", rhi::Format::float32_3, 1, "vec3  sheen_color_factor;\n\t")
	if (a_material.m_sheen_roughness.m_type == ror::Material::ComponentType::factor || a_material.m_sheen_roughness.m_type == ror::Material::ComponentType::factor_texture)
		create_component("sheen_roughness_factor", rhi::Format::float32_1, 1, "float sheen_roughness_factor;\n\t")
	if (a_material.m_clearcoat_normal.m_type == ror::Material::ComponentType::factor || a_material.m_clearcoat_normal.m_type == ror::Material::ComponentType::factor_texture)
		create_component("clearcoat_normal_factor", rhi::Format::float32_2, 1, "vec2  clearcoat_normal_factor;\n\t")
	if (a_material.m_clearcoat.m_type == ror::Material::ComponentType::factor || a_material.m_clearcoat.m_type == ror::Material::ComponentType::factor_texture)
		create_component("clearcoat_factor", rhi::Format::float32_1, 1, "float clearcoat_factor;\n\t")
	if (a_material.m_clearcoat_roughness.m_type == ror::Material::ComponentType::factor || a_material.m_clearcoat_roughness.m_type == ror::Material::ComponentType::factor_texture)
		create_component("clearcoat_roughness_factor", rhi::Format::float32_1, 1, "float clearcoat_roughness_factor;\n\t")
	if (a_material.m_metallic.m_type == ror::Material::ComponentType::factor || a_material.m_metallic.m_type == ror::Material::ComponentType::factor_texture)
		create_component("metallic_factor", rhi::Format::float32_1, 1, "float metallic_factor;\n\t")
	if (a_material.m_roughness.m_type == ror::Material::ComponentType::factor || a_material.m_roughness.m_type == ror::Material::ComponentType::factor_texture)
		create_component("roughness_factor", rhi::Format::float32_1, 1, "float roughness_factor;\n\t")
	if (a_material.m_occlusion.m_type == ror::Material::ComponentType::factor || a_material.m_occlusion.m_type == ror::Material::ComponentType::factor_texture)
		create_component("occlusion_factor", rhi::Format::float32_1, 1, "float occlusion_factor;\n\t")
	if (a_material.m_normal.m_type == ror::Material::ComponentType::factor || a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture)
		create_component("normal_factor", rhi::Format::float32_1, 1, "float normal_factor;\n\t")
	if (a_material.m_bent_normal.m_type == ror::Material::ComponentType::factor || a_material.m_bent_normal.m_type == ror::Material::ComponentType::factor_texture)
		create_component("bent_normal_factor", rhi::Format::float32_1, 1, "float bent_normal_factor;\n\t")
	if (a_material.m_height.m_type == ror::Material::ComponentType::factor || a_material.m_height.m_type == ror::Material::ComponentType::factor_texture)
		create_component("height_factor", rhi::Format::float32_1, 1, "float height_factor;\n\t")
	if (a_material.m_anisotropy.m_type == ror::Material::ComponentType::factor || a_material.m_anisotropy.m_type == ror::Material::ComponentType::factor_texture)
		create_component("anisotrophy_factor", rhi::Format::float32_1, 1, "float anisotrophy_factor;\n\t")
	if (a_material.m_opacity.m_type == ror::Material::ComponentType::factor || a_material.m_opacity.m_type == ror::Material::ComponentType::factor_texture)
		create_component("opacity_factor", rhi::Format::float32_1, 1, "float opacity_factor;\n\t")

	// Unconditional factor of reflectance needs to be there
	create_component("reflectance_factor", rhi::Format::float32_1, 1, "float reflectance_factor;\n")

	// TODO: The following needs some condition, add that later for subsurface scattering support
	// if (a_material.m_subsurface_color.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("\tvec4  subsurface_color_factor;\n\t");
	// if (a_material.m_subsurface_radius.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("vec4  subsurface_radius_factor;\n\t");
	// if (a_material.m_subsurface_scattering.m_type != ror::Material::MaterialComponentType::texture_only)
	//	output.append("float subsurface_scattering_factor;\n\t");

	if (output.empty())
		return "";

	result.append(output);
	result.append("} in_factors;\n");

	return result;
}

std::string get_material(const ror::Material &a_material)
{
#define stringify_helper(x) #x
#define stringify(x) stringify_helper(x)

#define get_material_component(name, default_value)                                     \
	if (a_material.m_##name.m_type != ror::Material::ComponentType::none)               \
		output.append("\tmaterial." stringify(name) " = get_" stringify(name) "();\n"); \
	else                                                                                \
		output.append("\tmaterial." stringify(name) " = " stringify(default_value) ";\n")

	(void) a_material;
	std::string output{"Material get_material()\n{\n\tMaterial material;\n\n"};

	// Defaults values for Material copied from Filament's defaults
	get_material_component(base_color, vec4(1.0));
	get_material_component(emissive, vec4(0.0));
	get_material_component(bent_normal, vec3(0.0, 0.0, 1.0));
	get_material_component(roughness, 1.0);
	get_material_component(metallic, 0.0);
	get_material_component(occlusion, 1.0);
	get_material_component(anisotropy, 0.0);
	get_material_component(clearcoat, 1.0);
	get_material_component(clearcoat_roughness, 0.0);
	get_material_component(clearcoat_normal, vec3(0.0, 0.0, 1.0));
	get_material_component(sheen_color, vec3(0.0));
	get_material_component(sheen_roughness, 0.0);
	get_material_component(subsurface_color, vec3(1.0));
	// get_material_component(subsurface, 12.234);

	// Some manual appends, not using the macro
	// TODO: View doesn't fit in material, this belongs in fragment but its a hack for now
	// This works around having to call get_normal() twice
	output.append("\tmaterial.view = normalize(in_per_view_uniforms.camera_position - in_vertex_position.xyz);\n");

	// If we have a normal map use the overloaded get_normal(N, V)
	if (a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture ||
		a_material.m_normal.m_type == ror::Material::ComponentType::texture)
		output.append("\tmaterial.normal = get_normal(in_vertex_normal, material.view);\n");
	else        // otherwise use the standard get_normal()
		get_material_component(normal, vec3(0.0, 0.0, 1.0));

	// Can't use the following because reflectance is not a Component
	if (a_material.m_metallic.m_type != ror::Material::ComponentType::none)
		output.append("\tmaterial.reflectance = get_reflectance();\n");
	else
		output.append("\tmaterial.reflectance = 0.5;\n");

	if (a_material.m_specular_glossyness.m_type != ror::Material::ComponentType::none)
	{
		output.append("\tmaterial.specular_color = get_specular().xyz;\n");
		output.append("\tmaterial.glossyness = get_specular().w;\n");
	}
	else
	{
		output.append("\tmaterial.specular_color = vec3(1.0);\n");
		output.append("\tmaterial.glossyness = 1.0;\n");
	}

	if (a_material.m_anisotropy.m_type != ror::Material::ComponentType::none)
	{
		output.append("\tmaterial.anisotropy_direction = get_anisotropy().xyz;\n");
		output.append("\tmaterial.anisotropy = get_anisotropy().w;\n");
	}
	else
	{
		output.append("\tmaterial.anisotropy_direction = vec3(1.0, 0.0, 0.0);\n");
		output.append("\tmaterial.anisotropy = 0.0;\n");
	}

	output.append("\tmaterial.height = 0.0;\n");
	output.append("\tmaterial.opacity = 1.0;\n");

	output.append("\n\treturn material;\n}\n");

	return output;
}

std::string fs_set_output(const ror::Material &a_material)
{
	std::string result{};

	// Read getters.frag.glsl resource and create a string_view
	auto            &getters_resource = ror::load_resource("shaders/getters.frag.glsl", ror::ResourceSemantic::shaders);
	std::string_view getters_code{reinterpret_cast<const char *>(getters_resource.data().data()), getters_resource.data().size()};

	result.append(get_material(a_material));
	result.append("\n");
	result.append(getters_code);
	result.append("\n");

	return result;
}

std::string fs_set_main(const ror::Material &a_material)
{
	// Read all the shader snippets
	// Read brdf.frag.glsl resource and create a string_view
	auto            &brdf_resource = ror::load_resource("shaders/brdf.frag.glsl", ror::ResourceSemantic::shaders);
	std::string_view brdf_code{reinterpret_cast<const char *>(brdf_resource.data().data()), brdf_resource.data().size()};

	// Read temporary_structs.frag.glsl resource and create a string_view
	auto            &temporary_structs_resource = ror::load_resource("shaders/temporary_structs.frag.glsl", ror::ResourceSemantic::shaders);
	std::string_view temporary_structs_code{reinterpret_cast<const char *>(temporary_structs_resource.data().data()), temporary_structs_resource.data().size()};

	// Read shading_standard.frag.glsl resource and create a string_view
	auto            &shading_standard_resource = ror::load_resource("shaders/shading_standard.frag.glsl", ror::ResourceSemantic::shaders);
	std::string_view shading_standard_code{reinterpret_cast<const char *>(shading_standard_resource.data().data()), shading_standard_resource.data().size()};

	// Read lighting.frag.glsl resource and create a string_view
	auto            &lighting_resource = ror::load_resource("shaders/lighting.frag.glsl", ror::ResourceSemantic::shaders);
	std::string_view lighting_code{reinterpret_cast<const char *>(lighting_resource.data().data()), lighting_resource.data().size()};

	// Read main.frag.glsl resource and create a string_view
	auto            &main_resource = ror::load_resource("shaders/main.frag.glsl", ror::ResourceSemantic::shaders);
	std::string_view main_code{reinterpret_cast<const char *>(main_resource.data().data()), main_resource.data().size()};

	std::string output{};

	output.append("\n");
	output.append(brdf_code);
	output.append("\n");
	output.append(temporary_structs_code);
	output.append("\n");
	output.append(fs_set_output(a_material));
	output.append("\n");
	output.append(shading_standard_code);
	output.append("\n");
	output.append(lighting_code);
	output.append("\n");
	output.append(main_code);

	return output;
}

std::string generate_primitive_fragment_shader(const ror::Mesh &a_mesh, const std::vector<ror::Material, rhi::BufferAllocator<ror::Material>> &a_materials, uint32_t a_primitive_index)
{
	const auto   &vertex_descriptor = a_mesh.m_attribute_vertex_descriptors[a_primitive_index];
	ror::Material material{};        // Default material if no material available for this mesh primitive

	if (a_mesh.m_material_indices[a_primitive_index] != -1)
		material = a_materials[ror::static_cast_safe<size_t>(a_mesh.m_material_indices[a_primitive_index])];

	std::string output{"#version 450\n\nprecision highp float;\nprecision highp int;\n\n"};        // TODO: abstract out version

	rhi::ShaderBuffer sb{"factors", rhi::Layout::std140}; // TODO: Move this out

	// write out inputs from fragment shader
	output.append(rhi::fragment_shader_input_output(vertex_descriptor));
	output.append(per_view_common(0, 0));
	output.append(per_frame_common(0, 1));
	output.append(material_samplers(material));
	output.append(material_factors_ubo(material, sb));
	output.append(fs_light_common(1, 2, 0, fs_directional_light_common_str));        // TODO: Make conditional, and abstract out lights_count, set and bindings
	output.append(fs_light_common(1, 2, 1, fs_point_light_common_str));              // TODO: Make conditional, and abstract out lights_count, set and bindings
	output.append(fs_light_common(1, 2, 2, fs_spot_light_common_str));               // TODO: Make conditional, and abstract out lights_count, set and bindings
	output.append(texture_lookups(material));
	output.append(fs_set_main(material));

	return output;
}

}        // namespace rhi
