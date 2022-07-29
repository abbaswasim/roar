#version 450

layout(location = 0) in highp vec2 in_vertex_texture_coord_0;
layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(in_vertex_texture_coord_0, 0.0, 1.0);
}
