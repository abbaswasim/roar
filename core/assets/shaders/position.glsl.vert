#version 450

#extension GL_EXT_shader_16bit_storage : require

precision highp float;
precision highp int;

layout (location = 0) in highp vec4 in_vertex_position;
layout (location = 0) out highp vec4 out_vertex_position;

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

layout(std430, set = 0, binding = 27) readonly buffer nodes_model
{
    mat4 node_model_mat4[];
} in_nodes_model;

layout(std140, set = 0, binding = 28) uniform nodes_offsets
{
	uvec4 node_offsets;
} in_nodes_offsets;

void world_transform_position(inout vec4 vertex_position)
{
    // You need to retain 'w' only after perspective transform for fixed function "prespective division"
    // hence reseting w=1.0 to make sure skinning or morphing hasn't messed it up
    // Details https://www.tomdalling.com/blog/modern-opengl/explaining-homogenous-coordinates-and-projective-geometry/
    uint node_index = in_nodes_offsets.node_offsets.x;
	vertex_position = in_nodes_model.node_model_mat4[node_index] * vec4(vertex_position.xyz, 1.0);
}

void set_position()
{
	vec4 vertex_position = in_vertex_position;
	world_transform_position(vertex_position);
	out_vertex_position = vertex_position;

	gl_Position = in_per_view_uniforms.projection_mat4 * in_per_view_uniforms.view_mat4 * vec4(vertex_position.xyz, 1.0);
}

void main()
{
	set_position();
}
