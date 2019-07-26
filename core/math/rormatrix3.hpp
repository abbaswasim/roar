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

#include "math/rorvector.hpp"

namespace ror
{
/**
 * Forward declaration for Matrix2 class used within Matrix3 class
 */
template <class _type>
class Matrix2;

/**
 * Forward declaration for Matrix4 class used within Matrix3 class
 */
template <class _type>
class Matrix4;

/**
 * Forward declaration for Matrix3x4 class used within Matrix3 class
 */
template <class _type>
class Matrix3x4;

/**
 * Forward declaration for AxisAngle class used within Matrix3 class
 */
template <class _type>
class AxisAngle;

/**
 * Forward declaration for EulerAngle class used within Matrix3 class
 */
template <class _type>
class EulerAngle;

/**
 * Forward declaration for Quaternion class used within Matrix3 class
 */
template <class _type>
class Quaternion;

/**
* Class for 3x3 matrix storage and manipulation.
* A Template version of the Matrix3 is provided but OpenGL ES only supports (_type == float32_t) matrices.
* This is a column-major matrix implementation.
* To transform a vector you will need to post-multiply it
* Matrix3 mat;
* Vector3 vec;
* auto result = mat * vec;
* Reference: https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/row-major-vs-column-major-vector
* +X points right and +Y points up, Z+ point "out of the screen" for the right hand system. Camera looking down the -Z
@ingroup RoarMath
*/
template <class _type>
class ROAR_ENGINE_ITEM Matrix3 final
{
  public:
	_type m_values[9];        //!< Matrix data in a C array Column-Major

	FORCE_INLINE Matrix3(_type a_scale = 1);                                      //! Default constructor
	FORCE_INLINE Matrix3(const Matrix3 &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Matrix3(Matrix3 &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Matrix3 &operator=(const Matrix3 &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Matrix3 &operator=(Matrix3 &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Matrix3() noexcept                            = default;        //! Destructor

	// TODO: Add constructors from axis angle and euler angles
	FORCE_INLINE explicit Matrix3(const Matrix4<_type> &a_other);
	FORCE_INLINE explicit Matrix3(const Matrix3x4<_type> &a_other);
	FORCE_INLINE explicit Matrix3(const _type *a_elements);
	FORCE_INLINE Matrix3(const Quaternion<_type> &a_quaternion);
	FORCE_INLINE Matrix3(_type a_0, _type a_1, _type a_2,         //!< Don't use in normal usage never provide a hand written matrix
						 _type a_3, _type a_4, _type a_5,         //!< If ever used make sure rows and colums are correct
						 _type a_6, _type a_7, _type a_8);        //!< a_0, a_1, a_2 is the first column not the first row

	FORCE_INLINE _type &get(uint32_t a_row, uint32_t a_column) noexcept;
	FORCE_INLINE void   set(const Matrix3 &a_matrix) noexcept;
	FORCE_INLINE void   set(const _type *a_elements) noexcept;
	FORCE_INLINE void   set(const Quaternion<_type> &a_quaternion);
	FORCE_INLINE void   set(_type a_0, _type a_1, _type a_2,                  //!< Don't use in normal usage never provide a hand written matrix
							_type a_3, _type a_4, _type a_5,                  //!< If ever used make sure rows and colums are correct
							_type a_6, _type a_7, _type a_8) noexcept;        //!< a_0, a_1, a_2 is the first column not the first row

	FORCE_INLINE void set_axis(uint32_t a_axis_index, const Vector3<_type> &a_axis) noexcept;
	FORCE_INLINE void set_x_axis(const Vector3<_type> &a_axis) noexcept;
	FORCE_INLINE void set_y_axis(const Vector3<_type> &a_axis) noexcept;
	FORCE_INLINE void set_z_axis(const Vector3<_type> &a_axis) noexcept;
	FORCE_INLINE void set_column(uint32_t a_index, const Vector3<_type> &a_column) noexcept;
	FORCE_INLINE Vector3<_type> x_axis() const;
	FORCE_INLINE Vector3<_type> y_axis() const;
	FORCE_INLINE Vector3<_type> z_axis() const;
	FORCE_INLINE Vector3<_type> column(uint32_t a_index) const;

	FORCE_INLINE explicit operator const _type *() const;        //!< Only bind to const arrays or const pointers type

	FORCE_INLINE Matrix3 &operator+=(const Matrix3 &a_matrix);
	FORCE_INLINE Matrix3 &operator-=(const Matrix3 &a_matrix);
	FORCE_INLINE Matrix3 &operator*=(const Matrix3 &a_matrix);
	FORCE_INLINE Matrix3 &operator*=(_type a_value);
	FORCE_INLINE Matrix3 operator-() const;

	FORCE_INLINE void identity() noexcept;
	FORCE_INLINE _type determinant() const noexcept;
	FORCE_INLINE bool  invert();                                       //!< Inverts the matrix and returns true. If inverse is not possile returns false
	FORCE_INLINE bool  inverse(Matrix3 &a_output_matrix) const;        //!< Sets a_output_matrix to inverse if its possible and returns true otherwise returns false
	FORCE_INLINE void  transpose() noexcept;                           //!< Transposes the matrix
	FORCE_INLINE Matrix3 transposed() const;                           //!< Returns transpose of the matrix unchanged

	FORCE_INLINE void normalize();            //!< Normalizes the matrix x, y, z axis
	FORCE_INLINE void orthogonalize();        //!< Orthogonalizes the basis axis using Gram–Schmidt process,
											  //!< it can fail if the axis are co-linear. It doesn't normalize the basis
	FORCE_INLINE bool is_orthogonal() const;
};

using Matrix3i = Matrix3<int32_t>;
using Matrix3f = Matrix3<float32_t>;
using Matrix3d = Matrix3<double64_t>;

const Matrix3f identity_matrix3i(
	1, 0, 0,
	0, 1, 0,
	0, 0, 1);

const Matrix3f identity_matrix3f(
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f);

const Matrix3f identity_matrix3d(
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 1.0);

const Matrix3f zero_matrix3f(
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f);

}        // namespace ror

#include "rormatrix3.hh"
