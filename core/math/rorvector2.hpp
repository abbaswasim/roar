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
#include <limits>

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM Vector3;

template <class _type>
class ROAR_ENGINE_ITEM Vector2 final
{
  public:
	using value_type = _type;
	using precision  = typename std::conditional<std::is_same<_type, double64_t>::value, double64_t, float32_t>::type;

	_type x;
	_type y;

	FORCE_INLINE          Vector2()                             = default;        //! Default constructor
	FORCE_INLINE          Vector2(const Vector2 &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          Vector2(Vector2 &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE Vector2 &operator=(const Vector2 &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Vector2 &operator=(Vector2 &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Vector2() noexcept                            = default;        //! Destructor

	FORCE_INLINE Vector2(_type a_x, _type a_y);        //! Create from two values
	FORCE_INLINE explicit Vector2(_type a_xy);         //! Sets all members to a_xy
	FORCE_INLINE explicit Vector2(Vector3<_type> a_xyz);

	template <class _other_type>
	FORCE_INLINE explicit operator Vector2<_other_type>() const;

	FORCE_INLINE explicit    operator _type *();
	FORCE_INLINE explicit    operator const _type *() const;
	FORCE_INLINE const _type operator[](int32_t a_index) const noexcept;
	FORCE_INLINE _type      &operator[](int32_t a_index) noexcept;

	FORCE_INLINE Vector2  operator+(const Vector2 &a_right) const noexcept;
	FORCE_INLINE Vector2  operator-(const Vector2 &a_right) const noexcept;
	FORCE_INLINE Vector2  operator*(const Vector2 &a_right) const noexcept;
	FORCE_INLINE Vector2  operator/(const Vector2 &a_right) const noexcept;
	FORCE_INLINE Vector2 &operator+=(const Vector2 &a_right) noexcept;
	FORCE_INLINE Vector2 &operator-=(const Vector2 &a_right) noexcept;
	FORCE_INLINE Vector2 &operator*=(const Vector2 &a_right) noexcept;
	FORCE_INLINE Vector2 &operator/=(const Vector2 &a_right) noexcept;

	FORCE_INLINE bool operator==(const Vector2 &a_right) const noexcept;
	FORCE_INLINE bool operator!=(const Vector2 &a_right) const noexcept;

	FORCE_INLINE Vector2  operator-() const noexcept;
	FORCE_INLINE Vector2  operator+(const _type a_scale) const noexcept;
	FORCE_INLINE Vector2  operator-(const _type a_scale) const noexcept;
	FORCE_INLINE Vector2  operator*(const _type a_scale) const noexcept;
	FORCE_INLINE Vector2  operator/(const _type a_scale) const noexcept;
	FORCE_INLINE Vector2 &operator+=(const _type a_xy) noexcept;
	FORCE_INLINE Vector2 &operator-=(const _type a_xy) noexcept;
	FORCE_INLINE Vector2 &operator*=(const _type a_scale) noexcept;
	FORCE_INLINE Vector2 &operator/=(const _type a_scale) noexcept;

	FORCE_INLINE void set(const _type a_x, const _type a_y) noexcept;
	FORCE_INLINE void set(const _type a_xy) noexcept;        //! Sets all members to a_xy
	FORCE_INLINE void set(const Vector2 &a_vector) noexcept;
	FORCE_INLINE void zero() noexcept;        //! Sets all members to zero
	FORCE_INLINE void clamp(_type a_minimum, _type a_maximum) noexcept;
	FORCE_INLINE void to(_type *a_distination) const noexcept;
	FORCE_INLINE void from(const _type *a_source) noexcept;

	FORCE_INLINE void    normalize();                                     //! Normalizes the vector
	FORCE_INLINE void    inverse();                                       //! Inverses the vector, no check for divide by zero
	FORCE_INLINE auto    normalized() const -> Vector2<precision>;        //! Returns the normalized version of the vector
	FORCE_INLINE Vector2 inversed() const;                                //! Returns the inversed version of the vector, no check for divide by zero
	FORCE_INLINE Vector2 abs() const;
	FORCE_INLINE auto    length() const -> precision;        //! Calculates magnitude of the vector but doesn't check for negative to sqrtf
	FORCE_INLINE _type   length_squared() const;
	FORCE_INLINE auto    dot_product(const Vector2 &a_other) const -> precision;
	FORCE_INLINE _type   minimum() const;
	FORCE_INLINE _type   maximum() const;
};

using Vector2i  = Vector2<int32_t>;
using Vector2ui = Vector2<uint32_t>;
using Vector2f  = Vector2<float32_t>;
using Vector2d  = Vector2<double64_t>;
using Vector2b  = Vector2<char8_t>;
using Vector2ub = Vector2<uchar8_t>;

using Color2i  = Vector2<int32_t>;
using Color2ui = Vector2<uint32_t>;
using Color2f  = Vector2<float32_t>;
using Color2d  = Vector2<double64_t>;
using Color2b  = Vector2<char8_t>;
using Color2ub = Vector2<uchar8_t>;

const Vector2f zero_vector2f{0.0f, 0.0f};
const Vector2f xaxis2f(1.0f, 0.0f);
const Vector2f yaxis2f(0.0f, 1.0f);
const Vector2f xaxis2f_negative(-1.0f, 0.0f);
const Vector2f yaxis2f_negative(0.0f, -1.0f);
const Vector2f min_vector2f(std::numeric_limits<float32_t>::min(), std::numeric_limits<float32_t>::min());
const Vector2f max_vector2f(std::numeric_limits<float32_t>::max(), std::numeric_limits<float32_t>::max());

const Vector2i zero_vector2i{0, 0};
const Vector2i xaxis2i(1, 0);
const Vector2i yaxis2i(0, 1);
const Vector2i negative_xaxis2i(-1, 0);
const Vector2i negative_yaxis2i(0, -1);
const Vector2i min_vector2i(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min());
const Vector2i max_vector2i(std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max());

const Vector2d zero_vector2d{0.0, 0.0};
const Vector2d xaxis2d(1.0, 0.0);
const Vector2d yaxis2d(0.0, 1.0);
const Vector2d negative_xaxis2d(-1.0, 0.0);
const Vector2d negative_yaxis2d(0.0, -1.0);
const Vector2d min_vector2d(std::numeric_limits<double64_t>::min(), std::numeric_limits<double64_t>::min());
const Vector2d max_vector2d(std::numeric_limits<double64_t>::max(), std::numeric_limits<double64_t>::max());

template <class _type>
using vector2_typename = typename std::enable_if<std::is_same<_type, typename ror::Vector2<typename _type::value_type>>::value>::type;

}        // namespace ror

#include "rorvector2.hh"
