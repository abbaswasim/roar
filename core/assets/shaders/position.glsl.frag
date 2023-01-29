#version 450

precision highp float;
precision highp int;

layout (location = 0) in highp  vec4 in_vertex_position;
layout (location = 0) out highp vec4 out_color;

void main()
{
	out_color = vec4(1.0, 1.0, 1.0, 1.0);
}
