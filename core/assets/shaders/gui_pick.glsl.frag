#version 450

layout(location = 0) in vec4 in_vertex_position;
layout(location = 1) in vec2 in_vertex_texture_coord_0;
layout(location = 2) in vec4 in_vertex_color_0;

layout(set = 0, binding = 0) uniform highp usampler2D base_color_sampler;

vec2 get_base_color_uvs()
{
	return in_vertex_texture_coord_0;
}

// This version will do bi-linear sampling if sampler allows it, hence using the one below is better option
uint get_base_color()
{
	vec2 base_color_uv = get_base_color_uvs();
	vec4 id = texture(base_color_sampler, base_color_uv);

	return uint(id.x);
}

uint get_index()
{
	ivec2 size = textureSize(base_color_sampler, 0);
	vec2 uv = get_base_color_uvs();
	ivec2 index = ivec2(size * uv);

	return texelFetch(base_color_sampler, index, 0).r;
}

vec3 hash_color(uint x)
{
    x ^= x >> 16;
    x *= 0x7feb352du;
    x ^= x >> 15;
    x *= 0x846ca68bu;
    x ^= x >> 16;

    return vec3(float((x >>  0) & 255u),
				float((x >>  8) & 255u),
				float((x >> 16) & 255u)
		) / 255.0;
}


layout(location = 0) out vec4 out_color;

void main()
{
	uint id = get_index();

	out_color = vec4(hash_color(id), 1.0);
}
