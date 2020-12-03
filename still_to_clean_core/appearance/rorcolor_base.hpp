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

#include "math/rorvector4.h"
#include "memory_manager/rormemory_check.h"

namespace ror
{
class ROAR_ENGINE_ITEM ColorBase
{
  public:
	FORCE_INLINE ColorBase(void);
	~ColorBase(void);

	FORCE_INLINE const Vector4f *get_ambient() const;
	FORCE_INLINE void            set_ambient(Vector4f &a_ambient);
	FORCE_INLINE const Vector4f *get_diffuse() const;
	FORCE_INLINE void            set_diffuse(Vector4f &a_diffuse);
	FORCE_INLINE const Vector4f *get_specular() const;
	FORCE_INLINE void            set_specular(Vector4f &a_specular);

  protected:
	Vector4f m_ambient;
	Vector4f m_diffuse;
	Vector4f m_specular;
};
}        // namespace ror

#include "rorcolor_base.hh"
