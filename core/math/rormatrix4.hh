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

#include "rormatrix4.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE Matrix4<_type>::Matrix4(_type a_scale) :
	m_values{a_scale, 0, 0, 0,
			 0, a_scale, 0, 0,
			 0, 0, a_scale, 0,
			 0, 0, 0, a_scale}
{}

template <class _type>
FORCE_INLINE Matrix4<_type>::Matrix4(const Matrix3<_type> &a_matrix) :
	m_values{a_matrix.m_values[0], a_matrix.m_values[1], a_matrix.m_values[2], 0,
			 a_matrix.m_values[3], a_matrix.m_values[4], a_matrix.m_values[5], 0,
			 a_matrix.m_values[6], a_matrix.m_values[7], a_matrix.m_values[8], 0,
			 0, 0, 0, 1}
{}

template <class _type>
FORCE_INLINE Matrix4<_type>::Matrix4(const Matrix3x4<_type> &a_matrix) :
	m_values{a_matrix.m_values[0], a_matrix.m_values[1], a_matrix.m_values[2], 0,
			 a_matrix.m_values[3], a_matrix.m_values[4], a_matrix.m_values[5], 0,
			 a_matrix.m_values[6], a_matrix.m_values[7], a_matrix.m_values[8], 0,
			 a_matrix.m_values[9], a_matrix.m_values[10], a_matrix.m_values[11], 1}
{}

template <class _type>
FORCE_INLINE Matrix4<_type>::Matrix4(const Matrix3<_type> &a_matrix, const Vector3<_type> &a_origin) :
	m_values{a_matrix.m_values[0], a_matrix.m_values[1], a_matrix.m_values[2], 0,
			 a_matrix.m_values[3], a_matrix.m_values[4], a_matrix.m_values[5], 0,
			 a_matrix.m_values[6], a_matrix.m_values[7], a_matrix.m_values[8], 0,
			 a_origin.x, a_origin.y, a_origin.z, 1}
{}

template <class _type>
FORCE_INLINE Matrix4<_type>::Matrix4(const _type *a_elements) :
	m_values{a_elements[0], a_elements[1], a_elements[2], a_elements[3],
			 a_elements[4], a_elements[5], a_elements[6], a_elements[7],
			 a_elements[8], a_elements[9], a_elements[10], a_elements[11],
			 a_elements[12], a_elements[13], a_elements[14], a_elements[15]}
{}

template <class _type>
FORCE_INLINE Matrix4<_type>::Matrix4(_type a_0, _type a_1, _type a_2, _type a_3,
									 _type a_4, _type a_5, _type a_6, _type a_7,
									 _type a_8, _type a_9, _type a_10, _type a_11,
									 _type a_12, _type a_13, _type a_14, _type a_15) :
	m_values{a_0, a_1, a_2, a_3,
			 a_4, a_5, a_6, a_7,
			 a_8, a_9, a_10, a_11,
			 a_12, a_13, a_14, a_15}
{}

template <class _type>
FORCE_INLINE _type &Matrix4<_type>::get(uint32_t a_row, uint32_t a_column) noexcept
{
	return this->m_values[a_row + a_column * 4];
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set(const Matrix4 &a_matrix) noexcept
{
	std::copy(a_matrix.m_values, a_matrix.m_values + 16, this->m_values);
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set(const _type *a_elements) noexcept
{
	std::copy(a_elements, a_elements + 16, this->m_values);
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set(_type a_0, _type a_1, _type a_2, _type a_3,
									  _type a_4, _type a_5, _type a_6, _type a_7,
									  _type a_8, _type a_9, _type a_10, _type a_11,
									  _type a_12, _type a_13, _type a_14, _type a_15) noexcept
{
	this->m_values[0]  = a_0;
	this->m_values[1]  = a_1;
	this->m_values[2]  = a_2;
	this->m_values[3]  = a_3;
	this->m_values[4]  = a_4;
	this->m_values[5]  = a_5;
	this->m_values[6]  = a_6;
	this->m_values[7]  = a_7;
	this->m_values[8]  = a_8;
	this->m_values[9]  = a_9;
	this->m_values[10] = a_10;
	this->m_values[11] = a_11;
	this->m_values[12] = a_12;
	this->m_values[13] = a_13;
	this->m_values[14] = a_14;
	this->m_values[15] = a_15;
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set_axis(uint32_t a_axis_index, const Vector4<_type> &a_axis) noexcept
{
	assert(a_axis_index <= 3 && "Index out of bounds");

	uint32_t axis_index = a_axis_index * 4;

	this->m_values[axis_index + 0] = a_axis.x;
	this->m_values[axis_index + 1] = a_axis.y;
	this->m_values[axis_index + 2] = a_axis.z;
	this->m_values[axis_index + 3] = a_axis.w;
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set_x_axis(const Vector3<_type> &a_axis) noexcept
{
	this->m_values[0] = a_axis.x;
	this->m_values[1] = a_axis.y;
	this->m_values[2] = a_axis.z;
	this->m_values[3] = 0;
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set_y_axis(const Vector3<_type> &a_axis) noexcept
{
	this->m_values[4] = a_axis.x;
	this->m_values[5] = a_axis.y;
	this->m_values[6] = a_axis.z;
	this->m_values[7] = 0;
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set_z_axis(const Vector3<_type> &a_axis) noexcept
{
	this->m_values[8]  = a_axis.x;
	this->m_values[9]  = a_axis.y;
	this->m_values[10] = a_axis.z;
	this->m_values[11] = 0;
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set_origin(const Vector3<_type> &a_origin) noexcept
{
	this->m_values[12] = a_origin.x;
	this->m_values[13] = a_origin.y;
	this->m_values[14] = a_origin.z;
	this->m_values[15] = 1;
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::set_column(uint32_t a_index, const Vector4<_type> &a_column) noexcept
{
	this->set_axis(a_index, a_column);
}

template <class _type>
FORCE_INLINE Vector3<_type> Matrix4<_type>::x_axis() const
{
	return Vector3<_type>(this->m_values[0], this->m_values[1], this->m_values[2]);
}

template <class _type>
FORCE_INLINE Vector3<_type> Matrix4<_type>::y_axis() const
{
	return Vector3<_type>(this->m_values[4], this->m_values[5], this->m_values[6]);
}

template <class _type>
FORCE_INLINE Vector3<_type> Matrix4<_type>::z_axis() const
{
	return Vector3<_type>(this->m_values[8], this->m_values[9], this->m_values[10]);
}

template <class _type>
FORCE_INLINE Vector3<_type> Matrix4<_type>::origin() const        //!< Get 3D origin and not the w component, that is always
{
	return Vector3<_type>(this->m_values[12], this->m_values[13], this->m_values[14]);
}

template <class _type>
FORCE_INLINE Vector4<_type> Matrix4<_type>::column(uint32_t a_index) const
{
	assert(a_index <= 3 && "Index out of bounds");

	uint32_t axis_index = a_index * 4;

	return Vector4<_type>(this->m_values[axis_index + 0],
						  this->m_values[axis_index + 1],
						  this->m_values[axis_index + 2],
						  this->m_values[axis_index + 3]);
}

template <class _type>
FORCE_INLINE Matrix4<_type>::operator const _type *() const
{
	return this->m_values;
}

template <class _type>
FORCE_INLINE Matrix4<_type> &Matrix4<_type>::operator+=(const Matrix4<_type> &a_matrix)
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
	this->m_values[9] += a_matrix.m_values[9];
	this->m_values[10] += a_matrix.m_values[10];
	this->m_values[11] += a_matrix.m_values[11];
	this->m_values[12] += a_matrix.m_values[12];
	this->m_values[13] += a_matrix.m_values[13];
	this->m_values[14] += a_matrix.m_values[14];
	this->m_values[15] += a_matrix.m_values[15];

	return *this;
}

template <class _type>
FORCE_INLINE Matrix4<_type> &Matrix4<_type>::operator-=(const Matrix4<_type> &a_matrix)
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
	this->m_values[9] -= a_matrix.m_values[9];
	this->m_values[10] -= a_matrix.m_values[10];
	this->m_values[11] -= a_matrix.m_values[11];
	this->m_values[12] -= a_matrix.m_values[12];
	this->m_values[13] -= a_matrix.m_values[13];
	this->m_values[14] -= a_matrix.m_values[14];
	this->m_values[15] -= a_matrix.m_values[15];

	return *this;
}

template <class _type>
FORCE_INLINE Matrix4<_type> &Matrix4<_type>::operator*=(const Matrix4<_type> &a_matrix)
{
	*this = *this * a_matrix;

	return *this;
}

template <class _type>
FORCE_INLINE Matrix4<_type> &Matrix4<_type>::operator*=(_type a_value)
{
	// Is this practical? Scaling translation as well as a_m[15]?
	this->m_values[0] *= a_value;
	this->m_values[1] *= a_value;
	this->m_values[2] *= a_value;
	this->m_values[3] *= a_value;
	this->m_values[4] *= a_value;
	this->m_values[5] *= a_value;
	this->m_values[6] *= a_value;
	this->m_values[7] *= a_value;
	this->m_values[8] *= a_value;
	this->m_values[9] *= a_value;
	this->m_values[10] *= a_value;
	this->m_values[11] *= a_value;
	this->m_values[12] *= a_value;
	this->m_values[13] *= a_value;
	this->m_values[14] *= a_value;
	this->m_values[15] *= a_value;

	return *this;
}

template <class _type>
FORCE_INLINE Matrix4<_type> Matrix4<_type>::operator-() const
{
	return Matrix4<_type>(-this->m_values[0], -this->m_values[1], -this->m_values[2], -this->m_values[3],
						  -this->m_values[4], -this->m_values[5], -this->m_values[6], -this->m_values[7],
						  -this->m_values[8], -this->m_values[9], -this->m_values[10], -this->m_values[11],
						  -this->m_values[12], -this->m_values[13], -this->m_values[14], -this->m_values[15]);
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::identity() noexcept
{
	this->m_values[0]  = 1;
	this->m_values[1]  = 0;
	this->m_values[2]  = 0;
	this->m_values[3]  = 0;
	this->m_values[4]  = 0;
	this->m_values[5]  = 1;
	this->m_values[6]  = 0;
	this->m_values[7]  = 0;
	this->m_values[8]  = 0;
	this->m_values[9]  = 0;
	this->m_values[10] = 1;
	this->m_values[11] = 0;
	this->m_values[12] = 0;
	this->m_values[13] = 0;
	this->m_values[14] = 0;
	this->m_values[15] = 1;
}

template <class _type>
FORCE_INLINE _type Matrix4<_type>::determinant() const noexcept
{
	_type a0 = this->m_values[0] * this->m_values[5] - this->m_values[4] * this->m_values[1];
	_type a1 = this->m_values[0] * this->m_values[9] - this->m_values[8] * this->m_values[1];
	_type a2 = this->m_values[0] * this->m_values[13] - this->m_values[12] * this->m_values[1];
	_type a3 = this->m_values[4] * this->m_values[9] - this->m_values[8] * this->m_values[5];
	_type a4 = this->m_values[4] * this->m_values[13] - this->m_values[12] * this->m_values[5];
	_type a5 = this->m_values[8] * this->m_values[13] - this->m_values[12] * this->m_values[9];
	_type b0 = this->m_values[2] * this->m_values[7] - this->m_values[6] * this->m_values[3];
	_type b1 = this->m_values[2] * this->m_values[11] - this->m_values[10] * this->m_values[3];
	_type b2 = this->m_values[2] * this->m_values[15] - this->m_values[14] * this->m_values[3];
	_type b3 = this->m_values[6] * this->m_values[11] - this->m_values[10] * this->m_values[7];
	_type b4 = this->m_values[6] * this->m_values[15] - this->m_values[14] * this->m_values[7];
	_type b5 = this->m_values[10] * this->m_values[15] - this->m_values[14] * this->m_values[11];

	return ((a0 * b5) - (a1 * b4) + (a2 * b3) + (a3 * b2) - (a4 * b1) + (a5 * b0));
}

// template <class _type>
// FORCE_INLINE void Matrix4<_type>::inverse_transpose()
// {
// An orthogonal matrix is a square matrix whose columns and rows are orthogonal unit vectors (i.e., orthonormal vectors), i.e.
// Transpose of an orthogonal matrix is also its inverse

// if orthonormal then return its transpose

// if (!inverse())
// return;

// this->transpose();
// }

template <class _type>
FORCE_INLINE bool mesa_glu_invert_matrix(const _type a_m[16], _type a_out[16])
{
	_type inv[16];

	inv[0]  = a_m[5] * a_m[10] * a_m[15] - a_m[5] * a_m[11] * a_m[14] - a_m[9] * a_m[6] * a_m[15] + a_m[9] * a_m[7] * a_m[14] + a_m[13] * a_m[6] * a_m[11] - a_m[13] * a_m[7] * a_m[10];
	inv[4]  = -a_m[4] * a_m[10] * a_m[15] + a_m[4] * a_m[11] * a_m[14] + a_m[8] * a_m[6] * a_m[15] - a_m[8] * a_m[7] * a_m[14] - a_m[12] * a_m[6] * a_m[11] + a_m[12] * a_m[7] * a_m[10];
	inv[8]  = a_m[4] * a_m[9] * a_m[15] - a_m[4] * a_m[11] * a_m[13] - a_m[8] * a_m[5] * a_m[15] + a_m[8] * a_m[7] * a_m[13] + a_m[12] * a_m[5] * a_m[11] - a_m[12] * a_m[7] * a_m[9];
	inv[12] = -a_m[4] * a_m[9] * a_m[14] + a_m[4] * a_m[10] * a_m[13] + a_m[8] * a_m[5] * a_m[14] - a_m[8] * a_m[6] * a_m[13] - a_m[12] * a_m[5] * a_m[10] + a_m[12] * a_m[6] * a_m[9];
	inv[1]  = -a_m[1] * a_m[10] * a_m[15] + a_m[1] * a_m[11] * a_m[14] + a_m[9] * a_m[2] * a_m[15] - a_m[9] * a_m[3] * a_m[14] - a_m[13] * a_m[2] * a_m[11] + a_m[13] * a_m[3] * a_m[10];
	inv[5]  = a_m[0] * a_m[10] * a_m[15] - a_m[0] * a_m[11] * a_m[14] - a_m[8] * a_m[2] * a_m[15] + a_m[8] * a_m[3] * a_m[14] + a_m[12] * a_m[2] * a_m[11] - a_m[12] * a_m[3] * a_m[10];
	inv[9]  = -a_m[0] * a_m[9] * a_m[15] + a_m[0] * a_m[11] * a_m[13] + a_m[8] * a_m[1] * a_m[15] - a_m[8] * a_m[3] * a_m[13] - a_m[12] * a_m[1] * a_m[11] + a_m[12] * a_m[3] * a_m[9];
	inv[13] = a_m[0] * a_m[9] * a_m[14] - a_m[0] * a_m[10] * a_m[13] - a_m[8] * a_m[1] * a_m[14] + a_m[8] * a_m[2] * a_m[13] + a_m[12] * a_m[1] * a_m[10] - a_m[12] * a_m[2] * a_m[9];
	inv[2]  = a_m[1] * a_m[6] * a_m[15] - a_m[1] * a_m[7] * a_m[14] - a_m[5] * a_m[2] * a_m[15] + a_m[5] * a_m[3] * a_m[14] + a_m[13] * a_m[2] * a_m[7] - a_m[13] * a_m[3] * a_m[6];
	inv[6]  = -a_m[0] * a_m[6] * a_m[15] + a_m[0] * a_m[7] * a_m[14] + a_m[4] * a_m[2] * a_m[15] - a_m[4] * a_m[3] * a_m[14] - a_m[12] * a_m[2] * a_m[7] + a_m[12] * a_m[3] * a_m[6];
	inv[10] = a_m[0] * a_m[5] * a_m[15] - a_m[0] * a_m[7] * a_m[13] - a_m[4] * a_m[1] * a_m[15] + a_m[4] * a_m[3] * a_m[13] + a_m[12] * a_m[1] * a_m[7] - a_m[12] * a_m[3] * a_m[5];
	inv[14] = -a_m[0] * a_m[5] * a_m[14] + a_m[0] * a_m[6] * a_m[13] + a_m[4] * a_m[1] * a_m[14] - a_m[4] * a_m[2] * a_m[13] - a_m[12] * a_m[1] * a_m[6] + a_m[12] * a_m[2] * a_m[5];
	inv[3]  = -a_m[1] * a_m[6] * a_m[11] + a_m[1] * a_m[7] * a_m[10] + a_m[5] * a_m[2] * a_m[11] - a_m[5] * a_m[3] * a_m[10] - a_m[9] * a_m[2] * a_m[7] + a_m[9] * a_m[3] * a_m[6];
	inv[7]  = a_m[0] * a_m[6] * a_m[11] - a_m[0] * a_m[7] * a_m[10] - a_m[4] * a_m[2] * a_m[11] + a_m[4] * a_m[3] * a_m[10] + a_m[8] * a_m[2] * a_m[7] - a_m[8] * a_m[3] * a_m[6];
	inv[11] = -a_m[0] * a_m[5] * a_m[11] + a_m[0] * a_m[7] * a_m[9] + a_m[4] * a_m[1] * a_m[11] - a_m[4] * a_m[3] * a_m[9] - a_m[8] * a_m[1] * a_m[7] + a_m[8] * a_m[3] * a_m[5];
	inv[15] = a_m[0] * a_m[5] * a_m[10] - a_m[0] * a_m[6] * a_m[9] - a_m[4] * a_m[1] * a_m[10] + a_m[4] * a_m[2] * a_m[9] + a_m[8] * a_m[1] * a_m[6] - a_m[8] * a_m[2] * a_m[5];

	_type det;
	det = a_m[0] * inv[0] + a_m[1] * inv[4] + a_m[2] * inv[8] + a_m[3] * inv[12];

	if (equal_zero(det))
		return false;

	det = static_cast<_type>(1) / det;

	for (int32_t i = 0; i < 16; ++i)
	{
		a_out[i] = inv[i] * det;
	}

	return true;
}

template <class _type>
FORCE_INLINE bool Matrix4<_type>::inverse(Matrix4 &a_output_matrix) const
{
	return mesa_glu_invert_matrix(this->m_values, a_output_matrix.m_values);
}

template <class _type>
FORCE_INLINE bool Matrix4<_type>::invert()
{
	return mesa_glu_invert_matrix(this->m_values, this->m_values);
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::transpose() noexcept
{
	_type temp;
	temp              = this->m_values[1];
	this->m_values[1] = this->m_values[4];
	this->m_values[4] = temp;

	temp              = this->m_values[2];
	this->m_values[2] = this->m_values[8];
	this->m_values[8] = temp;

	temp               = this->m_values[3];
	this->m_values[3]  = this->m_values[12];
	this->m_values[12] = temp;

	temp              = this->m_values[6];
	this->m_values[6] = this->m_values[9];
	this->m_values[9] = temp;

	temp               = this->m_values[7];
	this->m_values[7]  = this->m_values[13];
	this->m_values[13] = temp;

	temp               = this->m_values[11];
	this->m_values[11] = this->m_values[14];
	this->m_values[14] = temp;
}

template <class _type>
FORCE_INLINE Matrix4<_type> Matrix4<_type>::transposed() const
{
	return Matrix4<_type>(this->m_values[0], this->m_values[4], this->m_values[8], this->m_values[12],
						  this->m_values[1], this->m_values[5], this->m_values[9], this->m_values[13],
						  this->m_values[2], this->m_values[6], this->m_values[10], this->m_values[14],
						  this->m_values[3], this->m_values[7], this->m_values[11], this->m_values[15]);
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::normalize()
{
	Vector3<_type> xaxis = this->x_axis();
	Vector3<_type> yaxis = this->y_axis();
	Vector3<_type> zaxis = this->z_axis();

	xaxis.normalize();
	yaxis.normalize();
	zaxis.normalize();

	this->set(xaxis.x, xaxis.y, xaxis.z, 0,
			  yaxis.x, yaxis.y, yaxis.z, 0,
			  zaxis.x, zaxis.y, zaxis.z, 0,
			  this->m_values[12], this->m_values[13], this->m_values[14], 1);
}

template <class _type>
FORCE_INLINE void Matrix4<_type>::orthogonalize()
{
	if (!this->is_orthogonal())
	{
		// uses Gram-Schmidt method to orthogonalize the matrix
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
FORCE_INLINE bool Matrix4<_type>::is_orthogonal() const
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

template <class _type>
FORCE_INLINE Vector3<_type> Matrix4<_type>::perspective_transform(const Vector3<_type> &a_vector)
{
	_type w = this->m_values[3] * a_vector.x + this->m_values[7] * a_vector.y + this->m_values[11] * a_vector.z + this->m_values[15];

	if (equal_zero(w))
		w = static_cast<_type>(0);
	else
		w = static_cast<_type>(1) / w;

	return Vector3<_type>((this->m_values[0] * a_vector.x + this->m_values[4] * a_vector.y + this->m_values[8] * a_vector.z + this->m_values[12]) * w,
						  (this->m_values[1] * a_vector.x + this->m_values[5] * a_vector.y + this->m_values[9] * a_vector.z + this->m_values[13]) * w,
						  (this->m_values[2] * a_vector.x + this->m_values[6] * a_vector.y + this->m_values[10] * a_vector.z + this->m_values[14]) * w);
}

}        // namespace ror
