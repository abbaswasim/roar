Fragment get_fragment(const Material material)
{
	Fragment fragment;

	fragment.diffuse_color = material.base_color.rgb * (1.0 - material.metallic);
	fragment.position      = in_vertex_position.xyz;
	fragment.normal        = material.normal;
	fragment.view          = material.view;
	fragment.f0            = material.base_color.rgb * material.metallic + (material.reflectance * (1.0 - material.metallic));
	fragment.roughness     = material.roughness * material.roughness;
	fragment.metallic      = material.metallic;
	fragment.occlusion     = material.occlusion;
	fragment.height        = material.height;
	fragment.opacity       = material.opacity;

	return fragment;
}

// TODO: this one needs to be created via code
float get_alpha(const Material material)
{
	return 1.0;
}
