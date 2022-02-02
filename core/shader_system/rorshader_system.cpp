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

#include "foundation/rorutilities.hpp"
#include "graphics/rormodel.hpp"
#include "profiling/rorlog.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rortypes.hpp"
#include "shader_system/rorshader_system.hpp"
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
		case VertexFormat::float32_3x3:			return " mat3x3";
		case VertexFormat::float32_3x4:			return " mat3x4";
		case VertexFormat::float32_4x4:			return " mat4x4";
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
		case VertexFormat::float32_3x3:
		case VertexFormat::float32_3x4:
		case VertexFormat::float32_4x4:
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

std::string vs_morph_attribute_common(uint32_t a_count, const std::string &a_name, std::string a_type)
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

// TODO: Should these things be conditional?
const std::string vs_frame_common_str = R"com(
layout(std140, set = @, binding = @) uniform frame_transform
{
	mat4 mvp_mat4;
	// mat4 projection_mat4;
	// mat4 model_mat4;
	// mat4 view_mat4;
	mat3 normal_mat4;
	// vec4 camera_pos;
	// vec4 light_pos;
} in_frame_transforms;
)com";

std::string vs_frame_common(uint32_t a_set, uint32_t a_binding)
{
	auto str{vs_frame_common_str};

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
vec3 transform_normal(vec3 normal, uint index)
{
	vec4 rotation      = in_joint_transforms.joint_transforms[index].rotation;
	vec3 scale_inverse = in_joint_transforms.joint_transforms[index].scale_inverse;

	normal *= scale_inverse;                                                                       // apply the inverse of the non-uniform scales
	normal += 2.0 * cross(rotation.xyz, cross(rotation.xyz, normal) + rotation.w * normal);        // apply the rigid transform (valid only for unit quaternions)

	return normal;
}
)scomn";

const std::string vs_skin_common_position_str = R"scomp(
vec3 transform_position(vec3 vertex, uint index)
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
	normal = transform_normal(normal, in_vertex_bone_id_@.x) * in_vertex_weight_@.x +
			 transform_normal(normal, in_vertex_bone_id_@.y) * in_vertex_weight_@.y +
			 transform_normal(normal, in_vertex_bone_id_@.z) * in_vertex_weight_@.z +
			 transform_normal(normal, in_vertex_bone_id_@.w) * in_vertex_weight_@.w;
)ncom";

std::string vs_skin_normal(uint32_t a_joints_weights_count)
{
	auto result{vs_skin_normal_header_str};
	append_count_times(a_joints_weights_count, 8, result, vs_skin_normal_str);
	result.append("}");

	return result;
}

const std::string vs_skin_position_header_str{"\nvoid skin_position(inout vec3 position) \n{"};
const std::string vs_skin_position_str = R"pcom(
	position = transform_position(position, in_vertex_bone_id_@.x) * in_vertex_weight_@.x +
			   transform_position(position, in_vertex_bone_id_@.y) * in_vertex_weight_@.y +
			   transform_position(position, in_vertex_bone_id_@.z) * in_vertex_weight_@.z +
			   transform_position(position, in_vertex_bone_id_@.w) * in_vertex_weight_@.w;
)pcom";

std::string vs_skin_position(uint32_t a_joints_weights_count)
{
	auto result{vs_skin_position_header_str};
	append_count_times(a_joints_weights_count, 8, result, vs_skin_position_str);
	result.append("}");

	return result;
}

const std::string vs_set_position_str = R"spos(
void world_transform_position(inout vec4 vertex_position)
{
	// vertex_position = model * view * projection * vertex_position;
	vertex_position = in_frame_transforms.mvp_mat4 * vertex_position;
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
	vertex_normal = in_frame_transforms.normal_mat4 * vertex_normal;
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

			if (semantic < rhi::BufferSemantic::vertex_index)        // Only support vertex attributes that are bellow vertex_index, FIXME: what happens to custom ones?
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

	std::string result{"#version 450\n\n"};        // TODO: abstract out version

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
	result.append(vs_frame_common(0, 0));        // TODO: Abstract out set and binding

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

std::string generate_primitive_fragment_shader(const ror::Mesh &a_mesh, uint32_t a_index)
{
	auto &vertex_descriptor = a_mesh.m_attribute_vertex_descriptors[a_index];

	std::cout << "\nHere is a fragment shader\n\n";

	rhi::fragment_shader_input_output(vertex_descriptor, ror::Material{});

	std::cout << "\n\n\nEnd of shader";

	return "";
}

/*
#ifdef HAS_BASE_COLOR_TEXTURE
layout(set = 0, binding = 0) uniform sampler2D base_color_texture;
#endif

layout(location = 0) in vec4 in_pos;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec4 in_shadow_clip;

layout(location = 0) out vec4 o_color;

layout(set = 0, binding = 1) uniform GlobalUniform
{
	mat4 model;
	mat4 view_proj;
	vec3 camera_position;
}
global_uniform;

// Push constants come with a limitation in the size of data.
// The standard requires at least 128 bytes
layout(push_constant, std430) uniform PBRMaterialUniform
{
	vec4  base_color_factor;
	float metallic_factor;
	float roughness_factor;
}
pbr_material_uniform;

#include "lighting.h"

layout(set = 0, binding = 4) uniform LightsInfo
{
	Light directional_light;
} lights_info;

layout(set = 0, binding = 6) uniform highp sampler2DShadow tex_shadow;

*/
/*
The following results in something like:

Here is a fragment shader

layout (location = 0) in highp ivec3 in_vertex_normal;
layout (location = 1) in highp uvec3 in_vertex_position;
layout (location = 2) in highp ivec3 in_target_vertex_normal0;
layout (location = 3) in highp ivec3 in_target_vertex_position0;
layout (location = 4) in highp ivec3 in_target_vertex_normal1;
layout (location = 5) in highp ivec3 in_target_vertex_position1;
layout (location = 0) out vec4 out_color;

*/

std::string fragment_shader_input_output(const VertexDescriptor &a_vertex_descriptor, const ror::Material &a_material, uint32_t a_location_offset, uint32_t a_target_offset, std::string a_prefix)
{
	(void) a_vertex_descriptor;
	(void) a_material;
	(void) a_location_offset;
	(void) a_target_offset;
	(void) a_prefix;

	std::string result{};

	auto &attributes = a_vertex_descriptor.attributes();

	const std::string layout{"layout (location = "};
	const std::string in{" in"};
	const std::string precision{" highp"};

	for (auto &attrib : attributes)
	{
		auto semantic = attrib.semantics();
		if (semantic < rhi::BufferSemantic::vertex_index)        // Only support vertex attributes that are bellow vertex_index, FIXME: what happens to custom ones?
		{
			auto location     = attrib.location();
			auto format       = attrib.format();
			auto format_str   = attribute_format(format);
			auto semantic_str = get_format_semantic(semantic);

			std::string first_half{layout + std::to_string(location + a_location_offset) + ")"};
			std::string middle_half{precision + format_str};
			std::string second_half{(a_prefix != "" ? "_" + a_prefix : "") + "_" + semantic_str + (a_prefix != "" ? std::to_string(a_target_offset) : "") + ";\n"};

			std::string line{first_half + in + middle_half + in + second_half};
			result.append(line);
		}
	}

	std::string output{"layout (location = 0) out vec4 out_color;\n"};
	result.append(output);

	return result;
}

}        // namespace rhi
