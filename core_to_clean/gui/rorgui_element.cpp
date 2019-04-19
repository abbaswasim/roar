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

#include "rorgui_element.h"

namespace ror
{
ror::Mouse *   cguielement::m_mouse_device    = ror::Mouse::get_instance();
ror::Keyboard *cguielement::m_keyboard_device = ror::Keyboard::get_instance();

cguielement::~cguielement(void)
{
	rDELETE(this->m_call_back);
	rDELETE(this->m_background);
	rDELETE(this->m_foreground);
}

iguievent_listener *cguielement::get_event_listener()
{
	return (this->m_parent ? this->m_parent->get_event_listener() : nullptr);
}

bool cguielement::check_mouse_events(MouseEvent &a_event)
{
	if (a_event.m_x > this->m_absolute_position.x && a_event.m_x < this->m_absolute_position.x + this->m_bounds.x &&
	    a_event.m_yinverted > this->m_absolute_position.y && a_event.m_yinverted < this->m_absolute_position.y + this->m_bounds.y)
	{
		if (this->m_mouse_device->is_left_button_down())
		{
			this->m_pressed = true;
		}
		else
		{
			if (true == this->m_pressed)
			{
				if (nullptr != this->m_call_back)
				{
					(*this->m_call_back)();        // Here we call the CallBack for the Element Button
				}
				this->m_pressed = false;
			}
		}
		return true;
	}
	return false;
}

bool cguielement::check_keyboard_events(KeyboardEvent &a_event)
{
	return false;
}

bool cguielement::check_window_events(WindowEvent &a_event)
{
	if (nullptr == this->m_parent)        // If some one don't have a parent
	{
		if (rTOP_LEFT == this->m_alignment)        // Change Only Y
		{
			this->m_position.y = a_event.get_height() - (a_event.get_previous_height() - (this->m_position.y + this->m_bounds.y) + this->m_bounds.y);
		}
		else if (rBOTTOM_RIGHT == this->m_alignment)        // Change Only X
		{
			this->m_position.x = a_event.get_width() - (a_event.get_previous_width() - (this->m_position.x + this->m_bounds.x) + this->m_bounds.x);
		}
		else if (rTOP_RIGHT == this->m_alignment)        // Change Both
		{
			this->m_position.x = a_event.get_width() - (a_event.get_previous_width() - (this->m_position.x + this->m_bounds.x) + this->m_bounds.x);
			this->m_position.y = a_event.get_height() - (a_event.get_previous_height() - (this->m_position.y + this->m_bounds.y) + this->m_bounds.y);
		}
		else if (rBOTTOM_LEFT == this->m_alignment)        // Don't do anything
		{}
		this->m_absolute_position = this->m_position;
	}
	else
	{
		this->m_absolute_position = this->m_position + this->m_parent->get_position();
	}

	return true;
}

}        // namespace ror
