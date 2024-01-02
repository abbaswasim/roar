#version 450

precision highp float;
precision highp int;

layout(location = 0) in highp vec4 in_vertex_position;

layout(location = 0) out highp vec4 out_vertex_position;

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

float scale = 100.0;

void set_position()
{
	out_vertex_position = in_vertex_position;
	out_vertex_position.x *= -1;        // Since our environment is inside out we flip the X axis to see it correctly

	gl_Position = in_per_view_uniforms.projection_mat4 * in_per_view_uniforms.view_mat4 * vec4(in_vertex_position.xyz * scale, 1.0);

	// To stop the cubmap translating, else use the above version
	// gl_Position = in_per_view_uniforms.projection_mat4 * vec4(mat3(in_per_view_uniforms.view_mat4) * in_vertex_position.xyz * scale, 1.0);
	// gl_Position = gl_Position.xyww;
}

void main()
{
	set_position();
}
