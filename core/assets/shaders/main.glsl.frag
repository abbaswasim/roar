layout(location = 0) out vec4 out_color;

// set_render_mode placeholder that will be replaced when build shaders
@

// TODO: Try filament's comments about keeping diffuse and specular separate till the end
vec4 get_lit_color(const Material material, const Fragment fragment)
{
	vec3 color = vec3(0.0);

	apply_image_based_lighting(material, fragment, color);
	apply_directional_lighting(material, fragment, color);
	// apply_point_lighting(material, fragment, color);
	// apply_spot_lighting(material, fragment, color);

	return vec4(color, 0.0);
}

vec4 get_emissive_color(const Material material)
{
	return get_emissive_lighting(material);
}

// vec3  diffuse_color; // Spec says The base color has two different interpretations depending on the value of metalness.
// When the material is a metal, the base color is the specific measured reflectance value at normal incidence (F0).
// For a non-metal the base color represents the reflected diffuse color of the material.
// In this model it is not possible to specify a F0 value for non-metals, and a linear value of 4% (0.04) is used.

// the screen does pow(c, 2.2) to your colors, this is why you must provide pow(c, 1/2.2) to the display to get 'c'.

void main()
{
	Material material = get_material();
	Fragment fragment = get_fragment(material);

	vec4 diffuse_specular_color = get_lit_color(material, fragment);
	vec4 emissive_color         = get_emissive_color(material);

	diffuse_specular_color.a = material.base_color.a;
	emissive_color.rgb *= emissive_color.a;
	emissive_color.a = 0.0;

	out_color  = diffuse_specular_color + emissive_color;
	@out_color = material.base_color;        // If the material is unlit this will be uncommented otherwise left commented

	set_render_mode(material, fragment);
}
