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

#include "math/rorvector4.hpp"
#include "rorgizmo.hpp"
#include "settings/rorsettings.hpp"

namespace ror
{

void Gizmo::reset(const ror::Vector4f &a_origin)
{
	this->m_anchors_front.reset(a_origin);
	this->m_anchors_behind.reset(a_origin);
}

void Gizmo::rescale(float32_t a_world_scale)
{
	static const float scale_scale      = 0.70f;
	static const float rotate_scale     = 0.50f;
	static const float move_scale_scale = 0.30f;

	this->m_size              = 1.0f * a_world_scale;
	this->m_scale_scalar      = this->m_size * scale_scale;
	this->m_rotate_scalar     = this->m_size * rotate_scale;
	this->m_move_scale_scalar = this->m_size * move_scale_scale;
	this->m_bezier_ease       = 0.16f * a_world_scale;
}

constexpr float32_t move_scale_split{2.0f};
constexpr float32_t move_split{4.0f};

void Gizmo::shape()
{
	auto &center_position = this->m_anchors_front.anchor(this->m_center).new_center();

	// Move anchors
	this->m_anchors_behind.anchor(this->m_move[0]).point(0).y = center_position.y + this->m_size / move_split;
	this->m_anchors_behind.anchor(this->m_move[0]).point(1).y = center_position.y + this->m_size / move_scale_split;
	this->m_anchors_behind.anchor(this->m_move[0]).point(2).z = center_position.z + this->m_size / move_scale_split;
	this->m_anchors_behind.anchor(this->m_move[0]).point(3).z = center_position.z + this->m_size / move_split;

	this->m_anchors_behind.anchor(this->m_move[1]).point(0).x = center_position.x + this->m_size / move_split;
	this->m_anchors_behind.anchor(this->m_move[1]).point(1).x = center_position.x + this->m_size / move_scale_split;
	this->m_anchors_behind.anchor(this->m_move[1]).point(2).z = center_position.z + this->m_size / move_scale_split;
	this->m_anchors_behind.anchor(this->m_move[1]).point(3).z = center_position.z + this->m_size / move_split;

	this->m_anchors_behind.anchor(this->m_move[2]).point(0).x = center_position.x + this->m_size / move_split;
	this->m_anchors_behind.anchor(this->m_move[2]).point(1).x = center_position.x + this->m_size / move_scale_split;
	this->m_anchors_behind.anchor(this->m_move[2]).point(2).y = center_position.y + this->m_size / move_scale_split;
	this->m_anchors_behind.anchor(this->m_move[2]).point(3).y = center_position.y + this->m_size / move_split;

	this->m_anchors_front.anchor(this->m_move[3]).center().x     = center_position.x + this->m_size;
	this->m_anchors_front.anchor(this->m_move[4]).center().y     = center_position.y + this->m_size;
	this->m_anchors_front.anchor(this->m_move[5]).center().z     = center_position.z + this->m_size;
	this->m_anchors_front.anchor(this->m_move[3]).new_center().x = center_position.x + this->m_size;
	this->m_anchors_front.anchor(this->m_move[4]).new_center().y = center_position.y + this->m_size;
	this->m_anchors_front.anchor(this->m_move[5]).new_center().z = center_position.z + this->m_size;

	// Scale anchors
	this->m_anchors_behind.anchor(this->m_scale[0]).point(0)   = this->m_anchors_behind.anchor(this->m_move[0]).point(1);
	this->m_anchors_behind.anchor(this->m_scale[0]).point(1).y = center_position.y + this->m_scale_scalar;
	this->m_anchors_behind.anchor(this->m_scale[0]).point(2).z = center_position.z + this->m_scale_scalar;
	this->m_anchors_behind.anchor(this->m_scale[0]).point(3)   = this->m_anchors_behind.anchor(this->m_move[0]).point(2);

	this->m_anchors_behind.anchor(this->m_scale[1]).point(0)   = this->m_anchors_behind.anchor(this->m_move[1]).point(1);
	this->m_anchors_behind.anchor(this->m_scale[1]).point(1).x = center_position.x + this->m_scale_scalar;
	this->m_anchors_behind.anchor(this->m_scale[1]).point(2).z = center_position.z + this->m_scale_scalar;
	this->m_anchors_behind.anchor(this->m_scale[1]).point(3)   = this->m_anchors_behind.anchor(this->m_move[1]).point(2);

	this->m_anchors_behind.anchor(this->m_scale[2]).point(0)   = this->m_anchors_behind.anchor(this->m_move[2]).point(1);
	this->m_anchors_behind.anchor(this->m_scale[2]).point(1).x = center_position.x + this->m_scale_scalar;
	this->m_anchors_behind.anchor(this->m_scale[2]).point(2).y = center_position.y + this->m_scale_scalar;
	this->m_anchors_behind.anchor(this->m_scale[2]).point(3)   = this->m_anchors_behind.anchor(this->m_move[2]).point(2);

	float scale_bump{this->m_size / 16.0f};

	this->m_anchors_front.anchor(this->m_scale[3]).center().x     = center_position.x + scale_bump + this->m_scale_scalar * this->m_scale_scale[0];
	this->m_anchors_front.anchor(this->m_scale[4]).center().y     = center_position.y + scale_bump + this->m_scale_scalar * this->m_scale_scale[1];
	this->m_anchors_front.anchor(this->m_scale[5]).center().z     = center_position.z + scale_bump + this->m_scale_scalar * this->m_scale_scale[2];
	this->m_anchors_front.anchor(this->m_scale[3]).new_center().x = center_position.x + scale_bump + this->m_scale_scalar * this->m_scale_scale[0];
	this->m_anchors_front.anchor(this->m_scale[4]).new_center().y = center_position.y + scale_bump + this->m_scale_scalar * this->m_scale_scale[1];
	this->m_anchors_front.anchor(this->m_scale[5]).new_center().z = center_position.z + scale_bump + this->m_scale_scalar * this->m_scale_scale[2];

	// Rotate anchors
	this->m_anchors_behind.anchor(this->m_rotate[0]).point(0)   = this->m_anchors_behind.anchor(this->m_scale[0]).point(1);
	this->m_anchors_behind.anchor(this->m_rotate[0]).point(1).y = this->m_anchors_behind.anchor(this->m_scale[0]).point(1).y;
	this->m_anchors_behind.anchor(this->m_rotate[0]).point(1).z = this->m_anchors_behind.anchor(this->m_scale[0]).point(2).z;
	this->m_anchors_behind.anchor(this->m_rotate[0]).point(2)   = this->m_anchors_behind.anchor(this->m_scale[0]).point(2);

	this->m_anchors_behind.anchor(this->m_rotate[1]).point(0)   = this->m_anchors_behind.anchor(this->m_scale[1]).point(1);
	this->m_anchors_behind.anchor(this->m_rotate[1]).point(1).x = this->m_anchors_behind.anchor(this->m_scale[1]).point(1).x;
	this->m_anchors_behind.anchor(this->m_rotate[1]).point(1).z = this->m_anchors_behind.anchor(this->m_scale[1]).point(2).z;
	this->m_anchors_behind.anchor(this->m_rotate[1]).point(2)   = this->m_anchors_behind.anchor(this->m_scale[1]).point(2);

	this->m_anchors_behind.anchor(this->m_rotate[2]).point(0)   = this->m_anchors_behind.anchor(this->m_scale[2]).point(1);
	this->m_anchors_behind.anchor(this->m_rotate[2]).point(1).x = this->m_anchors_behind.anchor(this->m_scale[2]).point(1).x;
	this->m_anchors_behind.anchor(this->m_rotate[2]).point(1).y = this->m_anchors_behind.anchor(this->m_scale[2]).point(2).y;
	this->m_anchors_behind.anchor(this->m_rotate[2]).point(2)   = this->m_anchors_behind.anchor(this->m_scale[2]).point(2);
}

void Gizmo::update(float32_t a_world_scale)
{
	this->rescale(a_world_scale);

	if (this->m_anchors_front.moving(this->m_center))                                  // Only reset if the center is moving
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin

	auto &center = this->m_anchors_front.anchor(this->m_center);
	if (this->m_anchors_behind.moving(this->m_move[0]))        // yz-plane move
	{
		auto &moveyz          = this->m_anchors_behind.anchor(this->m_move[0]);
		auto  dy              = center.center().y - moveyz.center().y;
		auto  dz              = center.center().z - moveyz.center().z;
		center.new_center().y = moveyz.new_center().y + dy;
		center.new_center().z = moveyz.new_center().z + dz;
		center.center().y     = center.new_center().y;
		center.center().z     = center.new_center().z;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}
	else if (this->m_anchors_behind.moving(this->m_move[1]))        // xz-plane move
	{
		auto &movexz          = this->m_anchors_behind.anchor(this->m_move[1]);
		auto  dx              = center.center().x - movexz.center().x;
		auto  dz              = center.center().z - movexz.center().z;
		center.new_center().x = movexz.new_center().x + dx;
		center.new_center().z = movexz.new_center().z + dz;
		center.center().x     = center.new_center().x;
		center.center().z     = center.new_center().z;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}
	else if (this->m_anchors_behind.moving(this->m_move[2]))        // xy-plane move
	{
		auto &movexy          = this->m_anchors_behind.anchor(this->m_move[2]);
		auto  dx              = center.center().x - movexy.center().x;
		auto  dy              = center.center().y - movexy.center().y;
		center.new_center().x = movexy.new_center().x + dx;
		center.new_center().y = movexy.new_center().y + dy;
		center.center().x     = center.new_center().x;
		center.center().y     = center.new_center().y;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}
	else if (this->m_anchors_front.moving(this->m_move[3]))
	{
		auto &movex           = this->m_anchors_front.anchor(this->m_move[3]);
		center.new_center().x = movex.new_center().x - this->m_size;
		center.center().x     = center.new_center().x;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}
	else if (this->m_anchors_front.moving(this->m_move[4]))
	{
		auto &movex           = this->m_anchors_front.anchor(this->m_move[4]);
		center.new_center().y = movex.new_center().y - this->m_size;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}
	else if (this->m_anchors_front.moving(this->m_move[5]))
	{
		auto &movex           = this->m_anchors_front.anchor(this->m_move[5]);
		center.new_center().z = movex.new_center().z - this->m_size;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}
	else if (this->m_anchors_front.moving(this->m_scale[3]))
	{
		auto     &scalex = this->m_anchors_front.anchor(this->m_scale[3]);
		float32_t deltax = scalex.new_center().x - scalex.center().x;
		this->m_scale_scale[0] += deltax / a_world_scale;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}
	else if (this->m_anchors_front.moving(this->m_scale[4]))
	{
		auto     &scaley = this->m_anchors_front.anchor(this->m_scale[4]);
		float32_t deltay = scaley.new_center().y - scaley.center().y;
		this->m_scale_scale[1] += deltay / a_world_scale;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}
	else if (this->m_anchors_front.moving(this->m_scale[5]))
	{
		auto     &scalez = this->m_anchors_front.anchor(this->m_scale[5]);
		float32_t deltax = scalez.new_center().z - scalez.center().z;
		this->m_scale_scale[2] += deltax / a_world_scale;
		this->reset(this->m_anchors_front.anchor(this->m_center).new_center());        // Reset center to the origin
	}

	this->shape();
}

const ImU32 red         = ImGui::ColorConvertFloat4ToU32({1.0f, 0.0f, 0.0f, 1.0f});
const ImU32 green       = ImGui::ColorConvertFloat4ToU32({0.0f, 1.0f, 0.0f, 1.0f});
const ImU32 blue        = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 1.0f, 1.0f});
const ImU32 dim_red     = ImGui::ColorConvertFloat4ToU32({0.6f, 0.0f, 0.0f, 0.5f});
const ImU32 dim_green   = ImGui::ColorConvertFloat4ToU32({0.0f, 0.6f, 0.0f, 0.5f});
const ImU32 dim_blue    = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 0.6f, 0.5f});
const ImU32 white_col   = ImGui::ColorConvertFloat4ToU32({1.0f, 1.0f, 1.0f, 1.0f});
const ImU32 white_alpha = ImGui::ColorConvertFloat4ToU32({1.0f, 1.0f, 1.0f, 0.5f});
const ImU32 red_alpha   = ImGui::ColorConvertFloat4ToU32({1.0f, 0.0f, 0.0f, 0.3f});
const ImU32 green_alpha = ImGui::ColorConvertFloat4ToU32({0.0f, 1.0f, 0.0f, 0.3f});
const ImU32 blue_alpha  = ImGui::ColorConvertFloat4ToU32({0.0f, 0.0f, 1.0f, 0.3f});

void Gizmo::init(const ror::Vector4f &a_origin)
{
	// Side flat anchors on planes
	for (uint32_t i = 0; i < 3; ++i)
	{
		this->m_rotate[i] = i;
		this->m_scale[i]  = i + 3;
		this->m_move[i]   = i + 6;
	}

	// Circle anchors for move and scale
	this->m_center = 0;
	for (uint32_t i = 0; i < 3; ++i)
	{
		this->m_scale[i + 3] = i + 1;
		this->m_move[i + 3]  = i + 4;
	}

	const float32_t           radia[]{6.0f, 6.0f, 6.0f, 8.0f, 8.0f, 8.0f, 6.0f, 6.0f, 6.0f, 10.0f, 8.0f, 8.0f, 8.0f, 6.0f, 6.0f, 6.0f};
	const uint32_t            cols[]{dim_red, dim_green, dim_blue, dim_red, dim_green, dim_blue, dim_red, dim_green, dim_blue, white_col, red, green, blue, red, green, blue};
	const Anchors::AnchorType type[]{Anchors::AnchorType::bezier, Anchors::AnchorType::bezier, Anchors::AnchorType::bezier,
	                                 Anchors::AnchorType::pyramid, Anchors::AnchorType::pyramid, Anchors::AnchorType::pyramid,
	                                 Anchors::AnchorType::pyramid, Anchors::AnchorType::pyramid, Anchors::AnchorType::pyramid,
	                                 Anchors::AnchorType::circle,
	                                 Anchors::AnchorType::circle, Anchors::AnchorType::circle, Anchors::AnchorType::circle,
	                                 Anchors::AnchorType::circle, Anchors::AnchorType::circle, Anchors::AnchorType::circle};

	ror::Vector4f identity{0.0f, 0.0f, 0.0f, 1.0f};
	for (size_t i = 0; i < total_anchors; ++i)
	{
		Anchors::Anchor a{};
		a.center()     = a_origin;
		a.new_center() = a_origin;
		a.radius()     = radia[i] - 2.0f;
		a.color()      = cols[i];
		a.type()       = type[i];
		a.point(0)     = identity;
		a.point(1)     = identity;
		a.point(2)     = identity;
		a.point(3)     = identity;

		if (i < 9)
			this->m_anchors_behind.push_anchor(a);        // First all the plane anchors
		else
			this->m_anchors_front.push_anchor(a);        // The rest of the anchors
	}

	// Enable ribbon states
	for (size_t i = 0; i < 3; ++i)
	{
		this->m_anchors_behind.anchor(this->m_rotate[i]).has_ribbon(true);
		this->m_anchors_behind.anchor(this->m_scale[i]).has_ribbon(true);
	}

	this->reset(a_origin);
}

float32_t world_scale(const ror::Vector4f a_origin, const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	float32_t screen_size = ror::settings().m_gui.m_gizmo_size;

	auto view_projection_inverse = a_view_projection;
	auto res                     = view_projection_inverse.invert();
	assert(res);
	(void) res;

	bool          result{true};
	auto          center0 = ror::project_to_screen(a_origin, a_view_projection, a_viewport, result);
	ror::Vector4f center1{center0.x + screen_size, center0.y, center0.z, center0.w};

	auto a = ror::project_to_world(center0, view_projection_inverse, a_viewport);
	auto b = ror::project_to_world(center1, view_projection_inverse, a_viewport);

	return (a - b).length();
}

void Gizmo::draw(const ror::Matrix4f &a_view_projection, const ror::Vector4f &a_viewport)
{
	this->m_draw_list = draw_list();

	auto scale = world_scale(this->m_anchors_front.anchor(this->m_center).center(), a_view_projection, a_viewport);

	ImGuiIO &io = ImGui::GetIO();

	auto          mpos  = ImGui::GetMousePos();
	auto          mcpos = io.MouseClickedPos[0];        // Left click only
	ror::Vector2f mouse_position{mpos.x, mpos.y};
	ror::Vector2f left_mouse_position{mcpos.x, mcpos.y};
	bool          left_clicked{ImGui::IsMouseClicked(0)};
	bool          left_released{ImGui::IsMouseReleased(0)};

	this->m_anchors_behind.new_frame(left_clicked, left_released, mouse_position, left_mouse_position);
	this->m_anchors_behind.draw(this->m_draw_list, a_view_projection, a_viewport, left_clicked);

	this->update(scale);

	float thickness{4};

	const uint32_t colors[]{red_alpha, green_alpha, blue_alpha};
	for (size_t i = 0; i < 3; ++i)
	{
		bool p0result{false};
		bool p1result{false};
		bool p2result{false};
		auto bezierx = ror::project_to_screen(this->m_anchors_behind.anchor(this->m_rotate[i]).point(0), a_view_projection, a_viewport, p0result);
		auto beziery = ror::project_to_screen(this->m_anchors_behind.anchor(this->m_rotate[i]).point(1), a_view_projection, a_viewport, p1result);
		auto bezierz = ror::project_to_screen(this->m_anchors_behind.anchor(this->m_rotate[i]).point(2), a_view_projection, a_viewport, p2result);

		if (!p0result && !p1result && !p2result)
			continue;

		this->m_draw_list->AddBezierQuadratic({bezierx.x, bezierx.y}, {beziery.x, beziery.y}, {bezierz.x, bezierz.y}, colors[i], thickness);
	}

	bool center_result{false};
	auto center = ror::project_to_screen(this->m_anchors_front.anchor(this->m_center).center(), a_view_projection, a_viewport, center_result);

	bool movexresult{false};
	bool moveyresult{false};
	bool movezresult{false};
	auto movex = ror::project_to_screen(this->m_anchors_front.anchor(this->m_move[3]).center(), a_view_projection, a_viewport, movexresult);
	auto movey = ror::project_to_screen(this->m_anchors_front.anchor(this->m_move[4]).center(), a_view_projection, a_viewport, moveyresult);
	auto movez = ror::project_to_screen(this->m_anchors_front.anchor(this->m_move[5]).center(), a_view_projection, a_viewport, movezresult);

	bool scalexresult{false};
	bool scaleyresult{false};
	bool scalezresult{false};
	auto scalex = ror::project_to_screen(this->m_anchors_front.anchor(this->m_scale[3]).center(), a_view_projection, a_viewport, scalexresult);
	auto scaley = ror::project_to_screen(this->m_anchors_front.anchor(this->m_scale[4]).center(), a_view_projection, a_viewport, scaleyresult);
	auto scalez = ror::project_to_screen(this->m_anchors_front.anchor(this->m_scale[5]).center(), a_view_projection, a_viewport, scalezresult);

	bool bezierxresult{false};
	bool bezieryresult{false};
	bool bezierzresult{false};
	auto bezierx = ror::project_to_screen(this->m_anchors_behind.anchor(this->m_rotate[2]).point(0), a_view_projection, a_viewport, bezierxresult);
	auto beziery = ror::project_to_screen(this->m_anchors_behind.anchor(this->m_rotate[0]).point(0), a_view_projection, a_viewport, bezieryresult);
	auto bezierz = ror::project_to_screen(this->m_anchors_behind.anchor(this->m_rotate[1]).point(2), a_view_projection, a_viewport, bezierzresult);

	// Axis lines
	if (center_result && movexresult)
		this->m_draw_list->AddLine({movex.x, movex.y}, {center.x, center.y}, red, thickness);
	if (center_result && moveyresult)
		this->m_draw_list->AddLine({movey.x, movey.y}, {center.x, center.y}, green, thickness);
	if (center_result && movezresult)
		this->m_draw_list->AddLine({movez.x, movez.y}, {center.x, center.y}, blue, thickness);

	// Scale lines to the original scale positions
	if (scalexresult && bezierxresult)
		this->m_draw_list->AddLine({scalex.x, scalex.y}, {bezierx.x, bezierx.y}, white_alpha, std::max(1.0f, thickness / 2.0f));
	if (scaleyresult && bezieryresult)
		this->m_draw_list->AddLine({scaley.x, scaley.y}, {beziery.x, beziery.y}, white_alpha, std::max(1.0f, thickness / 2.0f));
	if (scalezresult && bezierzresult)
		this->m_draw_list->AddLine({scalez.x, scalez.y}, {bezierz.x, bezierz.y}, white_alpha, std::max(1.0f, thickness / 2.0f));

	this->m_anchors_front.new_frame(left_clicked, left_released, mouse_position, left_mouse_position);
	this->m_anchors_front.draw(this->m_draw_list, a_view_projection, a_viewport, left_clicked);
}

}        // namespace ror
