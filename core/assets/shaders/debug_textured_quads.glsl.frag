#version 450

layout(location = 0) in vec4 in_vertex_position;
layout(location = 1) in vec2 in_vertex_texture_coord_0;

layout(set = 0, binding = 0) uniform highp sampler2D base_color_sampler;

vec2 get_base_color_uvs()
{
	return in_vertex_texture_coord_0;
}

vec4 get_base_color()
{
	vec2 base_color_uv = get_base_color_uvs();
	return texture(base_color_sampler, base_color_uv);
}

layout(location = 0) out vec4 out_color;


void main()
{
	out_color = get_base_color();
}
