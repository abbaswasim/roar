// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN a_ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// Version: 1.0.0

#pragma once

#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorutilities.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "platform/rorglfw_wrapper.hpp"
#include "settings/rorsettings.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <vector>

namespace ror
{

template <class _type>
GLFWwindow *glfw_create_window(std::string a_window_title, int a_width, int a_height, int a_depth_bits, int a_samples, bool a_resizable, bool a_always_on_top)
{
	if (!glfwInit())
	{
		printf("GLFW Init failed, check if you have a working display set!\n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_DEPTH_BITS, a_depth_bits);
	glfwWindowHint(GLFW_SAMPLES, a_samples);
	glfwWindowHint(GLFW_RESIZABLE, a_resizable);
	glfwWindowHint(GLFW_FLOATING, a_always_on_top);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	auto window = glfwCreateWindow(a_width, a_height, a_window_title.c_str(), nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		printf("GLFW Windows can't be created\n");
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(window, glfw_key_callback<_type>);
	glfwSetWindowFocusCallback(window, glfw_window_focus_callback<_type>);             // Accepts focus as an integer, 0 is focus lost and anything else is focus gained
	glfwSetWindowSizeCallback(window, glfw_window_resize_callback<_type>);             // Accepts size in window/screen coordinate so for a 2x scaled retina this is half of the actual size in pixels
	glfwSetFramebufferSizeCallback(window, glfw_buffer_resize_callback<_type>);        // Accepts size in buffer coordinate for a 2x scaled retina this is the actual size in pixels
	glfwSetCursorPosCallback(window, glfw_mouse_move_callback<_type>);
	glfwSetMouseButtonCallback(window, glfw_mouse_button_callback<_type>);
	glfwSetScrollCallback(window, glfw_mouse_scroll_callback<_type>);
	glfwSetDropCallback(window, glfw_file_drop_callback<_type>);

	return window;
}

template <class _type>
GLFWwindowWrapper<_type>::GLFWwindowWrapper(_type *a_pointer) noexcept
{
	auto &settings = ror::settings();
	this->m_window = glfw_create_window<_type>(settings.m_roar_title, static_cast<int32_t>(settings.m_window.m_dimensions.z), static_cast<int32_t>(settings.m_window.m_dimensions.w),
	                                           static_cast<int32_t>(settings.m_depth_buffer_bits), static_cast<int32_t>(settings.m_multisample_count), settings.m_resizable, settings.m_always_on_top);
	glfwSetWindowUserPointer(this->m_window, a_pointer);
	glfw_register_for_global_events<_type>(this->m_window);
}

template <class _type>
GLFWwindowWrapper<_type>::~GLFWwindowWrapper() noexcept
{
	glfwDestroyWindow(this->m_window);
	glfwTerminate();
}

template <class _type>
FORCE_INLINE _type *glfw_user_pointer(GLFWwindow *a_window)
{
	auto *app = static_cast<_type *>(glfwGetWindowUserPointer(a_window));
	assert(app && "User pointer is null");

	return app;
}

template <class _type>
FORCE_INLINE void GLFWwindowWrapper<_type>::loop()
{
	auto *app = glfw_user_pointer<_type>(this->m_window);
	while (!glfwWindowShouldClose(this->m_window))
	{
		glfwPollEvents();
		app->update();
	}
}

template <class _type>
FORCE_INLINE GLFWwindow *GLFWwindowWrapper<_type>::window()
{
	return this->m_window;
}

template <class _type>
FORCE_INLINE ror::Vector4ui GLFWwindowWrapper<_type>::dimensions()
{
	/*
	  Almost all positions and sizes in GLFW are measured in screen coordinates relative to one of the two origins above.
	  This includes cursor positions, window positions and sizes, window frame sizes, monitor positions and video mode resolutions.

	  Two exceptions are the monitor physical size, which is measured in millimetres, and framebuffer size, which is measured in pixels.

	  if dimensions() is only used for calculing scale, that can also be retried from glfw directly:
	  float xscale, yscale;
	  GLFWmonitor* primary = glfwGetPrimaryMonitor();
	  glfwGetMonitorContentScale(primary, &xscale, &yscale);
	*/

	ror::Vector4i dimensions;

	glfwGetFramebufferSize(this->m_window, &dimensions.x, &dimensions.y);
	glfwGetWindowSize(this->m_window, &dimensions.z, &dimensions.w);

	return {static_cast<uint32_t>(dimensions.x), static_cast<uint32_t>(dimensions.y),
	        static_cast<uint32_t>(dimensions.z), static_cast<uint32_t>(dimensions.w)};
}

template <class _type>
FORCE_INLINE EventSystem &glfw_event_system(GLFWwindow *a_window)
{
	auto *app = glfw_user_pointer<_type>(a_window);

	return app->event_system();
}

template <class _type>
void glfw_register_drag_event(GLFWwindow *a_window, EventCode a_mouse_button, EventModifier a_modifier)
{
	auto                 &event_system = glfw_event_system<_type>(a_window);
	std::shared_ptr<bool> draging      = std::make_shared<bool>(false);        //! Using a pointer here because it needs to be captured by reference in the lambdas below

	auto drag_set = [draging](Event &a_event) {
		(void) a_event;
		*draging = true;
	};

	auto drag_reset = [draging](Event &a_event) {
		(void) a_event;
		*draging = false;
	};

	// Since this is using the same coordinates as mouse move, this will only work if inserted before a move
	// This way drag relies on previous frame move changing mouse position and then using this frame drag to calculate delta
	auto drag_test = [a_window, draging, &event_system, a_mouse_button](Event &a_event) {
		if (*draging)
		{
			EventModifier em           = glfw_window_modifier_state(a_window);
			auto          event_handle = create_event_handle(EventType::mouse, a_mouse_button, em, EventState::drag);
			auto          payload      = a_event.get_payload<ror::Vector2d>();
			event_system.notify({event_handle, true, payload});
		}
	};

	auto mouse_down_handle  = create_event_handle(EventType::mouse, a_mouse_button, a_modifier, EventState::down);
	auto mouse_click_handle = create_event_handle(EventType::mouse, a_mouse_button, a_modifier, EventState::click);
	auto mouse_move_handle  = create_event_handle(EventType::mouse, EventCode::none, a_modifier, EventState::move);

	event_system.subscribe(mouse_down_handle, drag_set);
	event_system.subscribe(mouse_click_handle, drag_reset);
	event_system.subscribe_early(mouse_move_handle, drag_test);        // Needs to be before any other move, read above
}

// Some global glfw events registration
template <class _type>
void glfw_register_for_global_events(GLFWwindow *a_window)
{
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.subscribe(keyboard_esc_click, [a_window](Event &a_event) { (void) a_event; glfwSetWindowShouldClose(a_window, true); });
	event_system.subscribe(keyboard_enter_command_click, [a_window](Event &a_event) {
		(void) a_event;
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();

		static bool fullscreen_mode{false};
		static int  width{};
		static int  height{};
		static int  xpos{};
		static int  ypos{};

		if (!fullscreen_mode)
		{
			glfwGetWindowPos(a_window, &xpos, &ypos);
			glfwGetWindowSize(a_window, &width, &height);
			assert(monitor && "Primary monitor can't be nullptr");

			const GLFWvidmode *mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(a_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			fullscreen_mode = true;
		}
		else
		{
			fullscreen_mode = false;
			glfwSetWindowMonitor(a_window, nullptr, xpos, ypos, width, height, GLFW_DONT_CARE);
		}
	});

	// One can use the following type of functions to implement mouse move, left click, scroll etc
	/*
	event_system.subscribe(mouse_move, [](Event &a_event) { (void) a_event; auto v = a_event.get_payload<ror::Vector2d>(); std::cout << "moving (" << v.x << "," << v.y << ")" << std::endl;});
	event_system.subscribe(mouse_left_mouse_click, [](Event &a_event) { (void) a_event; std::cout << "left clicking" << std::endl; });
	event_system.subscribe(mouse_right_mouse_click, [](Event &a_event) { (void) a_event; std::cout << "right clicking" << std::endl; });
	auto mouse_scroll = create_event_handle(EventType::mouse, EventCode::none, EventModifier::none, EventState::scroll);
	event_system.subscribe(mouse_scroll, [](Event &a_event) { (void) a_event; std::cout << "scrolling" << std::endl; });
	event_system.subscribe(mouse_left_mouse_drag, [](Event &a_event) { (void) a_event; std::cout << "left draging" << std::endl; });
	event_system.subscribe(mouse_right_mouse_drag, [](Event &a_event) { (void) a_event; std::cout << "right draging" << std::endl; });
	event_system.subscribe(mouse_middle_mouse_drag, [](Event &a_event) { (void) a_event; std::cout << "middle draging" << std::endl; });
	*/

	glfw_register_drag_event<_type>(a_window, EventCode::left_mouse, EventModifier::none);
	glfw_register_drag_event<_type>(a_window, EventCode::right_mouse, EventModifier::none);
	glfw_register_drag_event<_type>(a_window, EventCode::middle_mouse, EventModifier::none);

	glfw_register_drag_event<_type>(a_window, EventCode::left_mouse, EventModifier::command);
	glfw_register_drag_event<_type>(a_window, EventCode::right_mouse, EventModifier::command);
	glfw_register_drag_event<_type>(a_window, EventCode::middle_mouse, EventModifier::command);

	glfw_register_drag_event<_type>(a_window, EventCode::left_mouse, EventModifier::control);
	glfw_register_drag_event<_type>(a_window, EventCode::right_mouse, EventModifier::control);
	glfw_register_drag_event<_type>(a_window, EventCode::middle_mouse, EventModifier::control);

	glfw_register_drag_event<_type>(a_window, EventCode::left_mouse, EventModifier::shift);
	glfw_register_drag_event<_type>(a_window, EventCode::right_mouse, EventModifier::shift);
	glfw_register_drag_event<_type>(a_window, EventCode::middle_mouse, EventModifier::shift);
}

template <class _instance, class _surface>
auto glfw_create_surface(_instance a_instance, _surface &a_surface, void *a_window)
{
	return glfwCreateWindowSurface(a_instance, reinterpret_cast<GLFWwindow *>(a_window), nullptr, &a_surface);
}

// Accepts size in window/screen coordinate so for a 2x scaled retina this is half of the actual size in pixels
template <class _type>
void glfw_window_resize_callback(GLFWwindow *a_window, int a_width, int a_height)
{
	if (a_width <= 0 || a_height <= 0)
		return;

	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({ror::window_resize, true, ror::Vector2ui{static_cast<uint32_t>(a_width), static_cast<uint32_t>(a_height)}});
}

template <class _type>
void glfw_window_focus_callback(GLFWwindow *a_window, int a_focus)
{
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({ror::window_focus, true, static_cast<uint32_t>(a_focus)});
}

template <class _type>
void glfw_key_callback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mode)
{
	(void) a_scancode;

	EventCode     ec = glfw_key_to_event_code(a_key);
	EventModifier em = glfw_mode_to_event_modifier(a_mode);
	EventState    es = glfw_action_to_event_state(a_action);

	auto  event_handle = create_event_handle(EventType::keyboard, ec, em, es);
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({event_handle, true});
}

// Accepts size in buffer coordinate for a 2x scaled retina this is the actual size in pixels
template <class _type>
void glfw_buffer_resize_callback(GLFWwindow *a_window, int a_width, int a_height)
{
	if (a_width <= 0 || a_height <= 0)
		return;

	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({ror::buffer_resize, true, ror::Vector2ui{static_cast<uint32_t>(a_width), static_cast<uint32_t>(a_height)}});
}

template <class _type>
void glfw_mouse_move_callback(GLFWwindow *a_window, double a_x_pos, double a_y_pos)
{
	auto         &event_system      = glfw_event_system<_type>(a_window);
	EventModifier em                = glfw_window_modifier_state(a_window);
	auto          mouse_move_handle = create_event_handle(EventType::mouse, EventCode::none, em, EventState::move);

	event_system.notify({mouse_move_handle, true, ror::Vector2d{a_x_pos, a_y_pos}});
}

template <class _type>
void glfw_mouse_button_callback(GLFWwindow *a_window, int a_button, int a_action, int a_flags)
{
	EventModifier em = glfw_flag_to_event_modifier(a_flags);
	EventCode     ec = glfw_button_to_event_code(a_button);
	EventState    es = glfw_action_to_event_state(a_action);

	auto  event_handle = create_event_handle(EventType::mouse, ec, em, es);
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({event_handle, true});
}

template <class _type>
void glfw_mouse_scroll_callback(GLFWwindow *a_window, double a_x_offset, double a_y_offset)
{
	auto &event_system = glfw_event_system<_type>(a_window);

	EventModifier em           = glfw_window_modifier_state(a_window);
	auto          event_handle = create_event_handle(EventType::mouse, EventCode::none, em, EventState::scroll);

	event_system.notify({event_handle, true, ror::Vector2d{a_x_offset, a_y_offset}});
}

template <class _type>
void glfw_file_drop_callback(GLFWwindow *a_window, int a_count, const char **a_paths)
{
	(void) a_window;

	std::vector<std::string> paths;
	paths.reserve(static_cast<size_t>(a_count));

	for (int i = 0; i < a_count; i++)
		paths.emplace_back(a_paths[i]);

	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({ror::file_drop, true, paths});
}

}        // namespace ror
