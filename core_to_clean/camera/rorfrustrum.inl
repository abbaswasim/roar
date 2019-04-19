// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the 'Software'),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version: 1.0.0

namespace ror
{
FORCE_INLINE Frustrum::Frustrum(void)
{
	this->m_frustrum[rFRUSTRUM_LEFT]   = 0.0f;
	this->m_frustrum[rFRUSTRUM_RIGHT]  = 0.0f;
	this->m_frustrum[rFRUSTRUM_BOTTOM] = 0.0f;
	this->m_frustrum[rFRUSTRUM_TOP]    = 0.0f;
	this->m_frustrum[rFRUSTRUM_NEAR]   = 0.0f;
	this->m_frustrum[rFRUSTRUM_FAR]    = 0.0f;

	this->m_fov  = 45.0f;
	this->m_near = 1.0f;
	this->m_far  = 1000.0f;

	this->m_planes                   = rNew                   Planef[6];
	this->m_frustrum_bounding_cone   = rNew   ror::BoundingCone();
	this->m_frustrum_bounding_sphere = rNew ror::BoundingSphere();
}

FORCE_INLINE Frustrum::Frustrum(float32_t a_left, float32_t a_right, float32_t a_bottom, float32_t a_top, float32_t a_near, float32_t a_far)
{
	this->m_frustrum[rFRUSTRUM_LEFT]   = a_left;
	this->m_frustrum[rFRUSTRUM_RIGHT]  = a_right;
	this->m_frustrum[rFRUSTRUM_BOTTOM] = a_bottom;
	this->m_frustrum[rFRUSTRUM_TOP]    = a_top;
	this->m_frustrum[rFRUSTRUM_NEAR]   = a_near;
	this->m_frustrum[rFRUSTRUM_FAR]    = a_far;

	this->m_fov  = 45.0f;
	this->m_near = 1.0f;
	this->m_far  = 1000.0f;

	this->m_planes                   = rNew                   Planef[6];
	this->m_frustrum_bounding_cone   = rNew   ror::BoundingCone();
	this->m_frustrum_bounding_sphere = rNew ror::BoundingSphere();
}

FORCE_INLINE void Frustrum::set_frustrum(float32_t a_left, float32_t a_right, float32_t a_bottom, float32_t a_top, float32_t a_near, float32_t a_far)
{
	this->m_frustrum[rFRUSTRUM_LEFT]   = a_left;
	this->m_frustrum[rFRUSTRUM_RIGHT]  = a_right;
	this->m_frustrum[rFRUSTRUM_BOTTOM] = a_bottom;
	this->m_frustrum[rFRUSTRUM_TOP]    = a_top;
	this->m_frustrum[rFRUSTRUM_NEAR]   = a_near;
	this->m_frustrum[rFRUSTRUM_FAR]    = a_far;
}

FORCE_INLINE void Frustrum::set_frustrum(float32_t *a_frustrum)
{
	uint32_t size = rFRUSTRUM_MAX * sizeof(float32_t);
	memcpy(this->m_frustrum, a_frustrum, size);
}

FORCE_INLINE float32_t Frustrum::get_far() const
{
	return this->m_far;
}

FORCE_INLINE void Frustrum::set_far(float32_t a_far)
{
	this->m_far = a_far;
}

FORCE_INLINE float32_t Frustrum::get_near() const
{
	return this->m_near;
}

FORCE_INLINE void Frustrum::set_near(float32_t a_near)
{
	this->m_near = a_near;
}

FORCE_INLINE float32_t Frustrum::get_fov() const
{
	return this->m_fov;
}

FORCE_INLINE void Frustrum::set_fov(float32_t a_fov)
{
	this->m_fov = a_fov;
}

FORCE_INLINE void Frustrum::get_frustrum(float32_t &a_left, float32_t &a_right, float32_t &a_bottom, float32_t &a_top, float32_t &a_near, float32_t &a_far) const
{
	a_left   = this->m_left;
	a_right  = this->m_right;
	a_bottom = this->m_bottom;
	a_top    = this->m_top;
	a_near   = this->m_near;
	a_far    = this->m_far;
}

FORCE_INLINE const float32_t *Frustrum::get_frustrum() const
{
	return this->m_frustrum;
}

FORCE_INLINE void Frustrum::calculate_planes(const Matrix4f *a_model_view_projectio_matrix, bool a_normalize)
{
	// Left clipping plane
	this->m_planes[0].set(a_model_view_projectio_matrix->_14 + a_model_view_projectio_matrix->_11, a_model_view_projectio_matrix->_24 + a_model_view_projectio_matrix->_21,
	                      a_model_view_projectio_matrix->_34 + a_model_view_projectio_matrix->_31, a_model_view_projectio_matrix->_44 + a_model_view_projectio_matrix->_41);
	// Right clipping plane
	this->m_planes[1].set(a_model_view_projectio_matrix->_14 - a_model_view_projectio_matrix->_11, a_model_view_projectio_matrix->_24 - a_model_view_projectio_matrix->_21,
	                      a_model_view_projectio_matrix->_34 - a_model_view_projectio_matrix->_31, a_model_view_projectio_matrix->_44 - a_model_view_projectio_matrix->_41);
	// Top clipping plane
	this->m_planes[2].set(a_model_view_projectio_matrix->_14 - a_model_view_projectio_matrix->_12, a_model_view_projectio_matrix->_24 - a_model_view_projectio_matrix->_22,
	                      a_model_view_projectio_matrix->_34 - a_model_view_projectio_matrix->_32, a_model_view_projectio_matrix->_44 - a_model_view_projectio_matrix->_42);
	// Bottom clipping plane
	this->m_planes[3].set(a_model_view_projectio_matrix->_14 + a_model_view_projectio_matrix->_12, a_model_view_projectio_matrix->_24 + a_model_view_projectio_matrix->_22,
	                      a_model_view_projectio_matrix->_34 + a_model_view_projectio_matrix->_32, a_model_view_projectio_matrix->_44 + a_model_view_projectio_matrix->_42);
	// Near clipping plane
	this->m_planes[4].set(a_model_view_projectio_matrix->_14 + a_model_view_projectio_matrix->_13, a_model_view_projectio_matrix->_24 + a_model_view_projectio_matrix->_23,
	                      a_model_view_projectio_matrix->_34 + a_model_view_projectio_matrix->_33, a_model_view_projectio_matrix->_44 + a_model_view_projectio_matrix->_43);
	// Far clipping plane
	this->m_planes[5].set(a_model_view_projectio_matrix->_14 - a_model_view_projectio_matrix->_13, a_model_view_projectio_matrix->_24 - a_model_view_projectio_matrix->_23,
	                      a_model_view_projectio_matrix->_34 - a_model_view_projectio_matrix->_33, a_model_view_projectio_matrix->_44 - a_model_view_projectio_matrix->_43);

	if (a_normalize == true)
	{
		for (uint32_t i = 0; i < rFRUSTRUM_MAX; i++)
			this->m_planes[i].normalize_plane();
	}
}

FORCE_INLINE void Frustrum::calculate_bounding_cone(Vector3f *a_view_direction, Vector3f *a_camera_position, int32_t a_width, int32_t a_height)
{
	// set the properties of the frustum cone… vLookVector is the look vector from the view matrix in the
	// camera.  Position() returns the position of the camera.
	// fWidth is half the width of the screen (in pixels).
	// fHeight is half the height of the screen in pixels.
	// m_fFovRadians is the FOV of the frustum.

	// calculate the length of the fov triangle
	float32_t depth = (float32_t) a_height / tan(RADIANS(this->m_fov) * 0.5f);

	// calculate the corner of the screen
	float32_t corner = sqrt((float32_t)(a_width * a_width + a_height * a_height));

	// now calculate the new fov
	float32_t fov = atan(corner / depth);

	// apply to the cone
	this->m_frustrum_bounding_cone->set_direction(*a_view_direction);
	this->m_frustrum_bounding_cone->set_origin(*a_camera_position);
	this->m_frustrum_bounding_cone->set_expansion_angle(fov);
}

FORCE_INLINE void Frustrum::calculate_bounding_sphere(Vector3f *a_view_direction, Vector3f *a_camera_position)
{
	//calculate the radius of the frustum sphere
	float32_t view_length = this->m_far - this->m_near;

	// use some trig to find the height of the frustum at the far plane
	float32_t height = view_length * tan(RADIANS(this->m_fov) * 0.5f);

	// with an aspect ratio of 1, the width will be the same
	float32_t width = height;

	// halfway point between near/far planes starting at the origin and extending along the z axis
	Vector3f P(0.0f, 0.0f, this->m_near + view_length * 0.5f);

	// the calculate far corner of the frustum
	Vector3f Q(width, height, view_length);

	// the vector between P and Q
	Vector3f v_diff(P - Q);

	// the radius becomes the length of this vector
	this->m_frustrum_bounding_sphere->set_radius(v_diff.length());

	// calculate the center of the sphere
	Vector3f center = *a_camera_position + (*a_view_direction * ((view_length * 0.5f) + this->m_near));

	this->m_frustrum_bounding_sphere->set_center(center);
}

FORCE_INLINE EVisibiltyInFrustrum Frustrum::contains_sphere(const BoundingSphere *a_bounding_sphere, bool a_do_approximate_check) const
{
	if (a_do_approximate_check)
	{
		if (!this->m_frustrum_bounding_sphere->is_bounding_sphere_inside(a_bounding_sphere))
			return rOUTSIDE;
		if (this->m_frustrum_bounding_cone->is_bounding_sphere_inside(a_bounding_sphere))
			return rOUTSIDE;
	}
	float32_t distance;
	for (int32_t i = 0; i < rFRUSTRUM_MAX; ++i)
	{
		Vector3f plane_normal = this->m_planes[i].get_normal();
		distance              = plane_normal.dot_product(a_bounding_sphere->get_center_transformed()) + this->m_planes[i].get_distance();

		if (distance <= -a_bounding_sphere->get_radius())        // if this distance is < -sphere.radius, we are outside
			return rOUTSIDE;

		if ((float32_t) fabs(distance) < a_bounding_sphere->get_radius())        // else if the distance is between +- radius, then we intersect
			return rINTERSECTING;
	}

	return rINSIDE;
}

}        // namespace ror
