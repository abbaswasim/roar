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
FORCE_INLINE Viewport::Viewport(void) :
    m_xcoord(0),
    m_ycoord(0),
    m_width(1),
    m_height(1)
{
}

FORCE_INLINE int32_t Viewport::get_ycoord() const
{
	return this->m_ycoord;
}

FORCE_INLINE void Viewport::set_ycoord(int32_t a_ycoord)
{
	this->m_ycoord = a_ycoord;
}

FORCE_INLINE int32_t Viewport::get_xcoord() const
{
	return this->m_xcoord;
}

FORCE_INLINE void Viewport::set_xcoord(int32_t a_xcoord)
{
	this->m_xcoord = a_xcoord;
}

FORCE_INLINE void Viewport::set_height(int32_t a_height)
{
	this->m_height = a_height;
}

FORCE_INLINE int32_t Viewport::get_height() const
{
	return this->m_height;
}

FORCE_INLINE int32_t Viewport::get_width() const
{
	return this->m_width;
}

FORCE_INLINE void Viewport::set_width(int32_t a_width)
{
	this->m_width = a_width;
}

FORCE_INLINE void Viewport::set_viewport(int32_t a_xcoord, int32_t a_ycoord, int32_t a_width, int32_t a_height)
{
	this->m_xcoord = a_xcoord;
	this->m_ycoord = a_ycoord;
	this->m_width  = a_width;
	this->m_height = a_height;
}

FORCE_INLINE void Viewport::get_viwport(int32_t &a_xcoord, int32_t &a_ycoord, int32_t &a_width, int32_t &a_height) const
{
	a_xcoord = this->m_xcoord;
	a_ycoord = this->m_ycoord;
	a_height = this->m_height;
	a_width  = this->m_width;
}
//
//	FORCE_INLINE int32_t* CViewport::GetValues() const
//	{
//		return m_Values;
//	}
//
//	FORCE_INLINE void CViewport::SetValues( int32_t* a_Values )
//	{
//		m_Values = *a_Values;
//	}
//
//	FORCE_INLINE void CViewport::SetViewport( int32_t* a_Values )
//	{
//		m_Values = *a_Values;
//	}
//
}        // namespace ror
