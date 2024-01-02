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
#include "renderer/rorrenderer.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include "shader_system/rorshader_system.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace ror
{

std::string attribute_format(rhi::VertexFormat a_format)
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
		case rhi::VertexFormat::struct_0:        return " ";
		case rhi::VertexFormat::struct_1:        return " ";
		case rhi::VertexFormat::bool32_1:        return " bool";
		case rhi::VertexFormat::bool32_2:        return " bvec2";
		case rhi::VertexFormat::bool32_3:        return " bvec3";
		case rhi::VertexFormat::bool32_4:        return " bvec4";
		case rhi::VertexFormat::int8_1:          return " int";
		case rhi::VertexFormat::int8_2:          return " ivec2";
		case rhi::VertexFormat::int8_3:          return " ivec3";
		case rhi::VertexFormat::int8_4:          return " ivec4";
		case rhi::VertexFormat::int16_1:         return " int";
		case rhi::VertexFormat::int16_2:         return " ivec2";
		case rhi::VertexFormat::int16_3:         return " ivec3";
		case rhi::VertexFormat::int16_4:         return " ivec4";
		case rhi::VertexFormat::int32_1:         return " int";
		case rhi::VertexFormat::int32_2:         return " ivec2";
		case rhi::VertexFormat::int32_3:         return " ivec3";
		case rhi::VertexFormat::int32_4:         return " ivec4";
		case rhi::VertexFormat::half16_1:        return " int";
		case rhi::VertexFormat::half16_2:        return " ivec2";
		case rhi::VertexFormat::half16_3:        return " ivec3";
		case rhi::VertexFormat::half16_4:        return " ivec4";

		case rhi::VertexFormat::float32_1:       return " float";
		case rhi::VertexFormat::float32_2:       return " vec2";
		case rhi::VertexFormat::float32_3:       return " vec3";
		case rhi::VertexFormat::float32_4:       return " vec4";
		case rhi::VertexFormat::uint8_1:         return " uint";
		case rhi::VertexFormat::uint8_2:         return " uvec2";
		case rhi::VertexFormat::uint8_3:         return " uvec3";
		case rhi::VertexFormat::uint8_4:         return " uvec4";
		case rhi::VertexFormat::uint16_1:        return " uint";
		case rhi::VertexFormat::uint16_2:        return " uvec2";
		case rhi::VertexFormat::uint16_3:        return " uvec3";
		case rhi::VertexFormat::uint16_4:        return " uvec4";
		case rhi::VertexFormat::uint32_1:        return " uint";
		case rhi::VertexFormat::uint32_2:        return " uvec2";
		case rhi::VertexFormat::uint32_3:        return " uvec3";
		case rhi::VertexFormat::uint32_4:        return " uvec4";

		case rhi::VertexFormat::int8_1_norm:     return " int";
		case rhi::VertexFormat::int8_2_norm:     return " ivec2";
		case rhi::VertexFormat::int8_3_norm:     return " ivec3";
		case rhi::VertexFormat::int8_4_norm:     return " ivec4";
		case rhi::VertexFormat::int16_1_norm:    return " int";
		case rhi::VertexFormat::int16_2_norm:    return " ivec2";
		case rhi::VertexFormat::int16_3_norm:    return " ivec3";
		case rhi::VertexFormat::int16_4_norm:    return " ivec4";
		case rhi::VertexFormat::uint8_1_norm:    return " uint";
		case rhi::VertexFormat::uint8_2_norm:    return " uvec2";
		case rhi::VertexFormat::uint8_3_norm:    return " uvec3";
		case rhi::VertexFormat::uint8_4_norm:    return " uvec4";
		case rhi::VertexFormat::uint16_1_norm:   return " uint";
		case rhi::VertexFormat::uint16_2_norm:   return " uvec2";
		case rhi::VertexFormat::uint16_3_norm:   return " uvec3";
		case rhi::VertexFormat::uint16_4_norm:   return " uvec4";

		case rhi::VertexFormat::uint8_custom:    return " uint";
		case rhi::VertexFormat::uint16_custom:   return " uint";
		case rhi::VertexFormat::uint32_custom:   return " uint";
		case rhi::VertexFormat::float32_custom:  return " float";

		case rhi::VertexFormat::invalid:
		case rhi::VertexFormat::float32_2x2:
		case rhi::VertexFormat::float32_2x3:
		case rhi::VertexFormat::float32_2x4:
		case rhi::VertexFormat::float32_3x2:
		case rhi::VertexFormat::float32_3x3:
		case rhi::VertexFormat::float32_3x4:
		case rhi::VertexFormat::float32_4x2:
		case rhi::VertexFormat::float32_4x3:
		case rhi::VertexFormat::float32_4x4:
		case rhi::VertexFormat::float64_2x2:
		case rhi::VertexFormat::float64_2x3:
		case rhi::VertexFormat::float64_2x4:
		case rhi::VertexFormat::float64_3x2:
		case rhi::VertexFormat::float64_3x3:
		case rhi::VertexFormat::float64_3x4:
		case rhi::VertexFormat::float64_4x2:
		case rhi::VertexFormat::float64_4x3:
		case rhi::VertexFormat::float64_4x4:
		case rhi::VertexFormat::float64_1:
		case rhi::VertexFormat::float64_2:
		case rhi::VertexFormat::float64_3:
		case rhi::VertexFormat::float64_4:

		case rhi::VertexFormat::uint64_1:
		case rhi::VertexFormat::uint64_2:
		case rhi::VertexFormat::uint64_3:
		case rhi::VertexFormat::uint64_4:

		case rhi::VertexFormat::int1010102_norm:
		case rhi::VertexFormat::uint1010102_norm:
		case rhi::VertexFormat::int101111_norm:
		case rhi::VertexFormat::uint101111_norm:
		case rhi::VertexFormat::uint8_4_norm_bgra:
			ror::log_critical("Format {} not supported but requested", a_format);
			assert(0 && "Format not supported");
	}
	// clang-format on

	return "";
}

void replace_first(const std::string &a_search, const std::string &a_replacement, std::string &a_result)
{
	auto index = a_result.find(a_search);
	assert(index != std::string::npos && "Index is out of range in replace_first, this means could find search term");

	a_result.replace(index, a_search.length(), a_replacement);
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

std::string vs_morph_common(size_t a_weights_count, const ror::Renderer &a_renderer)
{
	const std::string morph_weights_str{"\nconst uint weights_count = @;\n\n"};

	auto str{morph_weights_str};
	replace_next_at(a_weights_count, str);

	auto morphs_weights_buffer  = a_renderer.shader_buffer("morphs_weights");
	auto morphs_weights_uniform = morphs_weights_buffer->to_glsl_string("readonly");

	str.append(morphs_weights_uniform);
	str.append("\n");

	return str;
}

const std::string vs_morph_stuff_header_str{"\nvoid morph_@(inout @ @)\n{\n\tuint weight_offset = in_nodes_offsets.node_offset.z;\n"};
const std::string vs_morph_stuff_line_str{"\t@ += in_morphs_weights.morph_weights[weight_offset + @] * in_target_@@;\n"};

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

	str.append("}\n");
	return str;
}

// TODO: These should be conditional
// There should be per_view vs per_renderable uniforms, view requires many more matrices while renderable only needs MVP, Normal matrices
// TODO: Separate this by view and renderable
std::string per_view_common(const ror::Renderer &a_renderer)
{
	auto per_view_buffer  = a_renderer.shader_buffer("per_view_uniform");
	auto per_view_uniform = per_view_buffer->to_glsl_string();

	auto model_buffer  = a_renderer.shader_buffer("nodes_models");
	auto model_uniform = model_buffer->to_glsl_string("readonly");

	auto nodes_offsets_buffer  = a_renderer.shader_buffer("nodes_offsets");
	auto nodes_offsets_uniform = nodes_offsets_buffer->to_glsl_string();

	per_view_uniform.append(model_uniform);
	per_view_uniform.append(nodes_offsets_uniform);

	per_view_uniform.append("\n");

	return per_view_uniform;
}

const std::string vs_skin_common_str = R"scom(
const uint joints_count = @;

struct trs_transform
{
	vec4 rotation;
	vec3 translation;
	vec3 scale;
@

layout(std430, set = @, binding = @) buffer joint_offset_uniform
{
	uint16_t joint_redirect[joints_count];
} in_joint_redirects;

layout(std140, set = 0, binding = 29) readonly buffer joint_inverse_bind_matrices
{
	mat4 joint_inverse_matrix[];
} in_joint_inverse_bind_matrices;

)scom";

const std::string vs_skin_common_normal_str = R"scomn(
// Source filament shaders from https://google.github.io/filament/Filament.html
vec3 skin_normal(vec3 normal, uint index)
{
	// vec4 rotation      = in_joint_redirects.joint_redirect[index].rotation;
	// vec3 scale_inverse = in_joint_redirects.joint_redirect[index].scale_inverse;

	// normal *= scale_inverse;                                                                       // apply the inverse of the non-uniform scales
	// normal += 2.0 * cross(rotation.xyz, cross(rotation.xyz, normal) + rotation.w * normal);        // apply the rigid transform (valid only for unit quaternions)

	return normal;
}
)scomn";

const std::string vs_skin_common_position_str = R"scomp(
vec3 skin_position(vec3 vertex, uint index)
{
	// vec4 rotation    = in_joint_redirects.joint_redirect[index].rotation;
	// vec3 translation = in_joint_redirects.joint_redirect[index].translation;
	// vec3 scale       = in_joint_redirects.joint_redirect[index].scale;

	// vertex *= scale;                                                                               // apply the non-uniform scales
	// vertex += 2.0 * cross(rotation.xyz, cross(rotation.xyz, vertex) + rotation.w * vertex);        // apply the rigid transform (valid only for unit quaternions)
	// vertex += translation;                                                                         // apply the translation

    uvec4 node_offset = in_nodes_offsets.node_offset;

	mat4 inverse_transform  = inverse(in_nodes_models.node_model[node_offset.x]);
	mat4 joint_inverse_bind = in_joint_inverse_bind_matrices.joint_inverse_matrix[index];
	mat4 joint_transform    = in_nodes_models.node_model[uint(in_joint_redirects.joint_redirect[index]) + node_offset.y];

    joint_transform = inverse_transform * joint_transform * joint_inverse_bind;

	return (joint_transform  * vec4(vertex, 1.0)).xyz;
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

const std::string vs_set_position_str = R"spos(
void world_transform_position(inout vec4 vertex_position)
{
    // You need to retain 'w' only after 'perspective' transform for fixed function "prespective division"
    // hence reseting w=1.0 to make sure skinning or morphing hasn't messed it up
    // Details https://www.tomdalling.com/blog/modern-opengl/explaining-homogenous-coordinates-and-projective-geometry/

    uint node_index = in_nodes_offsets.node_offset.x;
	vertex_position = in_nodes_models.node_model[node_index] * vec4(vertex_position.xyz, 1.0);
}

void set_position()
{
	vec4 vertex_position = in_vertex_position;

	@morph_vertex_position(vertex_position);
	@skin_position(vertex_position.xyz);
	world_transform_position(vertex_position);

	@out_vertex_position = vertex_position;

	gl_Position = in_per_view_uniform.projection_mat4 * in_per_view_uniform.view_mat4 * vec4(vertex_position.xyz, 1.0);
}
)spos";

const std::string vs_set_normal_str = R"snor(
void world_transform_normal(inout vec3 vertex_normal)
{
    uint node_index = in_nodes_offsets.node_offset.x;
    mat3 normal_matrix = mat3(in_nodes_models.node_model[node_index]);

/*
    // If there ever is non-uniform scale in any model matrices we need to do the following
    vec3 c0 = normal_matrix[0];
    vec3 c1 = normal_matrix[1];
    vec3 c2 = normal_matrix[2];

    c0 = normalize(c0);
    c1 = normalize(c1);
    c2 = normalize(c2);

    normal_matrix[0] = c0;
    normal_matrix[1] = c1;
    normal_matrix[2] = c2;

    normal_matrix = inverse(normal_matrix);
    normal_matrix = transpose(normal_matrix);
*/

	vertex_normal = normal_matrix * vertex_normal;
	vertex_normal = normalize(vertex_normal);
}

void set_normal()
{
	vec3 vertex_normal = in_vertex_normal;

	@morph_vertex_normal(vertex_normal);
	@skin_normal(vertex_normal);
	world_transform_normal(vertex_normal);

	out_vertex_normal = vertex_normal;
}
)snor";

// Unlike normals tangents can be xforms just like positions using 'skin_position' and 'world_transform_position'
const std::string vs_set_tangent_str = R"stan(
void set_tangent()
{
	vec4 vertex_tangent = in_vertex_tangent;

	@morph_vertex_tangent(vertex_tangent);
	@skin_position(vertex_tangent.xyz);
	world_transform_position(vertex_tangent);

	out_vertex_tangent = vertex_tangent;
}
)stan";

const std::string vs_set_texture_coordinate_str = R"suv0(
void set_texture_coordinate_@()
{
	vec2 vertex_texture_coordinate = in_vertex_texture_coord_@;

	@morph_vertex_texture_coord_@(vertex_texture_coordinate);
	out_vertex_texture_coord_@ = vertex_texture_coordinate;
}
)suv0";

std::string vs_set_texture_coordinate(uint32_t a_uvs_count, const std::array<uint32_t, 3> a_uv_morph_targets)
{
	assert(a_uvs_count < 4 && "Too many UV sets provided, only support upto 3");

	std::string result{};

	for (uint32_t i = 0; i < a_uvs_count; ++i)
	{
		auto str{vs_set_texture_coordinate_str};
		replace_all_ats(2, i, str);        // Replace the first 2 @s with numbers

		// Replace the next @ with comment or remove it
		if (a_uv_morph_targets[i] > 0)
			replace_next_at("", str);
		else
			replace_next_at("//", str);

		replace_all_ats(2, i, str);        // Replace the last 2 @s with numbers

		result.append(str);
	}

	return result;
}

const std::string vs_set_color_str = R"scol0(
void set_color_@()
{
	vec4 vertex_color = in_vertex_color_@;

	@morph_vertex_color_@(vertex_color);
	out_vertex_color_@ = vertex_color;
}
)scol0";

std::string vs_set_color(uint32_t a_colors_count, const std::array<uint32_t, 2> a_colors_morph_targets)
{
	assert(a_colors_count < 3 && "Too many color sets provided, only support upto 2");

	std::string result{};

	for (uint32_t i = 0; i < a_colors_count; ++i)
	{
		auto str{vs_set_color_str};
		replace_all_ats(2, i, str);        // Replace the first 2 @s with numbers

		// Replace the next @ with comment or remove it
		if (a_colors_morph_targets[i] > 0)
			replace_next_at("", str);
		else
			replace_next_at("//", str);

		replace_all_ats(2, i, str);        // Replace the last 2 @s with numbers

		result.append(str);
	}

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

std::string vertex_shader_input_output(const rhi::VertexDescriptor &a_vertex_descriptor, uint32_t a_target_offset, const std::string &a_prefix, bool a_output, bool a_depth_shadow)
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

			if (is_attribute_required_in_pass(semantic, a_depth_shadow))
			{
				auto location          = attrib.location();
				auto in_out_format_str = attribute_format(in_out_format);
				auto semantic_str      = get_format_semantic(semantic);

				std::string first_half{layout + std::to_string(location) + ")"};
				std::string middle_half{precision + in_out_format_str};
				std::string second_half{(a_prefix != "" ? "_" + a_prefix : "") + "_" + semantic_str + (a_prefix != "" ? std::to_string(a_target_offset) : "") + ";\n"};

				std::string line{first_half + in + middle_half + in + second_half};

				result.append(line);

				// TODO: Add support for outputting normal + tangent or TBN
				if (semantic < rhi::BufferSemantic::vertex_bone_id_0 && !a_depth_shadow)        // Only output vertex attributes that are bellow vertex_bone_id_0, FIXME: Add support for custom ones
					result.append(a_output ? std::string{first_half + out + middle_half + out + second_half} : "");
			}
		}
	}

	return result;
}

// TODO: Use https://github.com/aras-p/glsl-optimizer on the output for further optimising the GLSL
std::string generate_primitive_vertex_shader(const ror::Model &a_model, uint32_t a_mesh_index, uint32_t a_primitive_index, rhi::RenderpassType a_renderpass_type, const ror::Renderer &a_renderer)
{
	(void) a_renderer;
	const auto &mesh                     = a_model.meshes()[a_mesh_index];
	auto       &vertex_descriptor        = mesh.vertex_descriptor(a_primitive_index);
	auto       &vertex_target_descriptor = mesh.target_descriptor(a_primitive_index);
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
	auto        has_morphs               = mesh.has_morphs();
	auto        has_morphs_normal        = mesh.has_morphs(rhi::BufferSemantic::vertex_normal);
	auto        has_morphs_tangent       = mesh.has_morphs(rhi::BufferSemantic::vertex_tangent);
	auto        is_depth_shadow          = (a_renderpass_type == rhi::RenderpassType::depth || a_renderpass_type == rhi::RenderpassType::shadow);

	std::unordered_map<rhi::BufferSemantic, std::pair<uint32_t, bool>> targets_count{
	    {rhi::BufferSemantic::vertex_position, {0, true}},
	    {rhi::BufferSemantic::vertex_normal, {0, has_normal}},
	    {rhi::BufferSemantic::vertex_tangent, {0, has_tangent}},
	    {rhi::BufferSemantic::vertex_texture_coord_0, {0, has_uv_0}},
	    {rhi::BufferSemantic::vertex_texture_coord_1, {0, has_uv_1}},
	    {rhi::BufferSemantic::vertex_texture_coord_2, {0, has_uv_2}},
	    {rhi::BufferSemantic::vertex_color_0, {0, has_color_0}},
	    {rhi::BufferSemantic::vertex_color_1, {0, has_color_1}}};

	std::string result{"#version 450\n\n#extension GL_EXT_shader_16bit_storage : require\nprecision highp float;\nprecision highp int;\n\n"};        // TODO: abstract out version

	// Write out vertex shader input output
	result.append(ror::vertex_shader_input_output(vertex_descriptor, 0, "", true, is_depth_shadow));

	uint32_t j = 0;
	for (const auto &v : vertex_target_descriptor)
	{
		result.append(ror::vertex_shader_input_output(v, j++, "target", false, is_depth_shadow));

		const auto &attributes = v.attributes();

		for (auto &attrib : attributes)
		{
			const auto semantic = attrib.semantics();
			targets_count[semantic].first++;
		}
	}

	// Write out common methods
	result.append(per_view_common(a_renderer));

	// Only add morph common if mesh has weights and has morph targets
	if (has_morphs)
		result.append(vs_morph_common(mesh.weights_count(), a_renderer));

	// Write out common morph target functions
	for (auto &tc : targets_count)
	{
		const auto out_format_iter = attrib_in_out_format.find(tc.first);

		assert(out_format_iter != attrib_in_out_format.end() && "Can't find supported target attribute");

		auto in_out_format     = out_format_iter->second;
		auto in_out_format_str = attribute_format(in_out_format);
		auto semantic_str      = get_format_semantic(tc.first);

		if (tc.second.first > 0 && is_attribute_required_in_pass(tc.first, is_depth_shadow))
			result.append(vs_morph_attribute_common(tc.second.first, semantic_str, in_out_format_str));
	}

	if (has_bone_id_0)
	{
		assert(mesh.skin_index() > -1 && "Skin isn't valid in the mesh");
		const auto &skin = a_model.skins()[static_cast<size_t>(mesh.skin_index())];
		assert(skin.joints_count() > 0 && "No joints available in this skin");

		result.append(vs_skin_common(static_cast<uint32_t>(skin.joints_count()), settings().skin_joints_set(), settings().skin_joints_binding(), has_normal));
	}

	auto joint_sets{0u};
	if (has_bone_id_0)
		joint_sets++;

	if (has_bone_id_1)
		joint_sets++;

	if (has_bone_id_0 || has_bone_id_1)
	{
		if (has_normal)
			result.append(vs_skin_normal(joint_sets));        // Max 2 joint/weights sets

		result.append(vs_skin_position(joint_sets));        // Max 2 joint/weights sets
	}

#define setup_for_depth_shadow(morphs)  \
	if (has_morphs && morphs)           \
		replace_next_at("", tmp);       \
	else                                \
		replace_next_at("//", tmp);     \
	if (has_bone_id_0 || has_bone_id_1) \
		replace_next_at("", tmp);       \
	else                                \
		replace_next_at("//", tmp)

	{
		auto tmp{vs_set_position_str};
		setup_for_depth_shadow(true);

		if (is_depth_shadow)
			replace_next_at("//", tmp);        // Final @ to enable disable out_vertex_position
		else
			replace_next_at("", tmp);

		result.append(tmp);        // Add set_position
	}

	if (!is_depth_shadow)
	{
		if (has_normal)
		{
			auto tmp{vs_set_normal_str};
			setup_for_depth_shadow(has_morphs_normal);

			result.append(tmp);        // Add set_normal
		}

		if (has_tangent)
		{
			auto tmp{vs_set_tangent_str};
			setup_for_depth_shadow(has_morphs_tangent);
			result.append(tmp);        // Add set_tangent
		}
	}

	auto uv_sets{0u};
	if (has_uv_0)
		uv_sets++;

	if (has_uv_1)
		uv_sets++;

	if (has_uv_2)
		uv_sets++;

	if (uv_sets > 0 && !is_depth_shadow)
		result.append(vs_set_texture_coordinate(uv_sets, {targets_count[rhi::BufferSemantic::vertex_texture_coord_0].first,
		                                                  targets_count[rhi::BufferSemantic::vertex_texture_coord_1].first,
		                                                  targets_count[rhi::BufferSemantic::vertex_texture_coord_2].first}));        // Max 3 UV sets count

	auto color_sets{0u};
	if (has_color_0)
		color_sets++;

	if (has_color_1)
		color_sets++;

	if (color_sets > 0 && !is_depth_shadow)
		result.append(vs_set_color(color_sets, {targets_count[rhi::BufferSemantic::vertex_color_0].first,
		                                        targets_count[rhi::BufferSemantic::vertex_color_1].first}));        // Max 2 color sets count

	result.append(vs_set_main((is_depth_shadow ? false : has_normal),
	                          (is_depth_shadow ? false : has_tangent),
	                          false,        //  bent normal
	                          (is_depth_shadow ? false : uv_sets),
	                          (is_depth_shadow ? false : color_sets)));        // Add main as well

	return result;
}

// Vertex shader methods finish here, now starts fragment shader methods

// TODO: Work out the vec3 padding issues in this UBO
const std::string fs_directional_light_common_str = R"com(
const uint directional_lights_count = @;

struct Directional_lights
{
	mat4  mvp;
	vec3  color;
	vec3  direction;
	float intensity;
	float range;
};

layout(std140, set = @, binding = @) uniform directional_light_uniform
{
	Directional_lights directional_lights[directional_lights_count];
} in_directional_light_uniforms;
)com";

const std::string fs_point_light_common_str = R"com(
const uint point_lights_count = @;

struct Point_lights
{
	mat4  mvp;
	vec3  color;
	vec3  position;
	float intensity;
	float range;
};

layout(std140, set = @, binding = @) uniform point_light_uniform
{
	Point_lights point_lights[point_lights_count];
} in_point_light_uniforms;
)com";

const std::string fs_spot_light_common_str = R"com(
const uint spot_lights_count = @;

struct Spot_lights
{
	mat4  mvp;
	vec3  color;
	vec3  position;
	vec3  direction;
	float intensity;
	float range;
	float inner_angle;
	float outer_angle;
};

layout(std140, set = @, binding = @) uniform spot_light_uniform
{
	Spot_lights spot_lights[spot_lights_count];
} in_spot_light_uniforms;
)com";

std::string fs_light_common(uint32_t a_lights_count, uint32_t a_set, uint32_t a_binding, std::string light_code)
{
	replace_next_at(a_lights_count, light_code);
	replace_next_at(a_set, light_code);
	replace_next_at(a_binding, light_code);

	return light_code;
}

std::string fragment_shader_input_output(const rhi::VertexDescriptor &a_vertex_descriptor)
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

	// layout (location = 0) out vec4 out_color; is defined in main.glsl.frag

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
		std::string factor_cast_str{factor_type + "(in_material_factors." + a_comp_name + "_factor)"};
		bool        needs_normalize = a_comp_name == "normal" || a_comp_name == "bent_normal";

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
			if (needs_normalize)
				output_lookup.append("return normalize((texture(" + a_comp_name + "_sampler, " + a_comp_name + "_uv)" + a_swizzle + " * 2.0 - vec3(1.0))");
			else
				output_lookup.append("return texture(" + a_comp_name + "_sampler, " + a_comp_name + "_uv)" + a_swizzle);

			if (a_mat_comp.m_type == ror::Material::ComponentType::factor_texture)
			{
				output_lookup.append(" * ");
				if (needs_normalize)
				{
					auto tmp{"in_material_factors." + a_comp_name + "_factor"};
					output_lookup.append("vec3(" + tmp + ", " + tmp + ", 1.0)");        // This scale only scale X and Y not Z hence 1.0 in Z according to gltf spec
				}
				else
					output_lookup.append(factor_cast_str);
			}

			if (needs_normalize)
				output_lookup.append(")");
		}

		output_lookup.append(";\n}\n");

		return output_coords + output_lookup;
	}

	return "";
}

// Calls texture_lookup for all material components if it has textures
std::string texture_lookups(const ror::Material &a_material, bool a_has_tangent)
{
	std::string output{""};

	output.append(texture_lookup(a_material.m_base_color, "base_color"));
	output.append(texture_lookup(a_material.m_diffuse_color, "diffuse_color"));
	output.append(texture_lookup(a_material.m_specular_glossyness, "specular_glossyness"));
	output.append(texture_lookup(a_material.m_emissive, "emissive"));
	output.append(texture_lookup(a_material.m_anisotropy, "anisotropy", "vec4"));
	output.append(texture_lookup(a_material.m_transmission, "transmission", "vec2", ".xy"));
	output.append(texture_lookup(a_material.m_sheen_color, "sheen_color", "vec3", ".xyz"));
	output.append(texture_lookup(a_material.m_sheen_roughness, "sheen_roughness", "float", ".x"));
	output.append(texture_lookup(a_material.m_clearcoat_normal, "clearcoat_normal", "vec3", ".xyz"));
	output.append(texture_lookup(a_material.m_clearcoat, "clearcoat", "float", ".x"));
	output.append(texture_lookup(a_material.m_clearcoat_roughness, "clearcoat_roughness", "float", ".x"));
	output.append(texture_lookup(a_material.m_occlusion, "occlusion", "float", ".x"));        // Red component of ORM[H] texture
	output.append(texture_lookup(a_material.m_roughness, "roughness", "float", ".y"));        // Green component of ORM[H] texture
	output.append(texture_lookup(a_material.m_metallic, "metallic", "float", ".z"));          // Blue component of ORM[H] texture
	if (a_material.m_roughness.m_texture == a_material.m_height.m_texture)
		output.append(texture_lookup(a_material.m_height, "height", "float", ".w"));        // Alpha component of ORM[H] texture
	else
		output.append(texture_lookup(a_material.m_height, "height", "float", ".x"));        // Or the red component of the height texture
	output.append(texture_lookup(a_material.m_normal, "normal", "vec3", ".xyz"));
	// output.append(texture_lookup(a_material.m_tangent, "tangent", "vec3", ".xyz"));
	output.append(texture_lookup(a_material.m_bent_normal, "bent_normal", "vec3", ".xyz"));

	// If we have a normal map, lets add code to generate tangent frame in the shader
	if (a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture ||
	    a_material.m_normal.m_type == ror::Material::ComponentType::texture)
	{
		// Read tbn.glsl.frag resource and create a string_view
		auto       &tbn_resource = ror::load_resource("shaders/tbn.glsl.frag", ror::ResourceSemantic::shaders);
		std::string tbn_code{tbn_resource.data_copy()};

		if (a_has_tangent)
		{
			replace_next_at("//", tbn_code);
			replace_next_at("", tbn_code);
		}
		else
		{
			replace_next_at("", tbn_code);
			replace_next_at("//", tbn_code);
		}

		output.append(tbn_code);
	}
	else        // Otherwise add a simple normal map overloaded getter
	{
		output.append("vec3 get_normal(const in vec3 N)\n{\n\treturn normalize(N);\n}\n");
	}

	if (a_material.m_blend_mode == rhi::BlendMode::mask)
		output.append("\nfloat get_opacity()\n{\n\treturn in_material_factors.opacity_factor;\n}\n");

	output.append("\nfloat get_f0()\n{\n\treturn in_material_factors.f0_factor;\n}\n");

	return output;
}

std::string material_samplers(const ror::Material &a_material, bool a_add_shadow_map)
{
	// TODO: Understand how does samplers gets to have the same bindings as other things
	std::string       output{"\n"};
	const std::string set_binding{"layout(set = 0, binding = "};           // TODO: Abstract out the set and binding for all of the below
	const std::string type_precision{") uniform highp sampler2D "};        // TODO: Abstract out precision
	uint32_t          binding = 0;

#define output_append(x) output.append(set_binding + std::to_string(binding++) + type_precision + x)

	if (a_material.m_base_color.m_texture != -1)
		output_append("base_color_sampler;\n");
	if (a_material.m_diffuse_color.m_texture != -1)
		output_append("diffuse_color_sampler;\n");
	if (a_material.m_specular_glossyness.m_texture != -1)
		output_append("specular_glossyness_sampler;\n");
	if (a_material.m_emissive.m_texture != -1)
		output_append("emissive_sampler;\n");
	if (a_material.m_anisotropy.m_texture != -1)
		output_append("anisotropy_sampler;\n");
	if (a_material.m_transmission.m_texture != -1)
		output_append("transmission_sampler;\n");
	if (a_material.m_sheen_color.m_texture != -1)
		output_append("sheen_color_sampler;\n");
	if (a_material.m_sheen_roughness.m_texture != -1)
		output_append("sheen_roughness_sampler;\n");
	if (a_material.m_clearcoat_normal.m_texture != -1)
		output_append("clearcoat_normal_sampler;\n");
	if (a_material.m_clearcoat.m_texture != -1)
		output_append("clearcoat_sampler;\n");
	if (a_material.m_clearcoat_roughness.m_texture != -1)
		output_append("clearcoat_roughness_sampler;\n");
	if (a_material.m_metallic.m_texture != -1)
		output_append("metallic_sampler;\n");
	if (a_material.m_roughness.m_texture != -1)
		output_append("roughness_sampler;\n");
	if (a_material.m_occlusion.m_texture != -1)
		output_append("occlusion_sampler;\n");
	if (a_material.m_normal.m_texture != -1)
		output_append("normal_sampler;\n");
	if (a_material.m_bent_normal.m_texture != -1)
		output_append("bent_normal_sampler;\n");
	if (a_material.m_height.m_texture != -1)
		output_append("height_sampler;\n");

	if (a_add_shadow_map)
		output_append("shadow_map_sampler;\n");

	return output;
}

std::string get_material(const ror::Material &a_material, bool a_has_normal, bool a_has_color_0, bool a_has_color_1)
{
#define stringify_helper(x) #x
#define stringify(x) stringify_helper(x)

#define get_material_component(name, default_value)                                     \
	if (a_material.m_##name.m_type != ror::Material::ComponentType::none)               \
		output.append("\tmaterial." stringify(name) " = get_" stringify(name) "();\n"); \
	else                                                                                \
		output.append("\tmaterial." stringify(name) " = " stringify(default_value) ";\n")

	std::string output{"Material get_material()\n{\n\tMaterial material;\n\n"};

	// Using the expanded version for base_color
	if (a_material.m_base_color.m_type != ror::Material::ComponentType::none)
		output.append("\tmaterial.base_color = get_base_color()");
	else
		output.append("\tmaterial.base_color = vec4(1.0)");

	if (a_has_color_0)
		output.append(" * in_vertex_color_0");

	if (a_has_color_1)
		output.append(" * in_vertex_color_1");

	output.append(";\n");

	// get_material_component(base_color, vec4(1.0));

	if (a_material.m_blend_mode == rhi::BlendMode::mask)
		output.append("\tresolve_alpha(material.base_color.a);\n");

	// Defaults values for Material copied from Filament's defaults
	get_material_component(emissive, vec4(0.0));        // when adding emissive support, If there is valid emissive make sure its alpha is 0, so blending works, a in emissive must be ignored according to gltf spec
	get_material_component(bent_normal, vec3(0.0, 0.0, 1.0));
	// get_material_component(roughness, 1.0);
	// get_material_component(metallic, 0.0);
	get_material_component(height, 0.0);
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
	// Not using macro for roughness because I need to clamp it to minimum 4%
	if (a_material.m_roughness.m_type != ror::Material::ComponentType::none)
		if (ror::settings().m_clamp_material_roughness)
			output.append("\tmaterial.roughness = clamp(get_roughness(), 0.04, 1.0);\n");
		else
			output.append("\tmaterial.roughness = get_roughness();\n");
	else
		output.append("\tmaterial.roughness = 1.0;\n");

	// Not using macro for metallic because I need to clamp it
	if (a_material.m_metallic.m_type != ror::Material::ComponentType::none)
		if (ror::settings().m_clamp_material_metallic)
			output.append("\tmaterial.metallic = clamp(get_metallic(), 0.0, 1.0);\n");
		else
			output.append("\tmaterial.metallic = get_metallic();\n");
	else
		output.append("\tmaterial.metallic = 0.0;\n");

	// TODO: View doesn't fit in material, this belongs in fragment but its a hack for now
	// This works around having to call get_normal() twice
	output.append("\tmaterial.view = normalize(in_per_view_uniform.camera_position - in_vertex_position.xyz);\n");

	// If we have a normal map use the overloaded get_normal(N) which uses get_normal() internally, there must be vertex normal available
	if (a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture ||
	    a_material.m_normal.m_type == ror::Material::ComponentType::texture ||
	    a_has_normal)
	{
		assert(a_has_normal && "Model with a normal map doens't bring vertex normals, generate these");        // If this asserts just use the else part to generate the normals
		output.append("\tmaterial.normal = get_normal(in_vertex_normal);\n");
	}
	else        // otherwise lets generate a normal
		// Check this out if you have precision issues https://www.enkisoftware.com/devlogpost-20150131-1-Normal-generation-in-the-pixel-shader
		output.append("\tmaterial.normal = -normalize(cross(dFdx(in_vertex_position.xyz), dFdy(in_vertex_position.xyz)));\n");

	// Reflecance factor is always there
	output.append("\tmaterial.f0 = get_f0();\n");

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

	output.append("\n\treturn material;\n}\n");

	return output;
}

static const std::string resolve_alpha_code{R"alp(
void resolve_alpha(float alpha)
{
	float opacity = get_opacity();

	if (alpha < opacity)
		discard;
})alp"};

std::string fs_set_output(const ror::Material &a_material, bool a_has_normal, bool a_has_color_0, bool a_has_color_1)
{
	std::string result{};

	// Read getters.glsl.frag resource and create a string_view
	auto            &getters_resource = ror::load_resource("shaders/getters.glsl.frag", ror::ResourceSemantic::shaders);
	std::string_view getters_code{reinterpret_cast<const char *>(getters_resource.data(true).data()), getters_resource.data(true).size()};

	if (a_material.m_blend_mode == rhi::BlendMode::mask)
		result.append(resolve_alpha_code);

	result.append(get_material(a_material, a_has_normal, a_has_color_0, a_has_color_1));
	result.append("\n");
	result.append(getters_code);
	result.append("\n");

	return result;
}

void enable_render_modes(std::string &a_render_modes, const ror::Material &a_material, bool a_has_normal, bool a_has_color_0, bool a_has_color_1, bool a_has_uv0, bool a_has_uv1, bool a_has_uv2)
{
	auto has_base_color = a_material.m_base_color.m_type == ror::Material::ComponentType::factor_texture || a_material.m_base_color.m_type == ror::Material::ComponentType::texture;
	auto has_tangent    = a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture || a_material.m_normal.m_type == ror::Material::ComponentType::texture;
	auto has_roughness  = a_material.m_roughness.m_type == ror::Material::ComponentType::factor_texture || a_material.m_roughness.m_type == ror::Material::ComponentType::texture;
	auto has_metallic   = a_material.m_metallic.m_type == ror::Material::ComponentType::factor_texture || a_material.m_metallic.m_type == ror::Material::ComponentType::texture;
	auto has_occlusion  = a_material.m_occlusion.m_type == ror::Material::ComponentType::factor_texture || a_material.m_occlusion.m_type == ror::Material::ComponentType::texture;
	auto has_normal     = a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture || a_material.m_normal.m_type == ror::Material::ComponentType::texture;
	auto has_height     = a_material.m_height.m_type == ror::Material::ComponentType::factor_texture || a_material.m_height.m_type == ror::Material::ComponentType::texture;
	auto has_emissive   = a_material.m_emissive.m_type == ror::Material::ComponentType::factor_texture || a_material.m_emissive.m_type == ror::Material::ComponentType::texture;

	auto has_base_factor      = a_material.m_base_color.m_type == ror::Material::ComponentType::factor_texture || a_material.m_base_color.m_type == ror::Material::ComponentType::factor;
	auto has_roughness_factor = a_material.m_roughness.m_type == ror::Material::ComponentType::factor_texture || a_material.m_roughness.m_type == ror::Material::ComponentType::factor;
	auto has_metallic_factor  = a_material.m_metallic.m_type == ror::Material::ComponentType::factor_texture || a_material.m_metallic.m_type == ror::Material::ComponentType::factor;
	auto has_occlusion_factor = a_material.m_occlusion.m_type == ror::Material::ComponentType::factor_texture || a_material.m_occlusion.m_type == ror::Material::ComponentType::factor;
	auto has_normal_factor    = a_material.m_normal.m_type == ror::Material::ComponentType::factor_texture || a_material.m_normal.m_type == ror::Material::ComponentType::factor;
	auto has_emissive_factor  = a_material.m_emissive.m_type == ror::Material::ComponentType::factor_texture || a_material.m_emissive.m_type == ror::Material::ComponentType::factor;

	// UV 0, 1@
	if (a_has_uv0)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// UV 1, 1@
	if (a_has_uv1)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// UV 2, 1@
	if (a_has_uv2)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// Geometric Normal/And scalled 4-@s
	if (a_has_normal)
		replace_all_ats(4, "", a_render_modes);
	else
		replace_all_ats(4, "//", a_render_modes);

	// Geometric Tangent/Bitangent 2-@
	if (has_tangent)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	if (a_has_color_0)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	if (a_has_color_1)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// Base color
	if (has_base_color)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// Ambient Occlusion map 2-@
	if (has_occlusion)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	// Roughness map 2-@
	if (has_roughness)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	// Metallic map 2-@
	if (has_metallic)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	// Normal map 4-@, 2 for scaled
	if (has_normal)
		replace_all_ats(4, "", a_render_modes);
	else
		replace_all_ats(4, "//", a_render_modes);

	// Height map 2-@
	if (has_height)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	// Emissive map 1-@
	if (has_emissive)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// Material Normal Scaled 1-@
	if (has_normal)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// Material Factors

	// Base Color Factor 1-@
	if (has_base_factor)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// Occlusion factor 2-@
	if (has_occlusion_factor)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	// Roughness factor 2-@
	if (has_roughness_factor)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	// Metallic factor 2-@
	if (has_metallic_factor)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	// Normal factor 2-@
	if (has_normal_factor)
		replace_all_ats(2, "", a_render_modes);
	else
		replace_all_ats(2, "//", a_render_modes);

	// Emissive factor 1-@
	if (has_emissive_factor)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);

	// Base Color Factored 1-@
	if (has_base_factor)
		replace_next_at("", a_render_modes);
	else
		replace_next_at("//", a_render_modes);
}

std::string get_render_modes()
{
	auto       &render_modes_resource = ror::load_resource("shaders/render_modes.glsl.frag", ror::ResourceSemantic::shaders);
	std::string render_modes_code{render_modes_resource.data_copy()};

	auto    &setting = ror::settings();
	uint32_t mode{0};
	for (auto &render_mode : setting.m_gui.m_render_modes)
	{
		if (render_mode.c_str()[0] != '-')
		{
			std::string replacement{"in_per_frame_uniform.render_mode == " + std::to_string(mode)};
			std::string search_term{"\"" + render_mode + "\""};
			replace_first(search_term, replacement, render_modes_code);
		}
		mode++;
	}

	return render_modes_code;
}

std::string fs_set_main(const ror::Material &a_material, bool a_has_shadow, bool a_has_normal, bool a_has_color_0, bool a_has_color_1, bool a_has_uv0, bool a_has_uv1, bool a_has_uv2)
{
	// Read all the shader snippets
	// Read brdf.glsl.frag resource and create a string_view
	auto            &brdf_resource = ror::load_resource("shaders/brdf.glsl.frag", ror::ResourceSemantic::shaders);
	std::string_view brdf_code{reinterpret_cast<const char *>(brdf_resource.data(true).data()), brdf_resource.data(true).size()};

	// Read temporary_structs.glsl.frag resource and create a string_view
	auto            &temporary_structs_resource = ror::load_resource("shaders/temporary_structs.glsl.frag", ror::ResourceSemantic::shaders);
	std::string_view temporary_structs_code{reinterpret_cast<const char *>(temporary_structs_resource.data(true).data()), temporary_structs_resource.data(true).size()};

	// Read shadows.glsl.frag resource and create a string_view
	auto            &shadows_resource = ror::load_resource("shaders/shadows.glsl.frag", ror::ResourceSemantic::shaders);
	std::string_view shadows_code{reinterpret_cast<const char *>(shadows_resource.data(true).data()), shadows_resource.data(true).size()};

	// Read shading_standard.glsl.frag resource and create a string_view
	auto            &shading_standard_resource = ror::load_resource("shaders/shading_standard.glsl.frag", ror::ResourceSemantic::shaders);
	std::string_view shading_standard_code{reinterpret_cast<const char *>(shading_standard_resource.data(true).data()), shading_standard_resource.data(true).size()};

	// Read lighting.glsl.frag resource and create a string_view
	auto       &lighting_resource = ror::load_resource("shaders/lighting.glsl.frag", ror::ResourceSemantic::shaders);
	std::string lighting_code{lighting_resource.data_copy()};

	// Read main.glsl.frag resource and create a string_view
	auto       &main_resource = ror::load_resource("shaders/main.glsl.frag", ror::ResourceSemantic::shaders);
	std::string main_code{main_resource.data_copy()};

	std::string output{};

	output.append("\n");
	output.append(brdf_code);
	output.append("\n");
	output.append(temporary_structs_code);
	output.append("\n");
	output.append(fs_set_output(a_material, a_has_normal, a_has_color_0, a_has_color_1));
	output.append("\n");
	if (a_has_shadow)
	{
		output.append(shadows_code);
		output.append("\n");
		replace_next_at("get_shadow(directional.mvp);", lighting_code);
		replace_next_at("get_shadow(point.mvp);", lighting_code);
		replace_next_at("get_shadow(spot.mvp);", lighting_code);
	}
	else
	{
		replace_next_at("1.0;", lighting_code);
		replace_next_at("1.0;", lighting_code);
		replace_next_at("1.0;", lighting_code);
	}
	output.append(shading_standard_code);
	output.append("\n");
	output.append(lighting_code);
	output.append("\n");

	// Lets first add set_render_mode code to main
	auto render_modes = get_render_modes();
	enable_render_modes(render_modes, a_material, a_has_normal, a_has_color_0, a_has_color_1, a_has_uv0, a_has_uv1, a_has_uv2);
	replace_next_at(render_modes, main_code);

	if (a_material.m_material_model == rhi::MaterialModel::unlit)
		replace_next_at("", main_code);
	else
		replace_next_at("//", main_code);

	output.append(main_code);

	return output;
}

std::string generate_primitive_fragment_shader(const ror::Mesh &a_mesh, const materials_vector &a_materials, uint32_t a_primitive_index, rhi::RenderpassType a_passtype, const ror::Renderer &a_renderer, bool a_has_shadow)
{
	(void) a_renderer;
	const auto  is_depth_shadow   = (a_passtype == rhi::RenderpassType::depth || a_passtype == rhi::RenderpassType::shadow);
	const auto &vertex_descriptor = a_mesh.vertex_descriptor(a_primitive_index);
	const auto  type              = vertex_descriptor.type();
	const auto  has_uv0           = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_0);
	const auto  has_uv1           = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_1)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_1);
	const auto  has_uv2           = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_2)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_texture_coord_2);
	const auto  has_normal        = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_normal);
	const auto  has_tangent       = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_tangent)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_tangent);
	const auto  has_color_0       = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_0)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_0);
	const auto  has_color_1       = (type & ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_1)) == ror::enum_to_type_cast(rhi::BufferSemantic::vertex_color_1);

	auto material_index = a_mesh.material(a_primitive_index);
	assert(material_index != -1 && "Material index can't be -1");
	auto &material = a_materials[ror::static_cast_safe<size_t>(material_index)];

	auto per_frame_buffer  = a_renderer.shader_buffer("per_frame_uniform");
	auto per_frame_uniform = per_frame_buffer->to_glsl_string();

	std::string output{"#version 450\n\nprecision highp float;\nprecision highp int;\n\n"};        // TODO: abstract out version

	if (is_depth_shadow)
	{
		const std::string fs_depth_shadow_main{"void main()\n{\n}\n"};
		output.append(fs_depth_shadow_main);
		return output;
	}

	auto &setting = settings();

	// write out inputs from fragment shader
	output.append(ror::fragment_shader_input_output(vertex_descriptor));
	output.append(per_view_common(a_renderer));
	output.append(per_frame_uniform);
	output.append(material_samplers(material, a_has_shadow));
	output.append(material.m_shader_buffer.to_glsl_string());
	output.append(fs_light_common(1, setting.directional_light_set(), setting.directional_light_binding(), fs_directional_light_common_str));        // TODO: Make conditional, and abstract out lights_count
	output.append(fs_light_common(1, setting.point_light_set(), setting.point_light_binding(), fs_point_light_common_str));                          // TODO: Make conditional, and abstract out lights_count
	output.append(fs_light_common(1, setting.spot_light_set(), setting.spot_light_binding(), fs_spot_light_common_str));                             // TODO: Make conditional, and abstract out lights_count
	output.append(texture_lookups(material, has_tangent));
	output.append(fs_set_main(material, a_has_shadow, has_normal, has_color_0, has_color_1, has_uv0, has_uv1, has_uv2));

	return output;
}

// Some specialized functions/callbacks that are used to patch loaded shaders
// Name of this function is usually the name of the shader, in this case node_transform_glsl_comp is for node_transform.glsl.comp
void node_transform_glsl_comp(std::string &a_input, const ror::Renderer &a_renderer)
{
	replace_next_at(a_renderer.shader_buffer("per_frame_uniform")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("per_view_uniform")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("nodes_models")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("morphs_weights")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("node_transform_input")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("node_transform_output")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("animations")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("animations_sampler_input")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("animations_sampler_output")->to_glsl_string(), a_input);
	replace_next_at(a_renderer.shader_buffer("current_animations")->to_glsl_string(), a_input);
}

std::vector<std::string> &mesh_shaders_dependencies()
{
	// Keep updating when some shaders are not picked up by the shader update system
	// Ideally could be read from disk, but that will also read lots of other junk
	static std::vector<std::string> deps{
	    "tbn.glsl.frag",
	    "getters.glsl.frag",
	    "render_modes.glsl.frag",
	    "ibl_reference.glsl.frag",
	    "brdf.glsl.frag",
	    "temporary_structs.glsl.frag",
	    "shadows.glsl.frag",
	    "shading_standard.glsl.frag",
	    "lighting.glsl.frag",
	    "main.glsl.frag"};

	return deps;
}

// End of fragment shader code
}        // namespace ror
