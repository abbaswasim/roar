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
#include "math/rorvector4.hpp"
#include "math/rorvector_functions.hpp"
#include "profiling/rorlog.hpp"
#include "renderer/rorrenderer.hpp"

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

	this->m_mode_callback = [this](ror::Event &) {
		this->m_mode = this->m_mode == CameraMode::orbit ? CameraMode::fps : CameraMode::orbit;
	};

	this->m_reset_callback = [this](ror::Event &) {
		this->m_center = this->m_target;
		this->m_y_fov  = this->m_y_fov_target;
		this->update_projection();
		this->update_view();
	};

	this->m_event_system = &a_event_system;
}

void OrbitCamera::reset()
{
	this->update_view();
}

void OrbitCamera::orient_top()
{
	auto r      = ror::distance(this->m_eye, this->m_center);
	this->m_eye = ror::Vector3f(this->m_center.x, this->m_center.y + r, this->m_center.z);

	this->update_view(ror::zaxis3f_negative);
}

void OrbitCamera::orient_bottom()
{
	auto r      = ror::distance(this->m_eye, this->m_center);
	this->m_eye = ror::Vector3f(this->m_center.x, this->m_center.y - r, this->m_center.z);

	this->update_view(ror::zaxis3f);
}

void OrbitCamera::orient_left()
{
	auto r      = ror::distance(this->m_eye, this->m_center);
	this->m_eye = ror::Vector3f(this->m_center.x - r, this->m_center.x, this->m_center.z);

	this->update_view(ror::yaxis3f);
}

void OrbitCamera::orient_right()
{
	auto r      = ror::distance(this->m_eye, this->m_center);
	this->m_eye = ror::Vector3f(this->m_center.x + r, this->m_center.x, this->m_center.z);

	this->update_view(ror::yaxis3f);
}

void OrbitCamera::orient_front()
{
	auto r      = ror::distance(this->m_eye, this->m_center);
	this->m_eye = ror::Vector3f(this->m_center.x, this->m_center.y, this->m_center.z + r);

	this->update_view(ror::yaxis3f);
}

void OrbitCamera::orient_back()
{
	auto r      = ror::distance(this->m_eye, this->m_center);
	this->m_eye = ror::Vector3f(this->m_center.x, this->m_center.y, this->m_center.z - r);

	this->update_view(ror::yaxis3f);
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

void OrbitCamera::update_view(Vector3f a_up)        // Up default is {0.0f, 1.0f, 0.0f} the real world up vector to orient itself
{
	// Applying a translation matrix to a vector should have no effect. We can achieve this by setting the vector’s w component to 0, which zeroes out the translational component of a transformation matrix.
	// On the other hand, we need points to be able to translate, so we set their w component to 1. This gives us the 3D translation results we want.

	// Another way to calculate view transformation is to take the inverse of the camera’s model-to-world transformation.
	this->m_view = ror::make_look_at(this->m_eye, this->m_center, a_up);

	this->update_vectors();
}

void OrbitCamera::update_projection()
{
	// Setup perspective projection matrix
	this->m_aspect_ratio = this->m_width / this->m_height;
	ror::log_info("near = {} far = {} in {}", this->m_z_near, m_z_far, __FUNCTION__);
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

		this->m_projection = ror::make_ortho(-xmag, xmag, -ymag, ymag, this->m_z_near, this->m_z_far);
	}
}

void OrbitCamera::setup_frustums()
{
	this->update_view();
	this->update_projection();

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
	this->m_target = this->m_center;
	this->m_eye.x  = this->m_center.x;
	this->m_eye.y  = this->m_center.y + (diagonal.x / 4.0f);
	this->m_eye.z  = ((std::max(diagonal.x, diagonal.y) / 2.0f) / std::tan(ror::to_radians(this->m_y_fov / 2)));

	this->update_view();
	this->update_projection();
	this->setup_frustums();
}

void OrbitCamera::rotate(float32_t a_x_delta, float32_t a_y_delta)
{
	ror::AxisAnglef yinput{this->m_right, ror::to_radians(a_y_delta)};

	auto origin        = this->m_mode == CameraMode::orbit ? this->m_center : this->m_eye;
	auto eye_to_center = this->m_center - this->m_eye;

	if (eye_to_center.dot_product(this->m_forward) < 0.0f)        // Means eye to center is backwards to forward
		this->m_center = origin = this->m_eye + this->m_forward;

	auto translation0 = ror::matrix4_translation(-origin);
	auto translation1 = ror::matrix4_translation(origin);
	auto rotation_x   = ror::matrix4_rotation(yinput);
	auto rotation_y   = ror::matrix4_rotation_around_y(ror::to_radians(a_x_delta));

	if (this->m_mode == CameraMode::orbit)
		this->m_eye = translation1 * rotation_y * rotation_x * translation0 * this->m_eye;
	else
		this->m_center = translation1 * rotation_y * rotation_x * translation0 * this->m_center;

	static const Vector3f roar_world_up{0.0f, 1.0f, 0.0f};        //! World Up vector to orient itself

	// If current up and world up are closer together than don't reset
	if (this->m_up.dot_product(roar_world_up) > 0.2f)
		this->reset();
	else
	{
		yinput     = {this->m_right, ror::to_radians(-a_y_delta)};
		rotation_x = ror::matrix4_rotation(yinput);
		rotation_y = ror::matrix4_rotation_around_y(ror::to_radians(-a_x_delta));

		// Rotate the view matrix directly instead of reacreating it via look_at
		this->m_view *= translation1 * rotation_y * rotation_x * translation0;

		this->update_vectors();
	}

	this->update_normal();
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
	this->m_event_system->subscribe(mouse_control_scroll, this->m_zoom_callback);

	this->m_event_system->subscribe(keyboard_space_click, this->m_mode_callback);
	this->m_event_system->subscribe(keyboard_r_click, this->m_reset_callback);
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
	this->m_event_system->unsubscribe(mouse_control_scroll, this->m_zoom_callback);

	this->m_event_system->unsubscribe(keyboard_space_click, this->m_mode_callback);
}

void OrbitCamera::update(const Renderer &a_renderer) const
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

void OrbitCamera::upload(rhi::Device &)
{}

}        // namespace ror
