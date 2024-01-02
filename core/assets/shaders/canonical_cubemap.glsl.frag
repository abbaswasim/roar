#version 450

precision highp float;
precision highp int;

layout(location = 0) in vec4 in_vertex_position;

layout(set = 0, binding = 0) uniform highp samplerCube cube_map;

vec3 get_base_direction_vector()
{
	return normalize(in_vertex_position.xyz);
}

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 direction_vector = get_base_direction_vector();
	vec4 base_color = texture(cube_map, direction_vector);
	
	out_color = base_color;
}
