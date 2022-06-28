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

#include "event_system/rorevent_system.hpp"
#include "profiling/rorlog.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef _DARWIN_
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

namespace ror
{

template <class _type>
GLFWwindow *glfw_create_window(int a_width, int a_height);

EventCode     glfw_key_to_event_code(int a_key);
EventModifier glfw_key_to_event_modifier(int a_mode);
EventState    glfw_action_to_event_state(int a_state);
EventModifier glfw_button_to_event_modifier(int a_mode);

void glfw_error_callback(int a_error, const char *a_description);
template <class _type>
void glfw_window_resize_callback(GLFWwindow *a_window, int a_width, int a_height);
template <class _type>
void glfw_key_callback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mode);
template <class _type>
void glfw_buffer_resize_callback(GLFWwindow *a_window, int a_width, int a_height);
template <class _type>
void glfw_mouse_callback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mode);
template <class _type>
void glfw_mouse_move_callback(GLFWwindow *a_window, double a_x_pos, double a_y_pos);
template <class _type>
void glfw_mouse_button_callback(GLFWwindow *a_window, int a_button, int a_action, int a_flags);
template <class _type>
void glfw_mouse_scroll_callback(GLFWwindow *a_window, double a_x_offset, double a_y_offset);
template <class _type>
void glfw_gamepad_callback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mode);
template <class _type>
void glfw_file_drop_callback(GLFWwindow* a_window, int a_count, const char** a_paths);

template <class _type>
void glfw_register_drag_event(GLFWwindow *a_window, EventModifier a_mouse_button);
template <class _type>
void glfw_register_for_global_events(GLFWwindow *a_window);

template <class _type>
class ROAR_ENGINE_ITEM GLFWwindowWrapper final
{
  public:
	FORCE_INLINE                    GLFWwindowWrapper()                                     = delete;         //! Copy constructor
	FORCE_INLINE                    GLFWwindowWrapper(const GLFWwindowWrapper &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                    GLFWwindowWrapper(GLFWwindowWrapper &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE GLFWwindowWrapper &operator=(const GLFWwindowWrapper &a_other) = default;                    //! Copy assignment operator
	FORCE_INLINE GLFWwindowWrapper &operator=(GLFWwindowWrapper &&a_other) noexcept = default;                //! Move assignment operator

	FORCE_INLINE GLFWwindowWrapper(void *a_pointer);
	FORCE_INLINE ~GLFWwindowWrapper() noexcept;

	FORCE_INLINE void loop();

  protected:
  private:
	GLFWwindow *m_window;        //! The window to which all the calls are directed
};

}        // namespace ror

#include "platform/rorglfw_wrapper.hh"
