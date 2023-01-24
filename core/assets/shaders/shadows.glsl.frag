float get_shadow(const mat4 light_space)
{
    highp vec4 light_space_position = light_space * in_vertex_position;
    highp vec3 position = light_space_position.xyz * light_space_position.w;
    highp vec3 uv = position.xyz * 0.5 + 0.5;
    float depth = texture(shadow_map_sampler, uv.xy).r;

    if (uv.z < depth)
        return 1.0;

    return 0.0;
}