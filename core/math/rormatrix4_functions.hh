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

#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "math/rorvector3.hpp"
#include "rormatrix4_functions.hpp"

namespace ror
{
// Matrix4 operators and functions
template <class _type>
FORCE_INLINE Matrix4<_type> operator+(const Matrix4<_type> &a_lhs, const Matrix4<_type> &a_rhs)
{
	return Matrix4<_type>(a_lhs.m_values[0] + a_rhs.m_values[0], a_lhs.m_values[1] + a_rhs.m_values[1], a_lhs.m_values[2] + a_rhs.m_values[2], a_lhs.m_values[3] + a_rhs.m_values[3],
	                      a_lhs.m_values[4] + a_rhs.m_values[4], a_lhs.m_values[5] + a_rhs.m_values[5], a_lhs.m_values[6] + a_rhs.m_values[6], a_lhs.m_values[7] + a_rhs.m_values[7],
	                      a_lhs.m_values[8] + a_rhs.m_values[8], a_lhs.m_values[9] + a_rhs.m_values[9], a_lhs.m_values[10] + a_rhs.m_values[10], a_lhs.m_values[11] + a_rhs.m_values[11],
	                      a_lhs.m_values[12] + a_rhs.m_values[12], a_lhs.m_values[13] + a_rhs.m_values[13], a_lhs.m_values[14] + a_rhs.m_values[14], a_lhs.m_values[15] + a_rhs.m_values[15]);
}

template <class _type>
FORCE_INLINE Matrix4<_type> operator-(const Matrix4<_type> &a_lhs, const Matrix4<_type> &a_rhs)
{
	return Matrix4<_type>(a_lhs.m_values[0] - a_rhs.m_values[0], a_lhs.m_values[1] - a_rhs.m_values[1], a_lhs.m_values[2] - a_rhs.m_values[2], a_lhs.m_values[3] - a_rhs.m_values[3],
	                      a_lhs.m_values[4] - a_rhs.m_values[4], a_lhs.m_values[5] - a_rhs.m_values[5], a_lhs.m_values[6] - a_rhs.m_values[6], a_lhs.m_values[7] - a_rhs.m_values[7],
	                      a_lhs.m_values[8] - a_rhs.m_values[8], a_lhs.m_values[9] - a_rhs.m_values[9], a_lhs.m_values[10] - a_rhs.m_values[10], a_lhs.m_values[11] - a_rhs.m_values[11],
	                      a_lhs.m_values[12] - a_rhs.m_values[12], a_lhs.m_values[13] - a_rhs.m_values[13], a_lhs.m_values[14] - a_rhs.m_values[14], a_lhs.m_values[15] - a_rhs.m_values[15]);
}

template <class _type>
FORCE_INLINE Matrix4<_type> operator*(const Matrix4<_type> &a_lhs, const Matrix4<_type> &a_rhs)
{
	return Matrix4<_type>(a_lhs.m_values[0] * a_rhs.m_values[0] + a_lhs.m_values[4] * a_rhs.m_values[1] + a_lhs.m_values[8] * a_rhs.m_values[2] + a_lhs.m_values[12] * a_rhs.m_values[3],
	                      a_lhs.m_values[1] * a_rhs.m_values[0] + a_lhs.m_values[5] * a_rhs.m_values[1] + a_lhs.m_values[9] * a_rhs.m_values[2] + a_lhs.m_values[13] * a_rhs.m_values[3],
	                      a_lhs.m_values[2] * a_rhs.m_values[0] + a_lhs.m_values[6] * a_rhs.m_values[1] + a_lhs.m_values[10] * a_rhs.m_values[2] + a_lhs.m_values[14] * a_rhs.m_values[3],
	                      a_lhs.m_values[3] * a_rhs.m_values[0] + a_lhs.m_values[7] * a_rhs.m_values[1] + a_lhs.m_values[11] * a_rhs.m_values[2] + a_lhs.m_values[15] * a_rhs.m_values[3],

	                      a_lhs.m_values[0] * a_rhs.m_values[4] + a_lhs.m_values[4] * a_rhs.m_values[5] + a_lhs.m_values[8] * a_rhs.m_values[6] + a_lhs.m_values[12] * a_rhs.m_values[7],
	                      a_lhs.m_values[1] * a_rhs.m_values[4] + a_lhs.m_values[5] * a_rhs.m_values[5] + a_lhs.m_values[9] * a_rhs.m_values[6] + a_lhs.m_values[13] * a_rhs.m_values[7],
	                      a_lhs.m_values[2] * a_rhs.m_values[4] + a_lhs.m_values[6] * a_rhs.m_values[5] + a_lhs.m_values[10] * a_rhs.m_values[6] + a_lhs.m_values[14] * a_rhs.m_values[7],
	                      a_lhs.m_values[3] * a_rhs.m_values[4] + a_lhs.m_values[7] * a_rhs.m_values[5] + a_lhs.m_values[11] * a_rhs.m_values[6] + a_lhs.m_values[15] * a_rhs.m_values[7],

	                      a_lhs.m_values[0] * a_rhs.m_values[8] + a_lhs.m_values[4] * a_rhs.m_values[9] + a_lhs.m_values[8] * a_rhs.m_values[10] + a_lhs.m_values[12] * a_rhs.m_values[11],
	                      a_lhs.m_values[1] * a_rhs.m_values[8] + a_lhs.m_values[5] * a_rhs.m_values[9] + a_lhs.m_values[9] * a_rhs.m_values[10] + a_lhs.m_values[13] * a_rhs.m_values[11],
	                      a_lhs.m_values[2] * a_rhs.m_values[8] + a_lhs.m_values[6] * a_rhs.m_values[9] + a_lhs.m_values[10] * a_rhs.m_values[10] + a_lhs.m_values[14] * a_rhs.m_values[11],
	                      a_lhs.m_values[3] * a_rhs.m_values[8] + a_lhs.m_values[7] * a_rhs.m_values[9] + a_lhs.m_values[11] * a_rhs.m_values[10] + a_lhs.m_values[15] * a_rhs.m_values[11],

	                      a_lhs.m_values[0] * a_rhs.m_values[12] + a_lhs.m_values[4] * a_rhs.m_values[13] + a_lhs.m_values[8] * a_rhs.m_values[14] + a_lhs.m_values[12] * a_rhs.m_values[15],
	                      a_lhs.m_values[1] * a_rhs.m_values[12] + a_lhs.m_values[5] * a_rhs.m_values[13] + a_lhs.m_values[9] * a_rhs.m_values[14] + a_lhs.m_values[13] * a_rhs.m_values[15],
	                      a_lhs.m_values[2] * a_rhs.m_values[12] + a_lhs.m_values[6] * a_rhs.m_values[13] + a_lhs.m_values[10] * a_rhs.m_values[14] + a_lhs.m_values[14] * a_rhs.m_values[15],
	                      a_lhs.m_values[3] * a_rhs.m_values[12] + a_lhs.m_values[7] * a_rhs.m_values[13] + a_lhs.m_values[11] * a_rhs.m_values[14] + a_lhs.m_values[15] * a_rhs.m_values[15]);
}

template <class _type>
FORCE_INLINE bool operator==(const Matrix4<_type> &a_left, const Matrix4<_type> &a_right)
{
	constexpr uint32_t matrix_size = 16;

	for (uint32_t index = 0; index < matrix_size; ++index)
	{
		if (!decimal_equal(a_left.m_values[index], a_right.m_values[index]))
		{
			return false;
		}
	}

	return true;
}

template <class _type>
FORCE_INLINE bool operator!=(const Matrix4<_type> &a_left, const Matrix4<_type> &a_right)
{
	return !(a_left == a_right);
}

template <class _type>
FORCE_INLINE Vector3<_type> operator*(const Matrix4<_type> &a_matrix, const Vector3<_type> &a_vector)
{
	return Vector3<_type>(a_matrix.m_values[0] * a_vector.x + a_matrix.m_values[4] * a_vector.y + a_matrix.m_values[8] * a_vector.z + a_matrix.m_values[12],
	                      a_matrix.m_values[1] * a_vector.x + a_matrix.m_values[5] * a_vector.y + a_matrix.m_values[9] * a_vector.z + a_matrix.m_values[13],
	                      a_matrix.m_values[2] * a_vector.x + a_matrix.m_values[6] * a_vector.y + a_matrix.m_values[10] * a_vector.z + a_matrix.m_values[14]);
}

template <class _type>
FORCE_INLINE Vector4<_type> operator*(const Matrix4<_type> &a_matrix, const Vector4<_type> &a_vector)
{
	// TODO: Find out if this needs to be perspective corrected.
	return Vector4<_type>(a_matrix.m_values[0] * a_vector.x + a_matrix.m_values[4] * a_vector.y + a_matrix.m_values[8] * a_vector.z + a_matrix.m_values[12] * a_vector.w,
	                      a_matrix.m_values[1] * a_vector.x + a_matrix.m_values[5] * a_vector.y + a_matrix.m_values[9] * a_vector.z + a_matrix.m_values[13] * a_vector.w,
	                      a_matrix.m_values[2] * a_vector.x + a_matrix.m_values[6] * a_vector.y + a_matrix.m_values[10] * a_vector.z + a_matrix.m_values[14] * a_vector.w,
	                      a_matrix.m_values[3] * a_vector.x + a_matrix.m_values[7] * a_vector.y + a_matrix.m_values[11] * a_vector.z + a_matrix.m_values[15] * a_vector.w);
}

// Not the same as above M * v != v * M
template <class _type>
FORCE_INLINE Vector3<_type> operator*(const Vector3<_type> &a_vector, const Matrix4<_type> &a_matrix)
{
	return Vector3<_type>(a_matrix.m_values[0] * a_vector.x + a_matrix.m_values[1] * a_vector.y + a_matrix.m_values[2] * a_vector.z + a_matrix.m_values[3],
	                      a_matrix.m_values[4] * a_vector.x + a_matrix.m_values[5] * a_vector.y + a_matrix.m_values[6] * a_vector.z + a_matrix.m_values[7],
	                      a_matrix.m_values[8] * a_vector.x + a_matrix.m_values[9] * a_vector.y + a_matrix.m_values[10] * a_vector.z + a_matrix.m_values[11]);
}

template <class _type>
FORCE_INLINE Vector4<_type> operator*(const Vector4<_type> &a_vector, const Matrix4<_type> &a_matrix)
{
	return Vector4<_type>(a_matrix.m_values[0] * a_vector.x + a_matrix.m_values[1] * a_vector.y + a_matrix.m_values[2] * a_vector.z + a_matrix.m_values[3] * a_vector.w,
	                      a_matrix.m_values[4] * a_vector.x + a_matrix.m_values[5] * a_vector.y + a_matrix.m_values[6] * a_vector.z + a_matrix.m_values[7] * a_vector.w,
	                      a_matrix.m_values[8] * a_vector.x + a_matrix.m_values[9] * a_vector.y + a_matrix.m_values[10] * a_vector.z + a_matrix.m_values[11] * a_vector.w,
	                      a_matrix.m_values[12] * a_vector.x + a_matrix.m_values[13] * a_vector.y + a_matrix.m_values[14] * a_vector.z + a_matrix.m_values[15] * a_vector.w);
}

template <class _type>
FORCE_INLINE Matrix4<_type> operator*(const Matrix4<_type> &a_matrix, _type a_value)
{
	return Matrix4<_type>(a_matrix.m_values[0] * a_value, a_matrix.m_values[1] * a_value, a_matrix.m_values[2] * a_value, a_matrix.m_values[3] * a_value,
	                      a_matrix.m_values[4] * a_value, a_matrix.m_values[5] * a_value, a_matrix.m_values[6] * a_value, a_matrix.m_values[7] * a_value,
	                      a_matrix.m_values[8] * a_value, a_matrix.m_values[9] * a_value, a_matrix.m_values[10] * a_value, a_matrix.m_values[11] * a_value,
	                      a_matrix.m_values[12] * a_value, a_matrix.m_values[13] * a_value, a_matrix.m_values[14] * a_value, a_matrix.m_values[15] * a_value);
}

template <class _type>
FORCE_INLINE Matrix4<_type> operator+(const Matrix4<_type> &a_matrix, _type a_value)
{
	return Matrix4<_type>(a_matrix.m_values[0] + a_value, a_matrix.m_values[1] + a_value, a_matrix.m_values[2] + a_value, a_matrix.m_values[3] + a_value,
	                      a_matrix.m_values[4] + a_value, a_matrix.m_values[5] + a_value, a_matrix.m_values[6] + a_value, a_matrix.m_values[7] + a_value,
	                      a_matrix.m_values[8] + a_value, a_matrix.m_values[9] + a_value, a_matrix.m_values[10] + a_value, a_matrix.m_values[11] + a_value,
	                      a_matrix.m_values[12] + a_value, a_matrix.m_values[13] + a_value, a_matrix.m_values[14] + a_value, a_matrix.m_values[15] + a_value);
}

template <class _type>
FORCE_INLINE Matrix4<_type> operator-(const Matrix4<_type> &a_matrix, _type a_value)
{
	return Matrix4<_type>(a_matrix.m_values[0] - a_value, a_matrix.m_values[1] - a_value, a_matrix.m_values[2] - a_value, a_matrix.m_values[3] - a_value,
	                      a_matrix.m_values[4] - a_value, a_matrix.m_values[5] - a_value, a_matrix.m_values[6] - a_value, a_matrix.m_values[7] - a_value,
	                      a_matrix.m_values[8] - a_value, a_matrix.m_values[9] - a_value, a_matrix.m_values[10] - a_value, a_matrix.m_values[11] - a_value,
	                      a_matrix.m_values[12] - a_value, a_matrix.m_values[13] - a_value, a_matrix.m_values[14] - a_value, a_matrix.m_values[15] - a_value);
}

template <class _type>
FORCE_INLINE Matrix4<_type> operator/(const Matrix4<_type> &a_matrix, _type a_value)
{
	return Matrix4<_type>(a_matrix.m_values[0] / a_value, a_matrix.m_values[1] / a_value, a_matrix.m_values[2] / a_value, a_matrix.m_values[3] / a_value,
	                      a_matrix.m_values[4] / a_value, a_matrix.m_values[5] / a_value, a_matrix.m_values[6] / a_value, a_matrix.m_values[7] / a_value,
	                      a_matrix.m_values[8] / a_value, a_matrix.m_values[9] / a_value, a_matrix.m_values[10] / a_value, a_matrix.m_values[11] / a_value,
	                      a_matrix.m_values[12] / a_value, a_matrix.m_values[13] / a_value, a_matrix.m_values[14] / a_value, a_matrix.m_values[15] / a_value);
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_rotation_around_x(_type a_angle_in_radians)
{
	Matrix4<_type> matrix;

	matrix.m_values[5]  = std::cos(a_angle_in_radians);
	matrix.m_values[6]  = std::sin(a_angle_in_radians);
	matrix.m_values[9]  = -matrix.m_values[6];
	matrix.m_values[10] = matrix.m_values[5];

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_rotation_around_y(_type a_angle_in_radians)
{
	Matrix4<_type> matrix;

	matrix.m_values[0]  = std::cos(a_angle_in_radians);
	matrix.m_values[2]  = -std::sin(a_angle_in_radians);
	matrix.m_values[8]  = -matrix.m_values[2];
	matrix.m_values[10] = matrix.m_values[0];

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_rotation_around_z(_type a_angle_in_radians)
{
	Matrix4<_type> matrix;

	matrix.m_values[0] = std::cos(a_angle_in_radians);
	matrix.m_values[1] = std::sin(a_angle_in_radians);
	matrix.m_values[4] = -matrix.m_values[1];
	matrix.m_values[5] = matrix.m_values[0];

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_rotation(const AxisAngle<_type> &a_axis_angle)
{
	auto &axis             = a_axis_angle.m_axis;
	auto  angle_in_radians = a_axis_angle.m_angle_radians;

	_type sin_of_angle = std::sin(angle_in_radians);
	_type cos_of_angle = std::cos(angle_in_radians);

	Matrix4<_type> matrix;

	matrix.m_values[0] = cos_of_angle + (1 - cos_of_angle) * axis.x * axis.x;
	matrix.m_values[1] = (1 - cos_of_angle) * axis.y * axis.x + sin_of_angle * axis.z;
	matrix.m_values[2] = (1 - cos_of_angle) * axis.z * axis.x - sin_of_angle * axis.y;

	matrix.m_values[4] = (1 - cos_of_angle) * axis.x * axis.y - sin_of_angle * axis.z;
	matrix.m_values[5] = cos_of_angle + (1 - cos_of_angle) * axis.y * axis.y;
	matrix.m_values[6] = (1 - cos_of_angle) * axis.z * axis.y + sin_of_angle * axis.x;

	matrix.m_values[8]  = (1 - cos_of_angle) * axis.x * axis.z + sin_of_angle * axis.y;
	matrix.m_values[9]  = (1 - cos_of_angle) * axis.y * axis.z - sin_of_angle * axis.x;
	matrix.m_values[10] = cos_of_angle + (1 - cos_of_angle) * axis.z * axis.z;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_transformation(const Vector3<_type> &a_translation, const Quaternion<_type> &a_rotation)
{
	return matrix4_translation(a_translation) * matrix4_rotation(a_rotation);
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_transformation(const Vector3<_type> &a_translation, const Quaternion<_type> &a_rotation, const Vector3<_type> &a_scaling)
{
	return matrix4_translation(a_translation) * matrix4_rotation(a_rotation) * matrix4_scaling(a_scaling);
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_translation(_type a_x, _type a_y, _type a_z)
{
	Matrix4<_type> matrix;

	matrix.m_values[12] = a_x;
	matrix.m_values[13] = a_y;
	matrix.m_values[14] = a_z;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_translation(const Vector3<_type> &a_vector)
{
	return matrix4_translation(a_vector.x, a_vector.y, a_vector.z);
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_rotation(const Quaternion<_type> &a_quaternion)
{
	// From http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	Quaternion<_type> q = a_quaternion.normalized();

	_type x = q.x;
	_type y = q.y;
	_type z = q.z;
	_type w = q.w;

	_type xx = x * x;
	_type xy = x * y;
	_type xz = x * z;
	_type xw = x * w;

	_type yy = y * y;
	_type yz = y * z;
	_type yw = y * w;

	_type zz = z * z;
	_type zw = z * w;

	return Matrix4<_type>(1 - 2 * (yy + zz), 2 * (xy + zw), 2 * (xz - yw), 0,
	                      2 * (xy - zw), 1 - 2 * (xx + zz), 2 * (yz + xw), 0,
	                      2 * (xz + yw), 2 * (yz - xw), 1 - 2 * (xx + yy), 0,
	                      0, 0, 0, 1);
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_rotation(const Quaternion<_type> &a_quaternion, const Vector3<_type> &a_axis)
{
	// From http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm

	auto quat_rot = matrix4_rotation(a_quaternion);

	_type x = a_axis.x;
	_type y = a_axis.y;
	_type z = a_axis.z;

	quat_rot.m_values[12] = x - x * quat_rot.m_values[0] - y * quat_rot.m_values[4] - z * quat_rot.m_values[8];
	quat_rot.m_values[13] = y - x * quat_rot.m_values[1] - y * quat_rot.m_values[5] - z * quat_rot.m_values[9];
	quat_rot.m_values[14] = z - x * quat_rot.m_values[2] - y * quat_rot.m_values[6] - z * quat_rot.m_values[10];

	return quat_rot;
}

/** this conversion uses NASA standard aeroplane conventions as described on page:
 *   https://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm
 *   Coordinate System: right hand
 *   Positive angle: right hand
 *   Order of euler angles: heading first, then attitude, then bank
 */
template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_rotation(const EulerAngle<_type> &a_euler_angles)
{
	// Assuming the angles are in radians.
	_type ch = std::cos(a_euler_angles.m_heading);
	_type sh = std::sin(a_euler_angles.m_heading);
	_type ca = std::cos(a_euler_angles.m_attitude);
	_type sa = std::sin(a_euler_angles.m_attitude);
	_type cb = std::cos(a_euler_angles.m_bank);
	_type sb = std::sin(a_euler_angles.m_bank);

	return Matrix4<_type>(ch * ca, sa, -sh * ca, 0.0,
	                      sh * sb - ch * sa * cb, ca * cb, sh * sa * cb + ch * sb, 0.0,
	                      ch * sa * sb + sh * cb, -ca * sb, -sh * sa * sb + ch * cb, 0.0,
	                      0.0, 0.0, 0.0, 1.0);
}

/**
 * Calculates a rotation matrix that will rorate a_from to a_to
 * Source https://cs.brown.edu/research/pubs/pdfs/1999/Moller-1999-EBA.pdf
 */
template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_rotation(const Vector3<_type> &a_from, const Vector3<_type> &a_to)
{
	auto v = a_from.cross_product(a_to);
	auto c = a_from.dot_product(a_to);

	auto h = (static_cast<_type>(1) - c) / (static_cast<_type>(1) - c * c);

	ror::Matrix4f rot{};

	rot.m_values[0] = c + h * v.x * v.x;
	rot.m_values[1] = h * v.x * v.y + v.z;
	rot.m_values[2] = h * v.x * v.z - v.y;

	rot.m_values[4] = h * v.x * v.y - v.z;
	rot.m_values[5] = c + h * v.y * v.y;
	rot.m_values[6] = h * v.y * v.z + v.x;

	rot.m_values[8]  = h * v.x * v.z + v.y;
	rot.m_values[9]  = h * v.y * v.z - v.x;
	rot.m_values[10] = c + h * v.z * v.z;

	rot.normalize();

	return rot;
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_scaling(_type a_x, _type a_y, _type a_z)
{
	Matrix4<_type> matrix;

	matrix.m_values[0]  = a_x;
	matrix.m_values[5]  = a_y;
	matrix.m_values[10] = a_z;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_scaling(const Vector3<_type> &a_vector)
{
	return matrix4_scaling(a_vector.x, a_vector.y, a_vector.z);
}

// This one is useful if you know you only have rotation and translation
template <class _type>
FORCE_INLINE Matrix4<_type> inverse_rotation_translation(const Matrix4<_type> &a_matrix)
{
	// Rotation is inverted by transposing top 3x3
	return Matrix4<_type>(a_matrix.m_values[0], a_matrix.m_values[4], a_matrix.m_values[8], 0.0,
	                      a_matrix.m_values[1], a_matrix.m_values[5], a_matrix.m_values[9], 0.0,
	                      a_matrix.m_values[2], a_matrix.m_values[6], a_matrix.m_values[10], 0.0,

	                      -(a_matrix.m_values[0] * a_matrix.m_values[12] + a_matrix.m_values[1] * a_matrix.m_values[13] + a_matrix.m_values[2] * a_matrix.m_values[14]),
	                      -(a_matrix.m_values[4] * a_matrix.m_values[12] + a_matrix.m_values[5] * a_matrix.m_values[13] + a_matrix.m_values[6] * a_matrix.m_values[14]),
	                      -(a_matrix.m_values[8] * a_matrix.m_values[12] + a_matrix.m_values[6] * a_matrix.m_values[13] + a_matrix.m_values[10] * a_matrix.m_values[14]),
	                      1.0);
}

template <class _type>
FORCE_INLINE Matrix4<_type> make_look_at(const Vector3<_type> &a_eye, const Vector3<_type> &a_target, const Vector3<_type> &a_world_up, const Vector3<_type> &a_old_up, const Vector3<_type> &a_old_right)
{
	// compute the forward vector from a_view to a_target
	Vector3<_type> forward = a_target - a_eye;
	forward.normalize();        // make unit length

	// compute the right vector
	Vector3<_type> right = forward.cross_product_safe(a_world_up, a_old_right);
	right.normalize();

	// recompute the orthonormal up vector
	Vector3<_type> up = right.cross_product_safe(forward, a_old_up);
	up.normalize();

	// init 4x4 matrix
	Matrix4<_type> matrix{};

	matrix.m_values[0]  = right.x;
	matrix.m_values[4]  = right.y;
	matrix.m_values[8]  = right.z;
	matrix.m_values[1]  = up.x;
	matrix.m_values[5]  = up.y;
	matrix.m_values[9]  = up.z;
	matrix.m_values[2]  = -forward.x;
	matrix.m_values[6]  = -forward.y;
	matrix.m_values[10] = -forward.z;

	// set translation part by expanding the dot_product of each manually
	matrix.m_values[12] = -right.x * a_eye.x - right.y * a_eye.y - right.z * a_eye.z;
	matrix.m_values[13] = -up.x * a_eye.x - up.y * a_eye.y - up.z * a_eye.z;
	matrix.m_values[14] = forward.x * a_eye.x + forward.y * a_eye.y + forward.z * a_eye.z;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> make_frustum(_type a_left, _type a_right, _type a_bottom, _type a_top, _type a_z_near, _type a_z_far)
{
	_type rl = static_cast<_type>(1) / (a_right - a_left);
	_type tb = static_cast<_type>(1) / (a_top - a_bottom);
	_type fn = static_cast<_type>(1) / (a_z_far - a_z_near);
	_type nf = static_cast<_type>(1) / (a_z_near - a_z_far);

	Matrix4<_type> matrix;

	matrix.m_values[0]  = static_cast<_type>(2) * a_z_near * rl;
	matrix.m_values[5]  = static_cast<_type>(2) * a_z_near * tb;
	matrix.m_values[8]  = (a_right + a_left) * rl;
	matrix.m_values[9]  = (a_top + a_bottom) * tb;
	matrix.m_values[10] = a_z_far * nf;
	matrix.m_values[11] = -1;
	matrix.m_values[14] = -(a_z_far * a_z_near) * fn;
	matrix.m_values[15] = 0;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> make_infinite_frustum(_type a_left, _type a_right, _type a_bottom, _type a_top, _type a_z_near, _type a_z_far)
{
	Matrix4<_type> matrix = make_frustum(a_left, a_right, a_bottom, a_top, a_z_near, a_z_far);

	_type nudge = static_cast<_type>(1) - static_cast<_type>(1) / (1 << 23);        //!< Why the shift by 23, someday will find out

	matrix.m_values[10] = -static_cast<_type>(1) * nudge;
	matrix.m_values[14] = -static_cast<_type>(1) * a_z_near * nudge;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> make_ortho(_type a_left, _type a_right, _type a_bottom, _type a_top, _type a_z_near, _type a_z_far)
{
	_type rl = static_cast<_type>(1) / (a_right - a_left);
	_type tb = static_cast<_type>(1) / (a_top - a_bottom);
	_type fn = static_cast<_type>(1) / (a_z_far - a_z_near);

	Matrix4<_type> matrix(1);

	matrix.m_values[0]  = static_cast<_type>(2) * rl;
	matrix.m_values[5]  = static_cast<_type>(2) * tb;
	matrix.m_values[10] = -static_cast<_type>(1) * fn;
	matrix.m_values[12] = -(a_right + a_left) * rl;
	matrix.m_values[13] = -(a_top + a_bottom) * tb;
	matrix.m_values[14] = -a_z_near * fn;

	return matrix;
}

// All the matrices here are Right Handed that transforms to NDC 0 - 1 instead of -1 to 1
// Both metal and Vulkan are uses NDC [0 - 1] so this works out and is the only supported
template <class _type>
FORCE_INLINE Matrix4<_type> make_perspective(_type a_fov_in_radians, _type a_aspect_ratio, _type a_z_near, _type a_z_far)
{
	// Never set a_z_near to 0.0 otherwise there will be precision loss
	_type angle_in_radians = a_fov_in_radians / static_cast<_type>(2.0f);

	_type delta_z   = a_z_near - a_z_far;
	_type cotangent = static_cast<_type>(1.0f) / std::tan(angle_in_radians);

	Matrix4<_type> matrix;

	matrix.m_values[0]  = cotangent / a_aspect_ratio;
	matrix.m_values[5]  = cotangent;
	matrix.m_values[10] = a_z_far / delta_z;
	matrix.m_values[11] = -1;
	matrix.m_values[14] = a_z_near * a_z_far / delta_z;
	matrix.m_values[15] = 0;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix4<_type> make_infinite_perspective(_type a_fov_in_radians, _type a_aspect_ratio, _type a_z_near, _type a_z_far)
{
	// http://www.terathon.com/gdc07_lengyel.pdf
	// http://www.geometry.caltech.edu/pubs/UD12.pdf
	// By using this matrix make sure you clear your depth like glClearDepth(1.0 − n/f); to avoid rendering farther objects
	// But doesn't this defeats the purpose of having an infinite projection, where you don't want clipping to happen
	Matrix4<_type> out = make_perspective(a_fov_in_radians, a_aspect_ratio, a_z_near, static_cast<_type>(a_z_far));

	// Using epslion here might be losing all the benefit, epsilon is what lengyel suggests 2.4x10-7
	// NOTE: This is slightly different front what's in the pdf because its adjusted for Right Handedness from 0 - 1 instead of -1 to 1
	float epsilon    = 0.00000024f;
	out.m_values[10] = epsilon - static_cast<_type>(1);
	out.m_values[14] = (epsilon - static_cast<_type>(1)) * a_z_near;

	return out;
}

template <class _type>
FORCE_INLINE void ortho_update_depth(_type a_z_near, _type a_z_far, Matrix4<_type> &a_out_matrix)
{
	_type fn = static_cast<_type>(1) / (a_z_far - a_z_near);

	a_out_matrix.m_values[10] = -static_cast<_type>(1) * fn;
	a_out_matrix.m_values[14] = -a_z_near * fn;
}

template <class _type>
FORCE_INLINE void frustum_update_depth(_type a_z_near, _type a_z_far, Matrix4<_type> &a_out_matrix)
{
	_type nf = static_cast<_type>(1) / (a_z_near - a_z_far);
	_type fn = static_cast<_type>(1) / (a_z_far - a_z_near);

	a_out_matrix.m_values[10] = a_z_far * nf;
	a_out_matrix.m_values[14] = -(a_z_far * a_z_near) * fn;
}

template <class _type>
FORCE_INLINE void perspective_update_depth(_type a_z_near, _type a_z_far, Matrix4<_type> &a_out_matrix)
{
	_type nf = static_cast<_type>(1) / (a_z_near - a_z_far);

	a_out_matrix.m_values[10] = a_z_far * nf;
	a_out_matrix.m_values[14] = a_z_near * a_z_far * nf;
}

template <class _type>
FORCE_INLINE void decompose_into_translation_rotation_scaling(const Matrix4<_type> &a_matrix, Vector3<_type> &a_translation, Matrix3<_type> &a_rotation, Vector3<_type> &a_scaling)
{
	// https://erkaman.github.io/posts/model_matrix_recover.html
	a_translation.x = a_matrix.m_values[12];
	a_translation.y = a_matrix.m_values[13];
	a_translation.z = a_matrix.m_values[14];

	Vector3<_type> x = a_matrix.x_axis();
	Vector3<_type> y = a_matrix.y_axis();
	Vector3<_type> z = a_matrix.z_axis();

	a_scaling.x = x.length();
	a_scaling.y = y.length();
	a_scaling.z = z.length();

	a_rotation.set_x_axis(x.normalized());
	a_rotation.set_y_axis(y.normalized());
	a_rotation.set_z_axis(z.normalized());
}

template <class _type>
FORCE_INLINE Matrix4<_type> matrix4_interpolate(const Matrix4<_type> &a_matrix1, const Matrix4<_type> &a_matrix2, _type a_t)
{
	// TODO Find a better way of doing this use quaternions perhaps
	Matrix4<_type> temp;

	for (uint32_t i = 0; i < 16; ++i)
	{
		temp.m_values[i] = interpolate(a_matrix1.m_values[i], a_matrix2.m_values[i], a_t);
	}

	// Vector3<_type> x = temp.x_axis();
	// Vector3<_type> y = temp.y_axis();
	// Vector3<_type> z = temp.z_axis();
	// Vector3<_type> t = temp.origin();

	// temp.set_x_axis(x);
	// temp.set_y_axis(y);
	// temp.set_z_axis(z);
	// temp.set_origin(t);

	return temp;
}

template <class _type>
FORCE_INLINE ror::Vector4<_type> project_to_screen(const ror::Vector4<_type> &world_position, const ror::Matrix4<_type> &view_projection, const ror::Vector4<_type> &view_port, bool &result)
{
	result = true;
	ror::Vector4f output;

	output = view_projection * world_position;

	// Keep the w only normalize x,y,z
	// TODO: Think about what to do if output.w == 0.0f
	output.x /= output.w;
	output.y /= output.w;
	output.z /= output.w;

	// Map the (-1, 1) range of x, y into (0, 1) range
	output.x = output.x * 0.5f + 0.5f;
	output.y = output.y * 0.5f + 0.5f;
	// z is already in (0, 1) range
	// output.z = output.z * 0.5f + 0.5f;

	if (decimal_equal(output.w, 0.0f) || output.x < -output.w || output.x > output.w || output.y < -output.w || output.y > output.w)
		result = false;

	// Invert the y because of Metal and Vulkan viewports (left, top, right, bottom) = (0, 0, width, height)
	output.y = 1.0f - output.y;

	// Map x and y to viewport
	output.x = output.x * view_port.z + view_port.x;
	output.y = output.y * view_port.w + view_port.y;

	// Return w unchanged
	return output;
}

template <class _type>
FORCE_INLINE ror::Vector4<_type> project_to_world(const ror::Vector4<_type> &screen_position, const ror::Matrix4<_type> &view_projection_inverse, const ror::Vector4<_type> &view_port)
{
	// view_projection must be inverted externally
	ror::Vector4f output{screen_position};

	// Reverse the viewport transform
	output.x = (screen_position.x - view_port.x) / view_port.z;
	output.y = (screen_position.y - view_port.y) / view_port.w;
	// output.z = screen_position.z;
	// output.w = screen_position.w;

	// Reverse the viewport height transform
	output.y = 1.0f - output.y;

	// Reverse the (-1, 1) range to (0, 1) range
	// Convert screen position to metal/vulkan NDC space (-1, -1, 0) - (1, 1, 1)
	// Only x and y needs mapping z is in 0 - 1 range because its normalized
	output.x = output.x * 2.0f - 1.0f;
	output.y = output.y * 2.0f - 1.0f;

	// Reverse the w division
	output.x *= output.w;
	output.y *= output.w;
	output.z *= output.w;

	// Invert the projection transform
	output = view_projection_inverse * output;

	return output;
}

template <class _type>
FORCE_INLINE _type screen_to_world_scale(_type a_screen_size, const ror::Vector4<_type> a_origin, const ror::Matrix4<_type> &a_view_projection, const ror::Vector4<_type> &a_viewport)
{
	auto view_projection_inverse = a_view_projection;
	auto res                     = view_projection_inverse.invert();
	assert(res);
	(void) res;

	auto                result{true};
	auto                center0 = ror::project_to_screen(a_origin, a_view_projection, a_viewport, result);
	ror::Vector4<_type> center1{center0.x + a_screen_size, center0.y, center0.z, center0.w};

	auto a = ror::project_to_world(center0, view_projection_inverse, a_viewport);
	auto b = ror::project_to_world(center1, view_projection_inverse, a_viewport);

	return (a - b).length();
}

}        // namespace ror
