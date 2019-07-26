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

#pragma once

#include "rormatrix4.hpp"
#include "rorvector3.hpp"

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM Plane final
{
  public:
	FORCE_INLINE Plane()                         = default;                   //! Default constructor
	FORCE_INLINE Plane(const Plane &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Plane(Plane &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Plane &operator=(const Plane &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Plane &operator=(Plane &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Plane() noexcept                          = default;        //! Destructor

	FORCE_INLINE Plane(const Vector3<_type> &a_normal, const Vector3<_type> &a_point);                                         //!< Calculates plane from a point and unit normal
	FORCE_INLINE Plane(const Vector3<_type> &a_normal, _type a_distance);                                                      //!< Calculates plane from a unit normal and distance to origin
	FORCE_INLINE Plane(const Vector3<_type> &a_point1, const Vector3<_type> &a_point2, const Vector3<_type> &a_point3);        //!< Plane from 3 points
	FORCE_INLINE Plane(_type a_x, _type a_y, _type a_z, _type a_distance);

	bool operator==(const Plane &a_other) const noexcept;
	bool operator!=(const Plane &a_other) const noexcept;

	FORCE_INLINE void set(const Vector3<_type> &a_normal, const Vector3<_type> &a_point) noexcept;        //!< Calculates plane from a point and normal
	FORCE_INLINE void set(const Vector3<_type> &a_normal, _type a_distance) noexcept;
	FORCE_INLINE void set(const Vector3<_type> &a_point1, const Vector3<_type> &a_point2, const Vector3<_type> &a_point3) noexcept;
	FORCE_INLINE void set(_type a_x, _type a_y, _type a_z, _type a_distance) noexcept;
	FORCE_INLINE void set_normal(const Vector3<_type> &a_normal) noexcept;
	FORCE_INLINE void set_distance(_type a_distance) noexcept;

	FORCE_INLINE Vector3<_type> normal() const;
	FORCE_INLINE _type distance() const;
	FORCE_INLINE _type distance_to_point(const Vector3<_type> &a_point) const;
	FORCE_INLINE bool  line_intersection(const Vector3<_type> &a_point1, const Vector3<_type> &a_point2, Vector3<_type> &a_out) const;        // TODO Change to line
	FORCE_INLINE bool  segment_on_same_side(const Vector3<_type> &a_point1, const Vector3<_type> &a_point2) const;
	FORCE_INLINE bool  in_front(const Vector3<_type> &a_point) const;
	FORCE_INLINE void  normalize();        //!< Only call if you are sure the plane needs normalization
	FORCE_INLINE Vector3<_type> point_on_plane() const;

	FORCE_INLINE Vector3<_type> reflected_vector(const Vector3<_type> &a_p1, const Vector3<_type> &a_p2) const;
	FORCE_INLINE Vector3<_type> reflected_at_intersection_vector(const Vector3<_type> &a_p1, const Vector3<_type> &a_p2) const;

	FORCE_INLINE Matrix4<_type> reflection_matrix();                                           //!< Returns reflection matrix for this plane, this can be used to reflect points/vectors off of the plane
	FORCE_INLINE Matrix4<_type> projection_matrix(const Vector3<_type> &a_point);        //!< Returns projection matrix from a_point for this plane, this can be used to project points/vectors into the plane

  private:
	Vector3<_type> m_normal   = Vector3<_type>(0);        // Normal to the plane (has to be unit normal)
	_type          m_distance = 0;                        // Distance to the plane from origin, NOT constant factor D in plane equation
};

using Planei = Plane<int32_t>;
using Planef = Plane<float32_t>;
using Planed = Plane<double64_t>;

}        // namespace ror

#include "rorplane.hh"
