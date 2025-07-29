#version 450

layout(location = 0) in vec4 in_vertex_color_0;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = in_vertex_color_0;
}
