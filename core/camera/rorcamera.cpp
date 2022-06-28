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

namespace ror
{
void OrbitCamera::init(EventSystem &a_event_system)
{
	this->m_drag_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2d>())
		{
			auto vec2     = std::any_cast<ror::Vector2d>(e.m_payload);
			auto modifier = event_modifier(e.m_handle);
			switch (modifier)
			{
				case EventModifier::left_mouse:
					this->left_key_function(vec2.x, vec2.y);
					break;
				case EventModifier::middle_mouse:
					this->middle_key_function(vec2.x, vec2.y);
					break;
				case EventModifier::right_mouse:
					this->right_key_function(vec2.x, vec2.y);
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
		if (e.is_compatible<ror::Vector2i>())
		{
			auto vec2     = std::any_cast<ror::Vector2i>(e.m_payload);
			this->set_bounds(vec2.x, vec2.y);

			// Now update the MVP and the likes
			this->look_at();
		}
	};

	this->m_zoom_callback = [this](ror::Event &e) {
		if (e.is_compatible<ror::Vector2d>())
		{
			auto vec2     = std::any_cast<ror::Vector2d>(e.m_payload);
			this->zoom_by(-vec2.y);

			// Now update the MVP and the likes
			this->look_at();
		}
	};

	this->m_event_system = &a_event_system;
}

void OrbitCamera::enable()
{
	this->m_event_system->subscribe(mouse_left_mouse_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_middle_mouse_drag, this->m_drag_callback);
	this->m_event_system->subscribe(mouse_right_mouse_drag, this->m_drag_callback);

	this->m_event_system->subscribe(window_resize, this->m_resize_callback);
	this->m_event_system->subscribe(buffer_resize, this->m_resize_callback);

	this->m_event_system->subscribe(mouse_scroll, this->m_zoom_callback);
}

void OrbitCamera::disable()
{
	this->m_event_system->unsubscribe(mouse_left_mouse_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_middle_mouse_drag, this->m_drag_callback);
	this->m_event_system->unsubscribe(mouse_right_mouse_drag, this->m_drag_callback);

	this->m_event_system->unsubscribe(window_resize, this->m_resize_callback);
	this->m_event_system->unsubscribe(buffer_resize, this->m_resize_callback);

	this->m_event_system->unsubscribe(mouse_scroll, this->m_zoom_callback);
}

}        // namespace ror
