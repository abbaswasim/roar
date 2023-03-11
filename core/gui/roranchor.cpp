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

#include "foundation/rormacros.hpp"
#include "roranchor.hpp"
#include "settings/rorsettings.hpp"

namespace ror
{

FORCE_INLINE bool inside_rectangle(const ror::Vector2f &a_point, const ror::Vector2f &a_min, const ror::Vector2f &a_max)
{
	return a_point.x >= a_min.x && a_point.y >= a_min.y && a_point.x < a_max.x && a_point.y < a_max.y;
}

// ImGui version of triangle checks, modified for roar vectors
FORCE_INLINE bool inside_triangle(const ror::Vector4f &a, const ror::Vector4f &b, const ror::Vector4f &c, const ror::Vector2f &p)
{
	bool b1 = ((p.x - b.x) * (a.y - b.y) - (p.y - b.y) * (a.x - b.x)) < 0.0f;
	bool b2 = ((p.x - c.x) * (b.y - c.y) - (p.y - c.y) * (b.x - c.x)) < 0.0f;
	bool b3 = ((p.x - a.x) * (c.y - a.y) - (p.y - a.y) * (c.x - a.x)) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

// ImGui version of Quadratic Bezier in fill mode
FORCE_INLINE void AddBezierQuadraticFilled(ImDrawList *a_drawlist, const ImVec2 &p1, const ImVec2 &p2, const ImVec2 &p3, ImU32 col, int num_segments = 0)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	a_drawlist->PathLineTo(p1);
	a_drawlist->PathBezierQuadraticCurveTo(p2, p3, num_segments);
	a_drawlist->PathFillConvex(col);
}

Anchors::Anchor::Anchor(ror::Vector4f a_center, float32_t a_radius) :
    m_type(AnchorType::circle), m_center(a_center), m_new_center(a_center), m_radius(a_radius)

{}

Anchors::Anchor::Anchor(ror::Vector4f a_p1, ror::Vector4f a_p2, ror::Vector4f a_p3, AnchorType a_type) :
    m_type(a_type)
{
	this->m_points[0] = a_p1;
	this->m_points[1] = a_p2;
	this->m_points[2] = a_p3;

	this->m_center     = (a_p1 + a_p2 + a_p3) / 3.0f;
	this->m_center.w   = 1.0f;
	this->m_new_center = this->m_center;
}

Anchors::Anchor::Anchor(ror::Vector4f a_p1, ror::Vector4f a_p2, ror::Vector4f a_p3, ror::Vector4f a_p4, AnchorType a_type) :
    m_type(a_type)
{
	this->m_points[0] = a_p1;
	this->m_points[1] = a_p2;
	this->m_points[2] = a_p3;
	this->m_points[3] = a_p4;

	this->m_center     = (a_p1 + a_p3) / 2.0f;
	this->m_center.w   = 1.0f;
	this->m_new_center = this->m_center;
}

void Anchors::Anchor::draw(ImDrawList *a_drawlist, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport, uint32_t a_color, uint32_t a_click_color, uint32_t a_white, bool a_hovering)
{
	const auto  anchor_increment{2.0f};
	const auto  line_thickness{2.0f};
	const ImU32 orange_alpha = ImGui::ColorConvertFloat4ToU32({1.0f, 0.647f, 0.0f, 0.9f});

	auto center     = ror::project_to_screen(this->m_center, a_view_projection, a_viewport);
	auto new_center = ror::project_to_screen(this->m_new_center, a_view_projection, a_viewport);

	a_click_color = orange_alpha;

	if (this->m_clicked)
		a_drawlist->AddLine(ImVec2(new_center.x, new_center.y), ImVec2(center.x, center.y), a_white, line_thickness);

	if (this->m_type == AnchorType::circle)
	{
		if (this->m_clicked)
			a_drawlist->AddCircleFilled(ImVec2(center.x, center.y), this->m_radius + anchor_increment, a_color);

		if (this->m_clicked || a_hovering)
		{
			a_drawlist->AddCircleFilled(ImVec2(new_center.x, new_center.y), this->m_radius + anchor_increment, a_click_color);
			a_drawlist->AddCircle(ImVec2(new_center.x, new_center.y), this->m_radius + anchor_increment, a_white, 0, line_thickness);
		}
		else
			a_drawlist->AddCircleFilled(ImVec2(center.x, center.y), this->m_radius + anchor_increment, a_color);
	}
	else if (this->m_type == AnchorType::triangle)        // Simplification of bezier to a triangle
	{
		auto p0 = ror::project_to_screen(this->m_points[0], a_view_projection, a_viewport);
		auto p1 = ror::project_to_screen(this->m_points[1], a_view_projection, a_viewport);
		auto p2 = ror::project_to_screen(this->m_points[2], a_view_projection, a_viewport);

		if (this->m_clicked || a_hovering)

		{
			// a_drawlist->AddCircleFilled(ImVec2(center.x, center.y), this->m_radius + anchor_increment, a_color);

			a_drawlist->AddTriangleFilled(ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, a_click_color);
			a_drawlist->AddTriangle(ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, a_white);

			if (this->m_clicked && this->m_has_ribbon)
				this->make_ribbon(a_drawlist, new_center, center, line_thickness, orange_alpha, a_white);
		}
		else
		{
			a_drawlist->AddTriangleFilled(ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, a_color);
		}
	}
	else if (this->m_type == AnchorType::bezier)
	{
		auto p0 = ror::project_to_screen(this->m_points[0], a_view_projection, a_viewport);
		auto p1 = ror::project_to_screen(this->m_points[1], a_view_projection, a_viewport);
		auto p2 = ror::project_to_screen(this->m_points[2], a_view_projection, a_viewport);

		if (this->m_clicked || a_hovering)
		{
			// a_drawlist->AddCircleFilled(ImVec2(center.x, center.y), this->m_radius + anchor_increment, a_color);

			AddBezierQuadraticFilled(a_drawlist, ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, a_click_color);
			a_drawlist->AddBezierQuadratic(ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, a_white, line_thickness);
			a_drawlist->AddLine(ImVec2{p0.x, p0.y}, ImVec2{p2.x, p2.y}, a_white, line_thickness);

			if (this->m_clicked && this->m_has_ribbon)
				this->make_ribbon(a_drawlist, new_center, center, line_thickness, orange_alpha, a_white);
		}
		else
		{
			AddBezierQuadraticFilled(a_drawlist, ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, a_color);
		}
	}
	else if ((this->m_type == AnchorType::rectangle) || (this->m_type == AnchorType::pyramid))
	{
		auto p0 = ror::project_to_screen(this->m_points[0], a_view_projection, a_viewport);
		auto p1 = ror::project_to_screen(this->m_points[1], a_view_projection, a_viewport);
		auto p2 = ror::project_to_screen(this->m_points[2], a_view_projection, a_viewport);
		auto p3 = ror::project_to_screen(this->m_points[3], a_view_projection, a_viewport);

		if (this->m_clicked || a_hovering)
		{
			// a_drawlist->AddCircleFilled(ImVec2(center.x, center.y), this->m_radius + anchor_increment, a_color);

			a_drawlist->AddQuadFilled(ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, ImVec2{p3.x, p3.y}, orange_alpha);

			a_drawlist->AddLine(ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, a_white, line_thickness);
			a_drawlist->AddLine(ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, a_white, line_thickness);
			a_drawlist->AddLine(ImVec2{p2.x, p2.y}, ImVec2{p3.x, p3.y}, a_white, line_thickness);
			a_drawlist->AddLine(ImVec2{p3.x, p3.y}, ImVec2{p0.x, p0.y}, a_white, line_thickness);

			if (this->m_clicked && this->m_has_ribbon)
				this->make_ribbon(a_drawlist, new_center, center, line_thickness, orange_alpha, a_white, true);
		}
		else
		{
			a_drawlist->AddQuadFilled(ImVec2{p0.x, p0.y}, ImVec2{p1.x, p1.y}, ImVec2{p2.x, p2.y}, ImVec2{p3.x, p3.y}, a_color);
		}
	}
}

void Anchors::Anchor::make_ribbon(ImDrawList *a_drawlist, ror::Vector4f new_center, ror::Vector4f center, float line_thickness, uint32_t orange_alpha, uint32_t a_white, bool straight)
{
	const uint32_t values_count{25};
	const char    *angles[values_count]{"-180", "-165", "-150", "-135", "-120", "-105", "-90", "-75", "-60", "-45", "-30", "-15", "0", "15", "30", "45", "60", "75", "90", "105", "120", "135", "150", "165", "180"};
	const char    *scales[values_count]{"-10.0", "-8.0", "-4.0", "-2.0", "-1.0", "-0.8", "-0.4", "-0.2", "-0.1", "-0.05", "-0.02", "-0.01", "0.0", "0.01", "0.02", "0.05", "0.1", "0.2", "0.4", "0.8", "1.0", "2.0", "4.0", "8.0", "10.0"};

	const float angle_values[values_count]{-180.0f, -165.0f, -150.0f, -135.0f, -120.0f, -105.0f, -90.0f, -75.0f, -60.0f, -45.0f, -30.0f, -15.0f, 0.0f, 15.0f, 30.0f, 45.0f, 60.0f, 75.0f, 90.0f, 105.0f, 120.0f, 135.0f, 150.0f, 165.0f, 180.0f};
	const float scale_values[values_count]{-10.0f, -8.0f, -4.0f, -2.0f, -1.0f, -0.8f, -0.4f, -0.2f, -0.1f, -0.05f, -0.02f, -0.01f, 0.0f, 0.01f, 0.02f, 0.05f, 0.1f, 0.2f, 0.4f, 0.8f, 1.0f, 2.0f, 4.0f, 8.0f, 10.0f};

	int32_t next_index{values_count / 2};
	int32_t previous_index{next_index};

	const float32_t ribon_height{156.0f};        // half of what it should be
	const float32_t ribon_width{30.0f};
	const float32_t ribon_height_segments{6.0f / ribon_height};        // half of what it should be
	float32_t       bezier_offset{10.0f};
	float32_t       bulb_offset{ribon_width / 1.5f - 2.0f};
	float32_t       bulb_width{ribon_width - 2.0f};
	float32_t       text_offset{0.0f};
	const uint32_t  segments{26};
	const float32_t segment_space{ribon_height / segments};
	float32_t       line_thickness_copy{line_thickness};
	const auto      imgui_black = ImGui::ColorConvertFloat4ToU32({0, 0, 0, 1.0f});

	(void) straight;

	ImVec2 rp0{new_center.x, center.y - ribon_height};
	ImVec2 rp2{new_center.x, center.y + ribon_height};
	ImVec2 rp1{new_center.x, center.y};

	if (new_center.x < center.x)
	{
		if (!straight)
			rp1.x -= ribon_width;
		bezier_offset *= -1.0f;
		bulb_offset *= -1.0f;
		bulb_width *= -1.0f;
		line_thickness_copy *= -1.0f;
		text_offset = 15.0f;
	}
	else
	{
		if (!straight)
			rp1.x += ribon_width;

		text_offset = 5.0f;
	}

	a_drawlist->AddBezierQuadratic(rp0, rp1, rp2, orange_alpha, line_thickness * 20.0f);
	a_drawlist->AddBezierQuadratic(ImVec2{rp0.x - bezier_offset, rp0.y + 2.0f}, ImVec2{rp1.x - bezier_offset, rp1.y}, ImVec2{rp2.x - bezier_offset, rp2.y - 2.0f}, a_white, 1.0f);

	auto *values = straight ? scale_values : angle_values;
	auto *names  = straight ? scales : angles;

	for (uint32_t j = 1; j < segments; ++j)
	{
		float32_t t   = j * ribon_height_segments;
		float32_t oi  = 1.0f - t;
		float32_t is  = t * t;
		float32_t ois = 2.0f * oi * t;
		oi *= oi;
		float32_t x = oi * rp0.x + ois * rp1.x + is * rp2.x;
		float32_t y = oi * rp0.y + ois * rp1.y + is * rp2.y;

		float ri   = t * 2.0f - 1.0f;
		float tilt = straight ? 0.0f : ri * (ribon_height / 50.0f);

		auto color{a_white};
		auto d         = new_center.y - y;
		auto hit_point = std::abs(d);
		if (hit_point < segment_space)
		{
			color = imgui_black;
			if (d < 0)
			{
				previous_index = static_cast_safe<int32_t>(j) - 2;
				next_index     = static_cast_safe<int32_t>(j) - 1;
			}
			else
			{
				previous_index = static_cast_safe<int32_t>(j) - 1;
				next_index     = static_cast_safe<int32_t>(j);
			}

			auto min_value = values[std::max(0, previous_index)];
			auto max_value = values[std::min(static_cast<int32_t>(values_count) - 1, next_index)];

			this->m_ribbon_value = ror::interpolate(min_value, max_value, hit_point / segment_space);

			// Make the bubble
			AddBezierQuadraticFilled(a_drawlist, ImVec2{x + bulb_offset, y - 10.0f}, ImVec2{x + bulb_width, y}, ImVec2{x + bulb_offset, y + 10.0f}, orange_alpha);
			a_drawlist->AddLine(ImVec2{x - line_thickness_copy * 10.0f, y - 0.5f - tilt + d}, ImVec2{x - bezier_offset, y - 0.5f + d}, color);
		}

		a_drawlist->AddLine(ImVec2{x - line_thickness_copy * 10.0f, y - 0.5f - tilt}, ImVec2{x - bezier_offset, y - 0.5f}, color);
		a_drawlist->AddText(nullptr, color == imgui_black ? 13.0f : 12.0f, ImVec2{x - text_offset, y - 5.0f}, color, names[j - 1]);
	}
}

void Anchors::Anchor::recenter()
{
	if ((this->m_type == AnchorType::triangle) || (this->m_type == AnchorType::bezier))        // Simplification of bezier to a triangle
	{
		this->m_new_center   = (this->m_points[0] + this->m_points[1] + this->m_points[2]) / 3.0f;
		this->m_new_center.w = 1.0f;
		this->m_center       = this->m_new_center;
	}
	else if (this->m_type == AnchorType::rectangle)
	{
		this->m_new_center   = (this->m_points[0] + this->m_points[2]) / 2.0f;
		this->m_new_center.w = 1.0f;
		this->m_center       = this->m_new_center;
	}
	else if (this->m_type == AnchorType::pyramid)
	{
		auto c1   = (this->m_points[0] + this->m_points[2]) / 2.0f;
		auto c2   = (this->m_points[1] + this->m_points[3]) / 2.0f;
		this->m_new_center   = (c1 + c2) / 2.0f;
		this->m_new_center.w = 1.0f;
		this->m_center       = this->m_new_center;
	}
}

void Anchors::Anchor::move(ror::Vector4f &a_destination, const ror::Vector2f &a_delta, const ror::Matrix4f &a_view_projection, const ror::Matrix4f &a_view_projection_inverse, const ror::Vector4f &a_viewport)
{
	auto center = ror::project_to_screen(a_destination, a_view_projection, a_viewport);

	center.x -= a_delta.x;
	center.y -= a_delta.y;

	a_destination = ror::project_to_world(center, a_view_projection_inverse, a_viewport);
}

void Anchors::Anchor::move(const ror::Vector2f &a_delta, const ror::Matrix4f &a_view_projection, const ror::Matrix4f &a_view_projection_inverse, const ror::Vector4f &a_viewport)
{
	this->move(this->m_new_center, a_delta, a_view_projection, a_view_projection_inverse, a_viewport);

	// if (this->m_type == AnchorType::circle)
	// {
	// 	this->move(this->m_new_center, a_delta, a_view_projection, a_view_projection_inverse, a_viewport);
	// }
	// else if ((this->m_type == AnchorType::triangle) || (this->m_type == AnchorType::bezier))        // Simplification of bezier to a triangle
	// {
	// 	this->move(this->m_new_center, a_delta, a_view_projection, a_view_projection_inverse, a_viewport);
	// }
	// else if ((this->m_type == AnchorType::rectangle) || (this->m_type == AnchorType::pyramid))
	// {
	// 	// this->move(this->m_new_center, a_delta, a_view_projection, a_view_projection_inverse, a_viewport);
	// 	// this->move(this->m_points[0], a_delta, a_view_projection, a_view_projection_inverse, a_viewport);
	// 	// this->move(this->m_points[1], a_delta, a_view_projection, a_view_projection_inverse, a_viewport);
	// 	// this->move(this->m_points[2], a_delta, a_view_projection, a_view_projection_inverse, a_viewport);
	// 	// this->move(this->m_points[3], a_delta, a_view_projection, a_view_projection_inverse, a_viewport);

	// 	// this->m_new_center   = (this->m_points[0] + this->m_points[2]) / 2.0f;
	// 	// this->m_new_center.w = 1.0f;

	// }
}

bool Anchors::Anchor::inside(const ror::Vector2f &a_mouse_position, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	if (this->m_type == AnchorType::circle)
	{
		auto center = ror::project_to_screen(this->m_new_center, a_view_projection, a_viewport);
		return inside_rectangle(a_mouse_position, ror::Vector2f{center.x, center.y} - this->m_radius, ror::Vector2f{center.x, center.y} + this->m_radius);
	}
	else if ((this->m_type == AnchorType::triangle) || (this->m_type == AnchorType::bezier))        // Simplification of bezier to a triangle
	{
		auto p0 = ror::project_to_screen(this->m_points[0], a_view_projection, a_viewport);
		auto p1 = ror::project_to_screen(this->m_points[1], a_view_projection, a_viewport);
		auto p2 = ror::project_to_screen(this->m_points[2], a_view_projection, a_viewport);
		return inside_triangle(p0, p1, p2, a_mouse_position);
	}
	else if ((this->m_type == AnchorType::rectangle) || (this->m_type == AnchorType::pyramid))
	{
		auto p0 = ror::project_to_screen(this->m_points[0], a_view_projection, a_viewport);
		auto p1 = ror::project_to_screen(this->m_points[1], a_view_projection, a_viewport);
		auto p2 = ror::project_to_screen(this->m_points[2], a_view_projection, a_viewport);
		auto p3 = ror::project_to_screen(this->m_points[3], a_view_projection, a_viewport);
		return inside_triangle(p0, p1, p2, a_mouse_position) || inside_triangle(p0, p3, p2, a_mouse_position);
	}

	return false;
}

size_t Anchors::anchors_count()
{
	return this->m_anchors.size();
}

Anchors::Anchor &Anchors::anchor(size_t a_index)
{
	assert(a_index < this->m_anchors.size() && "Anchor index out of range");
	return this->m_anchors[a_index];
}

void Anchors::new_frame(bool a_mouse_down_state, bool a_mouse_up_state, ror::Vector2f a_mouse_position, ror::Vector2f a_mouse_clicked_position)
{
	this->m_mouse_position = a_mouse_position;

	if (a_mouse_down_state)        // Down
	{
		this->m_clicked_position = a_mouse_clicked_position;
		this->m_draging          = true;
		for (auto &anch : this->m_anchors)
			anch.recenter();
	}

	if (a_mouse_up_state)        // Up
	{
		this->m_draging          = false;
		this->m_clicked_position = ror::zero_vector2f;
		for (auto &anch : this->m_anchors)
		{
			anch.clicked(false);
			anch.center(anch.new_center());
			anch.ribbon_value(0.0f);
		}
	}
}

void Anchors::push_anchor(Anchor a_anchor)
{
	this->m_anchors.emplace_back(std::move(a_anchor));
}

bool Anchors::moving(size_t a_index)
{
	if (this->m_draging && this->m_anchors[a_index].clicked())
		return true;

	return false;
}

void Anchors::draw(ImDrawList *draw_list, const ror::Matrix4f &a_view_projection, ror::Vector4f a_viewport, bool a_mouse_down_state)
{
	auto  &setting = ror::settings();
	ImVec4 ac{setting.m_gui.m_anchor_color.x, setting.m_gui.m_anchor_color.y, setting.m_gui.m_anchor_color.z, setting.m_gui.m_anchor_color.w};
	ImVec4 acc{setting.m_gui.m_anchor_click_color.x, setting.m_gui.m_anchor_click_color.y, setting.m_gui.m_anchor_click_color.z, setting.m_gui.m_anchor_click_color.w};

	auto anchor_color       = ImGui::ColorConvertFloat4ToU32(ac);
	auto anchor_click_color = ImGui::ColorConvertFloat4ToU32(acc);

	auto view_projection_inverse = a_view_projection;
	auto res                     = view_projection_inverse.invert();
	assert(res);
	(void) res;

	auto delta = ror::zero_vector2f;
	if (this->m_draging)
		delta = this->m_clicked_position - this->m_mouse_position;

	for (auto &anch : this->m_anchors)
	{
		bool hover{false};
		if (anch.inside(this->m_mouse_position, a_view_projection, a_viewport))
		{
			hover = true;
			if (a_mouse_down_state)        // Down
			{
				anch.clicked(true);
				a_mouse_down_state = false;        // Don't want any more anchors to register the down event, but I do want to continue projecting all anchors to screen
			}
		}

		if (this->m_draging && anch.clicked())
		{
			anch.move(delta, a_view_projection, view_projection_inverse, a_viewport);
		}

		auto anc_col       = anch.color() != 0 ? anch.color() : anchor_color;
		auto anc_cli_col   = anch.color() != 0 ? (anch.color() & 0xC8FFFFFF) : anchor_click_color;
		auto anc_white_col = 0xC8FFFFFF;

		bool moving = hover || anch.clicked();
		anch.draw(draw_list, a_view_projection, a_viewport, anc_col, anc_cli_col, anc_white_col, hover);
		(moving == true ? ImGui::CaptureMouseFromApp() : (void) moving);
	}

	if (this->m_draging)
		this->m_clicked_position = this->m_mouse_position;
}

void Anchors::Anchor::reset(const ror::Vector4f &a_origin)
{
	this->m_center     = a_origin;
	this->m_new_center = a_origin;
	this->m_points[0]  = a_origin;
	this->m_points[1]  = a_origin;
	this->m_points[2]  = a_origin;
	this->m_points[3]  = a_origin;
}

void Anchors::reset(const ror::Vector4f &a_origin)
{
	for (auto &anchor : this->m_anchors)
		anchor.reset(a_origin);
}

}        // namespace ror
