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
FORCE_INLINE MouseEvent::MouseEvent(void)
{
	this->m_button_id = rNO_BUTTON;
	this->m_scroll    = rNO_MOVEMENT;
	this->m_x         = 0;
	this->m_y         = 1;
}

FORCE_INLINE MouseEvent::~MouseEvent(void)
{}

FORCE_INLINE MouseEvent::MouseEvent(int32_t a_x, int32_t a_y, int32_t a_yinverted, EMouseButton a_button_id, EMouseWheel a_scroll)
{
	this->m_x         = a_x;
	this->m_y         = a_y;
	this->m_yinverted = a_yinverted;
	this->m_button_id = a_button_id;
	this->m_scroll    = a_scroll;
}

FORCE_INLINE MouseEvent::MouseEvent(const MouseEvent &a_other)
{
	this->operator=(a_other);
}

FORCE_INLINE MouseEvent &MouseEvent::operator=(const MouseEvent &a_other)
{
	if (this != &a_other)
	{
		this->m_button_id = a_other.m_button_id;
		this->m_scroll    = a_other.m_scroll;
		this->m_x         = a_other.m_x;
		this->m_y         = a_other.m_y;
	}
	return *this;
}

FORCE_INLINE MouseEvent::EMouseButton MouseEvent::get_button_id() const
{
	return this->m_button_id;
}

FORCE_INLINE void MouseEvent::set_button_id(MouseEvent::EMouseButton a_button_id)
{
	this->m_button_id = a_button_id;
}

FORCE_INLINE MouseEvent::EMouseWheel MouseEvent::get_scroll() const
{
	return this->m_scroll;
}

FORCE_INLINE void MouseEvent::set_scroll(MouseEvent::EMouseWheel a_mouse_scroll)
{
	this->m_scroll = a_mouse_scroll;
}

FORCE_INLINE int32_t MouseEvent::get_x() const
{
	return this->m_x;
}

FORCE_INLINE void MouseEvent::set_x(int32_t a_x)
{
	this->m_x = a_x;
}

FORCE_INLINE int32_t MouseEvent::get_y() const
{
	return this->m_y;
}

FORCE_INLINE void MouseEvent::set_y(int32_t a_y)
{
	this->m_y = a_y;
}

FORCE_INLINE int32_t MouseEvent::get_yinverted() const
{
	return this->m_yinverted;
}

FORCE_INLINE void MouseEvent::set_yinverted(int32_t a_val)
{
	this->m_yinverted = a_val;
}

}        // namespace ror
