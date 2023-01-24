Light get_directional_light(const Material material, const Fragment fragment, const uint index)
{
	Light            light;
	Directional_lights directional = in_directional_light_uniforms.directional_lights[index];

	// Do direction setup here
	light.color                = directional.color.rgb;
	light.direction            = normalize(directional.direction);
	light.shadow               = @;
	light.intensity            = directional.intensity;
	light.attenuation          = 1.0;
	light.normal_dot_direction = saturate(dot(material.normal, light.direction));

	return light;
}

float point_light_attenuation(float light_distance, float light_radius)
{
	// Cem Yuksel, "Point Light Attenuuation Without Singularity" ACM SIGGRAPH 2020
	float dsqr_rsqr = (light_distance * light_distance) + (light_radius * light_radius);
	return 2.0 / dsqr_rsqr + light_distance * sqrt(dsqr_rsqr);
}

Light get_point_light(const Material material, const Fragment fragment, const uint index)
{
	Light      light;
	Point_lights point = in_point_light_uniforms.point_lights[index];

	light.color                = point.color;
	light.position             = point.position;
	light.direction            = normalize(point.position - fragment.position);
	light.shadow               = @;
	light.intensity            = point.intensity;
	light.attenuation          = point_light_attenuation(length(light.direction), 0.01);
	light.normal_dot_direction = saturate(dot(material.normal, light.direction));

	return light;
}

float spot_angle_attenuation(const Fragment fragment, Spot_lights spot, float light_angle_offset, float light_angle_scale)
{
	// From gltf punctual lights spec
	float cd                  = dot(spot.direction, normalize(spot.position - fragment.position));
	float angular_attenuation = saturate(cd * light_angle_scale + light_angle_offset);

	angular_attenuation *= angular_attenuation;

	return angular_attenuation;
}

Light get_spot_light(const Material material, const Fragment fragment, const uint index)
{
	Light     light;
	Spot_lights spot = in_spot_light_uniforms.spot_lights[index];

	light.color                = spot.color;
	light.position             = spot.position;
	light.direction            = normalize(spot.direction);
	light.shadow               = @;
	light.intensity            = spot.intensity;
	light.attenuation          = point_light_attenuation(length(light.direction), 0.01); // TODO: Check if this should be spot_angle_attenuation
	light.normal_dot_direction = saturate(dot(material.normal, light.direction));

	// From gltf punctual lights spec
	// TODO: Move these two to CPU code
	float light_angle_scale  = 1.0 / max(0.001, cos(spot.inner_angle) - cos(spot.outer_angle));
	float light_angle_offset = -cos(spot.outer_angle) * light_angle_scale;

	light.attenuation *= spot_angle_attenuation(fragment, spot, light_angle_offset, light_angle_scale);

	return light;
}

void apply_image_based_lighting(const Material material, const Fragment fragment, inout vec3 color)
{
	vec3 Fd = vec3(0.0);
	vec3 Fr = vec3(0.0);

	color += Fd + Fr;
}

void apply_directional_lighting(const Material material, const Fragment fragment, inout vec3 color)
{
	for (uint light_index = 0; light_index < directional_lights_count; ++light_index)
	{
		Light light = get_directional_light(material, fragment, light_index);
		color += shading_standard(fragment, light);
	}
}

void apply_point_lighting(const Material material, const Fragment fragment, inout vec3 color)
{
	for (uint light_index = 0; light_index < point_lights_count; ++light_index)
	{
		Light light = get_point_light(material, fragment, light_index);
		color += shading_standard(fragment, light);
	}
}

void apply_spot_lighting(const Material material, const Fragment fragment, inout vec3 color)
{
	for (uint light_index = 0; light_index < spot_lights_count; ++light_index)
	{
		Light light = get_spot_light(material, fragment, light_index);
		color += shading_standard(fragment, light);
	}
}

vec4 get_emissive_lighting(const Material material)
{
	return vec4(material.emissive);
}
