#version 450

layout(location = 0) in vec4 in_vertex_position;
layout(location = 1) in vec2 in_vertex_texture_coord_0;
layout(location = 2) in vec4 in_vertex_color_0;

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

float srg_to_linear(float inc)
{
	if (inc <= 0.04045)
		return inc / 12.92;
	else
		return pow((inc + 0.055) / 1.055, 2.4);
}

void main()
{
	vec4 base_color = get_base_color();
	vec4 color      = in_vertex_color_0;

	color.rgb    = vec3(srg_to_linear(color.r), srg_to_linear(color.g), srg_to_linear(color.b));
	base_color.a = srg_to_linear(base_color.a);

	out_color = color * base_color;

	vec4  shadow_color = vec4(0.0, 0.0, 0.0, 1.0);
	float tex_shadow   = base_color.b;
	base_color.b       = 1.0;

	// float tex_outline  = base_color.g;
	// base_color.g       = 1.0;
	// vec4 outline_color = vec4(0.0, 0.0, 0.0, 1.0);

	base_color.rgb *= base_color.a;
	color.rgb *= color.a;

	out_color = vec4(color) * base_color + shadow_color * tex_shadow;        // + outline_color * tex_outline;
}
