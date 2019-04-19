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

#ifndef _WIN32
typedef uint32_t WPARAM;
typedef uint64_t LPARAM;
#	define LOWORD(l) ((l) &0xffff)
#	define HIWORD(l) ((l) >> 16)
#endif

namespace ror
{
class ROAR_ENGINE_ITEM WindowEvent
{
  public:
	FORCE_INLINE WindowEvent(void);
	FORCE_INLINE WindowEvent(WPARAM a_width, LPARAM a_height, uint32_t a_previous_width, uint32_t a_previous_height);
	FORCE_INLINE WindowEvent(const WindowEvent &a_other);
	FORCE_INLINE ~WindowEvent(void);

	FORCE_INLINE WindowEvent &operator=(const WindowEvent &a_other);
	FORCE_INLINE uint32_t get_width() const;
	FORCE_INLINE uint32_t get_height() const;
	FORCE_INLINE WPARAM get_wparam() const;
	FORCE_INLINE void   set_wparam(WPARAM a_wparam);
	FORCE_INLINE LPARAM get_lparam() const;
	FORCE_INLINE void   set_lparam(LPARAM a_lparam);
	FORCE_INLINE uint32_t get_previous_width() const;
	FORCE_INLINE void     set_previous_width(uint32_t a_previous_width);
	FORCE_INLINE uint32_t get_previous_height() const;
	FORCE_INLINE void     set_previous_height(uint32_t a_previous_height);

	//		FORCE_INLINE	int32_t		GetX() const;
	//		FORCE_INLINE	void		SetX(int32_t a_X);
	//		FORCE_INLINE	int32_t		GetY() const;
	//		FORCE_INLINE	void		SetY(int32_t a_Y);

  protected:
  private:
	WPARAM   m_w_param;                // WParam, Width of the window
	LPARAM   m_l_param;                // LParam, Height of the window
	uint32_t m_previous_width;         // Previous Width of the window
	uint32_t m_previous_height;        // Previous Height of the window
	                                   //			int32_t	m_X;					// X Position of the window
	                                   //			int32_t	m_Y;					// y Position of the window
};
}        // namespace ror

#include "rorwindow_event.inl"
