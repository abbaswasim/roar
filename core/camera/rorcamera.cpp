// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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
#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rorline_soup.hpp"
#include "math/roraxis_angle.hpp"
#include "math/rormatrix3.hpp"
#include "math/rormatrix4.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "math/rorvector_functions.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"
#include <cmath>

namespace ror
{

// TODO: Add smoothing and dampning but currently just a lerp(old_property, new_property, 15%) every update should be fine
void OrbitCamera::init(EventSystem &a_event_system)
{
	this->m_move_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2d>())
		{
			auto vec2 = std::any_cast<ror::Vector2d>(e.m_payload);
			this->update_position_function(vec2.x, vec2.y);
		}
	};

	this->m_drag_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2d>())
		{
			float scale    = 1.0f;
			auto  vec2     = std::any_cast<ror::Vector2d>(e.m_payload);
			auto  code     = event_code(e.m_handle);
			auto  modifier = event_modifier(e.m_handle);

			if (modifier == EventModifier::command)
				scale = 0.01f;
			else if (modifier == EventModifier::control)
				scale = 2.5f;

			if (code == EventCode::left_mouse)
				this->left_key_drag(vec2.x, vec2.y, scale);
			else if (code == EventCode::middle_mouse)
				this->middle_key_drag(vec2.x, vec2.y, scale);
			else if (code == EventCode::right_mouse)
				this->right_key_drag(vec2.x, vec2.y, scale);
		}
	};

	this->m_resize_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2ui>())
		{
			auto vec2 = std::any_cast<ror::Vector2ui>(e.m_payload);
			this->bounds(static_cast<float32_t>(vec2.x), static_cast<float32_t>(vec2.y));
		}
		else
		{
			assert(0 && "Incompatible payload with resize callback");
		}
	};

	this->m_zoom_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2d>())
		{
			auto vec2 = std::any_cast<ror::Vector2d>(e.m_payload);
			this->zoom(vec2.x);
		}
		else
		{
			assert(0 && "Incompatible payload with zoom callback");
		}
	};

	this->m_forward_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2d>())
		{
			float32_t scale = 1.0f;

			auto vec2     = std::any_cast<ror::Vector2d>(e.m_payload);
			auto modifier = event_modifier(e.m_handle);

			if (modifier == EventModifier::command)
				scale = 0.01f;
			else if (modifier == EventModifier::control)
				scale = 2.5f;

			vec2 *= scale;

			this->forward(-vec2.y);
		}
		else
		{
			assert(0 && "Incompatible payload with forward callback");
		}
	};

	this->m_camera_mode_callback = [this](ror::Event &) {
		this->m_mode = this->m_mode == CameraMode::orbit ? CameraMode::fps : CameraMode::orbit;
	};

	this->m_event_system = &a_event_system;

	this->setup();
}

void OrbitCamera::orient_to_direction(Vector3f a_forward, Vector3f a_up_hint)
{
	auto radius = ror::distance(this->m_eye, this->m_target);
	if (radius < 0.0001f)
		radius = 1.0f;

	if (a_forward.length_squared() < 1e-6f)
		a_forward = ror::yaxis3f_negative;

	a_forward.normalize();

	if (std::abs(a_forward.dot_product(a_up_hint)) > 0.99f)
		a_up_hint = std::abs(a_forward.dot_product(ror::zaxis3f)) > 0.99f ? ror::xaxis3f : ror::zaxis3f;

	auto right = a_forward.cross_product(a_up_hint);
	if (right.length_squared() < 1e-6f)
		right = a_forward.cross_product(ror::xaxis3f);
	right.normalize();

	auto up = right.cross_product(a_forward);
	up.normalize();

	this->m_forward = a_forward;
	this->m_right   = right;
	this->m_up      = up;
	this->m_eye     = this->m_target - this->m_forward * radius;
}

void OrbitCamera::orient_top()
{
	this->orient_to_direction(ror::yaxis3f_negative, ror::zaxis3f_negative);
}

void OrbitCamera::orient_bottom()
{
	this->orient_to_direction(ror::yaxis3f, ror::zaxis3f);
}

void OrbitCamera::orient_left()
{
	this->orient_to_direction(ror::xaxis3f, ror::yaxis3f);
}

void OrbitCamera::orient_right()
{
	this->orient_to_direction(ror::xaxis3f_negative, ror::yaxis3f);
}

void OrbitCamera::orient_front()
{
	this->orient_to_direction(ror::zaxis3f_negative, ror::yaxis3f);
}

void OrbitCamera::orient_back()
{
	this->orient_to_direction(ror::zaxis3f, ror::yaxis3f);
}

void OrbitCamera::update_normal()
{
	// TODO: This is wrong, I don't have model matrix here, its somewhere in the compute shader world
	// Also remember if a tranpose of a matrix is possible then the order doesn't matter, transpose first then inverse or inverse first then transpose
	// For xforming nomrals calculate normal matrix from transpose of inverse of model (top 3x3) without translations. Transpose and Inverse removes non-uniform scale from it
	this->m_normal  = Matrix3f{this->m_view};        // NOTE: This should be model not view
	bool invertable = this->m_normal.invert();
	assert(invertable);
	(void) invertable;
	this->m_normal.transpose();
}

void OrbitCamera::update_vectors()
{
	this->m_right.x   = this->m_view.m_values[0];
	this->m_right.y   = this->m_view.m_values[4];
	this->m_right.z   = this->m_view.m_values[8];
	this->m_up.x      = this->m_view.m_values[1];
	this->m_up.y      = this->m_view.m_values[5];
	this->m_up.z      = this->m_view.m_values[9];
	this->m_forward.x = -this->m_view.m_values[2];
	this->m_forward.y = -this->m_view.m_values[6];
	this->m_forward.z = -this->m_view.m_values[10];
}

void OrbitCamera::update_view()
{
	auto world_up = ror::yaxis3f;
	auto forward  = this->m_target - this->m_eye;

	if (forward.length_squared() > 0.0f)
	{
		forward.normalize();

		if (std::abs(forward.dot_product(world_up)) > 0.99f)
			world_up = ror::zaxis3f;        // Avoid degenerate cross when looking straight down/up
	}

	// Up default is {0.0f, 1.0f, 0.0f} the real world up vector to orient itself
	this->m_view = ror::make_look_at(this->m_eye, this->m_target, world_up, this->m_up, this->m_right);

	this->update_vectors();
}

void OrbitCamera::update_projection()
{
	// Setup perspective projection matrix
	this->m_aspect_ratio = this->m_width / this->m_height;

	if (this->m_type == CameraType::perspective)
	{
		// assumes square pixels, if ever there is evidence of non-square pixels, provided by windowing system i.e. glfw use it here instead
		this->m_projection = ror::make_infinite_perspective(ror::to_radians(this->m_y_fov), this->m_aspect_ratio, this->m_z_near, this->m_z_far);
	}
	else
	{
		auto mag  = std::max(this->m_x_mag, this->m_y_mag);
		auto xmag = (mag * this->m_aspect_ratio) * 0.5f;
		auto ymag = mag * 0.5f;

		// TODO: Make me infinite
		this->m_projection = ror::make_ortho(-xmag, xmag, -ymag, ymag, this->m_z_near, this->m_z_far);
	}
}

void OrbitCamera::setup_frustums()
{
	const size_t cascade_index{0};

	this->m_frustums[cascade_index].fov(this->m_y_fov);
	this->m_frustums[cascade_index].far(this->m_z_far);
	this->m_frustums[cascade_index].near(this->m_z_near);
	this->m_frustums[cascade_index].aspect(this->m_aspect_ratio);
	this->m_frustums[cascade_index].setup(this->m_view);
}

void OrbitCamera::setup()
{
	auto diagonal  = (this->m_maximum - this->m_minimum);
	this->m_center = this->m_minimum + (diagonal / 2.0f);

	if (this->m_from_scene)
	{
		this->m_target = this->m_center;
		this->m_eye.x  = this->m_center.x;
		this->m_eye.y  = this->m_center.y + (diagonal.x / 4.0f);
		this->m_eye.z  = ((std::max(diagonal.x, diagonal.y) / 2.0f) / std::tan(ror::to_radians(this->m_y_fov / 2.0f)));
	}
}

void OrbitCamera::rotate(float32_t a_x_delta, float32_t a_y_delta)
{
	bool orbit_mode = this->m_mode == CameraMode::orbit;
	auto origin     = orbit_mode ? this->m_target : this->m_eye;

	auto view_vector   = this->m_target - this->m_eye;
	auto view_distance = view_vector.length();
	if (view_distance < 1e-4f)
		view_distance = 1.0f;

	auto rotation_matrix_y = ror::matrix4_rotation_around_y(ror::to_radians(a_x_delta));

	// First apply yaw
	if (orbit_mode)
	{
		auto eye_offset = this->m_eye - origin;
		eye_offset      = rotation_matrix_y * eye_offset;
		this->m_eye     = origin + eye_offset;
	}
	else
	{
		this->m_forward = rotation_matrix_y * this->m_forward;
	}

	this->m_up    = rotation_matrix_y * this->m_up;
	this->m_right = rotation_matrix_y * this->m_right;

	// Build a stable right vector after yaw
	auto forward = orbit_mode ? this->m_target - this->m_eye : this->m_forward;
	if (forward.length_squared() < 1e-6f)
		forward = ror::zaxis3f_negative;
	forward.normalize();

	auto right = forward.cross_product_safe(ror::yaxis3f, this->m_right);
	if (right.length_squared() < 1e-6f)
		right = this->m_right;
	right.normalize();

	ror::AxisAnglef pitch_input{right, ror::to_radians(a_y_delta)};
	auto            rotation_matrix_p = ror::matrix4_rotation(pitch_input);

	// Then apply pitch
	if (orbit_mode)
	{
		auto eye_offset = this->m_eye - origin;
		eye_offset      = rotation_matrix_p * eye_offset;
		this->m_eye     = origin + eye_offset;
	}
	else
	{
		forward = rotation_matrix_p * forward;
	}

	this->m_up = rotation_matrix_p * this->m_up;

	forward = orbit_mode ? this->m_target - this->m_eye : forward;
	if (forward.length_squared() < 1e-6f)
		forward = ror::zaxis3f_negative;
	forward.normalize();

	this->m_right = forward.cross_product_safe(ror::yaxis3f, right);
	if (this->m_right.length_squared() < 1e-6f)
		this->m_right = right;
	this->m_right.normalize();

	this->m_up = this->m_right.cross_product_safe(forward, this->m_up);
	this->m_up.normalize();

	this->m_forward = forward;

	if (!orbit_mode)
		this->m_target = this->m_eye + forward * view_distance;
}

void OrbitCamera::enable()
{
	this->m_event_system->subscribe(mouse_move, this->m_move_callback);

	this->m_event_system->subscribe(mouse_left_mouse_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_middle_mouse_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_right_mouse_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_left_mouse_command_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_middle_mouse_command_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_right_mouse_command_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_left_mouse_control_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_middle_mouse_control_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_right_mouse_control_drag, this->m_drag_callback);

	this->m_event_system->subscribe(buffer_resize, this->m_resize_callback);

	this->m_event_system->subscribe(mouse_scroll, this->m_forward_callback);
	this->m_event_system->subscribe(mouse_command_scroll, this->m_forward_callback);
	this->m_event_system->subscribe(mouse_control_scroll, this->m_forward_callback);

	this->m_event_system->subscribe(mouse_command_scroll, this->m_zoom_callback);

	this->m_event_system->subscribe(keyboard_space_click, this->m_camera_mode_callback);
}

void OrbitCamera::disable()
{
	this->m_event_system->unsubscribe(mouse_move, this->m_move_callback);

	this->m_event_system->unsubscribe(mouse_left_mouse_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_middle_mouse_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_right_mouse_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_left_mouse_command_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_middle_mouse_command_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_right_mouse_command_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_left_mouse_control_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_middle_mouse_control_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_right_mouse_control_drag, this->m_drag_callback);

	this->m_event_system->unsubscribe(buffer_resize, this->m_resize_callback);

	this->m_event_system->unsubscribe(mouse_scroll, this->m_forward_callback);
	this->m_event_system->unsubscribe(mouse_command_scroll, this->m_forward_callback);
	this->m_event_system->unsubscribe(mouse_control_scroll, this->m_forward_callback);

	this->m_event_system->unsubscribe(mouse_command_scroll, this->m_zoom_callback);

	this->m_event_system->unsubscribe(keyboard_space_click, this->m_camera_mode_callback);
}

void OrbitCamera::draw_camera_properties()
{
	auto &ls = ror::line_soup();

	static uint32_t lines{0xFFFFFFFF};
	static uint32_t red_x{0xFFFFFFFF};
	static uint32_t green_y{0xFFFFFFFF};
	static uint32_t blue_z{0xFFFFFFFF};
	static uint32_t targetl{0xFFFFFFFF};

	std::vector<float32_t> data{};

	ror::Vector3f eye{this->m_eye};
	eye.normalize();

	ror::push_point(data, ror::zero_vector3f, ror::red4f);
	ror::push_point(data, ror::xaxis3f, ror::red4f);

	ror::push_point(data, ror::zero_vector3f, ror::green4f);
	ror::push_point(data, ror::yaxis3f, ror::green4f);

	ror::push_point(data, ror::zero_vector3f, ror::blue4f);
	ror::push_point(data, ror::zaxis3f, ror::blue4f);

	ror::push_point(data, eye, ror::blue4f);
	ror::push_point(data, ror::zero_vector3f, ror::blue4f);

	ror::push_point(data, ror::zero_vector3f, ror::yellow4f);
	ror::push_point(data, this->m_up, ror::yellow4f);

	lines   = ls.push_lines(reinterpret_cast<uint8_t *>(data.data()), static_cast_safe<uint32_t>(data.size() / 14), lines);
	red_x   = ls.push_cross(xaxis3f, 0.5f, red_x, ror::red4f);
	green_y = ls.push_cross(yaxis3f, 0.5f, green_y, ror::green4f);
	blue_z  = ls.push_cross(zaxis3f, 0.5f, blue_z, ror::blue4f);
	targetl = ls.push_cross(this->m_target, 0.5f, targetl, ror::blue4f);
}

void OrbitCamera::update()
{
	// this->draw_camera_properties(); // Enable when needed to render the axis
	this->update_view();
	this->update_normal();
	this->update_projection();
	this->setup_frustums();
}

void OrbitCamera::upload(const Renderer &a_renderer)
{
	// TODO: Don't update me if nothing has changed
	auto per_view_uniform = a_renderer.shader_buffer("per_view_uniform");

	per_view_uniform->buffer_map();

	per_view_uniform->update("view_mat4", &this->m_view.m_values);
	per_view_uniform->update("projection_mat4", &this->m_projection.m_values);
	per_view_uniform->update("view_projection_mat4", &this->m_view_projection.m_values);
	per_view_uniform->update("inverse_projection_mat4", &this->m_inverse_projection.m_values);
	per_view_uniform->update("inverse_view_projection_mat4", &this->m_inverse_view_projection.m_values);
	per_view_uniform->update("normal_mat3", &this->m_normal.m_values);
	per_view_uniform->update("camera_position", &this->m_eye.x);

	ror::Vector4ui viewport{0, 0, static_cast<uint32_t>(this->m_width), static_cast<uint32_t>(this->m_height)};
	per_view_uniform->update("viewport", &viewport.x);

	per_view_uniform->buffer_unmap();
}
}        // namespace ror
