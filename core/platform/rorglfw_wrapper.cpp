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

#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "profiling/rorlog.hpp"
#include "rorglfw_wrapper.hpp"

namespace ror
{

EventCode glfw_key_to_event_code(int a_key)
{
	// clang-format off
	switch(a_key)
	{
		case GLFW_KEY_ESCAPE:			return EventCode::esc;
		case GLFW_KEY_SPACE:			return EventCode::space;
		case GLFW_KEY_APOSTROPHE:		return EventCode::apostrophe;
		case GLFW_KEY_COMMA:			return EventCode::comma;
		case GLFW_KEY_MINUS:			return EventCode::minus;
		case GLFW_KEY_PERIOD:			return EventCode::period;
		case GLFW_KEY_SLASH:			return EventCode::slash;
		case GLFW_KEY_0:				return EventCode::n0;
		case GLFW_KEY_1:				return EventCode::n1;
		case GLFW_KEY_2:				return EventCode::n2;
		case GLFW_KEY_3:				return EventCode::n3;
		case GLFW_KEY_4:				return EventCode::n4;
		case GLFW_KEY_5:				return EventCode::n5;
		case GLFW_KEY_6:				return EventCode::n6;
		case GLFW_KEY_7:				return EventCode::n7;
		case GLFW_KEY_8:				return EventCode::n8;
		case GLFW_KEY_9:				return EventCode::n9;
		case GLFW_KEY_SEMICOLON:		return EventCode::semicolon;
		case GLFW_KEY_EQUAL:			return EventCode::equal;
		case GLFW_KEY_A:				return EventCode::a;
		case GLFW_KEY_B:				return EventCode::b;
		case GLFW_KEY_C:				return EventCode::c;
		case GLFW_KEY_D:				return EventCode::d;
		case GLFW_KEY_E:				return EventCode::e;
		case GLFW_KEY_F:				return EventCode::f;
		case GLFW_KEY_G:				return EventCode::g;
		case GLFW_KEY_H:				return EventCode::h;
		case GLFW_KEY_I:				return EventCode::i;
		case GLFW_KEY_J:				return EventCode::j;
		case GLFW_KEY_K:				return EventCode::k;
		case GLFW_KEY_L:				return EventCode::l;
		case GLFW_KEY_M:				return EventCode::m;
		case GLFW_KEY_N:				return EventCode::n;
		case GLFW_KEY_O:				return EventCode::o;
		case GLFW_KEY_P:				return EventCode::p;
		case GLFW_KEY_Q:				return EventCode::q;
		case GLFW_KEY_R:				return EventCode::r;
		case GLFW_KEY_S:				return EventCode::s;
		case GLFW_KEY_T:				return EventCode::t;
		case GLFW_KEY_U:				return EventCode::u;
		case GLFW_KEY_V:				return EventCode::v;
		case GLFW_KEY_W:				return EventCode::w;
		case GLFW_KEY_X:				return EventCode::x;
		case GLFW_KEY_Y:				return EventCode::y;
		case GLFW_KEY_Z:				return EventCode::z;
		case GLFW_KEY_LEFT_BRACKET:		return EventCode::left_bracket;
		case GLFW_KEY_BACKSLASH:		return EventCode::backslash;
		case GLFW_KEY_RIGHT_BRACKET:	return EventCode::right_bracket;
		case GLFW_KEY_GRAVE_ACCENT:		return EventCode::grave_accent;
		case GLFW_KEY_ENTER:			return EventCode::enter;
		case GLFW_KEY_TAB:				return EventCode::tab;
		case GLFW_KEY_BACKSPACE:		return EventCode::back_space;
		case GLFW_KEY_INSERT:			return EventCode::insert;
		case GLFW_KEY_DELETE:			return EventCode::del;
		case GLFW_KEY_RIGHT:			return EventCode::right;
		case GLFW_KEY_LEFT:				return EventCode::left;
		case GLFW_KEY_DOWN:				return EventCode::down;
		case GLFW_KEY_UP:				return EventCode::up;
		case GLFW_KEY_PAGE_UP:			return EventCode::page_up;
		case GLFW_KEY_PAGE_DOWN:		return EventCode::page_down;
		case GLFW_KEY_HOME:				return EventCode::home;
		case GLFW_KEY_END:				return EventCode::end;
		case GLFW_KEY_CAPS_LOCK:		return EventCode::caps_lock;
		case GLFW_KEY_SCROLL_LOCK:		return EventCode::scroll_lock;
		case GLFW_KEY_NUM_LOCK:			return EventCode::num_lock;
		case GLFW_KEY_PRINT_SCREEN:		return EventCode::print_screen;
		case GLFW_KEY_PAUSE:			return EventCode::pause;
		case GLFW_KEY_F1:				return EventCode::f1;
		case GLFW_KEY_F2:				return EventCode::f2;
		case GLFW_KEY_F3:				return EventCode::f3;
		case GLFW_KEY_F4:				return EventCode::f4;
		case GLFW_KEY_F5:				return EventCode::f5;
		case GLFW_KEY_F6:				return EventCode::f6;
		case GLFW_KEY_F7:				return EventCode::f7;
		case GLFW_KEY_F8:				return EventCode::f8;
		case GLFW_KEY_F9:				return EventCode::f9;
		case GLFW_KEY_F10:				return EventCode::f10;
		case GLFW_KEY_F11:				return EventCode::f11;
		case GLFW_KEY_F12:				return EventCode::f12;
		case GLFW_KEY_F13:				return EventCode::f13;
		case GLFW_KEY_F14:				return EventCode::f14;
		case GLFW_KEY_F15:				return EventCode::f15;
		case GLFW_KEY_F16:				return EventCode::f16;
		case GLFW_KEY_F17:				return EventCode::f17;
		case GLFW_KEY_F18:				return EventCode::f18;
		case GLFW_KEY_F19:				return EventCode::f19;
		case GLFW_KEY_F20:				return EventCode::f20;
		case GLFW_KEY_F21:				return EventCode::f21;
		case GLFW_KEY_F22:				return EventCode::f22;
		case GLFW_KEY_F23:				return EventCode::f23;
		case GLFW_KEY_F24:				return EventCode::f24;
		case GLFW_KEY_F25:				return EventCode::f25;
		case GLFW_KEY_KP_0:				return EventCode::num0;
		case GLFW_KEY_KP_1:				return EventCode::num1;
		case GLFW_KEY_KP_2:				return EventCode::num2;
		case GLFW_KEY_KP_3:				return EventCode::num3;
		case GLFW_KEY_KP_4:				return EventCode::num4;
		case GLFW_KEY_KP_5:				return EventCode::num5;
		case GLFW_KEY_KP_6:				return EventCode::num6;
		case GLFW_KEY_KP_7:				return EventCode::num7;
		case GLFW_KEY_KP_8:				return EventCode::num8;
		case GLFW_KEY_KP_9:				return EventCode::num9;
		case GLFW_KEY_KP_DECIMAL:		return EventCode::decimal;
		case GLFW_KEY_KP_DIVIDE:		return EventCode::divide;
		case GLFW_KEY_KP_MULTIPLY:		return EventCode::multiply;
		case GLFW_KEY_KP_SUBTRACT:		return EventCode::subtract;
		case GLFW_KEY_KP_ADD:			return EventCode::add;
		case GLFW_KEY_KP_ENTER:			return EventCode::enter;
		case GLFW_KEY_KP_EQUAL:			return EventCode::equal;
		case GLFW_KEY_LEFT_SHIFT:		return EventCode::left_shift;
		case GLFW_KEY_LEFT_CONTROL:		return EventCode::left_control;
		case GLFW_KEY_LEFT_ALT:			return EventCode::left_alt;
		case GLFW_KEY_LEFT_SUPER:		return EventCode::left_super;
		case GLFW_KEY_RIGHT_SHIFT:		return EventCode::right_shift;
		case GLFW_KEY_RIGHT_CONTROL:	return EventCode::right_control;
		case GLFW_KEY_RIGHT_ALT:		return EventCode::right_alt;
		case GLFW_KEY_RIGHT_SUPER:		return EventCode::right_super;
		case GLFW_KEY_MENU:				return EventCode::menu;
	}
	// clang-format on

	return ror::EventCode::none;
}

// NOTE: Does not support multiple modifiers held at the same time
EventModifier glfw_mode_to_event_modifier(int a_mode)
{
	// clang-format off
	switch (a_mode)
	{
		case GLFW_MOD_SHIFT:        return EventModifier::shift;
		case GLFW_MOD_CONTROL:      return EventModifier::control;
		case GLFW_MOD_ALT:          return EventModifier::option;
		case GLFW_MOD_SUPER:        return EventModifier::command;
		case GLFW_MOD_CAPS_LOCK:    return EventModifier::caps_lock;
		case GLFW_MOD_NUM_LOCK:     return EventModifier::num_lock;
	}
	// clang-format on

	return {};
}

EventModifier glfw_flag_to_event_modifier(int a_flag)
{
	if ((a_flag & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
		return EventModifier::shift;
	else if ((a_flag & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
		return EventModifier::control;
	else if ((a_flag & GLFW_MOD_ALT) == GLFW_MOD_ALT)
		return EventModifier::option;
	else if ((a_flag & GLFW_MOD_SUPER) == GLFW_MOD_SUPER)
		return EventModifier::command;
	else if ((a_flag & GLFW_MOD_CAPS_LOCK) == GLFW_MOD_CAPS_LOCK)
		return EventModifier::caps_lock;
	else if ((a_flag & GLFW_MOD_NUM_LOCK) == GLFW_MOD_NUM_LOCK)
		return EventModifier::num_lock;

	return EventModifier::none;
}

EventState glfw_action_to_event_state(int a_action)
{
	// clang-format off
	switch (a_action)
	{
		case GLFW_PRESS:		return EventState::down;
		case GLFW_RELEASE:		return EventState::click;
		case GLFW_REPEAT:		return EventState::repeat;
	}
	// clang-format on

	return {};
}

EventCode glfw_button_to_event_code(int a_button)
{
	// clang-format off
	switch (a_button)
	{
		case GLFW_MOUSE_BUTTON_LEFT:	return EventCode::left_mouse;
		case GLFW_MOUSE_BUTTON_RIGHT:	return EventCode::right_mouse;
		case GLFW_MOUSE_BUTTON_MIDDLE:  return EventCode::middle_mouse;
	}
	// clang-format on
	return {};
}

EventModifier glfw_window_modifier_state(GLFWwindow *a_window)
{
	auto krd = glfwGetKey(a_window, GLFW_KEY_RIGHT_SUPER);
	auto kld = glfwGetKey(a_window, GLFW_KEY_LEFT_SUPER);
	if (krd == GLFW_PRESS || kld == GLFW_PRESS)
		return EventModifier::command;

	auto krc = glfwGetKey(a_window, GLFW_KEY_RIGHT_CONTROL);
	auto klc = glfwGetKey(a_window,GLFW_KEY_LEFT_CONTROL);
	if (krc == GLFW_PRESS || klc == GLFW_PRESS)
		return EventModifier::control;

	auto krs = glfwGetKey(a_window, GLFW_KEY_RIGHT_SHIFT);
	auto kls = glfwGetKey(a_window,GLFW_KEY_LEFT_SHIFT);
	if (krs == GLFW_PRESS || kls == GLFW_PRESS)
		return EventModifier::shift;

	auto kra = glfwGetKey(a_window, GLFW_KEY_RIGHT_ALT);
	auto kla = glfwGetKey(a_window,GLFW_KEY_LEFT_ALT);
	if (kra == GLFW_PRESS || kla == GLFW_PRESS)
		return EventModifier::option;

	auto krp = glfwGetKey(a_window, GLFW_KEY_CAPS_LOCK);
	if (krp == GLFW_PRESS)
		return EventModifier::caps_lock;

	auto krn = glfwGetKey(a_window, GLFW_KEY_NUM_LOCK);
	if (krn == GLFW_PRESS)
		return EventModifier::num_lock;

	return EventModifier::none;
}


void glfw_error_callback(int a_error, const char *a_description)
{
	(void) a_error;

	ror::log_error("glfw_error {}", a_description);
}

}        // namespace ror
