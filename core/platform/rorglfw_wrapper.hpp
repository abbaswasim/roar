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
#include "foundation/rormacros.hpp"
#include "math/rorvector4.hpp"

#ifdef ROR_RENDER_TYPE_VULKAN
#	include "rhi/vulkan/rorvulkan_common.hpp"
#else
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#if defined(ROR_OS_TYPE_ANY_APPLE)
#	define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(ROR_OS_TYPE_LINUX)
#	define GLFW_EXPOSE_NATIVE_X11
#endif

#include <GLFW/glfw3native.h>

namespace ror
{

template <class _type>
GLFWwindow *glfw_create_window(std::string a_window_title, int a_width, int a_height, int a_depth_bits, int a_samples, bool a_resizable, bool a_always_on_top);

EventCode     glfw_key_to_event_code(int a_key);
EventCode     glfw_button_to_event_code(int a_button);
EventModifier glfw_mode_to_event_modifier(int a_mode);
EventModifier glfw_flag_to_event_modifier(int a_flag);
EventModifier glfw_window_modifier_state(GLFWwindow *a_window);
EventState    glfw_action_to_event_state(int a_state);

void glfw_error_callback(int a_error, const char *a_description);
template <class _type>
void glfw_window_resize_callback(GLFWwindow *a_window, int a_width, int a_height);        // Accepts size in window/screen coordinates so for a 2x scaled retina this is half of the actual size in pixels
template <class _type>
void glfw_key_callback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mode);
template <class _type>
void glfw_buffer_resize_callback(GLFWwindow *a_window, int a_width, int a_height);        // Accepts size in buffer coordinate for a 2x scaled retina this is the actual size in pixels
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
void glfw_file_drop_callback(GLFWwindow *a_window, int a_count, const char **a_paths);

template <class _type>
void glfw_register_drag_event(GLFWwindow *a_window, EventModifier a_mouse_button);
template <class _type>
void glfw_register_for_global_events(GLFWwindow *a_window);

template <class _instance, class _surface>
auto glfw_create_surface(_instance a_instance, _surface &a_surface, void *a_window);

template <class _type>
class ROAR_ENGINE_ITEM GLFWwindowWrapper final
{
  public:
	FORCE_INLINE                    GLFWwindowWrapper()                                     = delete;         //! Copy constructor
	FORCE_INLINE                    GLFWwindowWrapper(const GLFWwindowWrapper &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                    GLFWwindowWrapper(GLFWwindowWrapper &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE GLFWwindowWrapper &operator=(const GLFWwindowWrapper &a_other)             = default;        //! Copy assignment operator
	FORCE_INLINE GLFWwindowWrapper &operator=(GLFWwindowWrapper &&a_other) noexcept         = default;        //! Move assignment operator

	FORCE_INLINE GLFWwindowWrapper(_type *a_pointer) noexcept;
	FORCE_INLINE ~GLFWwindowWrapper() noexcept;

	FORCE_INLINE void        loop();
	FORCE_INLINE GLFWwindow *window();
	FORCE_INLINE ror::Vector4ui dimensions();

  protected:
  private:
	GLFWwindow *m_window{nullptr};        //! The window to which all the calls are directed
};

}        // namespace ror

#include "platform/rorglfw_wrapper.hh"
