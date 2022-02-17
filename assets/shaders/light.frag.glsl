struct Light
{
	vec4  color;
	vec3  direction;
	float attenuation;
	vec3  normal_dot_direction;
	vec3  position;
	bool  shadowing;
};
