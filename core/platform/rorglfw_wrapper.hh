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
#include "math/rorvector2.hpp"
#include "platform/rorglfw_wrapper.hpp"
#include <cstddef>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

namespace ror
{

template <class _type>
GLFWwindow *glfw_create_window(int a_width, int a_height)
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
	auto window = glfwCreateWindow(a_width, a_height, "Roar Editor...", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		printf("GLFW Windows can't be created\n");
		exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(window, glfw_key_callback<_type>);
	glfwSetWindowSizeCallback(window, glfw_window_resize_callback<_type>);
	glfwSetFramebufferSizeCallback(window, glfw_buffer_resize_callback<_type>);
	glfwSetCursorPosCallback(window, glfw_mouse_move_callback<_type>);
	glfwSetMouseButtonCallback(window, glfw_mouse_button_callback<_type>);
	glfwSetScrollCallback(window, glfw_mouse_scroll_callback<_type>);
	glfwSetDropCallback(window, glfw_file_drop_callback<_type>);

	int width, height;

	glfwGetFramebufferSize(window, &width, &height);

	// TODO: Use the width and height

	return window;
}

template <class _type>
GLFWwindowWrapper<_type>::GLFWwindowWrapper(void *a_pointer)
{
	// TODO: Read size from settings
	this->m_window = glfw_create_window<_type>(1024, 768);
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
void GLFWwindowWrapper<_type>::loop()
{
	while (!glfwWindowShouldClose(this->m_window))
	{
		glfwPollEvents();
	}
}

template <class _type>
FORCE_INLINE _type *glfw_user_pointer(GLFWwindow *a_window)
{
	auto *app = static_cast<_type *>(glfwGetWindowUserPointer(a_window));
	assert(app && "User pointer is null");

	return app;
}

template <class _type>
FORCE_INLINE EventSystem &glfw_event_system(GLFWwindow *a_window)
{
	auto *app = glfw_user_pointer<_type>(a_window);

	return app->event_system();
}

// Some global glfw events registeration
template <class _type>
void glfw_register_for_global_events(GLFWwindow *a_window)
{
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.subscribe(keyboard_esc_click, [a_window](Event &a_event) { (void) a_event; glfwSetWindowShouldClose(a_window, true); });

	// One can use the following type of functions to implement mouse move, left click, scroll etc
	/*
	auto mouse_scroll = create_event_handle(EventType::mouse, EventCode::none, EventModifier::none, EventState::scroll);
	event_system.subscribe(mouse_move, [](Event &a_event) { (void) a_event; std::cout << "moving" << std::endl; });
	event_system.subscribe(mouse_left_mouse_click, [](Event &a_event) { (void) a_event; std::cout << "left clicking" << std::endl; });
	event_system.subscribe(mouse_right_mouse_click, [](Event &a_event) { (void) a_event; std::cout << "right clicking" << std::endl; });
	event_system.subscribe(mouse_scroll, [](Event &a_event) { (void) a_event; std::cout << "scrolling" << std::endl; });
	event_system.subscribe(mouse_left_mouse_drag, [](Event &a_event) { (void) a_event; std::cout << "draging" << std::endl; });
	*/

	std::shared_ptr<bool> draging = std::make_shared<bool>(false);        //! Using a pointer here because it needs to be captured by reference in the lambdas below

	auto drag_set = [draging](Event &a_event) {
		(void) a_event;
		*draging = true;
	};

	auto drag_reset = [draging](Event &a_event) {
		(void) a_event;
		*draging = false;
	};

	auto drag_test = [draging, event_system](Event &a_event) {
		(void) a_event;

		if (*draging)
		{
			auto event_handle = create_event_handle(EventType::mouse, EventCode::none, EventModifier::left_mouse, EventState::drag);
			event_system.notify({event_handle, true});
		}
	};

	event_system.subscribe(mouse_left_mouse_down, drag_set);
	event_system.subscribe(mouse_left_mouse_click, drag_reset);
	event_system.subscribe(mouse_move, drag_test);
}

template <class _type>
void glfw_window_resize_callback(GLFWwindow *a_window, int a_width, int a_height)
{
	(void) a_window;

	if (a_width == 0 || a_height == 0)
		return;

	auto *app = glfw_user_pointer<_type>(a_window);

	app->resize(a_width, a_height);
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

template <class _type>
void glfw_buffer_resize_callback(GLFWwindow *a_window, int a_width, int a_height)
{
	auto  event_handle = create_event_handle(EventType::buffer, EventCode::none, EventModifier::none, EventState::resize);
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({event_handle, true, ror::Vector2i{a_width, a_height}});
}

template <class _type>
void glfw_mouse_move_callback(GLFWwindow *a_window, double a_x_pos, double a_y_pos)
{
	auto  event_handle = create_event_handle(EventType::mouse, EventCode::none, EventModifier::none, EventState::move);
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({event_handle, true, ror::Vector2d{a_x_pos, a_y_pos}});
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
	auto  event_handle = create_event_handle(EventType::mouse, EventCode::none, EventModifier::none, EventState::scroll);
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({event_handle, true, ror::Vector2d{a_x_offset, a_y_offset}});
}

template <class _type>
void glfw_file_drop_callback(GLFWwindow* a_window, int a_count, const char** a_paths)
{
	(void) a_window;

	std::vector<std::string> paths;
	paths.reserve(static_cast<size_t>(a_count));

	for (int i = 0;  i < a_count;  i++)
		paths.emplace_back(a_paths[i]);

	auto  event_handle = create_event_handle(EventType::file, EventCode::none, EventModifier::none, EventState::drop);
	auto &event_system = glfw_event_system<_type>(a_window);
	event_system.notify({event_handle, true, paths});
}

}        // namespace ror
