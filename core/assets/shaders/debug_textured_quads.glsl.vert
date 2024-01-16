#version 450

layout(location = 0) in highp vec4 in_vertex_position;
layout(location = 1) in highp vec2 in_vertex_texture_coord_0;

layout(location = 0) out highp vec4 out_vertex_position;
layout(location = 1) out highp vec2 out_vertex_texture_coord_0;

void set_position()
{
	out_vertex_position = in_vertex_position;

	gl_Position = vec4(in_vertex_position.xyz, 1.0);
	// gl_Position.xyz = gl_Position.xyz / gl_Position.w;
	// gl_Position.w = 1.0;
}

void set_texture_coord_0()
{
	vec2 vertex_texture_coord_0 = in_vertex_texture_coord_0;
	out_vertex_texture_coord_0  = vertex_texture_coord_0;
}

void main()
{
	set_texture_coord_0();
	set_position();
}
