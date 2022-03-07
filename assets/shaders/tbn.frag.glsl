// From http://www.thetenthplanet.de/archives/1180

mat3 cotangent_frame(const in vec3 N, const in vec3 p, const in vec2 uv)
{
	// get edge vectors of the pixel triangle
	vec3 dp1  = dFdx(p);
	vec3 dp2  = dFdy(p);
	vec2 duv1 = dFdx(uv);
	vec2 duv2 = dFdy(uv);

	// solve the linear system
	vec3 dp2perp = cross(dp2, N);
	vec3 dp1perp = cross(N, dp1);
	vec3 T       = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B       = dp2perp * duv1.y + dp1perp * duv2.y;

	// construct a scale-invariant frame
	float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
	return mat3(T * invmax, B * invmax, N);
}

// assume N, the interpolated vertex normal and
// V, the view vector (vertex to eye)
// map is the normal map value usually via get_normal();

vec3 perturb_normal(const in vec3 N, const in vec3 V, const in vec2 texcoord, const in vec3 map)
{
	mat3 TBN = cotangent_frame(N, -V, texcoord);

	return normalize(TBN * map);
}

vec3 get_normal(const in vec3 N, const in vec3 V)
{
	vec3 pn   = perturb_normal(N, V, get_normal_uvs(), get_normal());

	// // If double sided do the following
	// if (!gl_FrontFacing)
	//	pn *= -1.0;

	return pn;
}
