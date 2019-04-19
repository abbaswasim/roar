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
class ROAR_ENGINE_ITEM MouseEvent
{
  public:
	enum EMouseButton
	{
		rNO_BUTTON,
		rLEFT_BUTTON,
		rMIDDLE_BUTTON,
		rRIGHT_BUTTON,
		rLEFT_BUTTON_DOUBLE,
		rMIDDLE_BUTTON_DOUBLE,
		rRIGHT_BUTTON_DOUBLE,
		rMAX_MOUSE_BUTTON
	};

	enum EMouseWheel
	{
		rNO_MOVEMENT     = 0,
		rUP_MOVEMENT     = -1,
		rDOWN_MOVEMENT   = 1,
		rMAX_MOUSE_WHEEL = 3
	};

	FORCE_INLINE MouseEvent(void);
	FORCE_INLINE MouseEvent(int32_t a_x, int32_t a_y, int32_t a_yinverted, EMouseButton a_button_id = rNO_BUTTON, EMouseWheel a_scroll = rNO_MOVEMENT);
	FORCE_INLINE MouseEvent(const MouseEvent &a_other);
	FORCE_INLINE ~MouseEvent(void);

	FORCE_INLINE MouseEvent &operator=(const MouseEvent &a_other);

	FORCE_INLINE EMouseButton get_button_id() const;
	FORCE_INLINE void         set_button_id(EMouseButton a_button_id);
	FORCE_INLINE EMouseWheel get_scroll() const;
	FORCE_INLINE void        set_scroll(EMouseWheel a_mouse_scroll);
	FORCE_INLINE int32_t get_x() const;
	FORCE_INLINE void    set_x(int32_t a_x);
	FORCE_INLINE int32_t get_y() const;
	FORCE_INLINE void    set_y(int32_t a_y);
	FORCE_INLINE int32_t get_yinverted() const;
	FORCE_INLINE void    set_yinverted(int32_t a_val);

	//	protected:
	//	private:
	EMouseButton m_button_id;        // Button of the mouse
	EMouseWheel  m_scroll;           // Either Up or Down
	int32_t      m_x;                // X Coordinates of The mouse
	int32_t      m_y;                // Y Coordinates of The mouse
	int32_t      m_yinverted;        // Y Coordinates Inverted for Collision detection again OpenGL Coordinates
};
}        // namespace ror

#include "rormouse_event.inl"
