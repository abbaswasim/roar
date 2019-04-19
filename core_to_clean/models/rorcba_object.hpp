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
#include "rorcba.h"

namespace ror
{
class ROAR_ENGINE_ITEM ccbaobject
{
  public:
	FORCE_INLINE ccbaobject(void);
	FORCE_INLINE ccbaobject(uint32_t a_cbaid, uint32_t a_cbasize, uint32_t a_reference_count);
	FORCE_INLINE ccbaobject(ccbaobject &a_other);
	~ccbaobject(void);

	FORCE_INLINE uint32_t get_cbaid() const;
	FORCE_INLINE void     set_cbaid(uint32_t &a_cbaid);

	FORCE_INLINE uint32_t get_cbasize() const;
	FORCE_INLINE void     set_cbasize(uint32_t a_cbaid);

	FORCE_INLINE uint32_t get_reference_count() const;
	FORCE_INLINE void     set_reference_count(uint32_t &a_cbaid);
	FORCE_INLINE uint32_t increment_reference_count();
	FORCE_INLINE uint32_t decrement_reference_count();

	FORCE_INLINE CCBA *get_cbapointer() const;
	FORCE_INLINE void  set_cbapointer(CCBA *a_cbapointer);

  private:
	uint32_t m_cbaid;                  // CBA ID used by The Renderer	// Not Used
	uint32_t m_cbasize;                // CBA Data size in Bytes		// Not Used
	uint32_t m_reference_count;        // How many times this CBA is Used
	CCBA *   m_cbapointer;             // Pointer to the data of the model
};
}        // namespace ror

#include "rorcba_object.inl"
