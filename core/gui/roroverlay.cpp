// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rorlight.hpp"
#include "gui/roranchor.hpp"
#include "gui/rorimgui_drawlist.hpp"
#include "math/roraxis_angle.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "math/rorvector_functions.hpp"
#include "roroverlay.hpp"
#include "rorroar_font.hpp"
#include "settings/rorsettings.hpp"
#include <cmath>
#include <cstdint>
#include <functional>
#include <ostream>
#include <vector>

namespace ror
{

Overlay::Overlay(const ror::Light &, OverlaySource a_source, uint32_t a_source_index) :
    m_type(OverlayType::light), m_source(a_source), m_source_index(a_source_index)
{
	this->add_light_anchors(this->light());
}

Overlay::Overlay(const ror::OrbitCamera &, OverlaySource a_source, uint32_t a_source_index) :
    m_type(OverlayType::camera), m_source(a_source), m_source_index(a_source_index)
{
	this->add_camera_anchors(this->camera());
}

auto get_perpendicular(const ror::Vector3f &a_normal)
{
	auto perpendicular = a_normal.cross_product(ror::yaxis3f);

	if (perpendicular == zero_vector3f)        // float compare is ok here, the operator is doing decimal_equal inside
	{
		perpendicular = a_normal.cross_product(ror::xaxis3f);

		if (perpendicular == zero_vector3f)        // float compare is ok here, the operator is doing decimal_equal inside
			perpendicular = a_normal.cross_product(ror::zaxis3f);
	}

	assert(perpendicular != zero_vector3f && "Can't find a perpendicular axis");

	perpendicular.normalize();

	return perpendicular;
}

ror::Vector4f point_light_anchor1(const ror::Light &a_light)
{
	return ror::Vector4f{a_light.m_position.x + a_light.m_range, a_light.m_position.y, a_light.m_position.z, 1.0f};
}

ror::Vector4f spot_light_anchor1(const ror::Light &a_light)
{
	auto normal = a_light.m_direction.normalized();
	return ror::Vector4f{a_light.m_position + (normal * a_light.m_range)};
}

ror::Vector4f directional_light_anchor1(const ror::Light &a_light)
{
	auto normal = a_light.m_direction.normalized();
	return ror::Vector4f{a_light.m_position + (normal * 60.0f)};
}

ror::Vector4f area_light_anchor1(const ror::Light &a_light)
{
	return ror::Vector4f{a_light.m_position.x + a_light.m_range, a_light.m_position.y, a_light.m_position.z, 1.0f};
}

ror::Vector4f spot_light_radius(const ror::Light &a_light, float32_t a_angle)
{
	auto radius = a_light.m_range * std::tan(a_angle);        // angle is already in radians
	auto normal = a_light.m_direction.normalized();
	auto center = a_light.m_position + (normal * a_light.m_range);

	auto perpendicular = get_perpendicular(normal);
	auto hand          = perpendicular * radius;

	return ror::Vector4f{center + hand};
}

ror::Vector4f spot_light_anchor2(const ror::Light &a_light)
{
	return spot_light_radius(a_light, a_light.m_outer_angle);
}

ror::Vector4f spot_light_anchor3(const ror::Light &a_light)
{
	return spot_light_radius(a_light, a_light.m_inner_angle);
}

void get_light_anchor_positions(const ror::Light &a_light, ror::Vector4f &anchor1, ror::Vector4f &anchor2, ror::Vector4f &anchor3)
{
	// clang-format off
	switch (a_light.m_type)
	{
		case Light::LightType::point:         anchor1 = point_light_anchor1(a_light);        break;
		case Light::LightType::spot:          anchor1 = spot_light_anchor1(a_light);
			                                  anchor2 = spot_light_anchor2(a_light);
											  anchor3 = spot_light_anchor3(a_light);         break;
		case Light::LightType::directional:   anchor1 = directional_light_anchor1(a_light);  break;
		case Light::LightType::area:          anchor1 = area_light_anchor1(a_light);         break;
	}
	// clang-format on
}

auto get_light_icon(const ror::Light &a_light)
{
	// clang-format off
	switch (a_light.m_type)
	{
		case Light::LightType::point:         return ROAR_ICON_POINT_LIGHT;
		case Light::LightType::spot:          return ROAR_ICON_SPOT_LIGHT;
		case Light::LightType::directional:   return ROAR_ICON_DIRECTIONAL_LIGHT;
		case Light::LightType::area:          return ROAR_ICON_AREA_LIGHT;
	}
	// clang-format on
}

void Overlay::add_light_anchors(const ror::Light &a_light)
{
	auto          icon{get_light_icon(a_light)};
	ror::Vector4f anchor1{};
	ror::Vector4f anchor2{};
	ror::Vector4f anchor3{};

	get_light_anchor_positions(a_light, anchor1, anchor2, anchor3);

	// Anchor0 is always position of the light
	Anchors::Anchor position{ror::Vector4f{a_light.m_position}, 10.0f, 32, icon};        // 10 is for collision rect and 32 is icon size
	position.dorment(true);
	this->m_anchors.push_anchor(position);

	// Anchor1 is scale for point light and direction for spot and directional light, maybe area as well
	Anchors::Anchor scale_or_direction{anchor1, 3.0f};
	this->m_anchors.push_anchor(scale_or_direction);

	if (a_light.m_type == Light::LightType::spot)
	{
		// Anchor2 is scale for point light on the other side and outter angle for spot light
		Anchors::Anchor outer_angle{anchor2, 3.0f};
		this->m_anchors.push_anchor(outer_angle);

		// Anchor4 inner angle for spot light
		Anchors::Anchor inner_angle{anchor3, 3.0f};
		this->m_anchors.push_anchor(inner_angle);
	}
}

void Overlay::add_camera_anchors(const ror::OrbitCamera &a_camera)
{
	(void) a_camera;
}

void Overlay::update_other_anchors(const ror::Light &a_light)
{
	ror::Vector4f anchor1_position{};
	ror::Vector4f anchor2_position{};
	ror::Vector4f anchor3_position{};

	get_light_anchor_positions(a_light, anchor1_position, anchor2_position, anchor3_position);

	auto &anchor1 = this->m_anchors.anchor(1);
	anchor1.center(anchor1_position);
	anchor1.new_center(anchor1_position);

	if (a_light.m_type == Light::LightType::spot)
	{
		auto &anchor2 = this->m_anchors.anchor(2);
		anchor2.center(anchor2_position);
		anchor2.new_center(anchor2_position);

		auto &anchor3 = this->m_anchors.anchor(3);
		anchor3.center(anchor3_position);
		anchor3.new_center(anchor3_position);
	}
}

void Overlay::update_light(bool a_left_released)
{
	auto &light = this->light();
	if (this->m_anchors.moving(0))        // Position of the light
	{
		light.m_position = this->m_anchors.anchor(0).position();
		this->update_other_anchors(light);
		light.m_dirty = true;
	}
	else if (this->m_anchors.moving(1))        // Scale for the point light, direction for spot and directional
	{
		auto  range_target = this->m_anchors.anchor(1).position();
		auto &p1           = light.m_position;
		light.m_range      = distance(p1, range_target);
		light.m_direction  = range_target - p1;        // Only valid for Directional and Spot lights

		ror::Vector4f scale_or_direction{};

		if (a_left_released)
		{
			// clang-format off
			switch (light.m_type)
			{
				case Light::LightType::point:       scale_or_direction = point_light_anchor1(light);       break;
				case Light::LightType::spot:        scale_or_direction = spot_light_anchor1(light);        break;
				case Light::LightType::directional: scale_or_direction = directional_light_anchor1(light); break;
				case Light::LightType::area:        scale_or_direction = area_light_anchor1(light);        break;
			}
			// clang-format on

			this->m_anchors.anchor(1).position(scale_or_direction);
		}
		this->update_other_anchors(light);
		light.m_dirty = true;
	}
	else if (light.m_type == Light::LightType::spot)
	{
		if (this->m_anchors.moving(2))        // Outer angle for spot light
		{
			auto  range_target  = this->m_anchors.anchor(2).position();
			auto &p1            = light.m_position;
			light.m_outer_angle = angle(range_target - p1, light.m_direction);

			if (a_left_released)
				this->m_anchors.anchor(2).position(spot_light_anchor2(light));
		}
		else if (this->m_anchors.moving(3))        // Inner angle for spot light
		{
			auto  range_target  = this->m_anchors.anchor(3).position();
			auto &p1            = light.m_position;
			light.m_inner_angle = angle(range_target - p1, light.m_direction);

			if (a_left_released)
				this->m_anchors.anchor(3).position(spot_light_anchor3(light));
		}
		light.m_dirty = true;
	}
}

void Overlay::update_camera(bool a_left_released)
{
	(void) a_left_released;
}

void Overlay::update(bool a_left_released)
{
	switch (this->m_type)
	{
		case OverlayType::camera:
			this->update_camera(a_left_released);
			break;
		case OverlayType::light:
			this->update_light(a_left_released);
			break;
	}
}

void Overlay::draw(ImDrawList *a_drawlist, ImFont *a_icon_font, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	// a_drawlist->AddText(a_icon_font, 32.0f, ImVec2{}, green, ROAR_ICON_CAMERA);

	// float32_t screen_size = ror::settings().m_gui.m_overlay_scale;
	// auto      scale       = screen_to_world_scale(screen_size, this->m_anchors.anchor(this->m_center).center(), a_view_projection, a_viewport);

	ImGuiIO &io = ImGui::GetIO();

	auto          mpos  = ImGui::GetMousePos();
	auto          mcpos = io.MouseClickedPos[0];        // Left click only
	ror::Vector2f mouse_position{mpos.x, mpos.y};
	ror::Vector2f left_mouse_position{mcpos.x, mcpos.y};
	bool          left_clicked{ImGui::IsMouseClicked(0)};
	bool          left_released{ImGui::IsMouseReleased(0)};

	this->update(left_released);

	switch (this->m_type)
	{
		case OverlayType::camera:
			this->create_camera(a_drawlist, a_icon_font, a_view_projection, a_viewport);
			break;
		case OverlayType::light:
			this->create_light(a_drawlist, a_icon_font, a_view_projection, a_viewport);
			break;
	}

	this->m_anchors.new_frame(left_clicked, left_released, mouse_position, left_mouse_position);
	this->m_anchors.draw(a_drawlist, a_view_projection, a_viewport, left_clicked);
}

void draw_circle(ImDrawList *a_drawlist, ror::Vector3f a_center, float32_t a_radius, ror::Vector3f a_normal, uint32_t a_color, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	a_normal.normalize();

	auto perpendicular = get_perpendicular(a_normal);
	perpendicular *= a_radius;

	const uint32_t  segments{30};
	const float32_t pi_seg{ror_pi * 2.0f / segments};

	a_drawlist->PathClear();
	for (uint32_t i = 0; i <= segments; ++i)
	{
		ror::AxisAnglef axis_angle{ror::Vector3f{a_normal}, static_cast<float32_t>(i) * pi_seg};
		auto            result{false};
		auto            rotation = ror::matrix4_rotation(axis_angle);
		auto            point    = a_center + (rotation * perpendicular);

		auto p = ror::project_to_screen(ror::Vector4f{point}, a_view_projection, a_viewport, result);
		if (result)
			a_drawlist->PathLineTo(ImVec2{p.x, p.y});
	}

	a_drawlist->PathStroke(a_color, ImDrawFlags_None, 1.0f);
}

void draw_point_light_bounds(ImDrawList *a_drawlist, const ror::Light &a_light, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	const ImU32 color = ImGui::ColorConvertFloat4ToU32({a_light.m_color.x, a_light.m_color.y, a_light.m_color.z, 1.0f});

	draw_circle(a_drawlist, a_light.m_position, a_light.m_range, xaxis3f, color, a_view_projection, a_viewport);
	draw_circle(a_drawlist, a_light.m_position, a_light.m_range, yaxis3f, color, a_view_projection, a_viewport);
	draw_circle(a_drawlist, a_light.m_position, a_light.m_range, zaxis3f, color, a_view_projection, a_viewport);
	draw_circle(a_drawlist, a_light.m_position, a_light.m_range, ror::Vector3f{1.0f, 1.0f, 0.0f}, color, a_view_projection, a_viewport);
	draw_circle(a_drawlist, a_light.m_position, a_light.m_range, ror::Vector3f{1.0f, -1.0f, 0.0f}, color, a_view_projection, a_viewport);
	draw_circle(a_drawlist, a_light.m_position, a_light.m_range, ror::Vector3f{0.0f, 1.0f, 1.0f}, color, a_view_projection, a_viewport);
	draw_circle(a_drawlist, a_light.m_position, a_light.m_range, ror::Vector3f{0.0f, 1.0f, -1.0f}, color, a_view_projection, a_viewport);
}

void draw_spot_light_bounds(ImDrawList *a_drawlist, const ror::Light &a_light, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	const ImU32 color = ImGui::ColorConvertFloat4ToU32({a_light.m_color.x, a_light.m_color.y, a_light.m_color.z, 1.0f});

	auto outer_radius = a_light.m_range * std::tan(a_light.m_outer_angle);        // angle is already in radians
	auto inner_radius = a_light.m_range * std::tan(a_light.m_inner_angle);        // angle is already in radians
	auto normal       = a_light.m_direction.normalized();
	auto center       = a_light.m_position + (normal * a_light.m_range);

	draw_circle(a_drawlist, center, outer_radius, normal, color, a_view_projection, a_viewport);
	draw_circle(a_drawlist, center, inner_radius, normal, color, a_view_projection, a_viewport);

	auto perpendicular = get_perpendicular(normal);
	auto hand1         = perpendicular * outer_radius;
	auto hand2         = perpendicular * inner_radius;

	uint32_t        segments{20};
	const float32_t pi_seg{ror_pi * 2.0f / static_cast<float32_t>(segments)};
	auto            result{false};
	auto            projected_center = ror::project_to_screen(ror::Vector4f{a_light.m_position}, a_view_projection, a_viewport, result);

	if (result)
	{
		for (uint32_t i = 0; i <= segments; ++i)
		{
			const ror::AxisAnglef axis_angle{ror::Vector3f{normal}, static_cast<float32_t>(i) * pi_seg};
			const auto            rotation = ror::matrix4_rotation(axis_angle);
			const auto            point1   = center + rotation * hand1;
			const auto            point2   = center + rotation * hand2;
			{
				auto p = ror::project_to_screen(ror::Vector4f{point1}, a_view_projection, a_viewport, result);
				if (result)
					a_drawlist->AddLine(ImVec2{p.x, p.y}, ImVec2{projected_center.x, projected_center.y}, color);
			}
			{
				auto p = ror::project_to_screen(ror::Vector4f{point2}, a_view_projection, a_viewport, result);
				if (result)
					a_drawlist->AddLine(ImVec2{p.x, p.y}, ImVec2{projected_center.x, projected_center.y}, color);
			}
		}
	}
}

void draw_directional_light_bounds(ImDrawList *a_drawlist, const ror::Light &a_light, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	const ImU32 color = ImGui::ColorConvertFloat4ToU32({a_light.m_color.x, a_light.m_color.y, a_light.m_color.z, 1.0f});

	auto size{20.0f};
	auto normal = a_light.m_direction.normalized();
	auto target = a_light.m_position + (normal * size * 3.0f);

	draw_circle(a_drawlist, a_light.m_position, size, normal, color, a_view_projection, a_viewport);

	auto perpendicular = get_perpendicular(normal);
	auto hand1         = perpendicular * size;

	uint32_t        segments{10};
	const float32_t pi_seg{ror_pi * 2.0f / static_cast<float32_t>(segments)};
	auto            result1{true};
	auto            result2{false};

	for (uint32_t i = 0; i <= segments; ++i)
	{
		ror::AxisAnglef axis_angle{ror::Vector3f{normal}, static_cast<float32_t>(i) * pi_seg};
		auto            rotation = ror::matrix4_rotation(axis_angle);
		auto            point1   = a_light.m_position + (rotation * hand1);
		auto            point2   = target + (rotation * hand1);

		auto p1 = ror::project_to_screen(ror::Vector4f{point1}, a_view_projection, a_viewport, result1);
		auto p2 = ror::project_to_screen(ror::Vector4f{point2}, a_view_projection, a_viewport, result2);

		if (result1 && result2)
			a_drawlist->AddLine(ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, color);
	}
}

void draw_area_light_bounds(ImDrawList *, const ror::Light &, const ror::Matrix4f &, const ror::Vector4f &)
{}

void Overlay::create_light(ImDrawList *a_drawlist, ImFont *a_icon_font, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	auto &light = this->light();
	auto  icon{get_light_icon(light)};

	auto        result{false};
	auto        light_pos = ror::project_to_screen(ror::Vector4f{light.m_position}, a_view_projection, a_viewport, result);
	const ImU32 color     = ImGui::ColorConvertFloat4ToU32({light.m_color.x, light.m_color.y, light.m_color.z, 1.0f});

	if (result)
	{
		auto icon_size{32.0f};
		a_drawlist->AddText(a_icon_font, icon_size, ImVec2{light_pos.x - icon_size * 0.5f, light_pos.y - icon_size * 0.5f}, color, icon);

		if (light.m_type == Light::LightType::point)
			draw_point_light_bounds(a_drawlist, light, a_view_projection, a_viewport);
		else if (light.m_type == Light::LightType::spot)
			draw_spot_light_bounds(a_drawlist, light, a_view_projection, a_viewport);
		else if (light.m_type == Light::LightType::directional)
			draw_directional_light_bounds(a_drawlist, light, a_view_projection, a_viewport);
		else if (light.m_type == Light::LightType::area)
			draw_area_light_bounds(a_drawlist, light, a_view_projection, a_viewport);
	}
}

void Overlay::create_camera(ImDrawList *a_drawlist, ImFont *a_icon_font, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	auto &camera = this->camera();

	auto result{false};
	auto camera_pos = ror::project_to_screen(ror::Vector4f{camera.eye()}, a_view_projection, a_viewport, result);

	if (result)
		a_drawlist->AddText(a_icon_font, 32.0f, ImVec2{camera_pos.x, camera_pos.y}, imgui_white, ROAR_ICON_CAMERA);

	// TODO: Add frustum shape drawing
}

void Overlays::init(ImFont *a_icon_font)
{
	this->m_roar_icon_font = a_icon_font;
}

void Overlays::draw(const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	this->m_draw_list = imgui_draw_list();

	// TODO: Move these inverses out into camera, also one in anchors
	auto view_projection_inverse = a_view_projection;
	auto res                     = view_projection_inverse.invert();
	assert(res);
	(void) res;

	for (auto &overlay : this->m_overlays)
		overlay.draw(this->m_draw_list, this->m_roar_icon_font, a_view_projection, a_viewport);
}

void Overlays::clear()
{
	this->m_overlays.clear();
}
}        // namespace ror
