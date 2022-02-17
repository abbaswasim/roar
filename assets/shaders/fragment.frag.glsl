// Not worried about alignment and vec3 usage here, its not going to be used in a UBO/SSBO, but rather an intermediate temporary
// Which should hopefully be removed by driver compilers

struct Fragment
{
	vec4  diffuse_color;
	vec3  sheen_color;
	vec3  subsurface_color;
	float subsurface_power;
	float roughness;
	float metallic;
	float sheen_roughness;
	float clearcoat;
	float clearcoat_roughness;
	float height;
	float opacity;
};
