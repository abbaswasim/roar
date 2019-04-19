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

#include "roraxis_angle.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE AxisAngle<_type>::AxisAngle(const Vector3<_type> &a_axis, _type a_radians)
{
	this->set(a_axis, a_radians);
}

template <class _type>
FORCE_INLINE void AxisAngle<_type>::set(const Vector3<_type> &a_axis, _type a_radians) noexcept
{
	this->m_axis          = a_axis;
	this->m_angle_radians = a_radians;

	this->m_axis.normalize();
}

template <class _type>
FORCE_INLINE AxisAngle<_type>::AxisAngle(_type a_axis_x, _type a_axis_y, _type a_axis_z, _type a_radians)
{
	this->set(a_axis_x, a_axis_y, a_axis_z, a_radians);
}

template <class _type>
FORCE_INLINE void AxisAngle<_type>::set(_type a_axis_x, _type a_axis_y, _type a_axis_z, _type a_radians) noexcept
{
	this->set(ror::Vector3<_type>(a_axis_x, a_axis_y, a_axis_z), a_radians);
}

template <class _type>
FORCE_INLINE bool AxisAngle<_type>::operator==(const AxisAngle &a_other) const noexcept
{
	return (this->m_axis == a_other.m_axis && decimal_equal(this->m_angle_radians, a_other.m_angle_radians));
}

template <class _type>
FORCE_INLINE bool AxisAngle<_type>::operator!=(const AxisAngle &a_other) const noexcept
{
	return !(*this == a_other);
}

template <class _type>
FORCE_INLINE Vector3<_type> AxisAngle<_type>::axis() const noexcept
{
	return this->m_axis;
}

template <class _type>
FORCE_INLINE _type AxisAngle<_type>::angle() const noexcept
{
	return this->m_angle_radians;
}

template <class _type>
FORCE_INLINE void AxisAngle<_type>::set_axis(const Vector3<_type> &a_axis) noexcept
{
	this->m_axis = a_axis;
	this->m_axis.normalize();
}

template <class _type>
FORCE_INLINE void AxisAngle<_type>::set_angle(_type a_radians) noexcept
{
	this->m_angle_radians = a_radians;
}

}        // namespace ror
