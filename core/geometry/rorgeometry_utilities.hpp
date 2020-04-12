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
 * @brief      Creates a cube with a_size in all 3 or 2 directions
 * @param      a_size a 2D or 3D vector defining its size
 * @param      a_origin Origin of the cube
 * @param      a_output A vector to append the vertices created to
 * @return     return void
 */
template <class _type>
void cube(const _type &a_size, const _type &a_origin, std::vector<_type> &a_output);

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

}        // namespace ror

#include "rorgeometry_utilities.hh"
