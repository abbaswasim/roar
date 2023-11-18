#version 450

precision highp float;
precision highp int;

layout (location = 0) out highp vec2 out_vertex_texture_coord_0;

void main() 
{
	out_vertex_texture_coord_0 = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(out_vertex_texture_coord_0 * 2.0f - 1.0f, 0.0f, 1.0f);
}
