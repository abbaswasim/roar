// Not worried about alignment and vec3 usage here, its not going to be used in a UBO/SSBO, but rather an intermediate temporary
// Which should hopefully be removed by driver compilers

// TODO: Combine this into a working set struct
// This struct is input of all material attributes into the fragment shader, collected for easy access
struct Material
{
	vec4  base_color;
	vec4  emissive;
	vec3  normal;
	vec3  view;
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
};

struct Light
{
	vec3  color;
	vec3  position;
	vec3  direction;
	float shadow;
	float intensity;
	float attenuation;
	float normal_dot_direction;
};

// This struct is the current fragment's working set, final color is diffuse_color + specular_color
struct Fragment
{
	vec3  diffuse_color;
	vec3  position;
	vec3  normal;
	vec3  view;
	vec3  f0;
	float roughness;
	float metallic;
	float occlusion;
	float height;
};
