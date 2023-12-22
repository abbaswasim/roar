#version 450

precision highp float;
precision highp int;

layout(location = 0) in vec2 in_vertex_uv;

layout(set = 0, binding = 0) uniform sampler2D ibl_texture;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 base_color = texture(ibl_texture, in_vertex_uv);

	out_color = vec4(base_color.xyz, 1.0);

	// out_color = vec4(0.0);
}
