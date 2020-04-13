// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2020
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

#include "rorgeometry_utilities.hpp"

namespace ror
{
template <class _type>
FORCE_INLINE void _cube_internal(const _type &a_minimum, const _type &a_maximum,
								 const _type &a_origin, std::vector<_type> &a_vertex_buffer)
{
	static_assert(!(std::is_same<_type, Vector4<typename _type::value_type>>::value), "Can't create a 4D Cube\n");

	uint32_t corners = 8;
	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		corners = 4;
	}

	// https://github.com/sgorsten/linalg algoritm for creating cube
	for (uint32_t i = 0; i < corners; ++i)
	{
		// Two copies of Vector3i or Vector2i arn't good but this isn't suppose to be real time
		_type corner = vector_select(vector_type<_type, uint32_t>(Vector3ui(i & 1, i & 2, i & 4)), a_maximum, a_minimum) + a_origin;
		a_vertex_buffer.emplace_back(corner);
	}
}

template <class _type>
FORCE_INLINE void cube(std::vector<_type> &a_vertex_buffer, const _type &a_minimum, const _type &a_maximum)
{
	_cube_internal(a_minimum, a_maximum, _type(0), a_vertex_buffer);
}

template <class _type, class _index_type = uint32_t>
FORCE_INLINE void cube(const _type &a_size, const _type &a_origin, std::vector<_type> &a_vertex_buffer,
					   std::vector<ror::Vector3<_index_type>> &a_index_buffer)
{
	cube(a_size, a_origin, a_vertex_buffer);

	// Lets calculate index buffer and triangles
	const _index_type indices[]  = {0, 1, 2, 2, 1, 3, 4, 5, 0, 0, 5, 1, 0, 1, 6, 6, 1, 7, 4, 0, 6, 6, 0, 2, 0, 5, 3, 3, 5, 7, 5, 4, 7, 7, 4, 6};
	uint32_t          triangles = 12;
	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		triangles = 2;
	}

	for (uint32_t i = 0; i < triangles; ++i)
	{
		a_index_buffer.emplace_back(ror::Vector3<_index_type>(indices[3 * i], indices[3 * i + 1], indices[3 * i + 2]));
	}
}

template <class _type, class _index_type = uint32_t>
FORCE_INLINE void cube(std::vector<_type> &a_vertex_buffer, std::vector<ror::Vector3<_index_type>> &a_index_buffer)
{
	cube(_type(1), _type(0), a_vertex_buffer, a_index_buffer);
}

template <class _type>
FORCE_INLINE void cube(const _type &a_size, const _type &a_origin, std::vector<_type> &a_vertex_buffer)
{
	_type minimum{-a_size / 2}, maximum{a_size / 2};
	_cube_internal(minimum, maximum, a_origin, a_vertex_buffer);
}

template <class _type>
FORCE_INLINE void cube(float32_t a_size, const _type &a_origin, std::vector<_type> &a_vertex_buffer)
{
	cube(_type(a_size), a_origin, a_vertex_buffer);
}

template <class _type>
FORCE_INLINE void cube(float32_t a_size, std::vector<_type> &a_vertex_buffer)
{
	cube(_type(a_size), _type(0), a_vertex_buffer);
}

template <class _type>
FORCE_INLINE std::vector<_type> cube(float32_t a_size)
{
	std::vector<_type> a_vertex_buffer;
	cube(_type(a_size), _type(0), a_vertex_buffer);
	return a_vertex_buffer;
}

template <class _type>
FORCE_INLINE void sphere(uint32_t a_samples)
{
	// std::vector<_type> a_vertex_buffer;
	// cube(_type(a_size), _type(0), a_vertex_buffer);
	// return a_vertex_buffer;
}

template <class _type>
FORCE_INLINE void add_normals(const std::vector<_type> &a_vertex_buffer,
							  uint32_t a_size, std::vector<_type> &a_output,
							  const std::vector<_type> *a_index_buffer)
{
}

template <class _type>
FORCE_INLINE void add_colors(const std::vector<_type> &a_vertex_buffer,
							 uint32_t a_size, std::vector<_type> &a_output,
							 const std::vector<_type> *a_index_buffer)
{
}

}        // namespace ror
