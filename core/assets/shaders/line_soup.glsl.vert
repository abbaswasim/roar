#version 450

precision highp float;
precision highp int;

layout (location = 0) in highp vec4 in_vertex_position;
layout (location = 1) in highp vec4 in_vertex_color_0;

layout (location = 0) out highp vec4 out_vertex_color_0;

// TODO: Make the set and binding updateable
layout(std140, set = 0, binding = 20) uniform per_view_uniform
{
	mat4    view_mat4;
	mat4    projection_mat4;
	mat4    view_projection_mat4;
	mat4    inverse_projection_mat4;
	mat4    inverse_view_projection_mat4;
	mat3    normal_mat3;
	uvec4   viewport;
	vec3    camera_position;
} in_per_view_uniform;

void main()
{
	out_vertex_color_0 = in_vertex_color_0;
	gl_Position = in_per_view_uniform.projection_mat4 * in_per_view_uniform.view_mat4 * vec4(in_vertex_position.xyz, 1.0);
}
