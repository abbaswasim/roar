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

namespace ror
{
FORCE_INLINE Mouse::Mouse(void)
{
}

FORCE_INLINE Mouse::~Mouse(void)
{
}

FORCE_INLINE bool Mouse::is_left_button_down()
{
	if (this->m_use_mouse)
	{
		if (this->m_buttons[0])
		{
			//m_Buttons[0] = false;
			return true;
		}
	}
	return false;
}

FORCE_INLINE bool Mouse::is_right_button_down()
{
	if (this->m_use_mouse)
	{
		if (this->m_buttons[1])
		{
			//m_Buttons[1] = false;
			return true;
		}
	}
	return false;
}

FORCE_INLINE bool Mouse::is_middle_button_down()
{
	if (this->m_use_mouse)
	{
		if (this->m_buttons[2])
		{
			//m_Buttons[2] = false;
			return true;
		}
	}
	return false;
}

FORCE_INLINE bool Mouse::is_any_button_down()
{
	if (this->m_use_mouse)
	{
		if (this->m_buttons[0] || this->m_buttons[1] || this->m_buttons[2])
			return true;
	}
	return false;
}

FORCE_INLINE void Mouse::set_left_button_down(bool a_value)
{
	if (this->m_use_mouse)
	{
		this->m_buttons[0] = a_value;
	}
}

FORCE_INLINE void Mouse::set_right_button_down(bool a_value)
{
	if (this->m_use_mouse)
	{
		this->m_buttons[1] = a_value;
	}
}

FORCE_INLINE void Mouse::set_middle_button_down(bool a_value)
{
	if (this->m_use_mouse)
	{
		this->m_buttons[2] = a_value;
	}
}

FORCE_INLINE void Mouse::initialize()
{
	this->m_use_mouse  = true;
	this->m_in_move    = false;
	this->m_buttons[0] = false;
	this->m_buttons[1] = false;
	this->m_buttons[2] = false;
}

FORCE_INLINE void Mouse::shutdown()
{
	this->m_use_mouse = false;
	this->m_in_move   = false;
}

//	FORCE_INLINE CMouse* CMouse::GetInstance()
//	{
//		if (m_UseMouse)
//		{
//			// Initialized during first access
//			static CMouse *instance = new CMouse();
//			return instance;
//		}
//	}

FORCE_INLINE int32_t Mouse::get_old_y() const
{
	return this->m_old_y;
}

FORCE_INLINE void Mouse::set_old_y(int32_t a_value)
{
	this->m_old_y = a_value;
}

FORCE_INLINE int32_t Mouse::get_old_x() const
{
	return this->m_old_x;
}

FORCE_INLINE void Mouse::set_old_x(int32_t a_value)
{
	this->m_old_x = a_value;
}

FORCE_INLINE void Mouse::set_old_xy(int32_t a_xvalue, int32_t a_yvalue)
{
	this->m_old_x = a_xvalue, this->m_old_y = a_yvalue;
}

FORCE_INLINE void Mouse::mouse_move(int32_t a_xcoordinate, int32_t a_ycoordinate)
{
	//		int32_t DifX = a_XCoordinate - m_OldX;
	//		int32_t DifY = a_YCoordinate - m_OldY;

	this->m_in_move = true;
	this->m_old_x   = a_xcoordinate;
	this->m_old_y   = a_ycoordinate;
}

FORCE_INLINE void Mouse::difference(int32_t &a_xcoordinate, int32_t &a_ycoordinate, int32_t &a_xdifference, int32_t &a_ydifference)
{
	this->m_in_move = false;
	a_xdifference   = a_xcoordinate - this->m_old_x;
	a_ydifference   = a_ycoordinate - this->m_old_y;

	this->m_old_x = a_xcoordinate;
	this->m_old_y = a_ycoordinate;
}

FORCE_INLINE bool Mouse::update()
{
	return true;
}

FORCE_INLINE void Mouse::get_in_move(bool a_in_move)
{
	this->m_in_move = a_in_move;
}

FORCE_INLINE bool Mouse::set_in_move() const
{
	return this->m_in_move;
}

}        // namespace ror
