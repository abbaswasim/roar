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

#include "math/rormatrix4.hpp"
#include "math/rorvector3.hpp"
#include "rortransform.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE Transform<_type>::Transform(const Quaternion<_type> &a_rotation, const Vector3<_type> &a_translation, const Vector3<_type> &a_scale) :
	m_rotation(a_rotation),
	m_translation(a_translation),
	m_scale(a_scale)
{}

template <class _type>
FORCE_INLINE void Transform<_type>::set(const Quaternion<_type> &a_rotation, const Vector3<_type> &a_translation, const Vector3<_type> &a_scale) noexcept
{
	this->m_rotation    = a_rotation;
	this->m_translation = a_translation;
	this->m_scale       = a_scale;
}

template <class _type>
FORCE_INLINE Transform<_type>::Transform(const Matrix3<_type> &a_matrix) noexcept
{
	this->set(a_matrix);
}

template <class _type>
FORCE_INLINE Transform<_type>::Transform(const Matrix4<_type> &a_matrix) noexcept
{
	this->set(a_matrix);
}

template <class _type>
FORCE_INLINE Transform<_type>::Transform(const Matrix3x4<_type> &a_matrix) noexcept
{
	this->set(a_matrix);
}

template <class _type>
FORCE_INLINE void Transform<_type>::set(const Matrix3<_type> &a_matrix) noexcept
{
	this->m_rotation = ror::Quaternion<_type>(a_matrix);
	// this->m_translation = a_matrix.translation(); // Default origin

	auto x = a_matrix.x_axis().length();
	auto y = a_matrix.y_axis().length();
	auto z = a_matrix.z_axis().length();

	this->m_scale = ror::Vector3<_type>(x, y, z);
}

template <class _type>
FORCE_INLINE void Transform<_type>::set(const Matrix4<_type> &a_matrix) noexcept
{
	this->m_rotation    = ror::Quaternion<_type>(a_matrix);
	this->m_translation = a_matrix.origin();

	auto x = a_matrix.x_axis().length();
	auto y = a_matrix.y_axis().length();
	auto z = a_matrix.z_axis().length();

	this->m_scale = ror::Vector3<_type>(x, y, z);
}

template <class _type>
FORCE_INLINE void Transform<_type>::set(const Matrix3x4<_type> &a_matrix) noexcept
{
	this->m_rotation    = ror::Quaternion<_type>(a_matrix);
	this->m_translation = a_matrix.origin();

	auto x = a_matrix.x_axis().length();
	auto y = a_matrix.y_axis().length();
	auto z = a_matrix.z_axis().length();

	this->m_scale = ror::Vector3<_type>(x, y, z);
}

template <class _type>
FORCE_INLINE Quaternion<_type> Transform<_type>::rotation() const
{
	return this->m_rotation;
}

template <class _type>
FORCE_INLINE Vector3<_type> Transform<_type>::translation() const
{
	return this->m_translation;
}

template <class _type>
FORCE_INLINE Vector3<_type> Transform<_type>::scale() const
{
	return this->m_scale;
}

template <class _type>
FORCE_INLINE void Transform<_type>::set_rotation(const Quaternion<_type> &a_rotation) noexcept
{
	this->m_rotation = a_rotation;
}

template <class _type>
FORCE_INLINE void Transform<_type>::set_translation(const Vector3<_type> &a_translation) noexcept
{
	this->m_translation = a_translation;
}

template <class _type>
FORCE_INLINE void Transform<_type>::set_scale(const Vector3<_type> &a_scale) noexcept
{
	this->m_scale = a_scale;
}

}        // namespace ror
