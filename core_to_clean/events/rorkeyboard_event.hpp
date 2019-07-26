// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2019
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

#pragma once

#include "memory_manager/rormemory_check.h"

namespace ror
{
class ROAR_ENGINE_ITEM KeyboardEvent
{
  public:
	enum EKeyboardKeys
	{
		rKEY_NONE         = 0,
		rKEY_PAUSE        = 19,
		rKEY_ENTER        = 13,
		rKEY_SPACE        = 32,
		rKEY_ESCAPE       = 27,
		rKEY_BACK_SPACE   = 8,
		rKEY_NUMPAD_PLUS  = 107,
		rKEY_NUMPAD_MINUS = 109,

		rKEY_END = 35,
		rKEY_HOME,
		rKEY_LEFT = 37,
		rKEY_UP,
		rKEY_RIGHT,
		rKEY_DOWN,

		rKEY_DELETE = 46,
		rKEY_0      = 48,
		rKEY_1,
		rKEY_2,
		rKEY_3,
		rKEY_4,
		rKEY_5,
		rKEY_6,
		rKEY_7,
		rKEY_8,
		rKEY_9,

		rKEY_A = 65,
		rKEY_B,
		rKEY_C,
		rKEY_D,
		rKEY_E,
		rKEY_F,
		rKEY_G,
		rKEY_H,
		rKEY_I,
		rKEY_J,
		rKEY_K,
		rKEY_L,
		rKEY_M,
		rKEY_N,
		rKEY_O,
		rKEY_P,
		rKEY_Q,
		rKEY_R,
		rKEY_S,
		rKEY_T,
		rKEY_U,
		rKEY_V,
		rKEY_W,
		rKEY_X,
		rKEY_Y,
		rKEY_Z,

		rKEY_NUMPAD0 = 96,
		rKEY_NUMPAD1,
		rKEY_NUMPAD2,
		rKEY_NUMPAD3,
		rKEY_NUMPAD4,
		rKEY_NUMPAD5,
		rKEY_NUMPAD6,
		rKEY_NUMPAD7,
		rKEY_NUMPAD8,
		rKEY_NUMPAD9,

		rKEY_F1 = 112,
		rKEY_F2,
		rKEY_F3,
		rKEY_F4,
		rKEY_F5,
		rKEY_F6,
		rKEY_F7,
		rKEY_F8,
		rKEY_F9,
		rKEY_F10,
		rKEY_F11,
		rKEY_F12,

		rKEY_L_SHIFT = 160,
		rKEY_R_SHIFT,
		rKEY_L_CONTROL,
		rKEY_R_CONTROL,
		rKEY_L_MENU,
		rKEY_R_MENU
	};

	FORCE_INLINE KeyboardEvent(void);
	FORCE_INLINE KeyboardEvent(EKeyboardKeys a_button_id);
	FORCE_INLINE KeyboardEvent(const KeyboardEvent &a_other);
	FORCE_INLINE ~KeyboardEvent(void);

	FORCE_INLINE KeyboardEvent &operator=(const KeyboardEvent &a_other);

	FORCE_INLINE EKeyboardKeys get_button_id() const;
	FORCE_INLINE void          set_button_id(EKeyboardKeys a_button_id);

  protected:
  private:
	EKeyboardKeys m_button_id;        // Button of the Keyboard
};
}        // namespace ror

#include "rorkeyboard_event.hh"
