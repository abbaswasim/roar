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

#include "foundation/rorsingleton.h"
#include "interfaces/roriinput_device.h"

namespace ror
{
// Singleton class
class ROAR_ENGINE_ITEM Mouse : public Singleton<Mouse>, public IInputDevice
{
  public:
	// Only Instance Access
	// Overrides from Base to check for m_UseMouse
	//FORCE_INLINE static CMouse* GetInstance();

	// Base Class Functions
	FORCE_INLINE void initialize();
	FORCE_INLINE bool update();
	FORCE_INLINE void shutdown();

	// Mouse functions
	FORCE_INLINE bool is_left_button_down();
	FORCE_INLINE bool is_right_button_down();
	FORCE_INLINE bool is_middle_button_down();
	FORCE_INLINE bool is_any_button_down();

	FORCE_INLINE void set_left_button_down(bool a_value);
	FORCE_INLINE void set_right_button_down(bool a_value);
	FORCE_INLINE void set_middle_button_down(bool a_value);

	FORCE_INLINE int32_t get_old_y() const;
	FORCE_INLINE void    set_old_y(int32_t a_value);
	FORCE_INLINE int32_t get_old_x() const;
	FORCE_INLINE void    set_old_x(int32_t a_value);

	FORCE_INLINE void set_old_xy(int32_t a_xvalue, int32_t a_yvalue);
	FORCE_INLINE void mouse_move(int32_t a_xcoordinate, int32_t a_ycoordinate);        // No Body is using this currently
	FORCE_INLINE void difference(int32_t &a_xcoordinate, int32_t &a_ycoordinate, int32_t &a_xdifference, int32_t &a_ydifference);
	FORCE_INLINE bool set_in_move() const;
	FORCE_INLINE void get_in_move(bool a_in_move);

  protected:
  private:
	// defined Private to Prevent creation
	FORCE_INLINE Mouse(void);
	FORCE_INLINE ~Mouse(void);

	// Make CSingleton<CMouse> a friend
	friend class Singleton<Mouse>;

	bool m_buttons[3];        // 3 Buttons of the mouse
	bool m_use_mouse;         // Flag for enabling and disabling Mouse
	bool m_in_move;           // Flag for saving if its Moving

	int32_t m_old_x;        // Previous X Coordinates of The mouse
	int32_t m_old_y;        // Previous Y Coordinates of The mouse
};
}        // namespace ror

#include "rormouse.inl"
