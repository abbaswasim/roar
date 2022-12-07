layout(location = 0) out vec4 out_color;

// TODO: Try filament's comments about keeping diffuse and specular separate till the end
vec4 get_lit_color(const Material material, const Fragment fragment)
{
	vec3 color = vec3(0.0);

	apply_image_based_lighting(material, fragment, color);
	apply_directional_lighting(material, fragment, color);
	apply_point_lighting(material, fragment, color);
	apply_spot_lighting(material, fragment, color);

	return vec4(color, 0.0);
}

vec4 get_emissive_color(const Material material)
{
	return get_emissive_lighting(material);
}

void main()
{
	Material material = get_material();
	Fragment fragment = get_fragment(material);

	vec4 diffuse_specular_color = get_lit_color(material, fragment);
	vec4 emissive_color         = get_emissive_color(material);

	diffuse_specular_color.a = material.base_color.a;

	out_color = diffuse_specular_color + emissive_color;
}
