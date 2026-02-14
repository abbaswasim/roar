// If you do lightspace xform in fragment shader per light, unlike some tutorials which does it in vertex shader, you can support multiple lights.
// TODO: If you do this in VS then you will have to send over multiple xforms which might be fine, try this for performance as well

// Based on the implementation in https://github.com/TheRealMJP/Shadows/blob/master/Shadows/Mesh.hlsl
vec2 compute_receiver_plane_depth_bias(vec3 tex_coord_dx, vec3 tex_coord_dy)
{
	vec2 bias_uv;

	bias_uv.x = tex_coord_dy.y * tex_coord_dx.z - tex_coord_dx.y * tex_coord_dy.z;
	bias_uv.y = tex_coord_dx.x * tex_coord_dy.z - tex_coord_dy.x * tex_coord_dx.z;

	bias_uv *= 1.0 / ((tex_coord_dx.x * tex_coord_dy.y) - (tex_coord_dx.y * tex_coord_dy.x));

	return bias_uv;
}

void get_correct_light_depth(inout float light_depth, vec3 shadow_pos_dx, vec3 shadow_pos_dy)
{
	vec2 shadow_map_size = vec2(textureSize(shadow_map_sampler, 0));
	vec2 texel_size      = 1.0 / shadow_map_size;

	vec2 plane_depth_bias = compute_receiver_plane_depth_bias(shadow_pos_dx, shadow_pos_dy);

	// Static depth biasing to make up for incorrect fractional sampling on the shadow map grid
	float fractional_sampling_error = dot(vec2(1.0) * texel_size, abs(plane_depth_bias));
	light_depth -= min(fractional_sampling_error, 0.01);
}

// Not great but works, not currently used
float simple_pcf(vec3 uv)
{
	// PCF
	float shadow     = 0.0;
	vec2  texel_size = 1.0 / textureSize(shadow_map_sampler, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadow_map_sampler, uv.xy + vec2(x, y) * texel_size).r;
			shadow += uv.z > pcfDepth ? 0.0 : 1.0;
		}
	}

	shadow /= 9.0;

	if (uv.z > 1.0)
		shadow = 0.0;

	return shadow;
}

// Get bias
float get_shadow(const mat4 light_space, const float ndotl)
{
	highp vec4 light_space_position = light_space * in_vertex_position;
	highp vec3 position             = light_space_position.xyz / light_space_position.w;

	position.xy = position.xy * 0.5 + 0.5;
	position.y  = 1.0 - position.y;        // Flip due to Metal/Vulkan

	vec3 ddX = dFdxFine(position);
	vec3 ddY = dFdyFine(position);

	highp vec2 uv = position.xy;

	// If sampling outside return fully in light
	if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
		return 1.0;

	get_correct_light_depth(position.z, ddX, ddY);

	float depth = texture(shadow_map_sampler, uv).r;
	return position.z > depth ? 0.1 : 1.0;

	// return simple_pcf(position);
}
