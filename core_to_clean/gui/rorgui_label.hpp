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

#include "rorgui_graphics_element.h"

namespace ror
{
class ROAR_ENGINE_ITEM cguilabel : public ror::cguigraphics_element
{
  public:
	FORCE_INLINE cguilabel(void);
	virtual ~cguilabel(void);

	FORCE_INLINE void set(cguielement *a_parent, std::string a_text, bool a_visible, bool a_ground, IFunctor *a_call_back, int32_t a_font_index, cguielement::EAlignment a_alignment, Vector4i &a_non_client_area);
	FORCE_INLINE void global_initialize();
	FORCE_INLINE void render();

  protected:
	// No Specialized Data
};
}        // namespace ror

#include "rorgui_label.hh"
