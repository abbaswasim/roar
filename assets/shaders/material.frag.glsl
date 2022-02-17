// Not worried about alignment and vec3 usage here, its not going to be used in a UBO/SSBO, but rather an intermediate temporary
// Which should hopefully be removed by driver compilers

struct Material
{
	vec4  diffuse_color;
	vec4  emissive;
	vec4  subsurface;
	vec3  normal;
	vec3  bent_normal;
	vec3  specular_color;
	vec3  anisotropy_direction;
	vec3  sheen_color;
	vec3  clearcoat_normal;
	vec3  subsurface_color;
	float roughness;
	float metallic;
	float reflectance;
	float occlusion;
	float glossyness;
	float anisotropy;
	float transmission_factor;
	float transmission_roughness;
	float sheen_roughness;
	float clearcoat;
	float clearcoat_roughness;
	float height;
	float opacity;
};
