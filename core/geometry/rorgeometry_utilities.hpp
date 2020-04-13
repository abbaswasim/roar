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

#include "math/rorvector_functions.hpp"

namespace ror
{
/**
 * @brief      Creates a cube with from minimum and maximum
 * @param      a_minimum Minimum of the box to create
 * @param      a_maximum Maximum of the box to create
 * @param      a_vertex_buffer A vector to append the vertices created to
 * @return     return void
 */
template <class _type>
FORCE_INLINE void cube(std::vector<_type> &a_vertex_buffer, const _type &a_minimum, const _type &a_maximum);

/**
 * @brief      Creates a cube with a_size in all 3 or 2 directions
 * @param      a_size a 2D or 3D vector defining its size
 * @param      a_origin Origin of the cube
 * @param      a_vertex_buffer A vector to append the vertices created to
 * @param      a_index_buffer A vector to append the indices created to
 * @return     return void
 */
template <class _type, class _index_type = uint32_t>
FORCE_INLINE void cube(const _type &a_size, const _type &a_origin,
					   std::vector<_type> &      a_vertex_buffer,
					   std::vector<_index_type> &a_index_buffer);

/**
 * @brief      Creates a cube with unit size 3 or 2 directions
 * @param      a_vertex_buffer A vector to append the vertices created to
 * @param      a_index_buffer A vector to append the indices created to
 * @return     return void
 */
template <class _type, class _index_type = uint32_t>
FORCE_INLINE void cube(std::vector<_type> &a_vertex_buffer, std::vector<_index_type> &a_index_buffer);

/**
 * @brief      Creates a cube with a_size in all 3 or 2 directions
 * @param      a_size a 2D or 3D vector defining its size
 * @param      a_origin Origin of the cube
 * @param      a_output A vector to append the vertices created to
 * @return     return void
 */
template <class _type>
FORCE_INLINE void cube(const _type &a_size, const _type &a_origin, std::vector<_type> &a_output);

/**
 * @brief      Creates a cube with size in all 3 or 2 directions
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @param      a_origin Origin of the cube
 * @param      a_output A vector to append the vertices created to
 * @return     return void
 */
template <class _type>
FORCE_INLINE void cube(float32_t a_size, const _type &a_origin, std::vector<_type> &a_output);

/**
 * @brief      Creates a cube with size in all 3 or 2 directions at origin
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @param      a_output A vector to append the vertices created to
 * @return     return void
 */
template <class _type>
FORCE_INLINE void cube(float32_t a_size, std::vector<_type> &a_output);

/**
 * @brief      Creates a cube with size in all 3 or 2 directions at origin
 * @param      a_size float defining its size in all 3 or 2 dimentions
 * @return     return A vector to append the vertices created to
 */
template <class _type>
FORCE_INLINE std::vector<_type> cube(float32_t a_size);

/**
 * @brief      Creates a sphere with samples number of points in all 3 or 2 directions at origin
 * @param      a_samples number of points
 * @return     return A vector to append the vertices created to
 */

// Sphere motivations
// https://stackoverflow.com/a/26127012 for Fibonacci sphere
template <class _type>
FORCE_INLINE void sphere(uint32_t a_samples, const _type &a_origin, std::vector<_type> &a_output);

/**
 * @brief      Creates normals for triangles
 * @param      a_vertex_buffer Vertex buffer to use for calculating normals
 * @param      a_index_buffer Optional index buffer
 * @param      a_size Amount of trianlges
 * @param      a_output Normals for each vertex are created in a_output
 * @return     return void
 */
template <class _type>
FORCE_INLINE void add_normals(const std::vector<_type> &a_vertex_buffer,
							  uint32_t a_size, std::vector<_type> &a_output,
							  const std::vector<_type> *a_index_buffer = nullptr);

/**
 * @brief      Creates colors for triangles
 * @param      a_vertex_buffer Vertex buffer to use for calculating colors
 * @param      a_index_buffer Optional index buffer
 * @param      a_size Amount of trianlges
 * @param      a_output Normals for each vertex are created in a_output
 * @return     return void
 */
template <class _type>
FORCE_INLINE void add_colors(const std::vector<_type> &a_vertex_buffer,
							 uint32_t a_size, std::vector<_type> &a_output,
							 const std::vector<_type> *a_index_buffer = nullptr);

}        // namespace ror

#include "rorgeometry_utilities.hh"
