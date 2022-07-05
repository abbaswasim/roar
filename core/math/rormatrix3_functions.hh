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

#include "rormatrix3_functions.hpp"

namespace ror
{
// Matrix3 operators and functions
template <class _type>
FORCE_INLINE Matrix3<_type> operator+(const Matrix3<_type> &a_lhs, const Matrix3<_type> &a_rhs)
{
	return Matrix3<_type>(a_lhs.m_values[0] + a_rhs.m_values[0], a_lhs.m_values[1] + a_rhs.m_values[1], a_lhs.m_values[2] + a_rhs.m_values[2],
	                      a_lhs.m_values[3] + a_rhs.m_values[3], a_lhs.m_values[4] + a_rhs.m_values[4], a_lhs.m_values[5] + a_rhs.m_values[5],
	                      a_lhs.m_values[6] + a_rhs.m_values[6], a_lhs.m_values[7] + a_rhs.m_values[7], a_lhs.m_values[8] + a_rhs.m_values[8]);
}

template <class _type>
FORCE_INLINE Matrix3<_type> operator-(const Matrix3<_type> &a_lhs, const Matrix3<_type> &a_rhs)
{
	return Matrix3<_type>(a_lhs.m_values[0] - a_rhs.m_values[0], a_lhs.m_values[1] - a_rhs.m_values[1], a_lhs.m_values[2] - a_rhs.m_values[2],
	                      a_lhs.m_values[3] - a_rhs.m_values[3], a_lhs.m_values[4] - a_rhs.m_values[4], a_lhs.m_values[5] - a_rhs.m_values[5],
	                      a_lhs.m_values[6] - a_rhs.m_values[6], a_lhs.m_values[7] - a_rhs.m_values[7], a_lhs.m_values[8] - a_rhs.m_values[8]);
}

template <class _type>
FORCE_INLINE Matrix3<_type> operator*(const Matrix3<_type> &a_lhs, const Matrix3<_type> &a_rhs)
{
	return Matrix3<_type>(a_lhs.m_values[0] * a_rhs.m_values[0] + a_lhs.m_values[3] * a_rhs.m_values[1] + a_lhs.m_values[6] * a_rhs.m_values[2],
	                      a_lhs.m_values[1] * a_rhs.m_values[0] + a_lhs.m_values[4] * a_rhs.m_values[1] + a_lhs.m_values[7] * a_rhs.m_values[2],
	                      a_lhs.m_values[2] * a_rhs.m_values[0] + a_lhs.m_values[5] * a_rhs.m_values[1] + a_lhs.m_values[8] * a_rhs.m_values[2],

	                      a_lhs.m_values[0] * a_rhs.m_values[3] + a_lhs.m_values[3] * a_rhs.m_values[4] + a_lhs.m_values[6] * a_rhs.m_values[5],
	                      a_lhs.m_values[1] * a_rhs.m_values[3] + a_lhs.m_values[4] * a_rhs.m_values[4] + a_lhs.m_values[7] * a_rhs.m_values[5],
	                      a_lhs.m_values[2] * a_rhs.m_values[3] + a_lhs.m_values[5] * a_rhs.m_values[4] + a_lhs.m_values[8] * a_rhs.m_values[5],

	                      a_lhs.m_values[0] * a_rhs.m_values[6] + a_lhs.m_values[3] * a_rhs.m_values[7] + a_lhs.m_values[6] * a_rhs.m_values[8],
	                      a_lhs.m_values[1] * a_rhs.m_values[6] + a_lhs.m_values[4] * a_rhs.m_values[7] + a_lhs.m_values[7] * a_rhs.m_values[8],
	                      a_lhs.m_values[2] * a_rhs.m_values[6] + a_lhs.m_values[5] * a_rhs.m_values[7] + a_lhs.m_values[8] * a_rhs.m_values[8]);
}

template <class _type>
FORCE_INLINE bool operator==(const Matrix3<_type> &a_left, const Matrix3<_type> &a_right)
{
	constexpr uint32_t matrix_size = 9;

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
FORCE_INLINE bool operator!=(const Matrix3<_type> &a_left, const Matrix3<_type> &a_right)
{
	return !(a_left == a_right);
}

template <class _type>
FORCE_INLINE Vector3<_type> operator*(const Matrix3<_type> &a_matrix, const Vector3<_type> &a_vector)
{
	return Vector3<_type>(a_matrix.m_values[0] * a_vector.x + a_matrix.m_values[3] * a_vector.y + a_matrix.m_values[6] * a_vector.z,
	                      a_matrix.m_values[1] * a_vector.x + a_matrix.m_values[4] * a_vector.y + a_matrix.m_values[7] * a_vector.z,
	                      a_matrix.m_values[2] * a_vector.x + a_matrix.m_values[5] * a_vector.y + a_matrix.m_values[8] * a_vector.z);
}

// Not the same as above because M * v != v * M
template <class _type>
FORCE_INLINE Vector3<_type> operator*(const Vector3<_type> &a_vector, const Matrix3<_type> &a_matrix)
{
	return Vector3<_type>(a_matrix.m_values[0] * a_vector.x + a_matrix.m_values[1] * a_vector.y + a_matrix.m_values[2] * a_vector.z,
	                      a_matrix.m_values[3] * a_vector.x + a_matrix.m_values[4] * a_vector.y + a_matrix.m_values[5] * a_vector.z,
	                      a_matrix.m_values[6] * a_vector.x + a_matrix.m_values[7] * a_vector.y + a_matrix.m_values[8] * a_vector.z);
}

template <class _type>
FORCE_INLINE Matrix3<_type> operator*(const Matrix3<_type> &a_matrix, _type a_value)
{
	return Matrix3<_type>(a_matrix.m_values[0] * a_value, a_matrix.m_values[1] * a_value, a_matrix.m_values[2] * a_value,
	                      a_matrix.m_values[3] * a_value, a_matrix.m_values[4] * a_value, a_matrix.m_values[5] * a_value,
	                      a_matrix.m_values[6] * a_value, a_matrix.m_values[7] * a_value, a_matrix.m_values[8] * a_value);
}

template <class _type>
FORCE_INLINE Matrix3<_type> operator+(const Matrix3<_type> &a_matrix, _type a_value)
{
	return Matrix3<_type>(a_matrix.m_values[0] + a_value, a_matrix.m_values[1] + a_value, a_matrix.m_values[2] + a_value,
	                      a_matrix.m_values[3] + a_value, a_matrix.m_values[4] + a_value, a_matrix.m_values[5] + a_value,
	                      a_matrix.m_values[6] + a_value, a_matrix.m_values[7] + a_value, a_matrix.m_values[8] + a_value);
}

template <class _type>
FORCE_INLINE Matrix3<_type> operator-(const Matrix3<_type> &a_matrix, _type a_value)
{
	return Matrix3<_type>(a_matrix.m_values[0] - a_value, a_matrix.m_values[1] - a_value, a_matrix.m_values[2] - a_value,
	                      a_matrix.m_values[3] - a_value, a_matrix.m_values[4] - a_value, a_matrix.m_values[5] - a_value,
	                      a_matrix.m_values[6] - a_value, a_matrix.m_values[7] - a_value, a_matrix.m_values[8] - a_value);
}

template <class _type>
FORCE_INLINE Matrix3<_type> operator/(const Matrix3<_type> &a_matrix, _type a_value)
{
	return Matrix3<_type>(a_matrix.m_values[0] / a_value, a_matrix.m_values[1] / a_value, a_matrix.m_values[2] / a_value,
	                      a_matrix.m_values[3] / a_value, a_matrix.m_values[4] / a_value, a_matrix.m_values[5] / a_value,
	                      a_matrix.m_values[6] / a_value, a_matrix.m_values[7] / a_value, a_matrix.m_values[8] / a_value);
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_rotation_around_x(_type a_angle_in_radians)
{
	Matrix3<_type> matrix;

	matrix.m_values[4] = std::cos(a_angle_in_radians);
	matrix.m_values[5] = std::sin(a_angle_in_radians);
	matrix.m_values[7] = -matrix.m_values[5];
	matrix.m_values[8] = matrix.m_values[4];

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_rotation_around_y(_type a_angle_in_radians)
{
	Matrix3<_type> matrix;

	matrix.m_values[0] = std::cos(a_angle_in_radians);
	matrix.m_values[2] = -std::sin(a_angle_in_radians);
	matrix.m_values[6] = -matrix.m_values[2];
	matrix.m_values[8] = matrix.m_values[0];

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_rotation_around_z(_type a_angle_in_radians)
{
	Matrix3<_type> matrix;

	matrix.m_values[0] = std::cos(a_angle_in_radians);
	matrix.m_values[1] = std::sin(a_angle_in_radians);
	matrix.m_values[3] = -matrix.m_values[1];
	matrix.m_values[4] = matrix.m_values[0];

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_rotation(const AxisAngle<_type> &a_axis_angle)
{
	auto &axis             = a_axis_angle.m_axis;
	auto  angle_in_radians = a_axis_angle.m_angle_radians;

	_type sin_of_angle = std::sin(angle_in_radians);
	_type cos_of_angle = std::cos(angle_in_radians);

	Matrix3<_type> matrix;

	matrix.m_values[0] = cos_of_angle + (1 - cos_of_angle) * axis.x * axis.x;
	matrix.m_values[1] = (1 - cos_of_angle) * axis.y * axis.x + sin_of_angle * axis.z;
	matrix.m_values[2] = (1 - cos_of_angle) * axis.z * axis.x - sin_of_angle * axis.y;

	matrix.m_values[3] = (1 - cos_of_angle) * axis.x * axis.y - sin_of_angle * axis.z;
	matrix.m_values[4] = cos_of_angle + (1 - cos_of_angle) * axis.y * axis.y;
	matrix.m_values[5] = (1 - cos_of_angle) * axis.z * axis.y + sin_of_angle * axis.x;

	matrix.m_values[6] = (1 - cos_of_angle) * axis.x * axis.z + sin_of_angle * axis.y;
	matrix.m_values[7] = (1 - cos_of_angle) * axis.y * axis.z - sin_of_angle * axis.x;
	matrix.m_values[8] = cos_of_angle + (1 - cos_of_angle) * axis.z * axis.z;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_transformation(const Quaternion<_type> &a_rotation)
{
	return matrix3_rotation(a_rotation);
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_transformation(const Quaternion<_type> &a_rotation, const Vector3<_type> &a_scaling)
{
	return matrix3_rotation(a_rotation) * matrix3_scaling(a_scaling);
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_rotation(const Quaternion<_type> &a_quaternion)
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

	return Matrix3<_type>(1 - 2 * (yy + zz), 2 * (xy + zw), 2 * (xz - yw),
	                      2 * (xy - zw), 1 - 2 * (xx + zz), 2 * (yz + xw),
	                      2 * (xz + yw), 2 * (yz - xw), 1 - 2 * (xx + yy));
}

/** this conversion uses NASA standard aeroplane conventions as described on page:
 *   https://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm
 *   Coordinate System: right hand
 *   Positive angle: right hand
 *   Order of euler angles: heading first, then attitude, then bank
 */
template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_rotation(const EulerAngle<_type> &a_euler_angles)
{
	// Assuming the angles are in radians.
	_type ch = std::cos(a_euler_angles.m_heading);
	_type sh = std::sin(a_euler_angles.m_heading);
	_type ca = std::cos(a_euler_angles.m_attitude);
	_type sa = std::sin(a_euler_angles.m_attitude);
	_type cb = std::cos(a_euler_angles.m_bank);
	_type sb = std::sin(a_euler_angles.m_bank);

	return Matrix3<_type>(ch * ca, sa, -sh * ca,
	                      sh * sb - ch * sa * cb, ca * cb, sh * sa * cb + ch * sb,
	                      ch * sa * sb + sh * cb, -ca * sb, -sh * sa * sb + ch * cb);
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_scaling(_type a_x, _type a_y, _type a_z)
{
	Matrix3<_type> matrix;

	matrix.m_values[0] = a_x;
	matrix.m_values[4] = a_y;
	matrix.m_values[8] = a_z;

	return matrix;
}

template <class _type>
FORCE_INLINE Matrix3<_type> matrix3_scaling(const Vector3<_type> &a_vector)
{
	return matrix3_scaling(a_vector.x, a_vector.y, a_vector.z);
}

// This one is useful if you know you only have rotation
template <class _type>
FORCE_INLINE Matrix3<_type> inverse_rotation(const Matrix3<_type> &a_matrix)
{
	// Rotation is inverted by transposing top 3x3
	return a_matrix.transposed();
}

template <class _type>
FORCE_INLINE void decompose_into_rotation_scaling(const Matrix3<_type> &a_matrix, Matrix3<_type> &a_rotation, Vector3<_type> &a_scaling)
{
	// https://erkaman.github.io/posts/model_matrix_recover.html
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
FORCE_INLINE Matrix3<_type> matrix3_interpolate(const Matrix3<_type> &a_matrix1, const Matrix3<_type> &a_matrix2, _type a_t)
{
	// TODO Find a better way of doing this use quaternions perhaps
	Matrix3<_type> temp;

	for (uint32_t i = 0; i < 9; ++i)
	{
		temp.m_values[i] = interpolate(a_matrix1.m_values[i], a_matrix2.m_values[i], a_t);
	}

	return temp;
}

}        // namespace ror
