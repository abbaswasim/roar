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

#include "rormatrix3x4.hpp"

/**
 * All Matrix operators and other global functions are defined here
 * Defining these operators non-member for matrices doesn't necessarily increase encapsulation
 * but it looks more cleaner as non-member
 */
namespace ror
{
// Matrix3x4 operators and functions
template <class _type>
FORCE_INLINE Matrix3x4<_type> operator+(const Matrix3x4<_type> &a_left, const Matrix3x4<_type> &a_right);

template <class _type>
FORCE_INLINE Matrix3x4<_type> operator-(const Matrix3x4<_type> &a_left, const Matrix3x4<_type> &a_right);

template <class _type>
FORCE_INLINE bool operator==(const Matrix3x4<_type> &a_left, const Matrix3x4<_type> &a_right);

template <class _type>
FORCE_INLINE bool operator!=(const Matrix3x4<_type> &a_left, const Matrix3x4<_type> &a_right);

template <class _type>
FORCE_INLINE Matrix3x4<_type> operator*(const Matrix3x4<_type> &a_lhs, const Matrix3x4<_type> &a_rhs);

template <class _type>
FORCE_INLINE Vector3<_type> operator*(const Matrix3x4<_type> &a_matrix, const Vector3<_type> &a_vector);        //!< Not perspective correct xform (no divide by w) use Matrix3x4::perspective_transform instead

template <class _type>
FORCE_INLINE Vector4<_type> operator*(const Matrix3x4<_type> &a_matrix, const Vector4<_type> &a_vector);

template <class _type>
FORCE_INLINE Vector3<_type> operator*(const Vector3<_type> &a_vector, const Matrix3x4<_type> &a_matrix);

template <class _type>
FORCE_INLINE Vector4<_type> operator*(const Vector4<_type> &a_vector, const Matrix3x4<_type> &a_matrix);

template <class _type>
FORCE_INLINE Matrix3x4<_type> operator+(const Matrix3x4<_type> &a_matrix, _type a_value);

template <class _type>
FORCE_INLINE Matrix3x4<_type> operator-(const Matrix3x4<_type> &a_matrix, _type a_value);

template <class _type>
FORCE_INLINE Matrix3x4<_type> operator*(const Matrix3x4<_type> &a_matrix, _type a_value);

template <class _type>
FORCE_INLINE Matrix3x4<_type> operator/(const Matrix3x4<_type> &a_matrix, _type a_value);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_rotation_around_x(_type a_angle_in_radians);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_rotation_around_y(_type a_angle_in_radians);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_rotation_around_z(_type a_angle_in_radians);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_transformation(const Vector3<_type> &a_translation, const Quaternion<_type> &a_rotation);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_transformation(const Vector3<_type> &a_translation, const Quaternion<_type> &a_rotation, const Vector3<_type> &a_scaling);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_translation(_type a_x, _type a_y, _type a_z);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_translation(const Vector3<_type> &a_vector);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_rotation(const Quaternion<_type> &a_quaternion);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_rotation(const Quaternion<_type> &a_quaternion, const Vector3<_type> &a_axis);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_rotation(const AxisAngle<_type> &a_axis);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_rotation(const EulerAngle<_type> &a_euler_angles);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_scaling(_type a_x, _type a_y, _type a_z);
template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_scaling(const Vector3<_type> &a_vector);

template <class _type>
FORCE_INLINE Matrix3x4<_type> inverse_rotation_translation(const Matrix3x4<_type> &a_matrix);

template <class _type>
FORCE_INLINE void decompose_into_translation_rotation_scaling(const Matrix3x4<_type> &a_matrix, Vector3<_type> &a_translation, Matrix3<_type> &a_rotation, Vector3<_type> &a_scaling);

template <class _type>
FORCE_INLINE Matrix3x4<_type> matrix3x4_interpolate(const Matrix3x4<_type> &a_matrix1, const Matrix3x4<_type> &a_matrix2, _type a_t);

}        // namespace ror

#include "rormatrix3x4_functions.inl"
