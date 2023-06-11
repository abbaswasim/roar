#version 450

precision highp float;
precision highp int;

layout(location = 0) in highp vec4 in_vertex_position;
layout(location = 1) in highp vec4 in_vertex_color_0;

layout(location = 0) out highp vec4 out_vertex_position;
layout(location = 1) out highp vec4 out_vertex_color_0;

// TODO: Patch with the renderer per_view_uniform so it doesn't go out of sync
layout(std140, set = 0, binding = 20) uniform per_view_uniform
{
	mat4 view_mat4;
	mat4 projection_mat4;
	mat4 view_projection_mat4;
	mat4 inverse_projection_mat4;
	mat4 inverse_view_projection_mat4;
	mat3 normal_mat3;
	vec3 camera_position;
}
in_per_view_uniforms;

void set_position()
{
	vec4 vertex_position = in_vertex_position;
	out_vertex_position  = vertex_position;
	gl_Position          = in_per_view_uniforms.projection_mat4 * in_per_view_uniforms.view_mat4 * vec4(vertex_position.xyz, 1.0);
}

void set_color_0()
{
	vec4 vertex_color  = in_vertex_color_0;
	out_vertex_color_0 = vertex_color;
}

void main()
{
	set_color_0();
	set_position();
}
