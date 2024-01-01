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

#include "foundation/rortypes.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"
#include "rorgeometry_utilities.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

namespace ror
{
/**
 * @brief      Returns 8 corners for a cube or 4 corners of a box
 * @param      a_minimum Starting point of the box
 * @param      a_maximum Finishing point of the box
 * @param      a_corners The amount of corners requested (8, 4)
 * @param      a_output Corners are returned here
 */
template <class _type>
FORCE_INLINE void _box_corners_internal(const _type &a_minimum, const _type &a_maximum, uint32_t a_corners, std::vector<_type> &a_output)
{
	a_output.reserve(a_output.size() + a_corners);

	// https://github.com/sgorsten/linalg algoritm for creating cube
	for (uint32_t i = 0; i < a_corners; ++i)
	{
		// Two copies of Vector3i or Vector2i arn't good but this isn't suppose to be real time
		_type corner = vector_select(vector_type<_type, uint32_t>(Vector3ui(i & 1, i & 2, i & 4)), a_minimum, a_maximum);
		a_output.emplace_back(corner);
	}
}

/**
 * @brief      Returns 8 corners for a cube or 4 corners of a box
 * @param      a_minimum Starting point of the box
 * @param      a_maximum Finishing point of the box
 * @param      a_vertex_buffer Corners are returned here
 */
template <class _type>
FORCE_INLINE void _box_points_internal(const _type &a_minimum, const _type &a_maximum, std::vector<_type> &a_vertex_buffer)
{
	static_assert(!(std::is_same<_type, Vector4<typename _type::value_type>>::value), "Can't create a 4D Cube\n");

	uint32_t corners = 8;
	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		corners = 4;
	}

	_box_corners_internal(a_minimum, a_maximum, corners, a_vertex_buffer);
}

/**
 * @brief      Returns 12 or 2 triangles created from 8 corners of a cube or 4 corners of a box
 * @param      a_minimum Starting point of the box
 * @param      a_maximum Finishing point of the box
 * @param      a_vertex_buffer Corners are returned here
 */
template <class _type>
FORCE_INLINE void _box_triangles_internal(const _type &a_minimum, const _type &a_maximum, std::vector<_type> &a_vertex_buffer, bool a_clock_wise)
{
	static_assert(!(std::is_same<_type, Vector4<typename _type::value_type>>::value), "Can't create a 4D Cube\n");

	const size_t indices_ccw[] = {1, 3, 5, 5, 3, 7, 4, 6, 0, 0, 6, 2, 4, 0, 5, 5, 0, 1, 2, 6, 3, 3, 6, 7, 0, 2, 1, 1, 2, 3, 5, 7, 4, 4, 7, 6};
	const size_t indices_cw[]  = {1, 5, 3, 5, 7, 3, 4, 0, 6, 0, 2, 6, 4, 5, 0, 5, 1, 0, 2, 3, 6, 3, 7, 6, 0, 1, 2, 1, 3, 2, 5, 4, 7, 4, 6, 7};

	uint32_t corners   = 8;
	uint32_t triangles = 12;

	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		corners   = 4;
		triangles = 2;
	}

	std::vector<_type> vertex_buffer;
	_box_corners_internal(a_minimum, a_maximum, corners, vertex_buffer);

	// Lets calculate index buffer and triangles
	a_vertex_buffer.reserve(a_vertex_buffer.size() + triangles * 9);
	const auto indices = a_clock_wise ? indices_cw : indices_ccw;
	for (uint32_t i = 0; i < triangles; ++i)
	{
		a_vertex_buffer.emplace_back(vertex_buffer[indices[3 * i]]);
		a_vertex_buffer.emplace_back(vertex_buffer[indices[3 * i + 1]]);
		a_vertex_buffer.emplace_back(vertex_buffer[indices[3 * i + 2]]);
	}
}

/**
 * @brief      Returns indices for a_triangles amount of triangles for a Cube or Box
 * @param      a_index_buffer Triangle indices are returned here
 * @param      a_triangles Amount of triangles requested (2 for a box, 12 for a Cube)
 */
template <class _index_type = uint32_t>
FORCE_INLINE void _box_triangles_indices_internal(uint32_t a_triangles, std::vector<ror::Vector3<_index_type>> &a_index_buffer, bool a_clock_wise)
{
	// Order of indices is +x, -x, +y, -y, +z, -z
	const std::vector<ror::Vector3<_index_type>> indices_ccw{{1, 3, 5}, {5, 3, 7}, {4, 6, 0}, {0, 6, 2}, {4, 0, 5}, {5, 0, 1}, {2, 6, 3}, {3, 6, 7}, {0, 2, 1}, {1, 2, 3}, {5, 7, 4}, {4, 7, 6}};
	const std::vector<ror::Vector3<_index_type>> indices_cw{{1, 5, 3}, {5, 7, 3}, {4, 0, 6}, {0, 2, 6}, {4, 5, 0}, {5, 1, 0}, {2, 3, 6}, {3, 7, 6}, {0, 1, 2}, {1, 3, 2}, {5, 4, 7}, {4, 6, 7}};

	const auto indices = a_clock_wise ? indices_cw : indices_ccw;
	a_index_buffer.reserve(a_index_buffer.size() + a_triangles);
	std::copy(indices.begin(), indices.begin() + a_triangles, std::back_inserter(a_index_buffer));
}

template <class _type>
FORCE_INLINE void _box_lines_internal(const _type &a_minimum, const _type &a_maximum, std::vector<_type> &a_vertex_buffer)
{
	static_assert(!(std::is_same<_type, Vector4<typename _type::value_type>>::value), "Can't create a 4D Box\n");

	const size_t indices[] = {0, 1, 1, 3, 3, 2, 2, 0, 0, 4, 1, 5, 2, 6, 3, 7, 4, 5, 5, 7, 7, 6, 6, 4};
	uint32_t     corners   = 8;
	uint32_t     lines     = 12;

	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		corners = 4;
		lines   = 4;
	}

	std::vector<_type> vertex_buffer;
	_box_corners_internal(a_minimum, a_maximum, corners, vertex_buffer);

	// Lets calculate index buffer and lines
	a_vertex_buffer.reserve(a_vertex_buffer.size() + lines);
	for (uint32_t i = 0; i < lines; ++i)
	{
		a_vertex_buffer.emplace_back(vertex_buffer[indices[2 * i]]);
		a_vertex_buffer.emplace_back(vertex_buffer[indices[2 * i + 1]]);
	}
}

template <class _index_type = uint32_t>
FORCE_INLINE void _box_lines_indices_internal(uint32_t a_lines, std::vector<ror::Vector2<_index_type>> &a_index_buffer)
{
	const std::vector<ror::Vector2<_index_type>> indices{{0, 1}, {1, 3}, {3, 2}, {2, 0}, {0, 4}, {1, 5}, {2, 6}, {3, 7}, {4, 5}, {5, 7}, {7, 6}, {6, 4}};
	a_index_buffer.reserve(a_index_buffer.size() + a_lines);
	std::copy(indices.begin(), indices.begin() + a_lines, std::back_inserter(a_index_buffer));
}

template <class _type>
FORCE_INLINE void make_box_triangles(std::vector<_type> &a_vertex_buffer, const _type &a_minimum, const _type &a_maximum, bool a_clock_wise)
{
	_box_triangles_internal(a_minimum, a_maximum, a_vertex_buffer, a_clock_wise);
}

template <class _type>
FORCE_INLINE void make_box_triangles(const _type &a_size, const _type &a_origin, std::vector<_type> &a_vertex_buffer, bool a_clock_wise)
{
	_type half_size = (a_size / 2);
	_type minimum{a_origin - half_size}, maximum{half_size + a_origin};

	_box_triangles_internal(minimum, maximum, a_vertex_buffer, a_clock_wise);
}

template <class _type>
FORCE_INLINE void make_box_triangles(float32_t a_size, const _type &a_origin, std::vector<_type> &a_vertex_buffer, bool a_clock_wise)
{
	make_box_triangles(_type(a_size), a_origin, a_vertex_buffer, a_clock_wise);
}

template <class _type>
FORCE_INLINE void make_box_triangles(float32_t a_size, std::vector<_type> &a_vertex_buffer, bool a_clock_wise)
{
	make_box_triangles(_type(a_size), _type(0), a_vertex_buffer, a_clock_wise);
}

template <class _type>
FORCE_INLINE std::vector<_type> make_box_triangles(float32_t a_size, bool a_clock_wise)
{
	std::vector<_type> a_vertex_buffer;
	make_box_triangles(_type(a_size), _type(0), a_vertex_buffer, a_clock_wise);
	return a_vertex_buffer;
}

template <class _type, class _index_type>
FORCE_INLINE void make_box_triangles_indexed(const _type &a_size, const _type &a_origin,
                                             std::vector<_type>                     &a_vertex_buffer,
                                             std::vector<ror::Vector3<_index_type>> &a_index_buffer, bool a_clock_wise)
{
	uint32_t triangles = 12;
	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		triangles = 2;
	}

	_type half_size = (a_size / 2);
	_type minimum{a_origin - half_size}, maximum{half_size + a_origin};

	_box_points_internal(minimum, maximum, a_vertex_buffer);
	_box_triangles_indices_internal(triangles, a_index_buffer, a_clock_wise);
}

template <class _type, class _index_type>
FORCE_INLINE void make_box_triangles_indexed(std::vector<_type> &a_vertex_buffer, std::vector<ror::Vector3<_index_type>> &a_index_buffer, bool a_clock_wise)
{
	make_box_triangles_indexed(_type(1), _type(0), a_vertex_buffer, a_index_buffer, a_clock_wise);
}

// Same set of functions for box but this time for lines instead of triangles
template <class _type>
FORCE_INLINE void make_box_lines(std::vector<_type> &a_vertex_buffer, const _type &a_minimum, const _type &a_maximum)
{
	_box_lines_internal(a_minimum, a_maximum, a_vertex_buffer);
}

template <class _type>
FORCE_INLINE void make_box_lines(const _type &a_size, const _type &a_origin, std::vector<_type> &a_vertex_buffer)
{
	_type half_size = (a_size / 2);
	_type minimum{a_origin - half_size}, maximum{half_size + a_origin};

	_box_lines_internal(minimum, maximum, a_vertex_buffer);
}

template <class _type>
FORCE_INLINE void make_box_lines(float32_t a_size, const _type &a_origin, std::vector<_type> &a_vertex_buffer)
{
	make_box_lines(_type(a_size), a_origin, a_vertex_buffer);
}

template <class _type>
FORCE_INLINE void make_box_lines(float32_t a_size, std::vector<_type> &a_vertex_buffer)
{
	make_box_lines(_type(a_size), _type(0), a_vertex_buffer);
}

template <class _type>
FORCE_INLINE std::vector<_type> make_box_lines(float32_t a_size)
{
	std::vector<_type> a_vertex_buffer;
	make_box_lines(_type(a_size), _type(0), a_vertex_buffer);
	return a_vertex_buffer;
}

template <class _type, class _index_type>
FORCE_INLINE void make_box_lines_indexed(const _type &a_size, const _type &a_origin,
                                         std::vector<_type>                     &a_vertex_buffer,
                                         std::vector<ror::Vector2<_index_type>> &a_index_buffer)
{
	uint32_t lines = 12;
	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		lines = 4;
	}

	_type half_size = (a_size / 2);
	_type minimum{a_origin - half_size}, maximum{half_size + a_origin};

	_box_points_internal(minimum, maximum, a_vertex_buffer);
	_box_lines_indices_internal(lines, a_index_buffer);
}

template <class _type, class _index_type>
FORCE_INLINE void make_box_lines_indexed(std::vector<_type> &a_vertex_buffer, std::vector<ror::Vector2<_index_type>> &a_index_buffer)
{
	make_box_lines_indexed(_type(1), _type(0), a_vertex_buffer, a_index_buffer);
}

template <class _type>
FORCE_INLINE void make_sphere_triangles(uint32_t a_samples)
{
	(void) a_samples;
}

template <class _type>
FORCE_INLINE void add_normals(const std::vector<_type> &a_vertex_buffer,
                              uint32_t a_size, std::vector<_type> &a_output,
                              const std::vector<_type> *a_index_buffer)
{
	(void) a_vertex_buffer;
	(void) a_size;
	(void) a_output;
	(void) a_index_buffer;
}

template <class _type>
FORCE_INLINE void add_colors(const std::vector<_type> &a_vertex_buffer,
                             uint32_t a_size, std::vector<_type> &a_output,
                             const std::vector<_type> *a_index_buffer)
{
	(void) a_vertex_buffer;
	(void) a_size;
	(void) a_output;
	(void) a_index_buffer;
}

}        // namespace ror
