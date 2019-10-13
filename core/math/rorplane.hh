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

#include "rorplane.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE Plane<_type>::Plane(const Vector3<_type> &a_normal, const Vector3<_type> &a_point)
{
	this->set(a_normal, a_point);
}

template <class _type>
FORCE_INLINE ror::Plane<_type>::Plane(const Vector3<_type> &a_normal, _type a_distance)
{
	this->set(a_normal, a_distance);
}

template <class _type>
FORCE_INLINE Plane<_type>::Plane(const Vector3<_type> &a_point1, const Vector3<_type> &a_point2, const Vector3<_type> &a_point3)
{
	this->set(a_point1, a_point2, a_point3);
}

template <class _type>
FORCE_INLINE ror::Plane<_type>::Plane(_type a_x, _type a_y, _type a_z, _type a_distance)
{
	this->set(a_x, a_y, a_z, a_distance);
}

template <class _type>
bool Plane<_type>::operator==(const Plane &a_other) const noexcept
{
	return (this->m_normal == a_other.m_normal) && decimal_equal(this->m_distance, a_other.m_distance);
}

template <class _type>
bool Plane<_type>::operator!=(const Plane &a_other) const noexcept
{
	return !(*this == a_other);
}

template <class _type>
FORCE_INLINE void Plane<_type>::set(const Vector3<_type> &a_normal, const Vector3<_type> &a_point) noexcept
{
	this->m_normal = a_normal;
	this->m_normal.normalize();

	this->m_distance = this->m_normal.dot_product(a_point);
}

template <class _type>
FORCE_INLINE void ror::Plane<_type>::set(const Vector3<_type> &a_normal, _type a_distance) noexcept
{
	this->m_normal   = a_normal;
	this->m_distance = a_distance;

	this->m_normal.normalize();
}

template <class _type>
FORCE_INLINE void Plane<_type>::set(const Vector3<_type> &a_point1, const Vector3<_type> &a_point2, const Vector3<_type> &a_point3) noexcept
{
	this->m_normal = (a_point3 - a_point1).cross_product((a_point2 - a_point1));
	this->m_normal.normalize();

	this->m_distance = this->m_normal.dot_product(a_point1);
}

template <class _type>
FORCE_INLINE void ror::Plane<_type>::set(_type a_x, _type a_y, _type a_z, _type a_distance) noexcept
{
	this->m_normal.x = a_x;
	this->m_normal.y = a_y;
	this->m_normal.z = a_z;
	this->m_distance = a_distance;

	this->m_normal.normalize();
}

template <class _type>
FORCE_INLINE void Plane<_type>::set_normal(const Vector3<_type> &a_normal) noexcept
{
	this->m_normal = a_normal;
	this->m_normal.normalize();
}

template <class _type>
FORCE_INLINE void Plane<_type>::set_distance(_type a_distance) noexcept
{
	this->m_distance = a_distance;
}

template <class _type>
FORCE_INLINE Vector3<_type> Plane<_type>::normal() const
{
	return this->m_normal;
}

template <class _type>
FORCE_INLINE _type Plane<_type>::distance() const
{
	return this->m_distance;
}

template <class _type>
FORCE_INLINE _type Plane<_type>::distance_to_point(const Vector3<_type> &a_point) const
{
	return this->m_normal.dot_product(a_point) - this->m_distance;
}

template <class _type>
FORCE_INLINE bool Plane<_type>::line_intersection(const Vector3<_type> &a_point1, const Vector3<_type> &a_point2, Vector3<_type> &a_out) const
{
	auto line = a_point2 - a_point1;
	auto lp   = this->m_normal.dot_product(a_point1) - this->m_distance;
	auto ld   = this->m_normal.dot_product(line);

	if (equal_zero(ld))
		return false;

	auto t = lp / ld;
	a_out  = a_point1 - line * t;

	return true;
}

template <class _type>
FORCE_INLINE bool Plane<_type>::segment_on_same_side(const Vector3<_type> &a_point1, const Vector3<_type> &a_point2) const
{
	_type d1 = this->distance_to_point(a_point1);
	_type d2 = this->distance_to_point(a_point2);

	if (d1 > 0 && d2 > 0)
		return true;

	if (d1 <= 0 && d2 <= 0)
		return true;

	return false;
}

template <class _type>
FORCE_INLINE bool Plane<_type>::in_front(const Vector3<_type> &a_point) const
{
	return this->distance_to_point(a_point) > 0;
}

template <class _type>
FORCE_INLINE void Plane<_type>::normalize()
{
	this->m_distance /= this->m_normal.length();
	this->m_normal.normalize();
}

template <class _type>
FORCE_INLINE Vector3<_type> Plane<_type>::point_on_plane() const
{
	return this->m_normal * this->m_distance;
}

template <class _type>
FORCE_INLINE Vector3<_type> Plane<_type>::reflected_vector(const Vector3<_type> &a_p1, const Vector3<_type> &a_p2) const
{
	auto vector = a_p2 - a_p1;
	return vector - (vector_projection(vector, this->m_normal) * 2);
}

template <class _type>
FORCE_INLINE Vector3<_type> Plane<_type>::reflected_at_intersection_vector(const Vector3<_type> &a_p1, const Vector3<_type> &a_p2) const
{
	auto reflected = this->reflected_vector(a_p1, a_p2);

	Vector3<_type> intersection;

	auto intersects = this->line_intersection(a_p1, a_p2, intersection);

	if (intersects)
		return reflected + intersection;

	return reflected;
}

template <class _type>
FORCE_INLINE Matrix4<_type> Plane<_type>::reflection_matrix()
{
	Matrix4<_type> ret;

	ret.m_values[0] = static_cast<_type>(1) - static_cast<_type>(2) * (this->m_normal.x * this->m_normal.x);
	ret.m_values[1] = -static_cast<_type>(2) * (this->m_normal.x * this->m_normal.y);
	ret.m_values[2] = -static_cast<_type>(2) * (this->m_normal.x * this->m_normal.z);
	ret.m_values[3] = -static_cast<_type>(2) * this->m_normal.x * this->m_distance;

	ret.m_values[4] = -static_cast<_type>(2) * (this->m_normal.y * this->m_normal.x);
	ret.m_values[5] = static_cast<_type>(1) - static_cast<_type>(2) * (this->m_normal.y * this->m_normal.y);
	ret.m_values[6] = -static_cast<_type>(2) * (this->m_normal.y * this->m_normal.z);
	ret.m_values[7] = -static_cast<_type>(2) * this->m_normal.y * this->m_distance;

	ret.m_values[8]  = -static_cast<_type>(2) * (this->m_normal.z * this->m_normal.x);
	ret.m_values[9]  = -static_cast<_type>(2) * (this->m_normal.z * this->m_normal.y);
	ret.m_values[10] = static_cast<_type>(1) - static_cast<_type>(2) * (this->m_normal.z * this->m_normal.z);
	ret.m_values[11] = -static_cast<_type>(2) * this->m_normal.z * this->m_distance;

	ret.m_values[12] = static_cast<_type>(0);
	ret.m_values[13] = static_cast<_type>(0);
	ret.m_values[14] = static_cast<_type>(0);
	ret.m_values[15] = static_cast<_type>(1);

	return ret;
}

template <class _type>
FORCE_INLINE Matrix4<_type> Plane<_type>::projection_matrix(const Vector3<_type> &a_point)
{
	_type cpn = a_point.dot_product(this->m_normal) - this->m_distance;

	return Matrix4<_type>(this->m_normal.x * a_point.x - cpn,
						  this->m_normal.x * a_point.y,
						  this->m_normal.x * a_point.z,
						  this->m_normal.x,

						  this->m_normal.y * a_point.x,
						  this->m_normal.y * a_point.y - cpn,
						  this->m_normal.y * a_point.z,
						  this->m_normal.y,

						  this->m_normal.z * a_point.x,
						  this->m_normal.z * a_point.y,
						  this->m_normal.z * a_point.z - cpn,
						  this->m_normal.z,

						  -this->m_distance * a_point.x,
						  -this->m_distance * a_point.y,
						  -this->m_distance * a_point.z,
						  -this->m_distance - cpn);
}

}        // namespace ror
