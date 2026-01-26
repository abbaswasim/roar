// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2024
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

#include "bounds/rorbounding.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "math/rormatrix4.hpp"
#include "math/rorplane.hpp"
#include "math/rorvector3.hpp"

namespace ror
{
class ROAR_ENGINE_ITEM Frustum final
{
  public:
	FORCE_INLINE          Frustum()                             = default;        //! Default constructor
	FORCE_INLINE          Frustum(const Frustum &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          Frustum(Frustum &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE Frustum &operator=(const Frustum &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Frustum &operator=(Frustum &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Frustum() noexcept                            = default;        //! Destructor

	void setup(const ror::Matrix4f &a_view);

	// clang-format off
	FORCE_INLINE constexpr auto &corners() const { return this->m_corners;       }
	FORCE_INLINE constexpr auto &center()  const { return this->m_center;        }
	FORCE_INLINE constexpr auto fov()      const { return this->m_fov;           }
	FORCE_INLINE constexpr auto far()      const { return this->m_far;           }
	FORCE_INLINE constexpr auto near()     const { return this->m_near;          }
	FORCE_INLINE constexpr auto aspect()   const { return this->m_aspect;        }
	FORCE_INLINE constexpr auto &box()     const { return this->m_bounding_box;  }

	FORCE_INLINE void fov(float32_t a_fov)        { this->m_fov = a_fov;       }
	FORCE_INLINE void far(float32_t a_far)        { this->m_far = a_far;       }
	FORCE_INLINE void near(float32_t a_near)      { this->m_near = a_near;     }
	FORCE_INLINE void aspect(float32_t a_aspect)  { this->m_aspect = a_aspect; }
	// clang-format on

  protected:
	float32_t            m_near{0.01f};              //! Near of the projection
	float32_t            m_far{1000.0f};             //! Far of the projection
	float32_t            m_fov{60.0f};               //! FOV of the frustrum, copies of when a camera setup it up
	float32_t            m_aspect{1.0f};             //! Aspect of the frustrum, copies of when a camera setup it up
	ror::Vector3f        m_corners[8];               //! The 8 corners of the frustum
	ror::Vector3f        m_center;                   //! The center of the frustum
	ror::Matrix4f        m_view{};                   //! View matrix of the frustum
	ror::Matrix4f        m_view_inverse{};           //! Inverse of the view matrix
	ror::Planef          m_planes[6];                // The planes that bounds a frustum
	ror::BoundingSpheref m_bounding_sphere{};        //! Bounding sphere of the frustum
	ror::BoundingBoxf    m_bounding_box{};           //! Bounding box of the frustum

  protected:
  private:
};

}        // namespace ror

// #include "rorfrustum.hh"
