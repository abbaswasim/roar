#version 450

layout(location = 0) in vec4 in_vertex_position;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = vec4(1.0, 1.0, 1.0, 0.5);
}
