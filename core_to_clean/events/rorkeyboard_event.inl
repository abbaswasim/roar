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
FORCE_INLINE KeyboardEvent::KeyboardEvent(void)
{
	this->m_button_id = rKEY_NONE;
}

FORCE_INLINE KeyboardEvent::~KeyboardEvent(void)
{}

FORCE_INLINE KeyboardEvent::KeyboardEvent(EKeyboardKeys a_button_id)
{
	this->m_button_id = a_button_id;
}

FORCE_INLINE KeyboardEvent::KeyboardEvent(const KeyboardEvent &a_other)
{
	this->operator=(a_other);
}

FORCE_INLINE KeyboardEvent &KeyboardEvent::operator=(const KeyboardEvent &a_other)
{
	if (this != &a_other)
	{
		this->m_button_id = a_other.m_button_id;
	}
	return *this;
}

FORCE_INLINE KeyboardEvent::EKeyboardKeys KeyboardEvent::get_button_id() const
{
	return this->m_button_id;
}

FORCE_INLINE void KeyboardEvent::set_button_id(KeyboardEvent::EKeyboardKeys a_button_id)
{
	this->m_button_id = a_button_id;
}

}        // namespace ror
