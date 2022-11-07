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
#include "math/rorvector3.hpp"

namespace ror
{
void OrbitCamera::init(EventSystem &a_event_system)
{
	this->m_move_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2d>())
		{
			auto vec2     = std::any_cast<ror::Vector2d>(e.m_payload);
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

			// Now update the MVP and the likes
			this->look_at();
		}
	};

	this->m_resize_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2ui>())
		{
			auto vec2 = std::any_cast<ror::Vector2ui>(e.m_payload);
			this->set_bounds(vec2.x, vec2.y);

			// Now update the MVP and the likes
			this->look_at();
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
			this->zoom_by(-vec2.y);

			// Now update the MVP and the likes
			this->look_at();
		}
		else
		{
			assert(0 && "Incompatible payload with zoom callback");
		}
	};

	this->m_event_system = &a_event_system;
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
}

void OrbitCamera::fill_shader_buffer()
{
	// This creates the per_view_uniforms UBO with variable name of in_per_view_uniforms
	this->m_shader_buffer.add_entry("mvp_mat4", rhi::Format::float32_4x4, 1);
	this->m_shader_buffer.add_entry("model_mat4", rhi::Format::float32_4x4, 1);
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
	this->m_shader_buffer.buffer_map();

	this->m_shader_buffer.update("mvp_mat4", &this->m_model_view_projection.m_values);
	this->m_shader_buffer.update("model_mat4", &this->m_model.m_values);
	this->m_shader_buffer.update("view_mat4", &this->m_view.m_values);
	this->m_shader_buffer.update("projection_mat4", &this->m_projection.m_values);
	this->m_shader_buffer.update("view_projection_mat4", &this->m_view_projection.m_values);
	this->m_shader_buffer.update("inverse_projection_mat4", &this->m_inverse_projection.m_values);
	this->m_shader_buffer.update("inverse_view_projection_mat4", &this->m_inverse_view_projection.m_values);
	this->m_shader_buffer.update("normal_mat3", &this->m_normal.m_values);
	this->m_shader_buffer.update("camera_position", &this->m_from.x);

	this->m_shader_buffer.buffer_unmap();
}

void OrbitCamera::upload(rhi::Device &a_device)
{
	// Looking to create a UBO for directional light like below
	/*
	  const uint directional_lights_count = @;
	  struct light_type
	  {
	      vec3  color;
	      vec3  direction;
	      float intensity;
	      mat4  mvp;
	  };

	  layout(std140, set = @, binding = @) uniform directional_light_uniform
	  {
	      light_type lights[directional_lights_count];
	  } in_directional_light_uniforms;
	*/
	this->fill_shader_buffer();
	this->m_shader_buffer.upload(a_device);

	this->update();
}

}        // namespace ror
