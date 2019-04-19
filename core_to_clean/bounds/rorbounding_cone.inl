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
FORCE_INLINE BoundingCone::BoundingCone(void) :
    m_origin(g_zero_vector3f),
    m_direction(g_zero_vector3f),
    m_expansion_angle(0.0f)
{
}

FORCE_INLINE Vector3f BoundingCone::get_origin() const
{
	return m_origin;
}

FORCE_INLINE void BoundingCone::set_origin(Vector3f &a_origin)
{
	m_origin = a_origin;
}

FORCE_INLINE Vector3f BoundingCone::get_direction() const
{
	return m_direction;
}

FORCE_INLINE void BoundingCone::set_direction(Vector3f &a_direction)
{
	m_direction = a_direction;
}

FORCE_INLINE float32_t BoundingCone::get_expansion_angle() const
{
	return m_expansion_angle;
}

FORCE_INLINE void BoundingCone::set_expansion_angle(float32_t a_expansion_angle)
{
	m_expansion_angle = a_expansion_angle;
	// We Now also have to calculate Sin and Cos of the angles
	m_sin_expansion_angle            = sin(m_expansion_angle);
	m_sin_expansion_angle_reciprocal = 1 / m_sin_expansion_angle;
	m_sin_expansion_angle_square     = m_sin_expansion_angle * m_sin_expansion_angle;

	m_cos_expansion_angle        = cos(m_expansion_angle);
	m_cos_expansion_angle_square = m_cos_expansion_angle * m_cos_expansion_angle;
}

FORCE_INLINE bool BoundingCone::is_point_inside(const Vector3f &a_point) const
{
	MESSAGEBOX("CBoundingCone Function..Not Implemented Yet so should never be called.");
	return true;
}

FORCE_INLINE void BoundingCone::add_bound_to_bound(IBounds *a_bounding)
{
	MESSAGEBOX("CBoundingCone Function..Not Implemented Yet so should never be called.");
}

FORCE_INLINE void BoundingCone::add_point_to_bound(const Vector3f &a_point)
{
	MESSAGEBOX("CBoundingCone Function..Not Implemented Yet so should never be called.");
}

FORCE_INLINE bool BoundingCone::is_bounding_inside(IBounds *a_bounding)
{
	MESSAGEBOX("CBoundingCone Function..Not Implemented Yet so should never be called.");
	return false;
}

FORCE_INLINE void BoundingCone::add_bounding_cone(const BoundingCone *a_bounding_sphere)
{
	MESSAGEBOX("CBoundingCone Function..Not Implemented Yet so should never be called.");
}

FORCE_INLINE bool BoundingCone::is_bounding_cone_inside(const BoundingCone *a_bounding_sphere) const
{
	MESSAGEBOX("CBoundingCone Function..Not Implemented Yet so should never be called.");
	return true;
}

FORCE_INLINE float32_t BoundingCone::sin_expansion_angle() const
{
	return m_sin_expansion_angle;
}

FORCE_INLINE void BoundingCone::sin_expansion_angle(float32_t a_sin_expansion_angle)
{
	m_sin_expansion_angle = a_sin_expansion_angle;
}

FORCE_INLINE float32_t BoundingCone::cos_expansion_angle() const
{
	return m_cos_expansion_angle;
}

FORCE_INLINE void BoundingCone::cos_expansion_angle(float32_t a_cos_expansion_angle)
{
	m_cos_expansion_angle = a_cos_expansion_angle;
}

FORCE_INLINE float32_t BoundingCone::sin_expansion_angle_square() const
{
	return m_sin_expansion_angle_square;
}

FORCE_INLINE void BoundingCone::sin_expansion_angle_square(float32_t a_sin_expansion_angle_square)
{
	m_sin_expansion_angle_square = a_sin_expansion_angle_square;
}

FORCE_INLINE float32_t BoundingCone::cos_expansion_angle_square() const
{
	return m_cos_expansion_angle_square;
}

FORCE_INLINE void BoundingCone::cos_expansion_angle_square(float32_t a_cos_expansion_angle_square)
{
	m_cos_expansion_angle_square = a_cos_expansion_angle_square;
}

FORCE_INLINE float32_t BoundingCone::sin_expansion_angle_reciprocal() const
{
	return m_sin_expansion_angle_reciprocal;
}

FORCE_INLINE void BoundingCone::sin_expansion_angle_reciprocal(float32_t a_sin_expansion_angle_reciprocal)
{
	m_sin_expansion_angle_reciprocal = a_sin_expansion_angle_reciprocal;
}

FORCE_INLINE bool BoundingCone::is_bounding_sphere_inside(const BoundingSphere *a_bounding_sphere) const
{
	/*
		U = K.vertex - (Sphere.radius*K.sinReciprocal)*K.axis;
		D = S.center - U;
		dsqr = Dot(D,D);
		e = Dot(K.axis,D);
		if ( e > 0 and e*e >= dsqr*K.cosSqr )
		{
		D = S.center - K.vertex;
		dsqr = Dot(D,D);
		e = -Dot(K.axis,D);
		if ( e > 0 and e*e >= dsqr*K.sinSqr )
		return dsqr <= S.radiusSqr;
		else
		return true;
		}
		return false;
		*/
	static float32_t bounding_sphere_radius        = a_bounding_sphere->get_radius();
	static float32_t bounding_sphere_radius_square = bounding_sphere_radius * bounding_sphere_radius;
	static Vector3f  U                             = this->m_origin - (this->m_direction * (a_bounding_sphere->get_radius() * this->m_sin_expansion_angle_reciprocal));
	static Vector3f  D                             = a_bounding_sphere->get_center() - U;
	static float32_t distance_square               = D.length_squared();
	static float32_t e                             = this->m_direction.dot_product(D);

	if (e > 0 && e * e >= distance_square * this->m_cos_expansion_angle_square)
	{
		D               = a_bounding_sphere->get_center() - this->m_origin;
		distance_square = D.length_squared();
		e               = -m_direction.dot_product(D);
		if (e > 0 && e * e >= distance_square * this->m_sin_expansion_angle_square)
			return (distance_square <= bounding_sphere_radius_square);
		else
			return true;
	}
	return false;
}

}        // namespace ror
