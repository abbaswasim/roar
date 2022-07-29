#version 450

layout(set = 0, binding = 0) uniform sampler2D diffuse_color_sampler;

layout(location = 0) in highp vec2 in_vertex_texture_coord_0;
layout(location = 0) out vec4 out_color;

void main()
{
	out_color = texture(diffuse_color_sampler, in_vertex_texture_coord_0);
}
