#version 450

#extension GL_EXT_shader_16bit_storage : require
precision highp float;
precision highp int;

layout (location = 0) in highp vec4 in_vertex_position;
layout (location = 0) out highp vec4 out_vertex_position;
layout (location = 1) in highp vec4 in_vertex_color_0;
layout (location = 1) out highp vec4 out_vertex_color_0;


layout(std140, set = 0, binding = 20) uniform per_view_uniform
{
	mat4 view_mat4;
	mat4 projection_mat4;
	mat4 view_projection_mat4;
	mat4 inverse_projection_mat4;
	mat4 inverse_view_projection_mat4;
	mat3 normal_mat3;
	vec3 camera_position;
} in_per_view_uniforms;

// Hard coded bindings, needs updating if anything changes
layout(std430, set = 0, binding = 21) readonly buffer nodes_model
{
    mat4 node_model_mat4[];
} in_nodes_model;

layout(std140, set = 0, binding = 22) uniform nodes_offsets
{
	uvec4 node_offsets;
} in_nodes_offsets;

void world_transform_position(inout vec4 vertex_position)
{
    uint node_index = in_nodes_offsets.node_offsets.x;
	vertex_position = in_nodes_model.node_model_mat4[node_index] * vec4(vertex_position.xyz, 1.0);
}

void set_position()
{
	vec4 vertex_position = in_vertex_position;

	//morph_vertex_position(vertex_position);
	//skin_position(vertex_position.xyz);
	world_transform_position(vertex_position);

	out_vertex_position = vertex_position;

	gl_Position = in_per_view_uniforms.projection_mat4 * in_per_view_uniforms.view_mat4 * vec4(vertex_position.xyz, 1.0);
}

void set_color_0()
{
	vec4 vertex_color = in_vertex_color_0;

	//morph_vertex_color_0(vertex_color);
	out_vertex_color_0 = vertex_color;
}

void main()
{
	set_color_0();
	set_position();
}
