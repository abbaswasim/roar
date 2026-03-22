#version 450

layout(location = 0) in vec4 in_vertex_position;
layout(location = 1) in vec2 in_vertex_texture_coord_0;
layout(location = 2) in vec4 in_vertex_color_0;

layout(set = 0, binding = 0) uniform highp sampler2DArray base_color_sampler;

vec2 get_base_color_uvs()
{
	return in_vertex_texture_coord_0;
}

vec4 get_base_color()
{
	vec2 base_color_uv = get_base_color_uvs();

	float layer = 0.0;
	vec2  quadrant_offset = vec2(0.0);

	if (base_color_uv.x < 0.5 && base_color_uv.y < 0.5)
		layer = 0.0;
	else if (base_color_uv.x < 0.5 && base_color_uv.y > 0.5)
	{
		layer = 1.0;
		quadrant_offset.y = 1.0;
	}
	else if (base_color_uv.x > 0.5 && base_color_uv.y < 0.5)
	{
		layer = 2.0;
		quadrant_offset.x = 1.0;
	}
	else if (base_color_uv.x > 0.5 && base_color_uv.y > 0.5)
	{
		layer            = 3.0;
		quadrant_offset  = vec2(1.0, 1.0);
	}

	// Remap each quadrant back into [0,1] so every layer samples its full texture
	vec2 local_uv = base_color_uv * 2.0 - quadrant_offset;

	return texture(base_color_sampler, vec3(local_uv, layer));
}

layout(location = 0) out vec4 out_color;

// source http://glampert.com/2014/01-26/visualizing-the-depth-buffer/
float linearize_depth(float depth)
{
	float z_near = 0.1;
	float z_far  = 100.0;

	return (2.0 * z_near) / (z_far + z_near - depth * (z_far - z_near));
}

void main()
{
	vec4 base_color = get_base_color();
	out_color       = vec4(linearize_depth(base_color.r));
}
