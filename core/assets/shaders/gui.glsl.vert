#version 450

precision highp float;
precision highp int;

layout (location = 0) in highp vec4 in_vertex_position;
layout (location = 1) in highp vec2 in_vertex_texture_coord_0;
layout (location = 2) in highp vec4 in_vertex_color_0;

layout (location = 0) out highp vec4 out_vertex_position;
layout (location = 1) out highp vec2 out_vertex_texture_coord_0;
layout (location = 2) out highp vec4 out_vertex_color_0;

layout(std140, set = 0, binding = 3) uniform gui_per_frame_uniform
{
	mat4 orthographic_projection;
} in_gui_per_frame_uniform;

void world_transform_position(inout vec4 vertex_position)
{
	vertex_position = in_gui_per_frame_uniform.orthographic_projection * vec4(vertex_position.xyz, 1.0);
}

void set_position()
{
	vec4 vertex_position = in_vertex_position;
	world_transform_position(vertex_position);
	out_vertex_position = vertex_position;
	gl_Position = vec4(vertex_position.xyz, 1.0);
}

void set_color_0()
{
	vec4 vertex_color = in_vertex_color_0;
	out_vertex_color_0 = vertex_color;
}

void set_texture_coord_0()
{
	vec2 vertex_texture_coord_0 = in_vertex_texture_coord_0;
	out_vertex_texture_coord_0 = vertex_texture_coord_0;
}

void main()
{
	set_color_0();
	set_texture_coord_0();
	set_position();
}
