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
#include "math/roraxis_angle.hpp"
#include "math/rormatrix3.hpp"
#include "math/rormatrix4.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorquaternion.hpp"
#include "math/rorvector3.hpp"

namespace ror
{

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
			auto modifier = event_modifier(e.m_handle);
			switch (modifier)
			{
				case EventModifier::left_mouse:
					this->left_key_drag(vec2.x, vec2.y);
					break;
				case EventModifier::middle_mouse:
					this->middle_key_drag(vec2.x, vec2.y);
					break;
				case EventModifier::right_mouse:
					this->right_key_drag(vec2.x, vec2.y);
					break;
				case EventModifier::none:
				case EventModifier::shift:
				case EventModifier::control:
				case EventModifier::command:
				case EventModifier::option:
				case EventModifier::caps_lock:
				case EventModifier::num_lock:
				case EventModifier::max:
					return;
			}
		}
	};

	this->m_resize_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2ui>())
		{
			auto vec2 = std::any_cast<ror::Vector2ui>(e.m_payload);
			this->bounds(vec2.x, vec2.y);
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
			this->forward(-vec2.y);
		}
		else
		{
			assert(0 && "Incompatible payload with zoom callback");
		}
	};

	this->m_mode_callback = [this](ror::Event &e) {
		(void) e;
		this->m_mode = this->m_mode == CameraMode::orbit ? CameraMode::fps : CameraMode::orbit;
	};

	this->m_event_system = &a_event_system;
}

void OrbitCamera::update_normal()
{
	// TODO: Simplify me
	// Also calculate normal matrix, for xforming normals, from transpose of inverse of model-view (top 3x3) without translations Transpose and Inverse removes non-uniform scale from it
	this->m_normal  = Matrix3f{this->m_view};
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
	static const Vector3f up{0.0f, 1.0f, 0.0f};        //! World Up vector to orient itself
	this->m_view = ror::make_look_at(this->m_eye, this->m_center, up);

	this->update_vectors();
}

void OrbitCamera::update_perspective()
{
	// Setup perspective projection matrix
	if (this->m_type == CameraType::perspective)
	{
		this->m_projection = ror::make_perspective(ror::to_radians(this->m_y_fov), static_cast<float>(this->m_width) / static_cast<float>(this->m_height), this->m_z_near, this->m_z_far);

		// Make infinite projections matrix
		float a_z_near = 0.1f;
		float epsilon  = 0.00000024f;

		this->m_projection.m_values[10] = epsilon - 1.0f;
		this->m_projection.m_values[14] = (epsilon - 2.0f) * a_z_near;
	}
	else
		this->m_projection = ror::make_ortho(0.0f, static_cast<float>(this->m_width), 0.0f, static_cast<float>(this->m_height), this->m_z_near, this->m_z_far);
}

void OrbitCamera::setup()
{
	auto diagonal  = (this->m_maximum - this->m_minimum);
	this->m_center = this->m_minimum + (diagonal / 2.0f);
	this->m_eye.x  = this->m_center.x;
	this->m_eye.y  = this->m_center.y + (diagonal.x / 4.0f);
	this->m_eye.z  = ((std::max(diagonal.x, diagonal.y) / 2.0f) / std::tan(ror::to_radians(this->m_y_fov / 2)));

	this->update_view();
}

void OrbitCamera::rotate(float32_t a_x_delta, float32_t a_y_delta)
{
	ror::AxisAnglef yinput{this->m_right, ror::to_radians(-a_y_delta)};

	auto origin = this->m_mode == CameraMode::orbit ? this->m_center : this->m_eye;

	auto translation0 = ror::matrix4_translation(-origin);
	auto translation1 = ror::matrix4_translation(origin);
	auto rotation_x   = ror::matrix4_rotation(yinput);
	auto rotation_y   = ror::matrix4_rotation_around_y(ror::to_radians(-a_x_delta));

	// Rotate the view matrix directly instead of reacreating it from Euler angles
	this->m_view *= translation1 * rotation_y * rotation_x * translation0;

	// Create inverse transformation of the view matrix delta to transform eye and center
	yinput     = ror::AxisAnglef{this->m_right, ror::to_radians(a_y_delta)};
	rotation_x = ror::matrix4_rotation(yinput);
	rotation_y = ror::matrix4_rotation_around_y(ror::to_radians(a_x_delta));

	if (this->m_mode == CameraMode::orbit)
		this->m_eye = translation1 * rotation_y * rotation_x * translation0 * this->m_eye;
	else
		this->m_center = translation1 * rotation_y * rotation_x * translation0 * this->m_center;

	this->update_vectors();
	this->update_normal();
}

void OrbitCamera::enable()
{
	this->m_event_system->subscribe(mouse_move, this->m_move_callback);
	this->m_event_system->subscribe(mouse_move, this->m_move_callback);
	this->m_event_system->subscribe(mouse_move, this->m_move_callback);

	this->m_event_system->subscribe(mouse_left_mouse_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_middle_mouse_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_right_mouse_drag, this->m_drag_callback);

	this->m_event_system->subscribe(window_resize, this->m_resize_callback);
	this->m_event_system->subscribe(buffer_resize, this->m_resize_callback);

	this->m_event_system->subscribe(mouse_scroll, this->m_zoom_callback);

	this->m_event_system->subscribe(keyboard_space_click, this->m_mode_callback);
}

void OrbitCamera::disable()
{
	this->m_event_system->unsubscribe(mouse_move, this->m_move_callback);
	this->m_event_system->unsubscribe(mouse_move, this->m_move_callback);
	this->m_event_system->unsubscribe(mouse_move, this->m_move_callback);

	this->m_event_system->unsubscribe(mouse_left_mouse_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_middle_mouse_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_right_mouse_drag, this->m_drag_callback);

	this->m_event_system->unsubscribe(window_resize, this->m_resize_callback);
	this->m_event_system->unsubscribe(buffer_resize, this->m_resize_callback);

	this->m_event_system->unsubscribe(mouse_scroll, this->m_zoom_callback);

	this->m_event_system->unsubscribe(keyboard_space_click, this->m_mode_callback);
}

void OrbitCamera::fill_shader_buffer()
{
	// This creates the per_view_uniforms UBO with variable name of in_per_view_uniforms
	this->m_shader_buffer.add_entry("view_mat4", rhi::Format::float32_4x4, 1);
	this->m_shader_buffer.add_entry("projection_mat4", rhi::Format::float32_4x4, 1);
	this->m_shader_buffer.add_entry("view_projection_mat4", rhi::Format::float32_4x4, 1);
	this->m_shader_buffer.add_entry("inverse_projection_mat4", rhi::Format::float32_4x4, 1);
	this->m_shader_buffer.add_entry("inverse_view_projection_mat4", rhi::Format::float32_4x4, 1);
	this->m_shader_buffer.add_entry("normal_mat3", rhi::Format::float32_3x3, 1);
	this->m_shader_buffer.add_entry("camera_position", rhi::Format::float32_3, 1);
}

void OrbitCamera::update()
{
	// TODO: Don't update me if nothing has changed
	this->m_shader_buffer.buffer_map();

	this->m_shader_buffer.update("view_mat4", &this->m_view.m_values);
	this->m_shader_buffer.update("projection_mat4", &this->m_projection.m_values);
	this->m_shader_buffer.update("view_projection_mat4", &this->m_view_projection.m_values);
	this->m_shader_buffer.update("inverse_projection_mat4", &this->m_inverse_projection.m_values);
	this->m_shader_buffer.update("inverse_view_projection_mat4", &this->m_inverse_view_projection.m_values);
	this->m_shader_buffer.update("normal_mat3", &this->m_normal.m_values);
	this->m_shader_buffer.update("camera_position", &this->m_eye.x);

	this->m_shader_buffer.buffer_unmap();
}

void OrbitCamera::upload(rhi::Device &a_device)
{
	this->fill_shader_buffer();
	this->m_shader_buffer.upload(a_device);

	this->update();
}

}        // namespace ror
