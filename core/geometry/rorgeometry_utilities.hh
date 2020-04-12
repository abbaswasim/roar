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
/**
 * @brief      Creates a cube with a_size in all 3 or 2 directions
 * @param      a_size a 2D or 3D vector defining its size
 * @param      a_origin Origin of the cube
 * @param      a_output A vector to append the vertices created to
 * @return     return void
 */
template <class _type>
void cube(const _type &a_size, const _type &a_origin, std::vector<_type> &a_output)
{
	static_assert(!(std::is_same<_type, Vector4<typename _type::value_type>>::value), "Can't create a 4D Cube\n");

	uint32_t corners = 8;
	if constexpr (std::is_same<_type, Vector2<typename _type::value_type>>::value)
	{
		corners = 4;
	}

	// https://github.com/sgorsten/linalg algoritm for creating cube
	_type minimum{-a_size / 2}, maximum{a_size / 2};
	for (uint32_t i = 0; i < corners; ++i)
	{
		// Two copies of Vector3i or Vector2i arn't good but this isn't suppose to be real time
		_type corner = vector_select(vector_type<_type, uint32_t>(Vector3ui(i & 1, i & 2, i & 4)), maximum, minimum) + a_origin;
		a_output.emplace_back(corner);
	}
}

/**
 * @brief      Creates a cube with size in all 3 or 2 directions
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @param      a_origin Origin of the cube
 * @param      a_output A vector to append the vertices created to
 * @return     return void
 */
template <class _type>
FORCE_INLINE void cube(float32_t a_size, const _type &a_origin, std::vector<_type> &a_output)
{
	cube(_type(a_size), a_origin, a_output);
}

/**
 * @brief      Creates a cube with size in all 3 or 2 directions at origin
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @param      a_output A vector to append the vertices created to
 * @return     return void
 */
template <class _type>
FORCE_INLINE void cube(float32_t a_size, std::vector<_type> &a_output)
{
	cube(_type(a_size), _type(0), a_output);
}

/**
 * @brief      Creates a cube with size in all 3 or 2 directions at origin
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @return     return A vector to append the vertices created to
 */
template <class _type>
FORCE_INLINE std::vector<_type> cube(float32_t a_size)
{
	std::vector<_type> a_output;
	cube(_type(a_size), _type(0), a_output);
	return a_output;
}

}        // namespace ror
