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

#include "rorvector2.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE Vector2<_type>::Vector2(_type a_x, _type a_y) :
	x(a_x),
	y(a_y)
{}

template <class _type>
FORCE_INLINE Vector2<_type>::Vector2(_type a_xy) :
	x(a_xy),
	y(a_xy)
{}

template <class _type>
template <class _other_type>
FORCE_INLINE Vector2<_type>::operator Vector2<_other_type>() const
{
	return Vector2<_other_type>(static_cast<_other_type>(x), static_cast<_other_type>(y));
}

template <class _type>
FORCE_INLINE Vector2<_type>::operator const _type *(void) const
{
	return (&x);
}

template <class _type>
FORCE_INLINE Vector2<_type>::operator _type *(void)
{
	return (&x);
}

template <class _type>
FORCE_INLINE const _type Vector2<_type>::operator[](int32_t a_index) const noexcept
{
	assert(a_index >= 0 && a_index <= 1 && "Index out of bounds");
	return (&x)[a_index];
}

template <class _type>
FORCE_INLINE _type &Vector2<_type>::operator[](int32_t a_index) noexcept
{
	assert(a_index >= 0 && a_index <= 1 && "Index out of bounds");
	return (&x)[a_index];
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator+(const Vector2<_type> &a_right) const noexcept
{
	return Vector2<_type>(this->x + a_right.x, this->y + a_right.y);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator-(const Vector2<_type> &a_right) const noexcept
{
	return Vector2<_type>(this->x - a_right.x, this->y - a_right.y);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator*(const Vector2<_type> &a_right) const noexcept
{
	return Vector2<_type>(x * a_right.x, y * a_right.y);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator/(const Vector2<_type> &a_right) const noexcept
{
	return Vector2<_type>((a_right.x < 0 || a_right.x > 0) ? x / a_right.x : 0,
						  (a_right.y < 0 || a_right.y > 0) ? y / a_right.y : 0);
}

template <class _type>
FORCE_INLINE Vector2<_type> &Vector2<_type>::operator+=(const Vector2<_type> &a_right) noexcept
{
	x += a_right.x;
	y += a_right.y;

	return *this;
}

template <class _type>
FORCE_INLINE Vector2<_type> &Vector2<_type>::operator-=(const Vector2<_type> &a_right) noexcept
{
	x -= a_right.x;
	y -= a_right.y;

	return *this;
}

template <class _type>
FORCE_INLINE Vector2<_type> &Vector2<_type>::operator*=(const Vector2 &a_right) noexcept
{
	x *= a_right.x;
	y *= a_right.y;

	return *this;
}

template <class _type>
FORCE_INLINE Vector2<_type> &Vector2<_type>::operator/=(const Vector2 &a_right) noexcept
{
	if (a_right.x < 0 || a_right.x > 0)
	{
		x /= a_right.x;
	}
	if (a_right.y < 0 || a_right.y > 0)
	{
		y /= a_right.y;
	}

	return *this;
}

template <class _type>
FORCE_INLINE bool Vector2<_type>::operator==(const Vector2 &a_right) const noexcept
{
	return (decimal_equal(x, a_right.x) && decimal_equal(y, a_right.y));
}

template <class _type>
FORCE_INLINE bool Vector2<_type>::operator!=(const Vector2 &a_right) const noexcept
{
	return !(*this == a_right);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator-() const noexcept
{
	return Vector2<_type>(-x, -y);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator+(const _type a_scale) const noexcept
{
	return Vector2(a_scale + x, a_scale + y);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator-(const _type a_scale) const noexcept
{
	return Vector2(-a_scale + x, -a_scale + y);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator*(const _type a_scale) const noexcept
{
	return Vector2(x * a_scale, y * a_scale);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::operator/(const _type a_scale) const noexcept
{
	return ((a_scale < 0 || a_scale > 0) ? Vector2<_type>(x / a_scale, y / a_scale) : Vector2<_type>(0, 0));
}

template <class _type>
FORCE_INLINE Vector2<_type> &Vector2<_type>::operator+=(const _type a_xy) noexcept
{
	x += a_xy;
	y += a_xy;

	return *this;
}

template <class _type>
FORCE_INLINE Vector2<_type> &Vector2<_type>::operator-=(const _type a_xy) noexcept
{
	x -= a_xy;
	y -= a_xy;

	return *this;
}

template <class _type>
FORCE_INLINE Vector2<_type> &Vector2<_type>::operator*=(const _type a_scale) noexcept
{
	x *= a_scale;
	y *= a_scale;

	return *this;
}

template <class _type>
FORCE_INLINE Vector2<_type> &Vector2<_type>::operator/=(const _type a_scale) noexcept
{
	if ((a_scale < 0 || a_scale > 0))
	{
		x /= a_scale;
		y /= a_scale;
	}

	return *this;
}

template <class _type>
FORCE_INLINE void Vector2<_type>::set(const _type a_x, const _type a_y) noexcept
{
	x = a_x;
	y = a_y;
}

template <class _type>
FORCE_INLINE void Vector2<_type>::set(const _type a_xy) noexcept
{
	x = y = a_xy;
}

template <class _type>
FORCE_INLINE void Vector2<_type>::set(const Vector2<_type> &a_vector) noexcept
{
	x = a_vector.x;
	y = a_vector.y;
}

template <class _type>
FORCE_INLINE void Vector2<_type>::zero() noexcept
{
	x = 0;
	y = 0;
}

template <class _type>
FORCE_INLINE void Vector2<_type>::clamp(_type a_minimum, _type a_maximum) noexcept
{
	x = x > a_maximum ? a_maximum : x < a_minimum ? a_minimum : x;
	y = y > a_maximum ? a_maximum : y < a_minimum ? a_minimum : y;
}

template <class _type>
FORCE_INLINE void Vector2<_type>::to(_type *a_distination) const noexcept
{
	assert(a_distination != nullptr && "nullptr Memory Reference");

	a_distination[0] = x;
	a_distination[1] = y;
}

template <class _type>
FORCE_INLINE void Vector2<_type>::from(const _type *a_source) noexcept
{
	assert(a_source != nullptr && "nullptr Memory Reference");

	x = a_source[0];
	y = a_source[1];
}

template <class _type>
FORCE_INLINE void Vector2<_type>::normalize()
{
	precision length = this->length();

	if (!(length > 0 || length < 0))
	{
		set(0, 0);
	}

	x = static_cast<_type>(static_cast<precision>(x) / length);
	y = static_cast<_type>(static_cast<precision>(y) / length);
}

template <class _type>
FORCE_INLINE void Vector2<_type>::inverse()
{
	x = (((x < 0) || (x > 0)) ? 1 / x : 0);
	y = (((y < 0) || (y > 0)) ? 1 / y : 0);
}

template <class _type>
FORCE_INLINE auto Vector2<_type>::normalized() const -> Vector2<precision>
{
	precision length = this->length();

	if (!(length > 0 || length < 0))
	{
		return Vector2<precision>(0, 0);
	}
	return Vector2<precision>(static_cast<precision>(x) / length, static_cast<precision>(y) / length);
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::inversed() const
{
	return Vector2((((x < 0) || (x > 0)) ? 1 / x : 0),
				   (((y < 0) || (y > 0)) ? 1 / y : 0));
}

template <class _type>
FORCE_INLINE Vector2<_type> Vector2<_type>::abs() const
{
	return Vector2(std::abs(x),
				   std::abs(y));
}

template <class _type>
FORCE_INLINE auto Vector2<_type>::length() const -> precision
{
	return std::sqrt(static_cast<precision>(x * x + y * y));
}

template <class _type>
FORCE_INLINE _type Vector2<_type>::length_squared() const
{
	return x * x + y * y;
}

template <class _type>
FORCE_INLINE auto Vector2<_type>::dot_product(const Vector2<_type> &a_other) const -> precision
{
	return static_cast<precision>(x * a_other.x + y * a_other.y);
}

template <class _type>
FORCE_INLINE _type Vector2<_type>::minimum() const
{
	return std::min(x, y);
}

template <class _type>
FORCE_INLINE _type Vector2<_type>::maximum() const
{
	return std::max(x, y);
}

}        // namespace ror
