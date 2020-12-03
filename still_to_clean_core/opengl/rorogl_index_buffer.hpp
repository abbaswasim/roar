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

#include "interfaces/roriindex_buffer.h"

namespace ror
{
class ROAR_ENGINE_ITEM OGLIndexBuffer : public ror::IIndexBuffer
{
  public:
	FORCE_INLINE OGLIndexBuffer(void);
	FORCE_INLINE OGLIndexBuffer(const OGLIndexBuffer *a_index_buffer);
	~OGLIndexBuffer(void);

	void pure_virtual_function(){};

	FORCE_INLINE uint32_t get_buffer_id() const;
	FORCE_INLINE void     set_buffer_id(uint32_t a_buffer_id);
	FORCE_INLINE void     load_index_buffer();
	FORCE_INLINE void     release_index_buffer();
	FORCE_INLINE void     bind_buffer(bool a_binding);
	// DrawElements Should be called to Draw the VBO Data associated with this IBO
	FORCE_INLINE void draw_elements(uint32_t a_group_number);

  private:
	uint32_t m_buffer_id;
};
}        // namespace ror

#include "rorogl_index_buffer.hh"
