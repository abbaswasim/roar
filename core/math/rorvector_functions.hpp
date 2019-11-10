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

#pragma once

#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"

#include <algorithm>

namespace ror
{
/**
 * For vectors make sure at least float32_t precision is provided for each implementation, for double64_t it will operate in double64_t precision
 * This stuff is really gross :)
 */
template <template <class> class _type, class _precision>
using vector_precision = typename std::conditional<std::is_same<typename _type<_precision>::value_type, double64_t>::value, _type<double64_t>, _type<float32_t>>::type;

/**
 * For scalars make sure at least float32_t precision is provided for each implementation, for double64_t it will operate in double64_t precision
 */
template <class _type>
using scalar_precision = typename std::conditional<std::is_same<typename _type::value_type, double64_t>::value, double64_t, float32_t>::type;

/**
 * @brief      Calculates distance
 * @details    Calculates distance between the two vectors provided
 * @param      a_first First vector
 * @param      a_second Second vector
 * @return     Distance between the two vectors
 */
template <class _type>
FORCE_INLINE auto distance(const _type &a_first, const _type &a_second) -> scalar_precision<_type>;

/**
 * @brief      Calculates distance squared
 * @details    Calculates distance squared between the two vectors provided
 * @param      a_first First vector
 * @param      a_second Second vector
 * @return     Squared distance between the two vectors
 */
template <class _type>
FORCE_INLINE auto distance_squared(const _type &a_first, const _type &a_second) -> scalar_precision<_type>;

/**
 * @brief      Calculates angle
 * @details    Calculates angle between the two vectors provided in radians
 * @param      a_first First vector
 * @param      a_second Second vector
 * @return     Angle between the two vectors
 */
template <class _type>
FORCE_INLINE auto angle(const _type &a_first, const _type &a_second) -> scalar_precision<_type>;

/**
 * @brief      Calculates scalar projection
 * @details    Calculates scalar projection of first onto second
 *             If one multiply scalar projection by unit vector of second you will get vector projection
 * @param      a_first First vector to calculate projection for
 * @param      a_second Second vector to calculate projection onto
 * @return     Scalar projection of a_first onto a_second
 */
template <class _type>
FORCE_INLINE auto scalar_projection(const _type &a_first, const _type &a_second) -> scalar_precision<_type>;

/**
 * @brief      Calculates Vector projection
 * @details    Calculates Vector projection of first onto second
 * @param      a_first First vector to calculate projection for
 * @param      a_second Second vector to calculate projection onto
 * @return     Vector projection of a_first onto a_second
 */
template <template <class> class _type, class _precision>
FORCE_INLINE auto vector_projection(const _type<_precision> &a_first, const _type<_precision> &a_second) -> vector_precision<_type, _precision>;

/**
 * @brief      Calculates Vector rejection
 * @details    Calculates Vector rejection of first onto second,
 *             Vector rejection is the line perpendicular to projection
 * @param      a_first First vector to calculate rejection for
 * @param      a_second Second vector to calculate rejection onto
 * @return     Vector rejection of a_first onto a_second
 */
template <template <class> class _type, class _precision>
FORCE_INLINE auto vector_rejection(const _type<_precision> &a_first, const _type<_precision> &a_second) -> vector_precision<_type, _precision>;

/**
 * @brief      Calculates linear interpolation
 * @details    Calculates linear interpolation between two vectors at distance t
 * @param      a_from First vector starting point of interpolation
 * @param      a_to Second vector ending point of interpolation
 * @param      a_t Distance of interpolation 0 means will result in from while 1 will result in to
 * @return     Linear interpolation between the two vectors
 */
template <template <class> class _type, class _precision>
FORCE_INLINE auto vector_lerp(const _type<_precision> &a_from, const _type<_precision> &a_to, scalar_precision<_type<_precision>> a_t) -> vector_precision<_type, _precision>;

/**
 * @brief      Reflection from incident around normal
 * @details    Calculates relection around the normal in the opposite direction of incident
 * @param      a_incident Incident vector to create reflection for
 * @param      a_normal Normal vector to create reflection around
 * @return     Reflection from incident vector around normal towards the refelcted direction
 */
template <template <class> class _type, class _precision>
FORCE_INLINE auto vector_reflect(const _type<_precision> &a_incident, const _type<_precision> &a_normal) -> vector_precision<_type, _precision>;

}        // namespace ror

#include "rorvector_functions.hh"
