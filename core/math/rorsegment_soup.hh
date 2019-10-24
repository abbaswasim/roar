// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2019
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

#include "rorsegment_soup.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE void Lines<_type, vector2_typename<_type>>::add_line(_type a_point1, _type a_point2)
{
	this->m_points.emplace_back(a_point1);
	this->m_points.emplace_back(a_point2);
}

template <class _type>
FORCE_INLINE size_t Lines<_type, vector2_typename<_type>>::get_count() const noexcept
{
	return this->m_points.size() / 2;
}

template <class _type>
FORCE_INLINE void Lines<_type, vector3_typename<_type>>::add_line(_type a_point1, _type a_point2)
{
	this->m_points.emplace_back(a_point1);
	this->m_points.emplace_back(a_point2);
}

template <class _type>
FORCE_INLINE size_t Lines<_type, vector3_typename<_type>>::get_count() const noexcept
{
	return this->m_points.size() / 2;
}

template <class _type>
FORCE_INLINE void LineStrip<_type, vector2_typename<_type>>::add_line(_type a_point1)
{
	this->m_points.emplace_back(a_point1);
}

// template <class _type>
// FORCE_INLINE void LineStrip<_type, vector2_typename<_type>>::add_line(_type a_point1, _type a_point2)
// {
//	this->m_points.emplace_back(a_point1);
//	this->m_points.emplace_back(a_point2);
// }

template <class _type>
FORCE_INLINE size_t LineStrip<_type, vector2_typename<_type>>::get_count() const noexcept
{
	return (this->m_points.size() > 0 ? this->m_points.size() - 1 : 0);
}

template <class _type>
FORCE_INLINE void LineStrip<_type, vector3_typename<_type>>::add_line(_type a_point1)
{
	this->m_points.emplace_back(a_point1);
}

// template <class _type>
// FORCE_INLINE void LineStrip<_type, vector3_typename<_type>>::add_line(_type a_point1, _type a_point2)
// {
//	this->m_points.emplace_back(a_point1);
//	this->m_points.emplace_back(a_point2);
// }

template <class _type>
FORCE_INLINE size_t LineStrip<_type, vector3_typename<_type>>::get_count() const noexcept
{
	return (this->m_points.size() > 0 ? this->m_points.size() - 1 : 0);
}

template <class _type>
FORCE_INLINE void Triangles<_type, vector2_typename<_type>>::add_triangle(_type a_point1, _type a_point2, _type a_point3)
{
	this->m_points.emplace_back(a_point1);
	this->m_points.emplace_back(a_point2);
	this->m_points.emplace_back(a_point3);
}

template <class _type>
FORCE_INLINE size_t Triangles<_type, vector2_typename<_type>>::get_count() const noexcept
{
	return this->m_points.size() / 3;
}

template <class _type>
FORCE_INLINE void Triangles<_type, vector3_typename<_type>>::add_triangle(_type a_point1, _type a_point2, _type a_point3)
{
	this->m_points.emplace_back(a_point1);
	this->m_points.emplace_back(a_point2);
	this->m_points.emplace_back(a_point3);
}

template <class _type>
FORCE_INLINE size_t Triangles<_type, vector3_typename<_type>>::get_count() const noexcept
{
	return this->m_points.size() / 3;
}

template <class _type>
FORCE_INLINE void TriangleStrip<_type, vector2_typename<_type>>::add_triangle(_type a_point1)
{
	this->m_points.emplace_back(a_point1);
}

// template <class _type>
// FORCE_INLINE void TriangleStrip<_type, vector2_typename<_type>>::add_triangle(_type a_point1, _type a_point2, _type a_point3)
// {
//	this->m_points.emplace_back(a_point1);
//	this->m_points.emplace_back(a_point2);
//	this->m_points.emplace_back(a_point3);
// }

template <class _type>
FORCE_INLINE size_t TriangleStrip<_type, vector2_typename<_type>>::get_count() const noexcept
{
	return (this->m_points.size() > 1 ? this->m_points.size() - 2 : 0);
}

template <class _type>
FORCE_INLINE void TriangleStrip<_type, vector3_typename<_type>>::add_triangle(_type a_point1)
{
	this->m_points.emplace_back(a_point1);
}

// template <class _type>
// FORCE_INLINE void TriangleStrip<_type, vector3_typename<_type>>::add_triangle(_type a_point1, _type a_point2, _type a_point3)
// {
//	this->m_points.emplace_back(a_point1);
//	this->m_points.emplace_back(a_point2);
//	this->m_points.emplace_back(a_point3);
// }

template <class _type>
FORCE_INLINE size_t TriangleStrip<_type, vector3_typename<_type>>::get_count() const noexcept
{
	// return this->m_points.size() - 2;
	return (this->m_points.size() > 1 ? this->m_points.size() - 2 : 0);
}

template <class _type>
FORCE_INLINE void QuadraticBeziers<_type, vector2_typename<_type>>::add_bezier(_type a_point1, _type a_point2, _type a_point3)
{
	this->m_points.emplace_back(a_point1);
	this->m_points.emplace_back(a_point2);
	this->m_points.emplace_back(a_point3);
}

template <class _type>
FORCE_INLINE size_t QuadraticBeziers<_type, vector2_typename<_type>>::get_count() const noexcept
{
	return this->m_points.size() / 3;
}

template <class _type>
FORCE_INLINE void QuadraticBeziers<_type, vector3_typename<_type>>::add_bezier(_type a_point1, _type a_point2, _type a_point3)
{
	this->m_points.emplace_back(a_point1);
	this->m_points.emplace_back(a_point2);
	this->m_points.emplace_back(a_point3);
}

template <class _type>
FORCE_INLINE size_t QuadraticBeziers<_type, vector3_typename<_type>>::get_count() const noexcept
{
	return this->m_points.size() / 3;
}

template <class _type>
FORCE_INLINE void CubicBeziers<_type, vector2_typename<_type>>::add_bezier(_type a_point1, _type a_point2, _type a_point3, _type a_point4)
{
	this->m_points.emplace_back(a_point1);
	this->m_points.emplace_back(a_point2);
	this->m_points.emplace_back(a_point3);
	this->m_points.emplace_back(a_point4);
}

template <class _type>
FORCE_INLINE size_t CubicBeziers<_type, vector2_typename<_type>>::get_count() const noexcept
{
	return this->m_points.size() / 4;
}

template <class _type>
FORCE_INLINE void CubicBeziers<_type, vector3_typename<_type>>::add_bezier(_type a_point1, _type a_point2, _type a_point3, _type a_point4)
{
	this->m_points.emplace_back(a_point1);
	this->m_points.emplace_back(a_point2);
	this->m_points.emplace_back(a_point3);
	this->m_points.emplace_back(a_point4);
}

template <class _type>
FORCE_INLINE size_t CubicBeziers<_type, vector3_typename<_type>>::get_count() const noexcept
{
	return this->m_points.size() / 4;
}
}        // namespace ror
