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
// is furnished to do sin_angle, subject to the following conditions:
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

#include "rorquaternion.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE Quaternion<_type>::Quaternion(_type a_w) :
    x(0),
    y(0),
    z(0),
    w(a_w)
{}

template <class _type>
FORCE_INLINE Quaternion<_type>::Quaternion(_type a_x, _type a_y, _type a_z, _type a_w) :
    x(a_x),
    y(a_y),
    z(a_z),
    w(a_w)
{}

template <class _type>
FORCE_INLINE Quaternion<_type>::Quaternion(const Matrix3<_type> &a_matrix)
{
	this->set(a_matrix);
}

template <class _type>
FORCE_INLINE Quaternion<_type>::Quaternion(const Matrix4<_type> &a_matrix)
{
	this->set(a_matrix);
}

template <class _type>
FORCE_INLINE Quaternion<_type>::Quaternion(const Matrix3x4<_type> &a_matrix)
{
	this->set(a_matrix);
}

// Calculates the shortest arc quaternion from two vectors
template <class _type>
FORCE_INLINE Quaternion<_type>::Quaternion(const Vector3<_type> &a_vector1, const Vector3<_type> &a_vector2)
{
	this->set(a_vector1, a_vector2);
}

template <class _type>
FORCE_INLINE Quaternion<_type>::Quaternion(const AxisAngle<_type> &a_axis_angle)
{
	this->set(a_axis_angle);
}

template <class _type>
FORCE_INLINE Quaternion<_type>::Quaternion(const EulerAngle<_type> &a_euler_angles)
{
	this->set(a_euler_angles);
}

template <class _type>
FORCE_INLINE void Quaternion<_type>::set(_type a_x, _type a_y, _type a_z, _type a_w) noexcept
{
	this->x = a_x;
	this->y = a_y;
	this->z = a_z;
	this->w = a_w;
}

#define QUAT_MAT_COMMON()                                  \
	_type t;                                               \
	if (m22 < 0)                                           \
	{                                                      \
		if (m00 > m11)                                     \
		{                                                  \
			t = 1 + m00 - m11 - m22;                       \
			this->set(t, m01 + m10, m20 + m02, m12 - m21); \
		}                                                  \
		else                                               \
		{                                                  \
			t = 1 - m00 + m11 - m22;                       \
			this->set(m01 + m10, t, m12 + m21, m20 - m02); \
		}                                                  \
	}                                                      \
	else                                                   \
	{                                                      \
		if (m00 < -m11)                                    \
		{                                                  \
			t = 1 - m00 - m11 + m22;                       \
			this->set(m20 + m02, m12 + m21, t, m01 - m10); \
		}                                                  \
		else                                               \
		{                                                  \
			t = 1 + m00 + m11 + m22;                       \
			this->set(m12 - m21, m20 - m02, m01 - m10, t); \
		}                                                  \
	}                                                      \
	*this *= static_cast<_type>(0.5) / std::sqrt(t)

template <class _type>
FORCE_INLINE void Quaternion<_type>::set(const Matrix3<_type> &a_matrix) noexcept
{
	_type m00 = a_matrix.m_values[0];
	_type m01 = a_matrix.m_values[1];
	_type m02 = a_matrix.m_values[2];

	_type m10 = a_matrix.m_values[3];
	_type m11 = a_matrix.m_values[4];
	_type m12 = a_matrix.m_values[5];

	_type m20 = a_matrix.m_values[6];
	_type m21 = a_matrix.m_values[7];
	_type m22 = a_matrix.m_values[8];

	QUAT_MAT_COMMON();
}

template <class _type>
FORCE_INLINE void Quaternion<_type>::set(const Matrix4<_type> &a_matrix) noexcept
{
	// https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
	_type m00 = a_matrix.m_values[0];
	_type m01 = a_matrix.m_values[1];
	_type m02 = a_matrix.m_values[2];

	_type m10 = a_matrix.m_values[4];
	_type m11 = a_matrix.m_values[5];
	_type m12 = a_matrix.m_values[6];

	_type m20 = a_matrix.m_values[8];
	_type m21 = a_matrix.m_values[9];
	_type m22 = a_matrix.m_values[10];

	QUAT_MAT_COMMON();

	/*
	// How about this code from Matrix4? it looks shorter
	w = 0.5f * sqrtf(a_matrix[0] + a_matrix[5] + a_matrix[10] + a_matrix[15]);
	w = w < 0.0001f ? 0.0004f : 4.0f * w;

	x = (a_matrix[6] - a_matrix[9]) / w;
	y = (a_matrix[8] - a_matrix[2]) / w;
	z = (a_matrix[1] - a_matrix[4]) / w;
	w /= 4.0f;
	*/
}

template <class _type>
FORCE_INLINE void Quaternion<_type>::set(const Matrix3x4<_type> &a_matrix) noexcept
{
	this->set(Matrix3<_type>(a_matrix));
}

#undef QUAT_MAT_COMMON

// Calculates the shortest arc quaternion from two vectors
template <class _type>
FORCE_INLINE void Quaternion<_type>::set(const Vector3<_type> &a_vector1, const Vector3<_type> &a_vector2) noexcept
{
	// http://lolengine.net/blog/2013/09/18/beautiful-maths-quaternion-from-vectors
	// Note: does not handle vectors 180 degress apart
	_type          sqrt_ab = std::sqrt(a_vector1.length_squared() * a_vector2.length_squared());
	Vector3<_type> cross   = a_vector1.cross_product(a_vector2);
	_type          dot     = a_vector1.dot_product(a_vector2);

	this->x = cross.x;
	this->y = cross.y;
	this->z = cross.z;
	this->w = sqrt_ab + dot;

	this->normalize();
}

template <class _type>
FORCE_INLINE void Quaternion<_type>::set(const AxisAngle<_type> &a_axis_angle) noexcept
{
	// Make sure a_axis is normalized
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
	_type half_angle     = static_cast<_type>(0.5) * a_axis_angle.m_angle_radians;
	_type sin_half_angle = std::sin(half_angle);

	this->x = a_axis_angle.m_axis.x * sin_half_angle;
	this->y = a_axis_angle.m_axis.y * sin_half_angle;
	this->z = a_axis_angle.m_axis.z * sin_half_angle;
	this->w = std::cos(half_angle);
}

template <class _type>
FORCE_INLINE void Quaternion<_type>::set(const EulerAngle<_type> &a_euler_angles) noexcept
{
	_type heading_Y  = static_cast<_type>(0.5) * a_euler_angles.m_heading;
	_type altitude_Z = static_cast<_type>(0.5) * a_euler_angles.m_attitude;
	_type bank_X     = static_cast<_type>(0.5) * a_euler_angles.m_bank;

	_type cos_heading  = std::cos(heading_Y);
	_type cos_attitude = std::cos(altitude_Z);
	_type cos_bank     = std::cos(bank_X);

	_type sin_heading  = std::sin(heading_Y);
	_type sin_attitude = std::sin(altitude_Z);
	_type sin_bank     = std::sin(bank_X);

	_type cos_heading_attitude = cos_heading * cos_attitude;
	_type sin_heading_attitude = sin_heading * sin_attitude;

	this->x = sin_heading_attitude * cos_bank + cos_heading_attitude * sin_bank;
	this->y = sin_heading * cos_attitude * cos_bank + cos_heading * sin_attitude * sin_bank;
	this->z = cos_heading * sin_attitude * cos_bank - sin_heading * cos_attitude * sin_bank;

	this->w = cos_heading_attitude * cos_bank - sin_heading_attitude * sin_bank;
}

template <class _type>
FORCE_INLINE Quaternion<_type> &Quaternion<_type>::operator=(const _type *a_array) noexcept
{
	assert(a_array != NULL && "a_array is NULL Quaternion::operator =");

	this->x = a_array[0];
	this->y = a_array[1];
	this->z = a_array[2];
	this->w = a_array[3];

	return *this;
}

template <class _type>
FORCE_INLINE Quaternion<_type> &Quaternion<_type>::operator+=(const Quaternion<_type> &a_rhs) noexcept
{
	this->x += a_rhs.x;
	this->y += a_rhs.y;
	this->z += a_rhs.z;
	this->w += a_rhs.w;

	return *this;
}

template <class _type>
FORCE_INLINE Quaternion<_type> &Quaternion<_type>::operator-=(const Quaternion<_type> &a_rhs) noexcept
{
	this->x -= a_rhs.x;
	this->y -= a_rhs.y;
	this->z -= a_rhs.z;
	this->w -= a_rhs.w;

	return *this;
}

template <class _type>
FORCE_INLINE Quaternion<_type> &Quaternion<_type>::operator*=(const Quaternion<_type> &a_rhs) noexcept
{
	*this = *this * a_rhs;

	return *this;
}

template <class _type>
FORCE_INLINE Quaternion<_type> &Quaternion<_type>::operator+=(_type a_scalar) noexcept
{
	this->x += a_scalar;
	this->y += a_scalar;
	this->z += a_scalar;
	this->w += a_scalar;

	return *this;
}

template <class _type>
FORCE_INLINE Quaternion<_type> &Quaternion<_type>::operator-=(_type a_scalar) noexcept
{
	this->x -= a_scalar;
	this->y -= a_scalar;
	this->z -= a_scalar;
	this->w -= a_scalar;

	return *this;
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::operator+(const Quaternion<_type> &a_rhs) noexcept
{
	return Quaternion<_type>(this->x + a_rhs.x,
	                         this->y + a_rhs.y,
	                         this->z + a_rhs.z,
	                         this->w + a_rhs.w);
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::operator-(const Quaternion<_type> &a_rhs) noexcept
{
	return Quaternion<_type>(this->x - a_rhs.x,
	                         this->y - a_rhs.y,
	                         this->z - a_rhs.z,
	                         this->w - a_rhs.w);
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::operator+(_type a_scalar) noexcept
{
	return Quaternion<_type>(this->x + a_scalar,
	                         this->y + a_scalar,
	                         this->z + a_scalar,
	                         this->w + a_scalar);
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::operator-(_type a_scalar) noexcept
{
	return Quaternion<_type>(this->x - a_scalar,
	                         this->y - a_scalar,
	                         this->z - a_scalar,
	                         this->w - a_scalar);
}

template <class _type>
FORCE_INLINE Quaternion<_type> &Quaternion<_type>::operator*=(_type a_scalar) noexcept
{
	this->x *= a_scalar;
	this->y *= a_scalar;
	this->z *= a_scalar;
	this->w *= a_scalar;

	return *this;
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::operator*(const Quaternion<_type> &a_rhs) const
{
	// q0q1 = (w0v1 + v0w1 + v0 X v1, w0w1 - v0 . v1)
	return Quaternion<_type>(this->w * a_rhs.x + this->x * a_rhs.w + this->y * a_rhs.z - this->z * a_rhs.y,
	                         this->w * a_rhs.y + this->y * a_rhs.w + this->z * a_rhs.x - this->x * a_rhs.z,
	                         this->w * a_rhs.z + this->z * a_rhs.w + this->x * a_rhs.y - this->y * a_rhs.x,
	                         this->w * a_rhs.w - this->x * a_rhs.x - this->y * a_rhs.y - this->z * a_rhs.z);
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::operator*(_type a_scalar) const
{
	return Quaternion<_type>(this->x * a_scalar, this->y * a_scalar, this->z * a_scalar, this->w * a_scalar);
}

template <class _type>
FORCE_INLINE Vector3<_type> Quaternion<_type>::operator*(const Vector3<_type> &a_vector) const
{
	_type xx = this->x * this->x;
	_type yy = this->y * this->y;
	_type zz = this->z * this->z;
	_type ww = this->w * this->w;

	_type xy = this->x * this->y;
	_type yz = this->y * this->z;
	_type zw = this->z * this->w;

	_type xz = this->x * this->z;
	_type xw = this->x * this->w;
	_type yw = this->y * this->w;

	_type a = a_vector.x;
	_type b = a_vector.y;
	_type c = a_vector.z;

	return Vector3<_type>(a * (xx - yy - zz + ww) + 2 * (b * (-zw + xy) + c * (xz + yw)),
	                      b * (-xx + yy - zz + ww) + 2 * (a * (xy + zw) + c * (yz - xw)),
	                      c * (-xx - yy + zz + ww) + 2 * (a * (xz - yw) + b * (yz + xw)));
}

template <class _type>
FORCE_INLINE Quaternion<_type>::operator _type *() noexcept
{
	return &this->x;
}

template <class _type>
FORCE_INLINE Quaternion<_type>::operator const _type *() const noexcept
{
	return &this->x;
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::operator-() const
{
	return Quaternion<_type>(-this->x, -this->y, -this->z, -this->w);
}

template <class _type>
FORCE_INLINE void Quaternion<_type>::identity()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 1;
}

template <class _type>
FORCE_INLINE void Quaternion<_type>::normalize()
{
	_type multiplier = static_cast<_type>(1) / this->magnitude();
	// No check for division by zero
	this->x *= multiplier;
	this->y *= multiplier;
	this->z *= multiplier;
	this->w *= multiplier;
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::normalized() const
{
	_type multiplier = static_cast<_type>(1) / this->magnitude();
	// No check for division by zero
	return Quaternion<_type>(this->x * multiplier, this->y * multiplier, this->z * multiplier, this->w * multiplier);
}

template <class _type>
FORCE_INLINE Quaternion<_type> Quaternion<_type>::conjugate() const
{
	// This could also be inverse if the quaternion is normalized
	return Quaternion<_type>(-this->x, -this->y, -this->z, this->w);
}

template <class _type>
FORCE_INLINE _type Quaternion<_type>::magnitude() const
{
	return std::sqrt(this->x * this->x +
	                 this->y * this->y +
	                 this->z * this->z +
	                 this->w * this->w);
}

template <class _type>
FORCE_INLINE _type Quaternion<_type>::magnitude_squared() const
{
	return this->x * this->x +
	       this->y * this->y +
	       this->z * this->z +
	       this->w * this->w;
}

template <class _type>
FORCE_INLINE bool Quaternion<_type>::inverse(Quaternion<_type> &a_output_quaternion) const
{
	a_output_quaternion = *this;

	return a_output_quaternion.invert();
}

template <class _type>
FORCE_INLINE bool Quaternion<_type>::invert()
{
	_type mag = this->magnitude_squared();

	if (!equal_zero(mag))
	{
		mag = static_cast<_type>(-1) / mag;

		this->x *= mag;
		this->y *= mag;
		this->z *= mag;

		this->w *= -mag;

		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE _type Quaternion<_type>::dot_product(const Quaternion<_type> &a_quaternion) const
{
	return this->x * a_quaternion.x + this->y * a_quaternion.y + this->z * a_quaternion.z + this->w * a_quaternion.w;
}

template <class _type>
FORCE_INLINE _type Quaternion<_type>::angle() const
{
	_type dot_product = this->magnitude_squared();

	if (dot_product < 0)
	{
		return static_cast<_type>(2) * std::acos(-this->w);
	}

	return static_cast<_type>(2) * std::acos(this->w);
}

template <class _type>
FORCE_INLINE Vector3<_type> Quaternion<_type>::axis() const
{
	_type hangle     = this->angle() / static_cast<_type>(2);
	_type sin_hangle = std::sin(hangle);

	return Vector3<_type>(this->x / sin_hangle, this->y / sin_hangle, this->z / sin_hangle).normalized();
}

template <class _type>
FORCE_INLINE AxisAngle<_type> Quaternion<_type>::axis_angle() const
{
	AxisAngle<_type> out;

	_type temp  = std::acos(this->w);
	_type scale = std::sin(temp);

	if ((scale - static_cast<_type>(0.005)) < 0 && 0 < (scale + static_cast<_type>(0.005)))
	{
		out.m_angle_radians = 0;
		out.m_axis.x        = 0;
		out.m_axis.y        = 0;
		out.m_axis.z        = 1;
	}
	else
	{
		out.m_angle_radians = temp * 2;
		out.m_axis.x        = this->x / scale;
		out.m_axis.y        = this->y / scale;
		out.m_axis.z        = this->z / scale;
		out.m_axis.normalize();
	}
	return out;
}

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_rotation_around_x(_type a_radians)
{
	_type angle_over_2 = a_radians * static_cast<_type>(0.5);
	return Quaternion<_type>(std::sin(angle_over_2), 0, 0, std::cos(angle_over_2));
}

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_rotation_around_y(_type a_radians)
{
	_type angle_over_2 = a_radians * static_cast<_type>(0.5);
	return Quaternion<_type>(0, std::sin(angle_over_2), 0, std::cos(angle_over_2));
}

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_rotation_around_z(_type a_radians)
{
	_type angle_over_2 = a_radians * static_cast<_type>(0.5);
	return Quaternion<_type>(0, 0, std::sin(angle_over_2), std::cos(angle_over_2));
}

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_lerp(const Quaternion<_type> &a_quaternion1, const Quaternion<_type> &a_quaternion2, _type a_t)
{
	Quaternion<_type> output;

	output.x = a_quaternion1.x + a_t * (a_quaternion2.x - a_quaternion1.x);
	output.y = a_quaternion1.y + a_t * (a_quaternion2.y - a_quaternion1.y);
	output.z = a_quaternion1.z + a_t * (a_quaternion2.z - a_quaternion1.z);
	output.w = a_quaternion1.w + a_t * (a_quaternion2.w - a_quaternion1.w);

	output.normalize();

	return output;
}

template <class _type>
FORCE_INLINE Quaternion<_type> quaternion_slerp(const Quaternion<_type> &a_quaternion1, const Quaternion<_type> &a_quaternion2, _type a_t)
{
	_type dot_product     = a_quaternion1.dot_product(a_quaternion2);
	_type sign_multiplier = static_cast<_type>(1);

	if (dot_product < 0)
	{
		dot_product     = -dot_product;
		sign_multiplier = static_cast<_type>(-1);
	}

	_type delta = static_cast<_type>(1) - dot_product * dot_product;

	if (delta >= ror_epsilon_squared)
	{
		_type sine         = std::sqrt(delta);
		_type angle        = std::atan2(sine, dot_product);
		_type one_over_sin = static_cast<_type>(1) / sine;

		_type scale0 = std::sin(angle * (static_cast<_type>(1) - a_t)) * one_over_sin;
		_type scale1 = std::sin(angle * a_t) * one_over_sin * sign_multiplier;

		Quaternion<_type> res(a_quaternion1.x * scale0 + a_quaternion2.x * scale1,
		                      a_quaternion1.y * scale0 + a_quaternion2.y * scale1,
		                      a_quaternion1.z * scale0 + a_quaternion2.z * scale1,
		                      a_quaternion1.w * scale0 + a_quaternion2.w * scale1);

		res.normalize();

		return res;
	}

	return quaternion_lerp(a_quaternion1, a_quaternion2, a_t);
}

template <class _type>
FORCE_INLINE _type quaternion_angle(const Quaternion<_type> &a_quaternion1, const Quaternion<_type> &a_quaternion2)
{
	// Calculates the shortest angle between the two quaternions
	_type sqt = std::sqrt(a_quaternion1.magnitude_squared() * a_quaternion2.magnitude_squared());
	_type dot = a_quaternion1.dot_product(a_quaternion2);

	if (dot < 0)
	{
		return std::acos(-dot / sqt) * static_cast<_type>(2);
	}
	else
	{
		return std::acos(dot / sqt) * static_cast<_type>(2);
	}
}

}        // namespace ror
