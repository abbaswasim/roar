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

#pragma once

#include "math/rorvector_functions.hpp"

namespace ror
{
template <class _type>
class ROAR_ENGINE_ITEM Ray final
{
  public:
	_type m_origin;           //! Starting point of ray
	_type m_direction;        //! Direction of the ray from m_origin. Always normalized (unit vector)

	FORCE_INLINE Ray()                       = default;                   //! Default constructor
	FORCE_INLINE Ray(const Ray &a_other)     = default;                   //! Copy constructor
	FORCE_INLINE Ray(Ray &&a_other) noexcept = default;                   //! Move constructor
	FORCE_INLINE Ray &operator=(const Ray &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Ray &operator=(Ray &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Ray() noexcept                        = default;        //! Destructor

	FORCE_INLINE Ray(_type a_origin, _type a_direction);

	FORCE_INLINE void set(_type a_origin, _type a_direction);
	FORCE_INLINE void set_normalized(_type a_origin, _type a_direction);        //! Direction is already normalized so won't do it again

	FORCE_INLINE _type origin();
	FORCE_INLINE _type direction();

	FORCE_INLINE void set_origin(_type a_origin);
	FORCE_INLINE void set_direction(_type a_direction);
	FORCE_INLINE void set_direction_normalized(_type a_direction);        //! Direction is already normalized so won't do it again

	FORCE_INLINE _type point_on_ray(scalar_precision<_type> a_t);

	FORCE_INLINE auto  distance_to_point(_type a_point) -> scalar_precision<_type>;
	FORCE_INLINE _type closest_point_on_ray(_type a_point);

  protected:
  private:
};

using Ray3 = Ray<Vector3f>;
using Ray2 = Ray<Vector2f>;

}        // namespace ror

#include "rorray.hh"
