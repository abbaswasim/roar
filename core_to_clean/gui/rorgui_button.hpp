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

#include "rorgui_graphics_element.h"

namespace ror
{
class ROAR_ENGINE_ITEM cguibutton : public ror::cguigraphics_element
{
  public:
	enum EButtonState
	{
		rBUTTON_UP,
		rBUTTON_OVER,
		rBUTTON_DOWN,
		rBUTTON_STATE_MAX
	};

	class ROAR_ENGINE_ITEM ButtonState        // Obsolete
	{
	  public:
		EButtonState m_state;               // State of the button
		std::string  m_display_text;        // A display text of this state
	};

	FORCE_INLINE cguibutton(void);
	virtual ~cguibutton(void);

	FORCE_INLINE void set(cguielement *a_parent, std::string a_text, bool a_visible, bool a_ground, IFunctor *a_call_back, int32_t a_font_index, cguielement::EAlignment a_alignment, Vector4i &a_non_client_area);
	FORCE_INLINE void global_initialize();
	FORCE_INLINE void render();
	FORCE_INLINE std::string get_states(cguibutton::EButtonState a_state) const;
	FORCE_INLINE void        set_states(cguibutton::EButtonState a_state, std::string &a_dispaly_string);
	FORCE_INLINE cguibutton::EButtonState get_current_state() const;
	FORCE_INLINE void                     set_current_state(cguibutton::EButtonState a_state);

	FORCE_INLINE bool check_mouse_events(MouseEvent &a_event);
	FORCE_INLINE bool check_keyboard_events(KeyboardEvent &a_event);
	//FORCE_INLINE bool						CheckWindowEvents(CWindowEvent &a_Event); // Usually Should not be needed

  protected:
	std::string m_states[3];        // All button states text
	//CVector4i				m_TextureOffsets[3];	// All button states' texture offsets
	EButtonState m_current_state;        // Current State of the button
};
}        // namespace ror

#include "rorgui_button.inl"
