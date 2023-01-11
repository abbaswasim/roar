// From https://github.com/KhronosGroup/glTF-Sample-Viewer/blob/master/source/Renderer/shaders/material_info.glsl#L97
// Generate tangent from normalized geometric normal, world position and texture coordinate using derivatives

vec3 get_tangent(const in vec3 n, const in vec3 p, const in vec2 uv)
{
	vec3 uv_dx = dFdx(vec3(uv, 0.0));
	vec3 uv_dy = dFdy(vec3(uv, 0.0));

	if (length(uv_dx) + length(uv_dy) <= 1e-6)
	{
		uv_dx = vec3(1.0, 0.0, 0.0);
		uv_dy = vec3(0.0, 1.0, 0.0);
	}

	vec3 t = (uv_dy.t * dFdx(p) - uv_dx.t * dFdy(p)) / (uv_dx.s * uv_dy.t - uv_dy.s * uv_dx.t);

	return normalize(t - n * dot(n, t));
}

vec4 get_tangent()
{
	// Only of the following will be enabled at a time
	@return vec4(get_tangent(in_vertex_normal, in_vertex_position.xyz, get_normal_uvs()), 1.0);
	@return in_vertex_tangent;
}

vec3 get_bitangent(const in vec3 n, const in vec4 t)
{
	return cross(n, t.xyz) * t.w;
}

// gn is the geometric normal of the vertex interpolated for the face
vec3 get_normal(const in vec3 gn)
{
	vec3 map = get_normal();

	vec4 gt = get_tangent();
	vec3 gb = get_bitangent(gn, gt);

	vec3 n = normalize(mat3(gt.xyz, gb, gn) * map);

	// // If double sided do the following
	// if (!gl_FrontFacing)
	//	n *= -1.0;

	return n;
}
