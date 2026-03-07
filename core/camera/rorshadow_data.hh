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

namespace ror
{

void fit_light_frustrum(ror::Scene &a_scene, const ror::Light &a_light, ror::Matrix4f &a_light_view_fit, ror::Matrix4f &a_projection_fit, ror::Vector3f &a_eye);
void fit_light_frustrum_cascades(ror::Scene &a_scene, const ror::Light &a_light, ror::Matrix4f &a_light_view_fit, ror::Matrix4f &a_projection_fit, ror::Vector3f &a_eye);
void render_scene_into_shadowmap(ror::Scene &a_scene, ror::EventSystem &a_event_system, ror::Renderer &a_renderer, const ror::Vector4ui &a_viewport, const ror::Vector3f &a_eye, const ror::Matrix4f &a_view, const ror::Matrix4f &a_projection);
void shadow_volume_debug_geometry(ror::Scene &a_scene, const std::array<ror::Frustum, ror::cascade_count> &a_frustums);

template <uint32_t _size>
FORCE_INLINE void ShadowData<_size>::build(ror::Scene &a_scene, ror::EventSystem &a_event_system, ror::Renderer &a_renderer, ror::Light &a_light, const ror::OrbitCamera &a_camera, const ror::Vector2ui &a_dimensions)
{
	(void) a_event_system;
	(void) a_scene;
	(void) a_renderer;

	this->fit_light_frustrum_cascades(a_scene, a_light, a_camera);

	this->m_viewport = {0, 0, a_dimensions.x, a_dimensions.y};

	// render_scene_into_shadowmap(a_scene, a_event_system, a_renderer, light_shadow_viewport, eye, view_fit, projection_fit);
}

template <uint32_t _size>
FORCE_INLINE void ShadowData<_size>::update_per_view_uniform(ror::Renderer &a_renderer, uint32_t a_cascade_index)
{
	// These what we need to prepare for rendering
	ror::Matrix4f projection_fit = this->m_projections[a_cascade_index];
	ror::Matrix4f view_fit       = this->m_views[a_cascade_index];
	ror::Vector3f eye            = this->m_eyes[a_cascade_index];

	a_renderer.update_per_view_uniform(view_fit, projection_fit, this->m_viewport, eye);
}

// NOTE: Projection deterimines the size of the box you see, while viewing determines where the box is located.
// In orthographics projection the viewing eye position really doesn't matter as long as the box covers the sceen
// The center of the viewing also doesn't matter as long as its not behind the eye, in which case you will look backwards
template <uint32_t _size>
void ShadowData<_size>::fit_light_frustrum_cascades(ror::Scene &a_scene, const ror::Light &a_light, const ror::OrbitCamera &a_camera)
{
	auto      &frustums        = a_camera.frustums();        // which has the following
	const auto light_direction = a_light.direction();

	for (uint32_t cascade = 0; cascade < ror::cascade_count; ++cascade)
	{
		auto &frustum         = frustums[cascade];
		auto &bounding_sphere = frustum.sphere();        // NOTE: We can't just get Bounding box of the Frustum as thats in world space and we need this in light space.
		                                                 // But we can build a canonical bounding space from sphere as thats in any space already, also big enough to overlap cascades
		                                                 // TODO: Check if this is correct or we are building very big sphere and box
		auto radius = bounding_sphere.radius();
		auto target = bounding_sphere.center();

		this->m_eyes[cascade]        = target - (light_direction * radius * 2.0f);        // Radius multiplied by 2.0f as no matter how far your eye is it doesn't matter for rendering but you will capture objects behind
		this->m_views[cascade]       = ror::make_look_at(this->m_eyes[cascade], target, ror::yaxis3f, ror::yaxis3f, ror::xaxis3f);
		this->m_projections[cascade] = ror::make_ortho(-radius, radius, -radius, radius, 0.0f, radius * 2.0f);
	}

	// If want to see the shadow volume geometry, uncomment below and provide event_system
	// shadow_volume_debug_geometry(a_scene, a_event_system, a_light_view_fit, a_projection_fit, a_eye, target, frustum_min_ls, frustum_max_ls);
	static bool once = true;
	if (once)
	{
		std::array<ror::Frustum, ror::cascade_count> frustum_array{};
		for (size_t i = 0; i < ror::cascade_count; ++i)
			frustum_array[i] = frustums[i];

		shadow_volume_debug_geometry(a_scene, frustum_array);
		once = false;
	}
}

}        // namespace ror
