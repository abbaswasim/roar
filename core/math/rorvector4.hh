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

namespace ror
{
template <class _type>
FORCE_INLINE Vector4<_type>::Vector4(_type a_x, _type a_y, _type a_z, _type a_w) :
	x(a_x),
	y(a_y),
	z(a_z),
	w(a_w)
{}

template <class _type>
FORCE_INLINE Vector4<_type>::Vector4(_type a_xyzw) :
	x(a_xyzw),
	y(a_xyzw),
	z(a_xyzw),
	w(a_xyzw)
{}

template <class _type>
FORCE_INLINE Vector4<_type>::Vector4(const Vector3<_type> &a_source, const _type a_w) :
	x(a_source.x),
	y(a_source.y),
	z(a_source.z),
	w(a_w)
{}

template <class _type>
template <class _other_type>
FORCE_INLINE Vector4<_type>::operator Vector4<_other_type>() const
{
	return Vector4<_other_type>(static_cast<_other_type>(x), static_cast<_other_type>(y), static_cast<_other_type>(z), static_cast<_other_type>(z));
}

template <class _type>
FORCE_INLINE Vector4<_type>::operator const _type *() const
{
	return (&x);
}

template <class _type>
FORCE_INLINE Vector4<_type>::operator _type *()
{
	return (&x);
}

template <class _type>
FORCE_INLINE const _type Vector4<_type>::operator[](int32_t a_index) const noexcept
{
	assert(a_index >= 0 && a_index <= 3 && "Index out of bounds");
	return (&x)[a_index];
}

template <class _type>
FORCE_INLINE _type &Vector4<_type>::operator[](int32_t a_index) noexcept
{
	assert(a_index >= 0 && a_index <= 3 && "Index out of bounds");
	return (&x)[a_index];
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator+(const Vector4<_type> &a_right) const noexcept
{
	return Vector4(a_right.x + x, a_right.y + y, a_right.z + z, a_right.w + w);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator-(const Vector4<_type> &a_right) const noexcept
{
	return Vector4<_type>(-a_right.x + x, -a_right.y + y, -a_right.z + z, -a_right.w + w);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator*(const Vector4<_type> &a_right) const noexcept
{
	return Vector4<_type>(x * a_right.x, y * a_right.y, z * a_right.z, w * a_right.w);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator/(const Vector4<_type> &a_right) const noexcept
{
	return Vector4<_type>((a_right.x < 0 || a_right.x > 0) ? x / a_right.x : 0,
						  (a_right.y < 0 || a_right.y > 0) ? y / a_right.y : 0,
						  (a_right.z < 0 || a_right.z > 0) ? z / a_right.z : 0,
						  (a_right.w < 0 || a_right.w > 0) ? w / a_right.w : 0);
}

template <class _type>
FORCE_INLINE Vector4<_type> &Vector4<_type>::operator+=(const Vector4<_type> &a_right) noexcept
{
	x += a_right.x;
	y += a_right.y;
	z += a_right.z;
	w += a_right.w;

	return *this;
}

template <class _type>
FORCE_INLINE Vector4<_type> &Vector4<_type>::operator-=(const Vector4<_type> &a_right) noexcept
{
	x -= a_right.x;
	y -= a_right.y;
	z -= a_right.z;
	w -= a_right.w;

	return *this;
}

template <class _type>
FORCE_INLINE Vector4<_type> &Vector4<_type>::operator*=(const Vector4 &a_right) noexcept
{
	x *= a_right.x;
	y *= a_right.y;
	z *= a_right.z;
	w *= a_right.w;

	return *this;
}

template <class _type>
FORCE_INLINE Vector4<_type> &Vector4<_type>::operator/=(const Vector4 &a_right) noexcept
{
	if (a_right.x < 0 || a_right.x > 0)
	{
		x /= a_right.x;
	}
	if (a_right.y < 0 || a_right.y > 0)
	{
		y /= a_right.y;
	}
	if (a_right.z < 0 || a_right.z > 0)
	{
		z /= a_right.z;
	}
	if (a_right.w < 0 || a_right.w > 0)
	{
		w /= a_right.w;
	}

	return *this;
}

template <class _type>
FORCE_INLINE bool Vector4<_type>::operator==(const Vector4 &a_right) const noexcept
{
	return (decimal_equal(x, a_right.x) && decimal_equal(y, a_right.y) && decimal_equal(z, a_right.z) && decimal_equal(w, a_right.w));
}

template <class _type>
FORCE_INLINE bool Vector4<_type>::operator!=(const Vector4 &a_right) const noexcept
{
	return !(*this == a_right);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator-() const noexcept
{
	return Vector4<_type>(-x, -y, -z, -w);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator+(const _type a_scale) const noexcept
{
	return Vector4(a_scale + x, a_scale + y, a_scale + z, a_scale + w);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator-(const _type a_scale) const noexcept
{
	return Vector4(-a_scale + x, -a_scale + y, -a_scale + z, -a_scale + w);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator*(const _type a_scale) const noexcept
{
	return Vector4(x * a_scale, y * a_scale, z * a_scale, w * a_scale);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::operator/(const _type a_scale) const noexcept
{
	return ((a_scale < 0 || a_scale > 0) ? Vector4(x / a_scale, y / a_scale, z / a_scale, w / a_scale) : Vector4(0, 0, 0, 0));
}

template <class _type>
FORCE_INLINE Vector4<_type> &Vector4<_type>::operator+=(const _type a_xyzw) noexcept
{
	x += a_xyzw;
	y += a_xyzw;
	z += a_xyzw;
	w += a_xyzw;

	return *this;
}

template <class _type>
FORCE_INLINE Vector4<_type> &Vector4<_type>::operator-=(const _type a_xyzw) noexcept
{
	x -= a_xyzw;
	y -= a_xyzw;
	z -= a_xyzw;
	w -= a_xyzw;

	return *this;
}

template <class _type>
FORCE_INLINE Vector4<_type> &Vector4<_type>::operator*=(const _type a_scale) noexcept
{
	x *= a_scale;
	y *= a_scale;
	z *= a_scale;
	w *= a_scale;

	return *this;
}

template <class _type>
FORCE_INLINE Vector4<_type> &Vector4<_type>::operator/=(const _type a_scale) noexcept
{
	if ((a_scale < 0 || a_scale > 0))
	{
		x /= a_scale;
		y /= a_scale;
		z /= a_scale;
		w /= a_scale;
	}

	return *this;
}

template <class _type>
FORCE_INLINE void Vector4<_type>::set(const _type a_x, const _type a_y, const _type a_z, _type a_w) noexcept
{
	x = a_x;
	y = a_y;
	z = a_z;
	w = a_w;
}

template <class _type>
FORCE_INLINE void Vector4<_type>::set(const _type a_xyzw) noexcept
{
	x = y = z = w = a_xyzw;
}

template <class _type>
FORCE_INLINE void Vector4<_type>::set(const Vector4<_type> &a_vector) noexcept
{
	x = a_vector.x;
	y = a_vector.y;
	z = a_vector.z;
	w = a_vector.w;
}

template <class _type>
FORCE_INLINE void Vector4<_type>::set(const Vector3<_type> &a_vector, const _type a_w /*= 1*/) noexcept
{
	x = a_vector.x;
	y = a_vector.y;
	z = a_vector.z;
	w = a_w;
}

template <class _type>
FORCE_INLINE void Vector4<_type>::zero() noexcept
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

template <class _type>
FORCE_INLINE void Vector4<_type>::clamp(_type a_minimum, _type a_maximum) noexcept
{
	x = x > a_maximum ? a_maximum : x < a_minimum ? a_minimum : x;
	y = y > a_maximum ? a_maximum : y < a_minimum ? a_minimum : y;
	z = z > a_maximum ? a_maximum : z < a_minimum ? a_minimum : z;
	w = w > a_maximum ? a_maximum : w < a_minimum ? a_minimum : w;
}

template <class _type>
FORCE_INLINE void Vector4<_type>::to(_type *a_distination) const noexcept
{
	assert(a_distination != nullptr && "nullptr Memory Reference");

	a_distination[0] = x;
	a_distination[1] = y;
	a_distination[2] = z;
	a_distination[3] = w;
}

template <class _type>
FORCE_INLINE void Vector4<_type>::from(const _type *a_source) noexcept
{
	assert(a_source != nullptr && "nullptr Memory Reference");

	x = a_source[0];
	y = a_source[1];
	z = a_source[2];
	w = a_source[3];
}

template <class _type>
FORCE_INLINE void Vector4<_type>::normalize()
{
	precision length = this->length();

	if (!(length > 0 || length < 0))
	{
		set(0, 0, 0, 0);
	}

	x = static_cast<_type>(static_cast<precision>(x) / length);
	y = static_cast<_type>(static_cast<precision>(y) / length);
	z = static_cast<_type>(static_cast<precision>(z) / length);
	w = static_cast<_type>(static_cast<precision>(w) / length);
}

template <class _type>
FORCE_INLINE void Vector4<_type>::inverse()
{
	x = (((x < 0) || (x > 0)) ? 1 / x : 0);
	y = (((y < 0) || (y > 0)) ? 1 / y : 0);
	z = (((z < 0) || (z > 0)) ? 1 / z : 0);
	w = (((w < 0) || (w > 0)) ? 1 / w : 0);
}

template <class _type>
FORCE_INLINE auto Vector4<_type>::normalized() const -> Vector4<precision>
{
	precision length = this->length();

	if (!(length > 0 || length < 0))
	{
		return Vector4<precision>(0, 0, 0, 0);
	}
	return Vector4<precision>(static_cast<precision>(x) / length, static_cast<precision>(y) / length, static_cast<precision>(z) / length, static_cast<precision>(w) / length);
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::inversed() const
{
	return Vector4((((x < 0) || (x > 0)) ? 1 / x : 0),
				   (((y < 0) || (y > 0)) ? 1 / y : 0),
				   (((z < 0) || (z > 0)) ? 1 / z : 0),
				   (((w < 0) || (w > 0)) ? 1 / w : 0));
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::abs() const
{
	return Vector4(std::abs(x),
				   std::abs(y),
				   std::abs(z),
				   std::abs(w));
}

template <class _type>
FORCE_INLINE Vector4<_type> Vector4<_type>::cross_product(const Vector4<_type> &a_other) const
{
	return Vector4(y * a_other.z - z * a_other.y,
				   a_other.x * z - a_other.z * x,
				   x * a_other.y - y * a_other.x,
				   1.0);
}

template <class _type>
FORCE_INLINE auto Vector4<_type>::length() const -> precision
{
	return std::sqrt(static_cast<precision>(x * x + y * y + z * z + w * w));
}

template <class _type>
FORCE_INLINE _type Vector4<_type>::length_squared() const
{
	return x * x + y * y + z * z + w * w;
}

template <class _type>
FORCE_INLINE auto Vector4<_type>::dot_product(const Vector4<_type> &a_other) const -> precision
{
	return static_cast<precision>(x * a_other.x + y * a_other.y + z * a_other.z + w * a_other.w);
}

template <class _type>
FORCE_INLINE _type Vector4<_type>::minimum() const
{
	return std::min(x, std::min(y, std::min(z, w)));
}

template <class _type>
FORCE_INLINE _type Vector4<_type>::maximum() const
{
	return std::max(x, std::max(y, std::max(z, w)));
}
}        // namespace ror
