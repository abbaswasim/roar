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

#include "bounds/rorbounding_cone.h"
#include "bounds/rorbounding_sphere.h"
#include "math/rormath_utility.h"
#include "math/rormatrix4.h"
#include "math/rorplane.h"
#include "math/rorvector3.h"

namespace ror
{
class ROAR_ENGINE_ITEM Viewport;
class ROAR_ENGINE_ITEM Frustrum
{
  public:
	// Ctor and Dtor
	FORCE_INLINE Frustrum(void);
	FORCE_INLINE Frustrum(float32_t a_left, float32_t a_right, float32_t a_bottom, float32_t a_top, float32_t a_near, float32_t a_far);
	virtual ~Frustrum(void);

	// Utility Names for the Planes
	enum
	{
		rFRUSTRUM_LEFT = 0,
		rFRUSTRUM_RIGHT,
		rFRUSTRUM_BOTTOM,
		rFRUSTRUM_TOP,
		rFRUSTRUM_NEAR,
		rFRUSTRUM_FAR,
		rFRUSTRUM_MAX
	};

	// Frustrum Accessors and Mutators Methods
	FORCE_INLINE float32_t get_fov() const;
	FORCE_INLINE void      set_fov(float32_t a_fov);
	FORCE_INLINE float32_t get_far() const;
	FORCE_INLINE void      set_far(float32_t a_far);
	FORCE_INLINE float32_t get_near() const;
	FORCE_INLINE void      set_near(float32_t a_near);
	FORCE_INLINE void      set_frustrum(float32_t a_left, float32_t a_right, float32_t a_bottom, float32_t a_top, float32_t a_near, float32_t a_far);
	FORCE_INLINE void      set_frustrum(float32_t *a_frustrum);
	FORCE_INLINE void      get_frustrum(float32_t &a_left, float32_t &a_right, float32_t &a_bottom, float32_t &a_top, float32_t &a_near, float32_t &a_far) const;
	FORCE_INLINE const float32_t *get_frustrum() const;
	FORCE_INLINE void             calculate_planes(const Matrix4f *a_model_view_projectio_matrix, bool a_normalize);
	FORCE_INLINE void             calculate_bounding_cone(Vector3f *a_view_direction, Vector3f *a_camera_position, int32_t a_screen_width, int32_t a_screen_height);
	FORCE_INLINE void             calculate_bounding_sphere(Vector3f *a_view_direction, Vector3f *a_camera_position);
	FORCE_INLINE EVisibiltyInFrustrum contains_sphere(const BoundingSphere *a_bounding_sphere, bool a_do_approximate_check) const;

  protected:
	friend class ROAR_ENGINE_ITEM Viewport;

	union
	{
		struct
		{
			float32_t m_left, m_right;        // Left , Right
			float32_t m_bottom, m_top;        // Bottom , Top
			float32_t m_near, m_far;          // Near , Far
		};
		float32_t m_frustrum[6];        // All Together
	};

	float32_t            m_fov;
	ror::Planef *        m_planes;        // Frustrum Planes
	ror::BoundingCone *  m_frustrum_bounding_cone;
	ror::BoundingSphere *m_frustrum_bounding_sphere;
};
}        // namespace ror

#include "rorfrustrum.hh"
