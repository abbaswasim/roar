// If you do lightspace xform in fragment shader per light, unline some tutorials which does it in vertex shader, you can support multiple lights.
// If you do this in VS then you will have to send over multiple xforms which might be fine

float get_shadow(const mat4 light_space, const float ndotl)
{
	highp vec4 light_space_position = light_space * in_vertex_position;
	highp vec3 position             = light_space_position.xyz * light_space_position.w;
	highp vec2 uv                   = position.xy * 0.5 + 0.5;

	// Flip due to Metal/Vulkan
	uv.y = 1.0 - uv.y;

	// If sampling outside return fully in light
	if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
	    return 1.0;

	// TODO: Add PCF
	float depth  = texture(shadow_map_sampler, uv).r;

    float max_bias = 0.001, min_bias = 0.00005; // Not great and needs to be idjusted per scene. Find a better way
    float bias = max(max_bias * (1.0 - ndotl), min_bias);
    float shadow = position.z - bias > depth ? 0.1 : 1.0;

	// return 1.0;
	return shadow;
}
