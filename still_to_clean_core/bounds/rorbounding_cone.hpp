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

#include "interfaces/roribounds.h"
#include "rorbounding_sphere.h"
#include "utility/rorutility.h"

namespace ror
{
class ROAR_ENGINE_ITEM BoundingCone : public IBounds
{
  public:
	FORCE_INLINE BoundingCone(void);
	~BoundingCone(void);

	// Methods from the base must be override
	FORCE_INLINE void add_point_to_bound(const Vector3f &a_point);
	FORCE_INLINE void add_bound_to_bound(IBounds *a_bounding);
	FORCE_INLINE bool is_point_inside(const Vector3f &a_point) const;
	FORCE_INLINE bool is_bounding_inside(IBounds *a_bounding);

	FORCE_INLINE void add_bounding_cone(const BoundingCone *a_bounding_sphere);
	FORCE_INLINE bool is_bounding_cone_inside(const BoundingCone *a_bounding_sphere) const;
	FORCE_INLINE bool is_bounding_sphere_inside(const BoundingSphere *a_bounding_sphere) const;

	FORCE_INLINE Vector3f get_origin() const;
	FORCE_INLINE void     set_origin(Vector3f &a_center);
	FORCE_INLINE Vector3f get_direction() const;
	FORCE_INLINE void     set_direction(Vector3f &a_center);
	FORCE_INLINE float32_t get_expansion_angle() const;
	FORCE_INLINE void      set_expansion_angle(float32_t a_radius);
	FORCE_INLINE float32_t sin_expansion_angle() const;
	FORCE_INLINE void      sin_expansion_angle(float32_t a_sin_expansion_angle);
	FORCE_INLINE float32_t cos_expansion_angle() const;
	FORCE_INLINE void      cos_expansion_angle(float32_t a_cos_expansion_angle);
	FORCE_INLINE float32_t sin_expansion_angle_square() const;
	FORCE_INLINE void      sin_expansion_angle_square(float32_t a_sin_expansion_angle_square);
	FORCE_INLINE float32_t cos_expansion_angle_square() const;
	FORCE_INLINE void      cos_expansion_angle_square(float32_t a_cos_expansion_angle_square);
	FORCE_INLINE float32_t sin_expansion_angle_reciprocal() const;
	FORCE_INLINE void      sin_expansion_angle_reciprocal(float32_t a_sin_expansion_angle_reciprocal);

  private:
	Vector3f  m_origin;                 // Origin of the Cone
	Vector3f  m_direction;              // Direction of the Cone
	float32_t m_expansion_angle;        // Defines the Expansion of the Cone

	float32_t m_sin_expansion_angle;        // Other Utilities
	float32_t m_cos_expansion_angle;
	float32_t m_sin_expansion_angle_square;
	float32_t m_cos_expansion_angle_square;
	float32_t m_sin_expansion_angle_reciprocal;
};
}        // namespace ror

#include "rorbounding_cone.hh"
