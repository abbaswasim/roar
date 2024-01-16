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

	void setup(const ror::Matrix4f &a_view_projection);

	// clang-format off
	FORCE_INLINE constexpr auto &corners() const { return this->m_corners; }
	FORCE_INLINE constexpr auto &center()  const { return this->m_center; }
	// clang-format on

  protected:
	ror::Vector3f        m_corners[8];                       //! The 8 corners of the frustum
	ror::Vector3f        m_center;                           //! The center of the frustum
	ror::Matrix4f        m_view_projection{};                //! View projection matrix of the frustum
	ror::Matrix4f        m_view_projection_inverse{};        //! Inverse of the view projection
	ror::Planef          m_planes[6];                        // The planes that bounds a frustum
	ror::BoundingSpheref m_bounding_sphere{};                //! Bounding sphere of the frustum

  protected:
  private:
};

}        // namespace ror

// #include "rorfrustum.hh"
