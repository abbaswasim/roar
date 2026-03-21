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

#include "camera/rorcamera.hpp"
#include "rorshadow_data.hpp"

#include <cassert>

namespace ror
{

template <uint32_t _size>
FORCE_INLINE void ShadowData<_size>::build(ror::Light &a_light, const ror::OrbitCamera &a_camera, const ror::Vector2ui &a_dimensions)
{
	this->fit_light_frustrum_cascades(a_light, a_camera);

	this->m_viewport = {0, 0, a_dimensions.x, a_dimensions.y};
}

// NOTE: Projection deterimines the size of the box you see, while viewing determines where the box is located.
// In orthographics projection the viewing eye position really doesn't matter as long as the box covers the sceen
// The center of the viewing also doesn't matter as long as its not behind the eye, in which case you will look backwards
template <uint32_t _size>
void ShadowData<_size>::fit_light_frustrum_cascades(const ror::Light &a_light, const ror::OrbitCamera &a_camera)
{
	const auto &frustums        = a_camera.frustums();
	const auto  light_direction = a_light.direction();

	for (uint32_t cascade = 0; cascade < ror::cascade_count; ++cascade)
	{
		const auto &frustum         = frustums[cascade];
		const auto &bounding_sphere = frustum.sphere();        // NOTE: We can't just get Bounding box of the Frustum as thats in world space and we need this in light space.
		                                                       // But we can build a canonical bounding space from sphere as thats in any space already, also big enough to overlap cascades
		const auto  radius = bounding_sphere.radius();         // Radius[cascade] has no space, no worries
		const auto &target = bounding_sphere.center();         // This is needed in world space and it already is, no xform needed

		this->m_eyes[cascade]        = target - (light_direction * radius);
		this->m_views[cascade]       = ror::make_look_at(this->m_eyes[cascade], target, ror::yaxis3f, ror::yaxis3f, ror::xaxis3f);
		this->m_projections[cascade] = ror::make_ortho(-radius, radius, -radius, radius, 0.0f, radius * 2.0f);
	}
}

}        // namespace ror
