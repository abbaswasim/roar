// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2026
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

#include "camera/rorcamera.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "graphics/rorlight.hpp"

namespace ror
{
template <uint32_t _size>
class ROAR_ENGINE_ITEM ShadowMap final
{
  public:
	FORCE_INLINE            ShadowMap()                             = default;        //! Default constructor
	FORCE_INLINE            ShadowMap(const ShadowMap &a_other)     = default;        //! Copy constructor
	FORCE_INLINE            ShadowMap(ShadowMap &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShadowMap &operator=(const ShadowMap &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE ShadowMap &operator=(ShadowMap &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~ShadowMap() noexcept                              = default;        //! Destructor

	// clang-format off
	FORCE_INLINE constexpr auto &eyes()           const { return this->m_eyes;           }
	FORCE_INLINE constexpr auto &viewports()      const { return this->m_viewports;      }
	FORCE_INLINE constexpr auto &views()          const { return this->m_views;          }
	FORCE_INLINE constexpr auto &projections()    const { return this->m_projections;    }
	// clang-format on

	void build(const ror::Light &a_light, const ror::OrbitCamera &a_camera, const ror::Vector2ui &a_dimensions);

  protected:
  private:
	ror::Vector3f  m_eyes[_size];
	ror::Vector4ui m_viewports[_size];
	ror::Matrix4f  m_views[_size];
	ror::Matrix4f  m_projections[_size];
};

}        // namespace ror

#include "rorshadow_map.hh"
