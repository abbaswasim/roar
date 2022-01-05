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

#include "roraxis_angle.hpp"
#include "rormatrix4.hpp"

namespace ror
{
/**
 * Standard quaternion class, only really valid for real values
 * One can still it for other data types but some precision will be lost
 */
template <class _type>
class ROAR_ENGINE_ITEM Quaternion final
{
  public:
	_type x = 0;
	_type y = 0;
	_type z = 0;
	_type w = 1;        // TODO: What happens if you have this and a no arg assign ctor?

	FORCE_INLINE             Quaternion(_type a_w = 1);                                  //! Default constructor
	FORCE_INLINE             Quaternion(const Quaternion &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             Quaternion(Quaternion &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Quaternion &operator=(const Quaternion &a_other) = default;             //! Copy assignment operator
	FORCE_INLINE Quaternion &operator=(Quaternion &&a_other) noexcept = default;         //! Move assignment operator
	FORCE_INLINE ~Quaternion() noexcept                               = default;         //! Destructor

	FORCE_INLINE Quaternion(_type a_x, _type a_y, _type a_z, _type a_w);
	FORCE_INLINE Quaternion(const Matrix3<_type> &a_matrix);
	FORCE_INLINE Quaternion(const Matrix4<_type> &a_matrix);
	FORCE_INLINE Quaternion(const Matrix3x4<_type> &a_matrix);
	FORCE_INLINE Quaternion(const Vector3<_type> &a_vector1, const Vector3<_type> &a_vector2);        //!< Calculates quaternion representing rotation from v1 to v2, no need to normalize v1, v2
	FORCE_INLINE Quaternion(const AxisAngle<_type> &a_axis_angle);
	FORCE_INLINE Quaternion(const EulerAngle<_type> &a_euler_radians);

	FORCE_INLINE void set(_type a_x, _type a_y, _type a_z, _type a_w) noexcept;
	FORCE_INLINE void set(const Matrix3<_type> &a_matrix) noexcept;
	FORCE_INLINE void set(const Matrix4<_type> &a_matrix) noexcept;
	FORCE_INLINE void set(const Matrix3x4<_type> &a_matrix) noexcept;
	FORCE_INLINE void set(const Vector3<_type> &a_vector1, const Vector3<_type> &a_vector2) noexcept;        //!< Calculates quaternion representing rotation from v1 to v2
	FORCE_INLINE void set(const AxisAngle<_type> &a_axis_angle) noexcept;
	FORCE_INLINE void set(const EulerAngle<_type> &a_euler_radians) noexcept;        //!< Euler angles are specified in vector order Y,Z,X

	FORCE_INLINE Quaternion &operator=(const _type *a_array) noexcept;
	FORCE_INLINE Quaternion &operator+=(const Quaternion &a_rhs) noexcept;
	FORCE_INLINE Quaternion &operator-=(const Quaternion &a_rhs) noexcept;
	FORCE_INLINE Quaternion &operator*=(const Quaternion &a_rhs) noexcept;
	FORCE_INLINE Quaternion &operator+=(_type a_scalar) noexcept;
	FORCE_INLINE Quaternion &operator-=(_type a_scalar) noexcept;
	FORCE_INLINE Quaternion &operator*=(_type a_scalar) noexcept;
	FORCE_INLINE Quaternion  operator*(const Quaternion &a_rhs) const;
	FORCE_INLINE Quaternion  operator*(const _type a_scalar) const;
	FORCE_INLINE Vector3<_type> operator*(const Vector3<_type> &a_vector) const;

	FORCE_INLINE Quaternion operator+(const Quaternion &a_rhs) noexcept;
	FORCE_INLINE Quaternion operator-(const Quaternion &a_rhs) noexcept;
	FORCE_INLINE Quaternion operator+(_type a_scalar) noexcept;
	FORCE_INLINE Quaternion operator-(_type a_scalar) noexcept;

	FORCE_INLINE explicit operator _type *() noexcept;                    //!< Only bind to const arrays or const pointers type
	FORCE_INLINE explicit operator const _type *() const noexcept;        //!< Only bind to const arrays or const pointers type
	FORCE_INLINE Quaternion<_type> operator-() const;

	FORCE_INLINE void       identity();
	FORCE_INLINE void       normalize();
	FORCE_INLINE Quaternion normalized() const;
	FORCE_INLINE Quaternion conjugate() const;
	FORCE_INLINE _type      magnitude() const;
	FORCE_INLINE _type      magnitude_squared() const;
	FORCE_INLINE bool       inverse(Quaternion &a_out_quaternion) const;
	FORCE_INLINE bool       invert();
	FORCE_INLINE _type      dot_product(const Quaternion &a_quaternion) const;
	FORCE_INLINE _type      angle() const;
	FORCE_INLINE Vector3<_type> axis() const;
	FORCE_INLINE AxisAngle<_type> axis_angle() const;
};

template <class _type>
FORCE_INLINE bool quaternion_inverse(Quaternion<_type> &a_out_quaternion);

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_rotation_around_x(_type a_radians);

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_rotation_around_y(_type a_radians);

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_rotation_around_z(_type a_radians);

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_lerp(const Quaternion<_type> &a_quaternion1, const Quaternion<_type> &a_quaternion2, _type a_t);

// Make sure passed quaternions are normalized
template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_slerp(const Quaternion<_type> &a_quaternion1, const Quaternion<_type> &a_quaternion2, _type a_t);

// Calculates the shortest angle between the two quaternions
template <class _type>
FORCE_INLINE _type quaternion_angle(const Quaternion<_type> &a_quaternion1, const Quaternion<_type> &a_quaternion2);

using Quaternionf = Quaternion<float32_t>;
using Quaterniond = Quaternion<double64_t>;

const Quaternionf identity_quaternionf(0.0f, 0.0f, 0.0f, 1.0f);
const Quaterniond identity_quaterniond(0.0, 0.0, 0.0, 1.0);

const Quaternionf zero_quaternionf(0.0f, 0.0f, 0.0f, 0.0f);
const Quaterniond zero_quaterniond(0.0, 0.0, 0.0, 0.0);

}        // namespace ror

#include "rorquaternion.hh"
