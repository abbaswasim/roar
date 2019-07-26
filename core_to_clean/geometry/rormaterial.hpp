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

#include "appearance/rorcolor_base.h"

namespace ror
{
class ROAR_ENGINE_ITEM Triangle;
class ROAR_ENGINE_ITEM TriangleGroup;
class ROAR_ENGINE_ITEM CCBA;
class ROAR_ENGINE_ITEM Material : public ror::ColorBase
{
  public:
	FORCE_INLINE Material(void);
	~Material(void);

	FORCE_INLINE float32_t get_shine() const;
	FORCE_INLINE void      set_shine(float32_t a_shine);
	FORCE_INLINE void      bind_material();
	FORCE_INLINE const Vector4f *get_emmision() const;
	FORCE_INLINE void            set_emmision(Vector4f a_emmision);
	FORCE_INLINE float32_t get_transparency() const;
	FORCE_INLINE void      set_transparency(float32_t a_transparence);

  private:
	friend class ROAR_ENGINE_ITEM Triangle;
	friend class ROAR_ENGINE_ITEM TriangleGroup;
	friend class ROAR_ENGINE_ITEM CCBA;

	Vector4f  m_emmision;
	float32_t m_shine;
	float32_t m_transparency;
};
}        // namespace ror

#include "rormaterial.hh"
