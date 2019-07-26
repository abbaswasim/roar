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
 * Forward declaration for Matrix3 class used within Matrix2 class
 */
template <class _type>
class Matrix3;

/**
 * Forward declaration for Matrix4 class used within Matrix2 class
 */
template <class _type>
class Matrix4;

/**
 * Forward declaration for AxisAngle class used within Matrix2 class
 */
template <class _type>
class AxisAngle;

/**
 * Forward declaration for Quaternion class used within Matrix2 class
 */
template <class _type>
class Quaternion;

/**
* Class for 2x2 matrix storage and manipulation.
* A Template version of the Matrix2 is provided but we will most probably only use only Matrix2f
* This is a column-major matrix implementation.
* To transform a vector you will need to post-multiply it
* Matrix2 mat;
* Vector2 vec;
* auto result = mat * vec;
* Reference: https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/row-major-vs-column-major-vector
* +X points right and +Y points up
@ingroup RoarMath
*/
template <class _type>
class ROAR_ENGINE_ITEM Matrix2 final
{
  public:
	_type m_values[4];        //!< Matrix data in a C array Column-Major

	FORCE_INLINE Matrix2(_type a_scale = 1);                                      //! Default constructor
	FORCE_INLINE Matrix2(const Matrix2 &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Matrix2(Matrix2 &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Matrix2 &operator=(const Matrix2 &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Matrix2 &operator=(Matrix2 &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Matrix2() noexcept                            = default;        //! Destructor

	FORCE_INLINE explicit Matrix2(const Matrix3<_type> &a_other);        //!< Discard other values might be useful
	FORCE_INLINE explicit Matrix2(const Matrix4<_type> &a_other);        //!< Discard other values might be useful
	FORCE_INLINE explicit Matrix2(const _type *a_elements);
	FORCE_INLINE Matrix2(_type a_0, _type a_1,         //!< Don't use in normal usage never provide a hand written matrix
						 _type a_2, _type a_3);        //!< If ever used make sure rows and colums are correct
													   //!< a_0, a_1, is the first column not the first row

	FORCE_INLINE _type &get(uint32_t a_row, uint32_t a_column) noexcept;
	FORCE_INLINE void   set(const Matrix2 &a_matrix) noexcept;
	FORCE_INLINE void   set(const _type *a_elements) noexcept;
	FORCE_INLINE void   set(_type a_0, _type a_1,                  //!< Don't use in normal usage never provide a hand written matrix
							_type a_2, _type a_3) noexcept;        //!< If ever used make sure rows and colums are correct
																   //!< a_0, a_1 is the first column not the first row

	FORCE_INLINE void set_axis(uint32_t a_axis_index, const Vector2<_type> &a_axis) noexcept;
	FORCE_INLINE void set_x_axis(const Vector2<_type> &a_axis) noexcept;
	FORCE_INLINE void set_y_axis(const Vector2<_type> &a_axis) noexcept;
	FORCE_INLINE void set_column(uint32_t a_index, const Vector2<_type> &a_column) noexcept;
	FORCE_INLINE Vector2<_type> x_axis() const;
	FORCE_INLINE Vector2<_type> y_axis() const;
	FORCE_INLINE Vector2<_type> column(uint32_t a_index) const;

	FORCE_INLINE explicit operator const _type *() const;        //!< Only bind to const arrays or const pointers type

	FORCE_INLINE Matrix2 &operator+=(const Matrix2 &a_matrix);
	FORCE_INLINE Matrix2 &operator-=(const Matrix2 &a_matrix);
	FORCE_INLINE Matrix2 &operator*=(const Matrix2 &a_matrix);
	FORCE_INLINE Matrix2 &operator*=(_type a_value);
	FORCE_INLINE Matrix2 operator-() const;

	FORCE_INLINE void identity() noexcept;
	FORCE_INLINE _type determinant() const noexcept;
	FORCE_INLINE bool  invert();                                       //!< Inverts the matrix and returns true. If inverse is not possile returns false
	FORCE_INLINE bool  inverse(Matrix2 &a_output_matrix) const;        //!< Sets a_output_matrix to inverse if its possible and returns true otherwise returns false
	FORCE_INLINE void  transpose() noexcept;                           //!< Transposes the matrix
	FORCE_INLINE Matrix2 transposed() const;                           //!< Returns transpose of the matrix unchanged

	FORCE_INLINE void normalize();            //!< Normalizes the matrix x, y axis
	FORCE_INLINE void orthogonalize();        //!< Orthogonalizes the basis axis using Gram–Schmidt process,
											  //!< it can fail if the axis are co-linear. It doesn't normalize the basis
	FORCE_INLINE bool is_orthogonal() const;
};

using Matrix2i = Matrix2<int32_t>;
using Matrix2f = Matrix2<float32_t>;
using Matrix2d = Matrix2<double64_t>;

const Matrix2f identity_matrix2i(
	1, 0,
	0, 1);

const Matrix2f identity_matrix2f(
	1.0f, 0.0f,
	0.0f, 1.0f);

const Matrix2f identity_matrix2d(
	1.0, 0.0,
	0.0, 1.0);

const Matrix2f zero_matrix2f(
	0.0f, 0.0f,
	0.0f, 0.0f);

}        // namespace ror

#include "rormatrix2.hh"
