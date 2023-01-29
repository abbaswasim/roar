#version 450

precision highp float;
precision highp int;

layout (location = 0) in highp vec4 in_vertex_position;
layout (location = 1) in highp vec4 in_vertex_color_0;

layout (location = 0) out highp vec4 out_color;

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

void main()
{
	out_color = in_vertex_color_0;

	// Grid rendering with alpha fade
	float zdelta = distance(in_per_view_uniforms.camera_position.xyz, in_vertex_position.xyz);
	float alpha = out_color.a;
	out_color.a = alpha / zdelta;
}
