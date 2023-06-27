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
#include <cstddef>
#include <string>
#include <vector>

namespace ror
{

template <class _type>
GLFWwindow *glfw_create_window(std::string a_window_title, int a_width, int a_height)
{
	if (!glfwInit())
	{
		printf("GLFW Init failed, check if you have a working display set!\n");
		exit(EXIT_FAILURE);
	}

	// TODO: Get these from settings
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// TODO: Get title etc from settings
	auto window = glfwCreateWindow(a_width, a_height, a_window_title.c_str(), nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		printf("GLFW Windows can't be created\n");
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(window, glfw_key_callback<_type>);
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
	this->m_window = glfw_create_window<_type>(settings.m_roar_title, static_cast<int32_t>(settings.m_window_dimensions.z), static_cast<int32_t>(settings.m_window_dimensions.w));
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
void glfw_register_drag_event(GLFWwindow *a_window, EventModifier a_mouse_button)
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

	auto drag_test = [draging, &event_system, a_mouse_button](Event &a_event) {
		if (*draging)
		{
			auto event_handle = create_event_handle(EventType::mouse, EventCode::none, a_mouse_button, EventState::drag);
			auto payload      = a_event.get_payload<ror::Vector2d>();
			event_system.notify({event_handle, true, payload});
		}
	};

	auto mouse_down_handle  = create_event_handle(EventType::mouse, EventCode::none, a_mouse_button, EventState::down);
	auto mouse_click_handle = create_event_handle(EventType::mouse, EventCode::none, a_mouse_button, EventState::click);

	event_system.subscribe(mouse_down_handle, drag_set);
	event_system.subscribe(mouse_click_handle, drag_reset);
	event_system.subscribe(mouse_move, drag_test);
}

// Some global glfw events registration
template <class _type>
void glfw_register_for_global_events(GLFWwindow *a_window)
{
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.subscribe(keyboard_esc_click, [a_window](Event &a_event) { (void) a_event; glfwSetWindowShouldClose(a_window, true); });

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

	glfw_register_drag_event<_type>(a_window, EventModifier::left_mouse);
	glfw_register_drag_event<_type>(a_window, EventModifier::right_mouse);
	glfw_register_drag_event<_type>(a_window, EventModifier::middle_mouse);
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
void glfw_key_callback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mode)
{
	(void) a_scancode;

	EventCode     ec = glfw_key_to_event_code(a_key);
	EventModifier em = glfw_key_to_event_modifier(a_mode);
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
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({ror::mouse_move, true, ror::Vector2d{a_x_pos, a_y_pos}});
}

template <class _type>
void glfw_mouse_button_callback(GLFWwindow *a_window, int a_button, int a_action, int a_flags)
{
	(void) a_flags;

	EventModifier em = glfw_button_to_event_modifier(a_button);
	EventState    es = glfw_action_to_event_state(a_action);

	auto  event_handle = create_event_handle(EventType::mouse, EventCode::none, em, es);
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({event_handle, true});
}

template <class _type>
void glfw_mouse_scroll_callback(GLFWwindow *a_window, double a_x_offset, double a_y_offset)
{
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({ror::mouse_scroll, true, ror::Vector2d{a_x_offset, a_y_offset}});
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
