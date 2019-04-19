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
FORCE_INLINE Keyboard::Keyboard(void)
{
}

FORCE_INLINE void Keyboard::setkey_down(rUsChar a_key)
{
	if (this->m_use_key_board)
	{
		this->m_keys[a_key] = true;
	}
}

FORCE_INLINE void Keyboard::setkey_up(rUsChar a_key)
{
	if (this->m_use_key_board)
	{
		this->m_keys[a_key] = false;
	}
}

FORCE_INLINE bool Keyboard::is_key_down(rUsChar a_key)
{
	if (this->m_use_key_board)
	{
		if (this->m_keys[a_key])
		{
			this->m_keys[a_key] = false;
			return true;
		}
		return false;
	}
	return false;
}

FORCE_INLINE bool Keyboard::is_key_up(rUsChar a_key)
{
	if (this->m_use_key_board)
	{
		return !this->m_keys[a_key];
	}
}

FORCE_INLINE void Keyboard::initialize()
{
	this->m_use_key_board = true;
}

FORCE_INLINE void Keyboard::shutdown()
{
	this->m_use_key_board = false;
}

FORCE_INLINE bool Keyboard::update()
{
	return true;
}
}        // namespace ror
