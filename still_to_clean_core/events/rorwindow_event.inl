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
FORCE_INLINE WindowEvent::WindowEvent(void)
{
	this->m_w_param         = 0;
	this->m_l_param         = 1;
	this->m_previous_width  = 0;
	this->m_previous_height = 0;
}

FORCE_INLINE WindowEvent::~WindowEvent(void)
{}

FORCE_INLINE WindowEvent::WindowEvent(WPARAM a_width, LPARAM a_height, uint32_t a_previous_width, uint32_t a_previous_height)
{
	this->m_w_param         = a_width;
	this->m_l_param         = a_height;
	this->m_previous_width  = a_previous_width;
	this->m_previous_height = a_previous_height;
}

FORCE_INLINE WindowEvent::WindowEvent(const WindowEvent &a_other)
{
	this->operator=(a_other);
}

FORCE_INLINE WindowEvent &WindowEvent::operator=(const WindowEvent &a_other)
{
	if (this != &a_other)
	{
		this->m_w_param = a_other.m_w_param;
		this->m_l_param = a_other.m_l_param;
	}
	return *this;
}

FORCE_INLINE WPARAM WindowEvent::get_wparam() const
{
	return this->m_w_param;
}

FORCE_INLINE void WindowEvent::set_wparam(WPARAM a_wparam)
{
	this->m_w_param = a_wparam;
}

FORCE_INLINE LPARAM WindowEvent::get_lparam() const
{
	return this->m_l_param;
}

FORCE_INLINE void WindowEvent::set_lparam(LPARAM a_lparam)
{
	this->m_l_param = a_lparam;
}

FORCE_INLINE uint32_t WindowEvent::get_previous_width() const
{
	return this->m_previous_width;
}

FORCE_INLINE void WindowEvent::set_previous_width(uint32_t a_previous_width)
{
	this->m_previous_width = a_previous_width;
}

FORCE_INLINE uint32_t WindowEvent::get_previous_height() const
{
	return this->m_previous_height;
}

FORCE_INLINE void WindowEvent::set_previous_height(uint32_t a_previous_height)
{
	this->m_previous_height = a_previous_height;
}

FORCE_INLINE uint32_t WindowEvent::get_width() const
{
	return LOWORD(this->m_l_param);
}

FORCE_INLINE uint32_t WindowEvent::get_height() const
{
	return HIWORD(this->m_l_param);
}

//	FORCE_INLINE	uint32_t CWindowEvent::GetX() const
//	{
//		return this->m_X;
//	}
//
//	FORCE_INLINE	void CWindowEvent::SetX( uint32_t a_X )
//	{
//		this->m_X = a_X;
//	}
//
//	FORCE_INLINE	uint32_t CWindowEvent::GetY() const
//	{
//		return this->m_Y;
//	}
//
//	FORCE_INLINE	void CWindowEvent::SetY( uint32_t a_Y )
//	{
//		this->m_Y = a_Y;
//	}

}        // namespace ror
