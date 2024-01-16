#version 450

layout(location = 0) in vec4 in_vertex_position;
layout(location = 1) in vec2 in_vertex_texture_coord_0;

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


// source http://glampert.com/2014/01-26/visualizing-the-depth-buffer/
float linearize_depth(float depth)
{
    float z_near = 0.1;
    float z_far  = 1000.0;

    return (2.0 * z_near) / (z_far + z_near - depth * (z_far - z_near));
}


void main()
{
	vec4 base_color = get_base_color();

	float linear_depth = linearize_depth(base_color.x);

	out_color = vec4(linear_depth, linear_depth, linear_depth, 1.0);
}
