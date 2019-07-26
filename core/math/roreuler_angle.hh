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

#include "roreuler_angle.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE EulerAngle<_type>::EulerAngle(const Vector3<_type> &a_euler) :
	m_heading(a_euler.y),
	m_attitude(a_euler.z),
	m_bank(a_euler.x)
{}

template <class _type>
FORCE_INLINE EulerAngle<_type>::EulerAngle(_type a_heading, _type a_attitude, _type a_bank) :
	m_heading(a_heading),
	m_attitude(a_attitude),
	m_bank(a_bank)
{}

template <class _type>
FORCE_INLINE void EulerAngle<_type>::set(const Vector3<_type> &a_euler) noexcept
{
	this->m_heading  = a_euler.y;
	this->m_attitude = a_euler.z;
	this->m_bank     = a_euler.x;
}

template <class _type>
FORCE_INLINE void EulerAngle<_type>::set(_type a_heading, _type a_attitude, _type a_bank) noexcept
{
	this->m_heading  = a_heading;
	this->m_attitude = a_attitude;
	this->m_bank     = a_bank;
}

template <class _type>
FORCE_INLINE bool EulerAngle<_type>::operator==(const EulerAngle &a_other) const noexcept
{
	return (decimal_equal(this->m_heading, a_other.m_heading) &&
			decimal_equal(this->m_attitude, a_other.m_attitude) &&
			decimal_equal(this->m_bank, a_other.m_bank));
}

template <class _type>
FORCE_INLINE bool EulerAngle<_type>::operator!=(const EulerAngle &a_other) const noexcept
{
	return !(*this == a_other);
}

template <class _type>
FORCE_INLINE EulerAngle<_type> &EulerAngle<_type>::operator+=(const EulerAngle &a_other) noexcept
{
	this->m_heading += a_other.m_heading;
	this->m_attitude += a_other.m_attitude;
	this->m_bank += a_other.m_bank;

	return *this;
}

template <class _type>
FORCE_INLINE EulerAngle<_type> &EulerAngle<_type>::operator-=(const EulerAngle &a_other) noexcept
{
	this->m_heading -= a_other.m_heading;
	this->m_attitude -= a_other.m_attitude;
	this->m_bank -= a_other.m_bank;

	return *this;
}

template <class _type>
FORCE_INLINE EulerAngle<_type> EulerAngle<_type>::operator+(const EulerAngle &a_other) const noexcept
{
	return EulerAngle<_type>(this->m_heading + a_other.m_heading,
							 this->m_attitude + a_other.m_attitude,
							 this->m_bank + a_other.m_bank);
}

template <class _type>
FORCE_INLINE EulerAngle<_type> EulerAngle<_type>::operator-(const EulerAngle &a_other) const noexcept
{
	return EulerAngle<_type>(this->m_heading - a_other.m_heading,
							 this->m_attitude - a_other.m_attitude,
							 this->m_bank - a_other.m_bank);
}

template <class _type>
FORCE_INLINE _type EulerAngle<_type>::heading() const noexcept
{
	return this->m_heading;
}

template <class _type>
FORCE_INLINE _type EulerAngle<_type>::attitude() const noexcept
{
	return this->m_attitude;
}

template <class _type>
FORCE_INLINE _type EulerAngle<_type>::bank() const noexcept
{
	return this->m_bank;
}

template <class _type>
FORCE_INLINE void EulerAngle<_type>::set_heading(_type a_heading) noexcept
{
	this->m_heading = a_heading;
}

template <class _type>
FORCE_INLINE void EulerAngle<_type>::set_attitude(_type a_attitude) noexcept
{
	this->m_attitude = a_attitude;
}

template <class _type>
FORCE_INLINE void EulerAngle<_type>::set_bank(_type a_bank) noexcept
{
	this->m_bank = a_bank;
}

}        // namespace ror
