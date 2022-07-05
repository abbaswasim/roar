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

#include "rorvector3.hpp"

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM Vector4 final
{
  public:
	using value_type = _type;
	using precision  = typename std::conditional<std::is_same<_type, double64_t>::value, double64_t, float32_t>::type;

	_type x;
	_type y;
	_type z;
	_type w;        // Points (w=1) and vectors (w=0)

	FORCE_INLINE          Vector4()                             = default;        //! Default constructor
	FORCE_INLINE          Vector4(const Vector4 &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          Vector4(Vector4 &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE Vector4 &operator=(const Vector4 &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Vector4 &operator=(Vector4 &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Vector4() noexcept                            = default;        //! Destructor

	FORCE_INLINE Vector4(_type a_x, _type a_y, _type a_z, _type a_w);
	FORCE_INLINE explicit Vector4(_type a_xyzw);        //! Sets all members to a_xyzw
	FORCE_INLINE explicit Vector4(const Vector3<_type> &a_source, const _type a_w = 1);

	template <class _other_type>
	FORCE_INLINE explicit operator Vector4<_other_type>() const;

	FORCE_INLINE explicit    operator _type *();
	FORCE_INLINE explicit    operator const _type *() const;
	FORCE_INLINE const _type operator[](int32_t a_index) const noexcept;
	FORCE_INLINE _type      &operator[](int32_t a_index) noexcept;

	FORCE_INLINE Vector4  operator+(const Vector4 &a_right) const noexcept;
	FORCE_INLINE Vector4  operator-(const Vector4 &a_right) const noexcept;
	FORCE_INLINE Vector4  operator*(const Vector4 &a_right) const noexcept;
	FORCE_INLINE Vector4  operator/(const Vector4 &a_right) const noexcept;
	FORCE_INLINE Vector4 &operator+=(const Vector4 &a_right) noexcept;
	FORCE_INLINE Vector4 &operator-=(const Vector4 &a_right) noexcept;
	FORCE_INLINE Vector4 &operator*=(const Vector4 &a_right) noexcept;
	FORCE_INLINE Vector4 &operator/=(const Vector4 &a_right) noexcept;

	FORCE_INLINE bool operator==(const Vector4 &a_right) const noexcept;
	FORCE_INLINE bool operator!=(const Vector4 &a_right) const noexcept;

	FORCE_INLINE Vector4  operator-() const noexcept;
	FORCE_INLINE Vector4  operator+(const _type a_scale) const noexcept;
	FORCE_INLINE Vector4  operator-(const _type a_scale) const noexcept;
	FORCE_INLINE Vector4  operator*(const _type a_scale) const noexcept;
	FORCE_INLINE Vector4  operator/(const _type a_scale) const noexcept;
	FORCE_INLINE Vector4 &operator+=(const _type a_xyzw) noexcept;
	FORCE_INLINE Vector4 &operator-=(const _type a_xyzw) noexcept;
	FORCE_INLINE Vector4 &operator*=(const _type a_scale) noexcept;
	FORCE_INLINE Vector4 &operator/=(const _type a_scale) noexcept;

	FORCE_INLINE void set(const _type a_x, const _type a_y, const _type a_z, const _type a_w) noexcept;
	FORCE_INLINE void set(const _type a_xyzw) noexcept;        //! Sets all members to a_xyzw
	FORCE_INLINE void set(const Vector4 &a_vector) noexcept;
	FORCE_INLINE void set(const Vector3<_type> &a_vector, const _type a_w = 1) noexcept;
	FORCE_INLINE void zero() noexcept;        //! Sets all members to zero
	FORCE_INLINE void clamp(_type a_minimum, _type a_maximum) noexcept;
	FORCE_INLINE void to(_type *a_distination) const noexcept;
	FORCE_INLINE void from(const _type *a_source) noexcept;

	FORCE_INLINE void    normalize();                                     //! Normalizes the vector
	FORCE_INLINE void    inverse();                                       //! Inverses the vector, no check for divide by zero
	FORCE_INLINE auto    normalized() const -> Vector4<precision>;        //! Returns the normalized version of the vector;
	FORCE_INLINE Vector4 inversed() const;                                //! Returns the inversed version, doesn't check for divide by zero
	FORCE_INLINE Vector4 abs() const;
	FORCE_INLINE Vector4 cross_product(const Vector4 &a_other) const;
	FORCE_INLINE auto    length() const -> precision;        //! Calculates magnitude, doesn't check for negative to sqrtf
	FORCE_INLINE _type   length_squared() const;
	FORCE_INLINE auto    dot_product(const Vector4 &a_other) const -> precision;
	FORCE_INLINE _type   minimum() const;
	FORCE_INLINE _type   maximum() const;
};

using Vector4i  = Vector4<int32_t>;
using Vector4ui = Vector4<uint32_t>;
using Vector4f  = Vector4<float32_t>;
using Vector4d  = Vector4<double64_t>;
using Vector4b  = Vector4<char8_t>;
using Vector4ub = Vector4<uchar8_t>;

using Color4i  = Vector4<int32_t>;
using Color4ui = Vector4<uint32_t>;
using Color4f  = Vector4<float32_t>;
using Color4d  = Vector4<double64_t>;
using Color4b  = Vector4<char8_t>;
using Color4ub = Vector4<uchar8_t>;

const Vector4f zero_vector4f(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4f xaxis4f(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4f yaxis4f(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4f zaxis4f(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4f waxis4f(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4f xaxis4f_negative(-1.0f, 0.0f, 0.0f, 0.0f);
const Vector4f yaxis4f_negative(0.0f, -1.0f, 0.0f, 0.0f);
const Vector4f zaxis4f_negative(0.0f, 0.0f, -1.0f, 0.0f);
const Vector4f waxis4f_negative(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4f white(1.0f, 1.0f, 1.0f, 1.0f);
const Vector4f half_white(0.2f, 0.2f, 0.2f, 1.0f);
const Vector4f black(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4f min_vector4f(std::numeric_limits<float32_t>::min(), std::numeric_limits<float32_t>::min(), std::numeric_limits<float32_t>::min(), std::numeric_limits<float32_t>::min());
const Vector4f max_vector4f(std::numeric_limits<float32_t>::max(), std::numeric_limits<float32_t>::max(), std::numeric_limits<float32_t>::max(), std::numeric_limits<float32_t>::max());

const Vector4i zero_vector4i(0, 0, 0, 0);
const Vector4i xaxis4i(1, 0, 0, 0);
const Vector4i yaxis4i(0, 1, 0, 0);
const Vector4i zaxis4i(0, 0, 1, 0);
const Vector4i waxis4i(0, 0, 0, 0);
const Vector4i negative_xaxis4i(-1, 0, 0, 0);
const Vector4i negative_yaxis4i(0, -1, 0, 0);
const Vector4i negative_zaxis4i(0, 0, -1, 0);
const Vector4i negative_waxis4i(0, 0, 0, 0);
const Vector4i min_vector4i(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min());
const Vector4i max_vector4i(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max());

const Vector4d zero_vector4d(0.0, 0.0, 0.0, 0.0);
const Vector4d xaxis4d(1.0, 0.0, 0.0, 0.0);
const Vector4d yaxis4d(0.0, 1.0, 0.0, 0.0);
const Vector4d zaxis4d(0.0, 0.0, 1.0, 0.0);
const Vector4d waxis4d(0.0, 0.0, 0.0, 0.0);
const Vector4d negative_xaxis4d(-1.0, 0.0, 0.0, 0.0);
const Vector4d negative_yaxis4d(0.0, -1.0, 0.0, 0.0);
const Vector4d negative_zaxis4d(0.0, 0.0, -1.0, 0.0);
const Vector4d negative_waxis4d(0.0, 0.0, 0.0, 0.0);
const Vector4d min_vector4d(std::numeric_limits<double64_t>::min(), std::numeric_limits<double64_t>::min(), std::numeric_limits<double64_t>::min(), std::numeric_limits<double64_t>::min());
const Vector4d max_vector4d(std::numeric_limits<double64_t>::max(), std::numeric_limits<double64_t>::max(), std::numeric_limits<double64_t>::max(), std::numeric_limits<double64_t>::max());

const Vector4f zero_point4f(0.0f, 0.0f, 0.0f, 1.0f);
const Vector4f min_point4f(std::numeric_limits<float32_t>::min(), std::numeric_limits<float32_t>::min(), std::numeric_limits<float32_t>::min(), 1.0f);
const Vector4f max_point4f(std::numeric_limits<float32_t>::max(), std::numeric_limits<float32_t>::max(), std::numeric_limits<float32_t>::max(), 1.0f);

const Vector4i zero_point4i(0, 0, 0, 1);
const Vector4i min_point4i(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min(), 1.0f);
const Vector4i max_point4i(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max(), 1.0f);

const Vector4d zero_point4d(0.0, 0.0, 0.0, 1.0);
const Vector4d min_point4d(std::numeric_limits<double64_t>::min(), std::numeric_limits<double64_t>::min(), std::numeric_limits<double64_t>::min(), 1.0f);
const Vector4d max_point4d(std::numeric_limits<double64_t>::max(), std::numeric_limits<double64_t>::max(), std::numeric_limits<double64_t>::max(), 1.0f);

template <class _type>
using vector4_typename = typename std::enable_if<std::is_same<_type, typename ror::Vector4<typename _type::value_type>>::value>::type;

}        // namespace ror

#include "rorvector4.hh"
