vec3 get_F0(const Material material)
{
	// m_base_color defines the diffuse albedo for non-metallic objects and Fresnel reflectance at normal incidence (f0) for metallic ones,
	// material.f0 is either the default 0.04 (4%) or one calculated from provided ior
	// In filament its remapped to what they call "reflecance" and a defaul of 0.5. f0 is calculated in shader like 0.16 * reflectance * reflectance, which also ends up 0.04 for 0.5 default value
	// I am not using the remapping here. And it could be used in editor interface if required, for example, you can get reflectance value from user and convert it to f0 to send to shaders
	return (material.base_color.rgb * material.metallic) + (material.f0 * (1.0 - material.metallic));
}

Fragment get_fragment(const Material material)
{
	Fragment fragment;

	fragment.diffuse_color = material.base_color.rgb * (1.0 - material.metallic);
	fragment.position      = in_vertex_position.xyz;
	fragment.normal        = material.normal;
	fragment.view          = material.view;
	fragment.F0            = get_F0(material); 
	fragment.roughness     = material.roughness * material.roughness;
	fragment.metallic      = material.metallic;
	fragment.occlusion     = material.occlusion;
	fragment.height        = material.height;

	return fragment;
}
