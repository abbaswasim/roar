// Based on https://github.com/KhronosGroup/glTF-Sample-Viewer/blob/main/source/Renderer/shaders/ibl.glsl
vec3 diffuse_light(vec3 n)
{
	return texture(irradiance_sampler, n).rgb;
}

vec4 specular_sample(vec3 reflection, float lod)
{
	reflection.x *= -1;        // Since my cubemap is flipped in x as well
	return textureLod(radiance_sampler, reflection, lod);
}

vec3 ibl_radiance_ggx(vec3 n, vec3 v, float roughness, vec3 F0, float specularWeight)
{
	float NdotV      = clamp(dot(n, v), 0.0, 1.0);
	float lod        = roughness * float(10);        // Clamping this to 10 LODs only, although the cubemap might have more
	vec3  reflection = normalize(reflect(-v, n));

	vec2 brdf_sample_point = clamp(vec2(NdotV, roughness), vec2(0.0, 0.0), vec2(1.0, 1.0));
	vec2 f_ab              = texture(brdf_integration_sampler, brdf_sample_point).rg;
	vec4 specularSample    = specular_sample(reflection, lod);

	vec3 specularLight = specularSample.rgb;

	// see https://bruop.github.io/ibl/#single_scattering_results at Single Scattering Results
	// Roughness dependent fresnel, from Fdez-Aguera
	vec3 Fr     = max(vec3(1.0 - roughness), F0) - F0;
	vec3 k_S    = F0 + Fr * pow(1.0 - NdotV, 5.0);
	vec3 FssEss = k_S * f_ab.x + f_ab.y;

	return specularWeight * specularLight * FssEss;
}

// specularWeight is introduced with KHR_materials_specular, but in roar its 1.0
vec3 ibl_radiance_lambertian(vec3 n, vec3 v, float roughness, vec3 diffuseColor, vec3 F0, float specularWeight)
{
	float NdotV             = clamp(dot(n, v), 0.0, 1.0);
	vec2  brdf_sample_point = clamp(vec2(NdotV, roughness), vec2(0.0, 0.0), vec2(1.0, 1.0));
	vec2  f_ab              = texture(brdf_integration_sampler, brdf_sample_point).rg;

	vec3 irradiance = diffuse_light(n);

	// see https://bruop.github.io/ibl/#single_scattering_results at Single Scattering Results
	// Roughness dependent fresnel, from Fdez-Aguera

	vec3 Fr     = max(vec3(1.0 - roughness), F0) - F0;
	vec3 k_S    = F0 + Fr * pow(1.0 - NdotV, 5.0);
	vec3 FssEss = specularWeight * k_S * f_ab.x + f_ab.y;        // <--- GGX / specular light contribution (scale it down if the specularWeight is low)

	// Multiple scattering, from Fdez-Aguera
	float Ems    = (1.0 - (f_ab.x + f_ab.y));
	vec3  F_avg  = specularWeight * (F0 + (1.0 - F0) / 21.0);
	vec3  FmsEms = Ems * FssEss * F_avg / (1.0 - F_avg * Ems);
	vec3  k_D    = diffuseColor * (1.0 - FssEss + FmsEms);        // we use +FmsEms as indicated by the formula in the blog post (might be a typo in the implementation)

	return (FmsEms + k_D) * irradiance;
}
