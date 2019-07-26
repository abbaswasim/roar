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
#include "rorindex_attributes.h"

namespace ror
{
class ROAR_ENGINE_ITEM IIndexBuffer
{
  public:
	FORCE_INLINE IIndexBuffer(void);
	FORCE_INLINE IIndexBuffer(const IIndexBuffer *a_index_buffer);
	virtual ~IIndexBuffer(void);

	virtual void pure_virtual_function() = 0;        // Just to make sure this is an Interface

	FORCE_INLINE IndexAttributes *get_attributes() const;
	FORCE_INLINE void             set_attributes(IndexAttributes *a_attributes);
	FORCE_INLINE uint32_t get_no_of_groups_in_ibo() const;
	FORCE_INLINE void     set_no_of_groups_in_ibo(uint32_t a_no_of_groups_in_ibo);
	FORCE_INLINE uint32_t get_size_of_ibo() const;
	FORCE_INLINE void     set_size_of_ibo(uint32_t a_size_of_ibo);
	FORCE_INLINE rUsShort *get_ibodata() const;
	FORCE_INLINE void      set_ibodata(rUsShort *a_ibodata);
	FORCE_INLINE void *    map_buffer(rGLEnum a_mapping);
	FORCE_INLINE void      un_map_buffer();

  protected:
	IndexAttributes *m_attributes;                 // Array of Attributes
	uint32_t         m_no_of_groups_in_ibo;        // One IBO can have many Groups of Indices

	uint32_t  m_size_of_ibo;        // Size of IBO in bytes
	rUsShort *m_ibodata;            // Actual data of the VBO, Just a utility in-case we need to update or change
};
}        // namespace ror

#include "roriindex_buffer.hh"
