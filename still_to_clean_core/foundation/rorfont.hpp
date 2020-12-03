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
//#include INCLUDERENDERER
//#include "RORSingleton.h"

namespace ror
{
class ROAR_ENGINE_ITEM Font
{
  public:
	FORCE_INLINE Font(std::string a_font_name = "Courier New", int32_t a_height = 12, bool a_bold = true, bool a_italic = false);
	FORCE_INLINE ~Font(void);

	void             font_init(std::string a_font_name, int32_t a_height, bool a_bold, bool a_italic);
	FORCE_INLINE void print(int32_t x, int32_t y, const char8_t *string, ...);
	FORCE_INLINE void safe_print(int32_t x, int32_t y, const char8_t *string, ...);
	FORCE_INLINE void font_cleanup();
	FORCE_INLINE void begin_text_mode(void);
	FORCE_INLINE void end_text_mode(void);
	FORCE_INLINE uint32_t get_height() const;
	FORCE_INLINE void     set_height(uint32_t a_height);
	FORCE_INLINE uint32_t get_width() const;
	FORCE_INLINE void     set_width(uint32_t a_width);

  private:
	uint32_t m_text_base;
	uint32_t m_text_mode_list;
	uint32_t m_height;
	uint32_t m_width;
};
}        // namespace ror

#include "rorfont.hh"
