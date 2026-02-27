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
#include "renderer/rorrenderer.hpp"
#include "settings/rorsettings.hpp"
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
			auto vec2     = std::any_cast<ror::Vector2d>(e.m_payload);
			auto code     = event_code(e.m_handle);
			auto modifier = event_modifier(e.m_handle);

			if (code == EventCode::left_mouse)
				this->left_key_drag(vec2.x, vec2.y, modifier);
			else if (code == EventCode::middle_mouse)
				this->middle_key_drag(vec2.x, vec2.y, modifier);
			else if (code == EventCode::right_mouse)
				this->right_key_drag(vec2.x, vec2.y, modifier);
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

	this->m_camera_top_view = [this](Event &) {
		this->orient_top();
	};

	this->m_camera_bottom_view = [this](Event &) {
		this->orient_bottom();
	};

	this->m_camera_left_view = [this](Event &) {
		this->orient_left();
	};

	this->m_camera_right_view = [this](Event &) {
		this->orient_right();
	};

	this->m_camera_front_view = [this](Event &) {
		this->orient_front();
	};

	this->m_camera_back_view = [this](Event &) {
		this->orient_back();
	};

	this->m_event_system = &a_event_system;

	this->setup();
}

void OrbitCamera::orient_to_direction(Vector3f a_forward, Vector3f a_up_hint)
{
	auto radius = ror::distance(this->m_eye, this->m_target);

	if (radius < 0.0001f)
		radius = 1.0f;

	auto right = a_forward.cross_product(a_up_hint);
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
	// Up default is {0.0f, 1.0f, 0.0f} the real world up vector to orient itself
	this->m_view = ror::make_look_at(this->m_eye, this->m_target, ror::yaxis3f, this->m_up, this->m_right);

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

void OrbitCamera::build_cascades()
{
	// The bigger the lambda is the smaller are the first few splits for example
	// for lambda = 0.5  splits are 0.13028727, 0.2772918, 0.49387014, 1, just about right
	// for lambda = 0.95 splits are 0.02254583, 0.07685446, 0.2633533, 1, too conservative
	float32_t lambda = this->m_cascade_lambda;        // Default is 0.5 but can be adjusted in the settings or scene which overrides the settings for current run

	float32_t range = this->m_z_far - this->m_z_near;
	float32_t ratio = this->m_z_far / this->m_z_near;

	// Calculate split depths based on view camera frustum
	// Based on method presented in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html with implementations
	// https://johanmedestrom.wordpress.com/2016/03/18/opengl-cascaded-shadow-maps/ and
	// https://github.com/SaschaWillems/Vulkan/examples/shadowmappingcascade/shadowmappingcascade.cpp
	for (uint32_t i = 0; i < cascade_count; i++)
	{
		float32_t power           = (i + 1) / static_cast<float32_t>(cascade_count);
		float32_t log             = this->m_z_near * std::pow(ratio, power);        // Calculate logrithmic split
		float32_t uniform         = this->m_z_near + range * power;                 // Calculate normal split
		float32_t depth           = lambda * (log - uniform) + uniform;             // The equation is (d = lambda(log) + (1 - lambda) * uniform) but its simplified here
		this->m_cascade_splits[i] = (depth - this->m_z_near) / range;               // Normalise the depth range from [n,f] to [0,1]
	}
}

void OrbitCamera::setup_frustums()
{
	this->build_cascades();

	float32_t near  = this->m_z_near;
	float32_t far   = this->m_z_far;
	float32_t range = this->m_z_far - this->m_z_near;
	// float32_t ratio = this->m_z_far / this->m_z_near;

	for (uint32_t i = 0; i < cascade_count; i++)
	{
		auto cascade_split = this->m_cascade_splits[i];
		if (i > 0)
			near = this->m_cascade_splits[i - 1] * range;

		far = cascade_split * range;

		this->m_frustums[i].near(near);
		this->m_frustums[i].far(far);
		this->m_frustums[i].fov(this->m_y_fov);
		this->m_frustums[i].aspect(this->m_aspect_ratio);

		this->m_frustums[i].setup(this->m_view);
	}
}

// TODO: Fit near and far to the geometry as close as possible.
// Before splitting the view frustum, we might adjust the camera's near and far planes so that the view frustum contains the visible objects as
// tightly as possible. This will minimize the amount of empty space in the view frustum, increasing the available shadow-map resolution.
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

void OrbitCamera::rotate_eye_target(const ror::Matrix4f &a_rotation_matrix, const ror::Vector3f &a_origin)
{
	this->rotate_at_origin(a_rotation_matrix, a_origin, this->m_eye);
	this->rotate_at_origin(a_rotation_matrix, a_origin, this->m_target);
}

void OrbitCamera::yaw(float32_t a_x_delta, const ror::Vector3f &a_origin)
{
	auto rotation_matrix = ror::matrix4_rotation_around_y(ror::to_radians(a_x_delta));
	rotate_eye_target(rotation_matrix, a_origin);
}

void OrbitCamera::pitch(float32_t a_y_delta, const ror::Vector3f &a_origin)
{
	auto right = this->m_right;

	ror::AxisAnglef pitch_input{right, ror::to_radians(a_y_delta)};
	auto            rotation_matrix = ror::matrix4_rotation(pitch_input);

	rotate_eye_target(rotation_matrix, a_origin);
}

void OrbitCamera::roll(float32_t a_delta, const ror::Vector3f &a_origin)
{
	// NOTE: Not currently working
	(void) a_origin;
	ror::AxisAnglef roll_input{this->m_forward, ror::to_radians(a_delta)};
	auto            rotation_matrix = ror::matrix4_rotation(roll_input);

	this->m_up    = rotation_matrix * this->m_up;
	this->m_right = rotation_matrix * this->m_right;

	this->m_up.normalize();
	this->m_right.normalize();
}

void OrbitCamera::rotate(float32_t a_x_delta, float32_t a_y_delta, EventModifier a_modifier)
{
	auto origin = this->m_mode == CameraMode::orbit ? this->m_center : this->m_eye;

	float32_t scale = 1.0f;
	if (a_modifier == EventModifier::command)
		scale = 0.01f;
	else if (a_modifier == EventModifier::control)
		scale = 2.5f;

	a_x_delta *= scale;
	a_y_delta *= scale;

	const float32_t simuluation_tick = ror::settings().m_simulation_step;

	if (a_modifier == EventModifier::option)
	{
		auto roll_delta = static_cast<float32_t>((a_x_delta + a_y_delta) * 0.5f);
		consume_in_chunks(roll_delta, simuluation_tick, [this, origin](float32_t step) {
			this->roll(step, origin);
			this->update_view();
		});
		return;
	}

	// Order is important don't re-order, roll is not applied as usual and applied in isolation
	consume_in_chunks(a_x_delta, simuluation_tick, [this, origin](float32_t step) {
		this->yaw(step, origin);
		this->update_view();
	});

	consume_in_chunks(a_y_delta, simuluation_tick, [this, origin](float32_t step) {
		this->pitch(step, origin);
		this->update_view();
	});
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
	this->m_event_system->subscribe(mouse_left_mouse_option_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_middle_mouse_option_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_right_mouse_option_drag, this->m_drag_callback);

	this->m_event_system->subscribe(buffer_resize, this->m_resize_callback);

	this->m_event_system->subscribe(mouse_scroll, this->m_forward_callback);
	this->m_event_system->subscribe(mouse_command_scroll, this->m_forward_callback);
	this->m_event_system->subscribe(mouse_control_scroll, this->m_forward_callback);

	this->m_event_system->subscribe(mouse_command_scroll, this->m_zoom_callback);

	this->m_event_system->subscribe(keyboard_space_click, this->m_camera_mode_callback);

	this->m_event_system->subscribe(keyboard_num8_click, this->m_camera_top_view);
	this->m_event_system->subscribe(keyboard_num2_click, this->m_camera_bottom_view);
	this->m_event_system->subscribe(keyboard_num4_click, this->m_camera_left_view);
	this->m_event_system->subscribe(keyboard_num6_click, this->m_camera_right_view);
	this->m_event_system->subscribe(keyboard_num5_click, this->m_camera_front_view);
	this->m_event_system->subscribe(keyboard_num0_click, this->m_camera_back_view);
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
	this->m_event_system->unsubscribe(mouse_left_mouse_option_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_middle_mouse_option_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_right_mouse_option_drag, this->m_drag_callback);

	this->m_event_system->unsubscribe(buffer_resize, this->m_resize_callback);

	this->m_event_system->unsubscribe(mouse_scroll, this->m_forward_callback);
	this->m_event_system->unsubscribe(mouse_command_scroll, this->m_forward_callback);
	this->m_event_system->unsubscribe(mouse_control_scroll, this->m_forward_callback);

	this->m_event_system->unsubscribe(mouse_command_scroll, this->m_zoom_callback);

	this->m_event_system->unsubscribe(keyboard_space_click, this->m_camera_mode_callback);

	this->m_event_system->unsubscribe(keyboard_num8_click, this->m_camera_top_view);
	this->m_event_system->unsubscribe(keyboard_num2_click, this->m_camera_bottom_view);
	this->m_event_system->unsubscribe(keyboard_num4_click, this->m_camera_left_view);
	this->m_event_system->unsubscribe(keyboard_num6_click, this->m_camera_right_view);
	this->m_event_system->unsubscribe(keyboard_num5_click, this->m_camera_front_view);
	this->m_event_system->unsubscribe(keyboard_num0_click, this->m_camera_back_view);
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
	// this->draw_camera_properties();        // Enable when needed to render the axis

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
