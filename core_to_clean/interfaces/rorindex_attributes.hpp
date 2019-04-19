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

namespace ror
{
class ROAR_ENGINE_ITEM IndexAttributes
{
  public:
	FORCE_INLINE IndexAttributes();
	FORCE_INLINE ~IndexAttributes();

	FORCE_INLINE uint32_t get_offset() const;
	FORCE_INLINE void     set_offset(uint32_t a_offset);
	FORCE_INLINE uint32_t get_no_of_attribute() const;
	FORCE_INLINE void     set_no_of_attribute(uint32_t &a_no_of_attribute);
	FORCE_INLINE rUsShort *get_ibodata() const;
	FORCE_INLINE void      set_ibodata(rUsShort *a_ibodata);
	FORCE_INLINE uint32_t get_indices_minimum() const;
	FORCE_INLINE void     set_indices_minimum(uint32_t &a_indices_min);
	FORCE_INLINE uint32_t get_indices_maximum() const;
	FORCE_INLINE void     set_indices_maximum(uint32_t &a_indices_max);

  protected:
  private:
	uint32_t  m_offset;                  // The offset in the VBO for current Vertex Attribute
	uint32_t  m_no_of_attributes;        // Total number of Indices in this Attribute
	rUsShort *m_ibodata;                 // Data for this Attribute (CIndexAttributes does not take ownership)

	uint32_t m_indices_minimum;        // Minimum and Maximum Indices in the Group
	uint32_t m_indices_maximum;
};
}        // namespace ror

#include "rorindex_attributes.inl"
