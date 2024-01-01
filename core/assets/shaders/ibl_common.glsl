const float PI = 3.14159265359;

uint hash(uint a)
{
	a = (a + 0x7ed55d16) + (a << 12);
	a = (a ^ 0xc761c23c) ^ (a >> 19);
	a = (a + 0x165667b1) + (a << 5);
	a = (a + 0xd3a2646c) ^ (a << 9);
	a = (a + 0xfd7046c5) + (a << 3);
	a = (a ^ 0xb55a4f09) ^ (a >> 16);
	return a;
}

// Based on https://github.com/JoeyDeVries/LearnOpenGL/tree/master/src/6.pbr/2.2.2.ibl_specular_textured
vec2 world_to_spherical(vec3 v)
{
	const vec2 inv_atan = vec2(0.1591, 0.3183);        // (1/2PI, 1/PI)
	vec2       uv       = vec2(atan(v.x, v.z), asin(v.y));

	uv *= inv_atan;
	uv += 0.5;

	uv.y = 1.0 - uv.y;

	return uv;
}

vec3 invocation_to_direction(vec2 cube_size)
{
	vec2 xy = vec2(gl_GlobalInvocationID.xy);

	vec3 uvz = vec3(((xy / cube_size) * 2.0) - 1.0, 1.0);        // Normalize then scale to -1 to 1
	uvz      = normalize(uvz);

	return uvz;
}

// Rotates the vector in (x, y, positive z) to all other 6 sides
void rotate_to_cube_face(uint face, inout vec3 uvz)
{
	if (face == 0)              // +X
	{
		uvz.xz = uvz.zx;
		uvz.z *= -1;
	}
	else if (face == 1)        // -X
	{
		uvz.xz = uvz.zx;
		uvz.x *= -1;
	}
	else if (face == 2)        // +Y
	{
		uvz.yz = uvz.zy;
		uvz.y *= -1;
	}
	else if (face == 3)        // -Y
	{
		uvz.yz = uvz.zy;
		uvz.z *= -1;
	}
	else if (face == 4)        // +Z
	{}                         // Already in +Z orientation
	else if (face == 5)        // -Z
	{
		uvz.x *= -1;
		uvz.z *= -1;
	}
}

/*
For each cube face gives offset to its location in the following single mip-patch
(0,0)
|----------------|----------------|
|      w         |                |
|                |                |
| h              |                |
|       -X       |       +X       |
|                |                |
|                |                |
|                |                |
|----------------|----------------|
|                |
|                |
|                |
|       +Y       |
|                |
|                |
|                |
|----------------|
|                |
|                |
|                |
|      -Y        |
|                |
|                |
|                |
|----------------|----------------|
|                |                |
|                |                |
|                |                |
|      +Z        |       -Z       |
|                |                |
|                |                |
|                |                |
|----------------|----------------| (1.0, 1.0)
*/
vec4 patch_offset(uint face, uint w, uint h)
{
	if (face == 0)              // +x
	{
		return vec4(w, 0, 2 * w, h);
	}
	else if (face == 1)        // -x
	{
		return vec4(0, 0, w, h);
	}
	else if (face == 2)        // +y
	{
		return vec4(0, h, w, 2 * h);
	}
	else if (face == 3)        // -y
	{
		return vec4(0, 2 * h, w, 3 * h);
	}
	else if (face == 4)        // +z
	{
		return vec4(0, 3 * h, w, 4 * h);
	}
	else if (face == 5)        // -z
	{
		return vec4(w, 3 * h, 2 * w, 4 * h);
	}

	return vec4(0, 0, w, h);
}

vec2 cubemap_sampling_uvs(vec3 direction_vector)
{
	vec2 uv = world_to_spherical(direction_vector);
	return uv;
}

// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float RadicalInverse_VdC(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10;        // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}
