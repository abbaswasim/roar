// Modified version of filament's shading_model_standard.fs shader
vec3 specular_lobe(const Fragment fragment, const vec3 h, float NoV, float NoL, float NoH, float LoH)
{
	float D = distribution(fragment.roughness, NoH, h);
	float V = visibility(fragment.roughness, NoV, NoL);
	vec3  F = fresnel(fragment.f0, LoH);

	return (D * V) * F;
}

vec3 diffuse_lobe(const Fragment fragment, float NoV, float NoL, float LoH)
{
	return fragment.diffuse_color * diffuse(fragment.roughness, NoV, NoL, LoH);
}

vec3 shading_standard(const Fragment fragment, const Light light)
{
	// TODO: Change these to match my material inputs
	vec3  h   = normalize(fragment.view + light.direction);
	float NoV = saturate(dot(fragment.normal, fragment.view));        // fragment.NoV;
	float NoL = saturate(light.normal_dot_direction);
	float NoH = saturate(dot(fragment.normal, h));
	float LoH = saturate(dot(light.direction, h));

	vec3 Fd = diffuse_lobe(fragment, NoV, NoL, LoH);
	vec3 Fr = specular_lobe(fragment, h, NoV, NoL, NoH, LoH);

	vec3 color = Fd + Fr;

	return (color * light.color) * (light.intensity * light.attenuation * NoL * light.shadow * fragment.occlusion);
}

// End of filament's shading_model_standard.fs shader
