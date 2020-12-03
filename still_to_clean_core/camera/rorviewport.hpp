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

#include "foundation/rormacros.h"

namespace ror
{
class ROAR_ENGINE_ITEM Frustrum;
class ROAR_ENGINE_ITEM Viewport
{
  public:
	FORCE_INLINE Viewport(void);
	virtual ~Viewport(void);

	// Accessors and Mutator functions

	FORCE_INLINE void set_ycoord(int32_t a_ycoord);
	FORCE_INLINE void set_xcoord(int32_t a_xcoord);
	FORCE_INLINE void set_height(int32_t a_height);
	FORCE_INLINE void set_width(int32_t a_width);
	FORCE_INLINE void set_values(int32_t *a_values);
	FORCE_INLINE void set_viewport(int32_t *a_values);
	FORCE_INLINE void set_viewport(int32_t a_xcoord, int32_t a_ycoord, int32_t a_width, int32_t a_height);
	FORCE_INLINE int32_t get_ycoord() const;
	FORCE_INLINE int32_t get_xcoord() const;
	FORCE_INLINE int32_t get_height() const;
	FORCE_INLINE int32_t get_width() const;
	FORCE_INLINE void    get_viwport(int32_t &a_xcoord, int32_t &a_ycoord, int32_t &a_width, int32_t &a_height) const;
	FORCE_INLINE int32_t *get_values() const;

  protected:
	friend class ROAR_ENGINE_ITEM Frustrum;

	// Data needed for Viewport
	union
	{
		struct
		{
			int32_t m_xcoord, m_ycoord;
			int32_t m_width, m_height;
		};
		int32_t m_values[4];
	};
};
}        // namespace ror

#include "rorviewport.hh"
