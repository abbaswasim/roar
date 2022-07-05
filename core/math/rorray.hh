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

#include "rorray.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE Ray<_type>::Ray(_type a_origin, _type a_direction) :
    m_origin(a_origin),
    m_direction(a_direction)
{
	this->m_direction.normalize();
}

template <class _type>
FORCE_INLINE void Ray<_type>::set(_type a_origin, _type a_direction)
{
	this->m_origin    = a_origin;
	this->m_direction = a_direction;
	this->m_direction.normalize();
}

template <class _type>
FORCE_INLINE void Ray<_type>::set_normalized(_type a_origin, _type a_direction)
{
	this->m_origin    = a_origin;
	this->m_direction = a_direction;
}

template <class _type>
FORCE_INLINE _type Ray<_type>::origin()
{
	return this->m_origin;
}

template <class _type>
FORCE_INLINE _type Ray<_type>::direction()
{
	return this->m_direction;
}

template <class _type>
FORCE_INLINE void Ray<_type>::set_origin(_type a_origin)
{
	this->m_origin = a_origin;
}

template <class _type>
FORCE_INLINE void Ray<_type>::set_direction(_type a_direction)
{
	this->m_direction = a_direction;
	this->m_direction.normalize();
}

template <class _type>
FORCE_INLINE void Ray<_type>::set_direction_normalized(_type a_direction)
{
	this->m_direction = a_direction;
}

template <class _type>
FORCE_INLINE _type Ray<_type>::point_on_ray(scalar_precision<_type> a_t)
{
	return this->m_origin + (this->m_direction * static_cast<typename _type::value_type>(a_t));
}

template <class _type>
FORCE_INLINE auto Ray<_type>::distance_to_point(_type a_point) -> scalar_precision<_type>
{
	auto to_point   = a_point - this->m_origin;
	auto projection = scalar_projection(to_point, this->m_direction);

	if (projection < 0)
	{
		return distance(a_point, this->m_origin);
	}

	auto point = this->point_on_ray(projection);
	return distance(a_point, point);
}

template <class _type>
FORCE_INLINE _type Ray<_type>::closest_point_on_ray(_type a_point)
{
	auto to_point   = a_point - this->m_origin;
	auto projection = scalar_projection(to_point, this->m_direction);

	if (projection < 0)
	{
		return this->m_origin;
	}

	return this->point_on_ray(projection);
}

}        // namespace ror
