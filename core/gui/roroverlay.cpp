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
#include "roroverlay.hpp"
#include "rorroar_font.hpp"
#include "settings/rorsettings.hpp"

namespace ror
{

Overlay::Overlay(const ror::Light &a_light, OverlaySource a_source, uint32_t a_source_index) :
    m_source(a_source), m_source_index(a_source_index)
{
	// clang-format off
	switch (a_light.m_type)
	{
		case Light::LightType::point:          this->m_type = OverlayType::point_light;        break;
		case Light::LightType::spot:           this->m_type = OverlayType::spot_light;         break;
		case Light::LightType::directional:    this->m_type = OverlayType::directional_light;  break;
		case Light::LightType::area:           this->m_type = OverlayType::area_light;         break;
	}
	// clang-format on
	this->add_light_anchors(this->light());
}

Overlay::Overlay(const ror::OrbitCamera &, OverlaySource a_source, uint32_t a_source_index) :
    m_type(OverlayType::camera), m_source(a_source), m_source_index(a_source_index)
{
	this->add_camera_anchors(this->camera());
}

void Overlay::add_light_anchors(const ror::Light &a_light)
{
	(void) a_light;
	Anchors::Anchor position{ror::Vector4f{a_light.m_position}, 5.0f};
	this->m_anchors.push_anchor(position);
}

void Overlay::add_camera_anchors(const ror::OrbitCamera &a_camera)
{
	(void) a_camera;
}

void Overlay::update_light(bool a_left_released)
{
	auto &light = this->light();
	if (this->m_anchors.moving(0))
	{
		auto new_pos = this->m_anchors.anchor(0).new_center();

		light.m_position.x = new_pos.x;
		light.m_position.y = new_pos.y;
		light.m_position.z = new_pos.z;
	}
	else
	{
		auto pos           = this->m_anchors.anchor(0).center();
		light.m_position.x = pos.x;
		light.m_position.y = pos.y;
		light.m_position.z = pos.z;
	}
	if (a_left_released)
	{}

	// clang-format off
	// switch (light.m_type)
	// {
	// 	case Light::LightType::point:         icon = ROAR_ICON_POINT_LIGHT;       break;
	// 	case Light::LightType::spot:          icon = ROAR_ICON_SPOT_LIGHT;        break;
	// 	case Light::LightType::directional:   icon = ROAR_ICON_DIRECTIONAL_LIGHT; break;
	// 	case Light::LightType::area:          icon = ROAR_ICON_AREA_LIGHT;        break;
	// }
	// clang-format on
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
		case OverlayType::point_light:
		case OverlayType::directional_light:
		case OverlayType::spot_light:
		case OverlayType::area_light:
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

	this->m_anchors.new_frame(left_clicked, left_released, mouse_position, left_mouse_position);
	this->m_anchors.draw(a_drawlist, a_view_projection, a_viewport, left_clicked);

	this->update(left_released);

	switch (this->m_type)
	{
		case OverlayType::camera:
			this->create_camera(a_drawlist, a_icon_font, a_view_projection, a_viewport);
			break;
		case OverlayType::point_light:
		case OverlayType::directional_light:
		case OverlayType::spot_light:
		case OverlayType::area_light:
			this->create_light(a_drawlist, a_icon_font, a_view_projection, a_viewport);
			break;
	}
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
		ror::AxisAnglef axis_angle{ror::Vector3f{a_normal}, i * pi_seg};
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
	const float32_t pi_seg{ror_pi * 2.0f / segments};
	auto            result{false};
	auto            projected_center = ror::project_to_screen(ror::Vector4f{a_light.m_position}, a_view_projection, a_viewport, result);

	if (result)
	{
		for (uint32_t i = 0; i <= segments; ++i)
		{
			const ror::AxisAnglef axis_angle{ror::Vector3f{normal}, i * pi_seg};
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
	const float32_t pi_seg{ror_pi * 2.0f / segments};
	auto            result1{true};
	auto            result2{false};

	for (uint32_t i = 0; i <= segments; ++i)
	{
		ror::AxisAnglef axis_angle{ror::Vector3f{normal}, i * pi_seg};
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
	auto  icon{""};

	// clang-format off
	switch (light.m_type)
	{
		case Light::LightType::point:         icon = ROAR_ICON_POINT_LIGHT;       break;
		case Light::LightType::spot:          icon = ROAR_ICON_SPOT_LIGHT;        break;
		case Light::LightType::directional:   icon = ROAR_ICON_DIRECTIONAL_LIGHT; break;
		case Light::LightType::area:          icon = ROAR_ICON_AREA_LIGHT;        break;
	}
	// clang-format on
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
}        // namespace ror
