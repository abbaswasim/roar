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

	// uv.y = 1.0 - uv.y; // If Y is not flipped anywhere else, it can be done here too

	return uv;
}

// Takes UVs in single image [0..1] and turns them into + shape image for each face
vec2 uv_to_plus_uv(vec2 uv, uint face)
{
	float w = 1.0 / 4.0;
	float h = 1.0 / 3.0;

	uv.x *= w;
	uv.y *= h;

	if (face == 0)
	{
		uv.x += 2.0 * w;
		uv.y += h;
	}
	else if (face == 1)
	{
		uv.y += h;
	}
	else if (face == 2)
	{
		uv.x += w;
	}
	else if (face == 3)
	{
		uv.x += w;
		uv.y += 2.0 * h;
	}
	else if (face == 4)
	{
		uv.x += w;
		uv.y += h;
	}
	else if (face == 5)
	{
		uv.x += 3.0 * w;
		uv.y += h;
	}

	return uv;
}

// Calculates cubemap major axis (face) and UVs from a 3D vector
// Its not used for regular skybox because it ends up with dark borders in some faces
// But for things like convolute we don't have any option, and the border doesn't matter much
vec2 world_to_plus(vec3 v)
{
	vec3 vb = vec3(abs(v.x), abs(v.y), abs(v.z));
	uint face;
	vec2 uv = v.xy;

	if (vb.x >= vb.y && vb.x >= vb.z)
	{
		uv.x = v.z / vb.x;
		uv.y = v.y / vb.x;

		uv = uv * 0.5 + 0.5;

		if (v.x < 0.0)
		{
			face = 1;
			uv.y = 1.0 - uv.y;
		}
		else
		{
			face = 0;
			uv.x = 1.0 - uv.x;
			uv.y = 1.0 - uv.y;
		}
	}
	else if (vb.z >= vb.x && vb.z >= vb.y)
	{
		uv.x = v.x / vb.z;
		uv.y = v.y / vb.z;

		uv = uv * 0.5 + 0.5;

		if (v.z < 0.0)
		{
			face = 5;
			uv.x = 1.0 - uv.x;
			uv.y = 1.0 - uv.y;
		}
		else
		{
			face = 4;
			uv.y = 1.0 - uv.y;
		}
	}
	else        // if (vb.y >= vb.x && vb.y >= vb.z)
	{
		uv.x = v.x / vb.y;
		uv.y = v.z / vb.y;

		uv = uv * 0.5 + 0.5;

		if (v.y < 0.0)        // -Y, NOTE: the index is swapped here to make it obvious, the rotation for +Y is actually the one for -Y, I do this instead of flipping Y uv coordinate
		{
			face = 3;
			uv.y = 1.0 - uv.y;
		}
		else
		{
			face = 2;
		}
	}

	return uv_to_plus_uv(uv, face);
}

vec2 get_uv(vec2 cube_size)
{
	vec2 xy = vec2(gl_GlobalInvocationID.xy);
	return xy / cube_size;
}

vec3 invocation_to_direction(vec2 cube_size)
{
	vec2 uv = get_uv(cube_size);
	uv.y    = 1.0 - uv.y;

	vec3 uvz = vec3((uv * 2.0) - 1.0, 1.0);        // Normalize then scale to -1 to 1
	uvz      = normalize(uvz);

	return uvz;
}

// Rotates the vector in (x, y, positive z) to all other 6 sides
void rotate_to_cube_face(uint face, inout vec3 uvz)
{
	if (face == 0)        // +X
	{
		uvz.xz = uvz.zx;
		uvz.z *= -1;
	}
	else if (face == 1)        // -X
	{
		uvz.xz = uvz.zx;
		uvz.x *= -1;
	}
	else if (face == 3)        // -Y, NOTE: the index is swapped here to make it obvious, the rotation for +Y is actually the one for -Y, I do this instead of flipping Y uv coordinate
	{
		uvz.yz = uvz.zy;
		uvz.y *= -1;
	}
	else if (face == 2)        // +Y
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
	if (face == 0)        // +x
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

// type == 0 is unknown
// type == 1 is equirectangular,
// type == 2 is + shape, like
/*
                 |----------------|
                 |        2       |
                 |                |
                 |                |
                 |       +Y       |
                 |                |
                 |                |
                 |                |
|----------------|----------------|----------------|----------------|
|      1         |        4       |        0       |       5        |
|                |                |                |                |
|                |                |                |                |
| h              |                |                |                |
|       -X       |       +Z       |       +X       |       -Z       |
|                |                |                |                |
|                |                |                |                |
|      w         |                |                |                |
|----------------|----------------|----------------|----------------|
                 |        3       |
                 |                |
                 |                |
                 |       -Y       |
                 |                |
                 |                |
                 |                |
                 |----------------|
*/
// This is the one used by skybox
vec2 cubemap_sampling_uvs(vec3 direction_vector, vec2 uv, uint face, uint type)
{
	if (type == 1)
		uv = world_to_spherical(direction_vector);
	else if (type == 2)
		uv = uv_to_plus_uv(uv, face);

	return uv;
}

// This is the one used by convolute for irradiance
vec2 cubemap_sampling_uvs(vec3 direction_vector, uint type)
{
	vec2 uv;

	if (type == 1)
		uv = world_to_spherical(direction_vector);
	else if (type == 2)
		uv = world_to_plus(direction_vector);

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
