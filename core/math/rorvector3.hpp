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

#include "roar.hpp"
#include <cmath>

namespace ror
{
/**
Template class for 3-component vector storage and manipulation.
@ingroup RoarMath
*/
template <class _type>
class ROAR_ENGINE_ITEM Vector3 final
{
  public:
	/**
	 * Internal type for clients to query if needed
	 */
	using value_type = _type;

	/**
	 * This makes sure you get at-least single precision floating point precision for some methods
	 * If double precision floating point is requested you will get double precision
	 */
	using precision = typename std::conditional<std::is_same<_type, double64_t>::value, double64_t, float32_t>::type;

	_type x;        //!< x component of the vector
	_type y;        //!< y component of the vector
	_type z;        //!< z component of the vector

	FORCE_INLINE Vector3()                           = default;                   //! Default constructor
	FORCE_INLINE Vector3(const Vector3 &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Vector3(Vector3 &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Vector3 &operator=(const Vector3 &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Vector3 &operator=(Vector3 &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Vector3() noexcept                            = default;        //! Destructor

	FORCE_INLINE Vector3(_type a_x, _type a_y, _type a_z);
	FORCE_INLINE explicit Vector3(_type a_xyz);        //! Sets all members to a_xyz

	template <class _other_type>
	FORCE_INLINE explicit operator Vector3<_other_type>() const;

	FORCE_INLINE explicit    operator _type *();
	FORCE_INLINE explicit    operator const _type *() const;
	FORCE_INLINE const _type operator[](int32_t a_index) const noexcept;
	FORCE_INLINE _type &operator[](int32_t a_index) noexcept;

	FORCE_INLINE Vector3 operator+(const Vector3 &a_right) const noexcept;
	FORCE_INLINE Vector3 operator-(const Vector3 &a_right) const noexcept;
	FORCE_INLINE Vector3 operator*(const Vector3 &a_right) const noexcept;
	FORCE_INLINE Vector3 operator/(const Vector3 &a_right) const noexcept;
	FORCE_INLINE Vector3 &operator+=(const Vector3 &a_right) noexcept;
	FORCE_INLINE Vector3 &operator-=(const Vector3 &a_right) noexcept;
	FORCE_INLINE Vector3 &operator*=(const Vector3 &a_right) noexcept;
	FORCE_INLINE Vector3 &operator/=(const Vector3 &a_right) noexcept;

	FORCE_INLINE bool operator==(const Vector3 &a_right) const noexcept;
	FORCE_INLINE bool operator!=(const Vector3 &a_right) const noexcept;

	FORCE_INLINE Vector3 operator-() const noexcept;
	FORCE_INLINE Vector3 operator+(const _type a_scale) const noexcept;
	FORCE_INLINE Vector3 operator-(const _type a_scale) const noexcept;
	FORCE_INLINE Vector3 operator*(const _type a_scale) const noexcept;
	FORCE_INLINE Vector3 operator/(const _type a_scale) const noexcept;
	FORCE_INLINE Vector3 &operator+=(const _type a_xyz) noexcept;
	FORCE_INLINE Vector3 &operator-=(const _type a_xyz) noexcept;
	FORCE_INLINE Vector3 &operator*=(const _type a_scale) noexcept;
	FORCE_INLINE Vector3 &operator/=(const _type a_scale) noexcept;

	FORCE_INLINE void set(const _type a_x, const _type a_y, const _type a_z) noexcept;
	FORCE_INLINE void set(const _type a_xyz) noexcept;        //! Sets all members to a_xyz
	FORCE_INLINE void set(const Vector3 &a_vector) noexcept;
	FORCE_INLINE void zero() noexcept;        //! Sets all members to zero
	FORCE_INLINE void clamp(_type a_minimum, _type a_maximum) noexcept;
	FORCE_INLINE void to(_type *a_distination) const noexcept;
	FORCE_INLINE void from(const _type *a_source) noexcept;

	FORCE_INLINE void    normalize();                                     //! Normalizes the vector
	FORCE_INLINE void    inverse();                                       //! Inverses the vector, check for divide by zero sets elements to zero
	FORCE_INLINE auto    normalized() const -> Vector3<precision>;        //! Returns the normalized version of the vector
	FORCE_INLINE Vector3 inversed() const;                                //! Returns the inversed version of the vector, no check for divide by zero
	FORCE_INLINE Vector3 abs() const;
	FORCE_INLINE Vector3 cross_product(const Vector3 &a_other) const;
	FORCE_INLINE auto    length() const -> precision;        //! Calculates magnitude of the vector but doesn't check for negative to sqrtf
	FORCE_INLINE _type   length_squared() const;
	FORCE_INLINE auto    dot_product(const Vector3 &a_other) const -> precision;
	FORCE_INLINE _type   minimum() const;
	FORCE_INLINE _type   maximum() const;
};        // namespace ror

using Vector3i  = Vector3<int32_t>;
using Vector3ui = Vector3<uint32_t>;
using Vector3f  = Vector3<float32_t>;
using Vector3d  = Vector3<double64_t>;
using Vector3b  = Vector3<char8_t>;
using Vector3ub = Vector3<uchar8_t>;

using Color3i  = Vector3<int32_t>;
using Color3ui = Vector3<uint32_t>;
using Color3f  = Vector3<float32_t>;
using Color3d  = Vector3<double64_t>;
using Color3b  = Vector3<char8_t>;
using Color3ub = Vector3<uchar8_t>;

const Vector3f zero_vector3f(0.0f, 0.0f, 0.0f);
const Vector3f xaxis3f(1.0f, 0.0f, 0.0f);
const Vector3f yaxis3f(0.0f, 1.0f, 0.0f);
const Vector3f zaxis3f(0.0f, 0.0f, 1.0f);
const Vector3f xaxis3f_negative(-1.0f, 0.0f, 0.0f);
const Vector3f yaxis3f_negative(0.0f, -1.0f, 0.0f);
const Vector3f zaxis3f_negative(0.0f, 0.0f, -1.0f);
const Vector3f white3(1.0f, 1.0f, 1.0f);
const Vector3f half_white3(0.2f, 0.2f, 0.2f);
const Vector3f black3(0.0f, 0.0f, 0.0f);

const Vector3i zero_vector3i(0, 0, 0);
const Vector3i xaxis3i(1, 0, 0);
const Vector3i yaxis3i(0, 1, 0);
const Vector3i zaxis3i(0, 0, 1);
const Vector3i negative_xaxis3i(-1, 0, 0);
const Vector3i negative_yaxis3i(0, -1, 0);
const Vector3i negative_zaxis3i(0, 0, -1);

const Vector3d zero_vector3d(0.0, 0.0, 0.0);
const Vector3d xaxis3d(1.0, 0.0, 0.0);
const Vector3d yaxis3d(0.0, 1.0, 0.0);
const Vector3d zaxis3d(0.0, 0.0, 1.0);
const Vector3d negative_xaxis3d(-1.0, 0.0, 0.0);
const Vector3d negative_yaxis3d(0.0, -1.0, 0.0);
const Vector3d negative_zaxis3d(0.0, 0.0, -1.0);


template <class _type>
using vector3_typename = typename std::enable_if<std::is_same<_type, typename ror::Vector3<typename _type::value_type>>::value>::type;

}        // namespace ror

#include "rorvector3.hh"
