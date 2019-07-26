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

#include "rormatrix3.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE Matrix3<_type>::Matrix3(_type a_scale) :
	m_values{a_scale, 0, 0,
			 0, a_scale, 0,
			 0, 0, a_scale}
{}

template <class _type>
FORCE_INLINE Matrix3<_type>::Matrix3(const Matrix4<_type> &a_matrix) :
	m_values{a_matrix.m_values[0], a_matrix.m_values[1], a_matrix.m_values[2],
			 a_matrix.m_values[4], a_matrix.m_values[5], a_matrix.m_values[6],
			 a_matrix.m_values[8], a_matrix.m_values[9], a_matrix.m_values[10]}
{}

template <class _type>
FORCE_INLINE Matrix3<_type>::Matrix3(const Matrix3x4<_type> &a_matrix) :
	m_values{a_matrix.m_values[0], a_matrix.m_values[1], a_matrix.m_values[2],
			 a_matrix.m_values[3], a_matrix.m_values[4], a_matrix.m_values[5],
			 a_matrix.m_values[6], a_matrix.m_values[7], a_matrix.m_values[8]}
{}

template <class _type>
FORCE_INLINE Matrix3<_type>::Matrix3(const _type *a_elements) :
	m_values{a_elements[0], a_elements[1], a_elements[2],
			 a_elements[3], a_elements[4], a_elements[5],
			 a_elements[6], a_elements[7], a_elements[8]}
{}

template <class _type>
FORCE_INLINE Matrix3<_type>::Matrix3(const Quaternion<_type> &a_quaternion)
{
	this->set(a_quaternion);
}

template <class _type>
FORCE_INLINE Matrix3<_type>::Matrix3(_type a_0, _type a_1, _type a_2,
									 _type a_3, _type a_4, _type a_5,
									 _type a_6, _type a_7, _type a_8) :
	m_values{a_0, a_1, a_2,
			 a_3, a_4, a_5,
			 a_6, a_7, a_8}
{}

template <class _type>
FORCE_INLINE _type &Matrix3<_type>::get(uint32_t a_row, uint32_t a_column) noexcept
{
	return this->m_values[a_row + a_column * 3];
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set(const Matrix3 &a_matrix) noexcept
{
	std::copy(a_matrix.m_values, a_matrix.m_values + 9, this->m_values);
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set(const _type *a_elements) noexcept
{
	std::copy(a_elements, a_elements + 9, this->m_values);
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set(const Quaternion<_type> &a_quaternion)
{
	// From http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	_type multiplier = static_cast<_type>(1) / a_quaternion.magnitude();

	// Normalize by hand
	_type x = a_quaternion.x * multiplier;
	_type y = a_quaternion.y * multiplier;
	_type z = a_quaternion.z * multiplier;
	_type w = a_quaternion.w * multiplier;

	_type xx = x * x;
	_type xy = x * y;
	_type xz = x * z;
	_type xw = x * w;

	_type yy = y * y;
	_type yz = y * z;
	_type yw = y * w;

	_type zz = z * z;
	_type zw = z * w;

	this->m_values[0] = 1 - 2 * (yy + zz);
	this->m_values[1] = 2 * (xy + zw);
	this->m_values[2] = 2 * (xz - yw);

	this->m_values[3] = 2 * (xy - zw);
	this->m_values[4] = 1 - 2 * (xx + zz);
	this->m_values[5] = 2 * (yz + xw);

	this->m_values[6] = 2 * (xz + yw);
	this->m_values[7] = 2 * (yz - xw);
	this->m_values[8] = 1 - 2 * (xx + yy);
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set(_type a_0, _type a_1, _type a_2,
									  _type a_3, _type a_4, _type a_5,
									  _type a_6, _type a_7, _type a_8) noexcept
{
	this->m_values[0] = a_0;
	this->m_values[1] = a_1;
	this->m_values[2] = a_2;
	this->m_values[3] = a_3;
	this->m_values[4] = a_4;
	this->m_values[5] = a_5;
	this->m_values[6] = a_6;
	this->m_values[7] = a_7;
	this->m_values[8] = a_8;
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set_axis(uint32_t a_axis_index, const Vector3<_type> &a_axis) noexcept
{
	assert(a_axis_index >= 0 && a_axis_index <= 2 && "Index out of bounds");

	uint32_t axis_index = a_axis_index * 3;

	this->m_values[axis_index + 0] = a_axis.x;
	this->m_values[axis_index + 1] = a_axis.y;
	this->m_values[axis_index + 2] = a_axis.z;
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set_x_axis(const Vector3<_type> &a_axis) noexcept
{
	this->m_values[0] = a_axis.x;
	this->m_values[1] = a_axis.y;
	this->m_values[2] = a_axis.z;
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set_y_axis(const Vector3<_type> &a_axis) noexcept
{
	this->m_values[3] = a_axis.x;
	this->m_values[4] = a_axis.y;
	this->m_values[5] = a_axis.z;
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set_z_axis(const Vector3<_type> &a_axis) noexcept
{
	this->m_values[6] = a_axis.x;
	this->m_values[7] = a_axis.y;
	this->m_values[8] = a_axis.z;
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::set_column(uint32_t a_index, const Vector3<_type> &a_column) noexcept
{
	this->set_axis(a_index, a_column);
}

template <class _type>
FORCE_INLINE Vector3<_type> Matrix3<_type>::x_axis() const
{
	return Vector3<_type>(this->m_values[0], this->m_values[1], this->m_values[2]);
}

template <class _type>
FORCE_INLINE Vector3<_type> Matrix3<_type>::y_axis() const
{
	return Vector3<_type>(this->m_values[3], this->m_values[4], this->m_values[5]);
}

template <class _type>
FORCE_INLINE Vector3<_type> Matrix3<_type>::z_axis() const
{
	return Vector3<_type>(this->m_values[6], this->m_values[7], this->m_values[8]);
}

template <class _type>
FORCE_INLINE Vector3<_type> Matrix3<_type>::column(uint32_t a_index) const
{
	assert(a_index >= 0 && a_index <= 2 && "Index out of bounds");

	uint32_t axis_index = a_index * 3;

	return Vector3<_type>(this->m_values[axis_index + 0],
						  this->m_values[axis_index + 1],
						  this->m_values[axis_index + 2]);
}

template <class _type>
FORCE_INLINE Matrix3<_type>::operator const _type *() const
{
	return this->m_values;
}

template <class _type>
FORCE_INLINE Matrix3<_type> &Matrix3<_type>::operator+=(const Matrix3<_type> &a_matrix)
{
	this->m_values[0] += a_matrix.m_values[0];
	this->m_values[1] += a_matrix.m_values[1];
	this->m_values[2] += a_matrix.m_values[2];
	this->m_values[3] += a_matrix.m_values[3];
	this->m_values[4] += a_matrix.m_values[4];
	this->m_values[5] += a_matrix.m_values[5];
	this->m_values[6] += a_matrix.m_values[6];
	this->m_values[7] += a_matrix.m_values[7];
	this->m_values[8] += a_matrix.m_values[8];

	return *this;
}

template <class _type>
FORCE_INLINE Matrix3<_type> &Matrix3<_type>::operator-=(const Matrix3<_type> &a_matrix)
{
	this->m_values[0] -= a_matrix.m_values[0];
	this->m_values[1] -= a_matrix.m_values[1];
	this->m_values[2] -= a_matrix.m_values[2];
	this->m_values[3] -= a_matrix.m_values[3];
	this->m_values[4] -= a_matrix.m_values[4];
	this->m_values[5] -= a_matrix.m_values[5];
	this->m_values[6] -= a_matrix.m_values[6];
	this->m_values[7] -= a_matrix.m_values[7];
	this->m_values[8] -= a_matrix.m_values[8];

	return *this;
}

template <class _type>
FORCE_INLINE Matrix3<_type> &Matrix3<_type>::operator*=(const Matrix3<_type> &a_matrix)
{
	*this = *this * a_matrix;

	return *this;
}

template <class _type>
FORCE_INLINE Matrix3<_type> &Matrix3<_type>::operator*=(_type a_value)
{
	this->m_values[0] *= a_value;
	this->m_values[1] *= a_value;
	this->m_values[2] *= a_value;
	this->m_values[3] *= a_value;
	this->m_values[4] *= a_value;
	this->m_values[5] *= a_value;
	this->m_values[6] *= a_value;
	this->m_values[7] *= a_value;
	this->m_values[8] *= a_value;

	return *this;
}

template <class _type>
FORCE_INLINE Matrix3<_type> Matrix3<_type>::operator-() const
{
	return Matrix3<_type>(-this->m_values[0], -this->m_values[1], -this->m_values[2],
						  -this->m_values[3], -this->m_values[4], -this->m_values[5],
						  -this->m_values[6], -this->m_values[7], -this->m_values[8]);
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::identity() noexcept
{
	this->m_values[0] = 1;
	this->m_values[1] = 0;
	this->m_values[2] = 0;
	this->m_values[3] = 0;
	this->m_values[4] = 1;
	this->m_values[5] = 0;
	this->m_values[6] = 0;
	this->m_values[7] = 0;
	this->m_values[8] = 1;
}

template <class _type>
FORCE_INLINE _type Matrix3<_type>::determinant() const noexcept
{
	return (this->m_values[0] * (this->m_values[4] * this->m_values[8] - this->m_values[5] * this->m_values[7]) -
			this->m_values[3] * (this->m_values[1] * this->m_values[8] - this->m_values[2] * this->m_values[7]) +
			this->m_values[6] * (this->m_values[1] * this->m_values[5] - this->m_values[2] * this->m_values[4]));
}

// template <class _type>
// FORCE_INLINE void Matrix3<_type>::inverse_transpose()
// {
// An orthogonal matrix is a square matrix whose columns and rows are orthogonal unit vectors (i.e., orthonormal vectors), i.e.
// Transpose of an orthogonal matrix is also its inverse

// if orthonormal then return its transpose

// if (!inverse())
// return;

// this->transpose();
// }

template <class _type>
FORCE_INLINE bool Matrix3<_type>::inverse(Matrix3 &a_output_matrix) const
{
	_type det = this->determinant();

	if (equal_zero(det))
		return false;        //singular matrix, not invertible

	// the matrix is invertible, proceed with cofactor expansion
	a_output_matrix.m_values[0] = (this->m_values[4] * this->m_values[8] - this->m_values[5] * this->m_values[7]);
	a_output_matrix.m_values[1] = -(this->m_values[1] * this->m_values[8] - this->m_values[2] * this->m_values[7]);
	a_output_matrix.m_values[2] = (this->m_values[1] * this->m_values[5] - this->m_values[2] * this->m_values[4]);
	a_output_matrix.m_values[3] = -(this->m_values[3] * this->m_values[8] - this->m_values[5] * this->m_values[6]);
	a_output_matrix.m_values[4] = (this->m_values[0] * this->m_values[8] - this->m_values[2] * this->m_values[6]);
	a_output_matrix.m_values[5] = -(this->m_values[0] * this->m_values[5] - this->m_values[2] * this->m_values[3]);
	a_output_matrix.m_values[6] = (this->m_values[3] * this->m_values[7] - this->m_values[4] * this->m_values[6]);
	a_output_matrix.m_values[7] = -(this->m_values[0] * this->m_values[7] - this->m_values[1] * this->m_values[6]);
	a_output_matrix.m_values[8] = (this->m_values[0] * this->m_values[4] - this->m_values[1] * this->m_values[3]);

	//multiply with reciprocal to obtain inverted matrix
	_type det_multiplier = static_cast<_type>(1) / det;

	for (int i = 0; i < 9; i++)
	{
		a_output_matrix.m_values[i] = det_multiplier * a_output_matrix.m_values[i];
	}

	return true;
}

template <class _type>
FORCE_INLINE bool Matrix3<_type>::invert()
{
	Matrix3<_type> tmp{};
	if (this->inverse(tmp))
	{
		*this = tmp;
		return true;
	}

	return false;
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::transpose() noexcept
{
	_type temp;
	temp              = this->m_values[1];
	this->m_values[1] = this->m_values[3];
	this->m_values[3] = temp;

	temp              = this->m_values[2];
	this->m_values[2] = this->m_values[6];
	this->m_values[6] = temp;

	temp              = this->m_values[5];
	this->m_values[5] = this->m_values[7];
	this->m_values[7] = temp;
}

template <class _type>
FORCE_INLINE Matrix3<_type> Matrix3<_type>::transposed() const
{
	return Matrix3<_type>(this->m_values[0], this->m_values[3], this->m_values[6],
						  this->m_values[1], this->m_values[4], this->m_values[7],
						  this->m_values[2], this->m_values[5], this->m_values[8]);
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::normalize()
{
	Vector3<_type> xaxis = this->x_axis();
	Vector3<_type> yaxis = this->y_axis();
	Vector3<_type> zaxis = this->z_axis();

	xaxis.normalize();
	yaxis.normalize();
	zaxis.normalize();

	this->set(xaxis.x, xaxis.y, xaxis.z,
			  yaxis.x, yaxis.y, yaxis.z,
			  zaxis.x, zaxis.y, zaxis.z);
}

template <class _type>
FORCE_INLINE void Matrix3<_type>::orthogonalize()
{
	if (!this->is_orthogonal())
	{
		// uses Gram-Schmidt process to orthogonalize the matrix
		Vector3<_type> w1 = this->x_axis();
		Vector3<_type> v2 = this->y_axis();
		Vector3<_type> v3 = this->z_axis();

		Vector3<_type> w2 = v2 - vector_projection(v2, w1);
		Vector3<_type> w3 = v3 - vector_projection(v3, w1) - vector_projection(v3, w2);

		this->set_x_axis(w1);
		this->set_y_axis(w2);
		this->set_z_axis(w3);
	}
}

template <class _type>
FORCE_INLINE bool Matrix3<_type>::is_orthogonal() const
{
	Vector3<_type> right   = this->x_axis();
	Vector3<_type> up      = this->y_axis();
	Vector3<_type> forward = this->z_axis();

	_type angle1 = angle(right, up);
	_type angle2 = angle(up, forward);
	_type angle3 = angle(forward, right);

	if (std::abs(angle1 - ror_half_pi) < ror_epsilon_relaxed &&
		std::abs(angle2 - ror_half_pi) < ror_epsilon_relaxed &&
		std::abs(angle3 - ror_half_pi) < ror_epsilon_relaxed)
	{
		return true;
	}

	return false;
}

}        // namespace ror
