#version 450

precision highp float;
precision highp int;

layout(location = 0) in highp vec4 in_vertex_position;
layout(location = 1) in highp vec2 in_vertex_texture_coord0;

layout(location = 0) out highp vec2 out_vertex_uv;

layout(std140, set = 0, binding = 20) uniform per_view_uniform
{
	mat4  view_mat4;
	mat4  projection_mat4;
	mat4  view_projection_mat4;
	mat4  inverse_projection_mat4;
	mat4  inverse_view_projection_mat4;
	mat3  normal_mat3;
	uvec4 viewport;
	vec3  camera_position;
} in_per_view_uniforms;

float scale = -100.0; // This will make it big and Negative scale will flip the normals

void main()
{
	out_vertex_uv = in_vertex_texture_coord0;

	gl_Position = in_per_view_uniforms.projection_mat4 * in_per_view_uniforms.view_mat4 * vec4(in_vertex_position.xyz * vec3(scale), 1.0);

	// To stop the cubmap translating, else use the above version
	// gl_Position = in_per_view_uniforms.projection_mat4 * vec4((mat3(in_per_view_uniforms.view_mat4) * (in_vertex_position.xyz * vec3(scale))).xyz, 1.0);
}
