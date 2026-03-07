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
#include "graphics/rorlight.hpp"

namespace ror
{
template <uint32_t _cascade_count>
class ROAR_ENGINE_ITEM ShadowData final
{
  public:
	FORCE_INLINE             ShadowData()                              = default;        //! Default constructor
	FORCE_INLINE             ShadowData(const ShadowData &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             ShadowData(ShadowData &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShadowData &operator=(const ShadowData &a_other)      = default;        //! Copy assignment operator
	FORCE_INLINE ShadowData &operator=(ShadowData &&a_other) noexcept  = default;        //! Move assignment operator
	FORCE_INLINE ~ShadowData() noexcept                                = default;        //! Destructor

	// clang-format off
	FORCE_INLINE constexpr auto &eyes()           const { return this->m_eyes;           }
	FORCE_INLINE constexpr auto &viewport()       const { return this->m_viewport;       }
	FORCE_INLINE constexpr auto &views()          const { return this->m_views;          }
	FORCE_INLINE constexpr auto &projections()    const { return this->m_projections;    }
	// clang-format on

	void build(ror::Scene &a_scene, ror::EventSystem &a_event_system, ror::Renderer &a_renderer, ror::Light &a_light, const ror::OrbitCamera &a_camera, const ror::Vector2ui &a_dimensions);
	void update_per_view_uniform(ror::Renderer &a_renderer, uint32_t a_cascade_index);

	void fit_light_frustrum_cascades(const ror::Light &a_light, const ror::OrbitCamera &a_camera);

  protected:
  private:
	ror::Vector4ui m_viewport;                           //! Viewport for each cascade, as all have the same dimensions
	ror::Vector3f  m_eyes[_cascade_count];               //! Eye positions for each cascade
	ror::Matrix4f  m_views[_cascade_count];              //! Views matrices for each cascade
	ror::Matrix4f  m_projections[_cascade_count];        //! Projection matrices for each cascade
};

}        // namespace ror

#include "rorshadow_data.hh"
