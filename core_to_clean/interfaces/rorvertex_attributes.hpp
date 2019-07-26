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
class ROAR_ENGINE_ITEM VertexAttributes
{
  public:
	enum EVertexAttribute
	{
		rVERTICES,
		rNORMALS,
		rTEXCOORDS,
		rTANGENTS,
		rBINORMALS,
		rVERTEX_ATTRIBUTES_MAX
	};

	FORCE_INLINE VertexAttributes();
	FORCE_INLINE ~VertexAttributes();

	FORCE_INLINE EVertexAttribute get_vertex_attribute() const;
	FORCE_INLINE void             set_vertex_attribute(EVertexAttribute a_vertex_attribute);
	FORCE_INLINE uint32_t get_offset() const;
	FORCE_INLINE void     set_offset(uint32_t a_offset);
	FORCE_INLINE uint32_t get_no_of_components() const;
	FORCE_INLINE void     set_no_of_components(uint32_t a_no_of_component);
	FORCE_INLINE uint32_t get_size_of_attribute() const;
	FORCE_INLINE float32_t *get_vbodata() const;
	FORCE_INLINE void       set_vbodata(float32_t *a_vbodata);

  protected:
  private:
	FORCE_INLINE void set_size_of_attribute(uint32_t &a_size_of_attribute);

	EVertexAttribute m_vertex_attribute;         // Can be Vertices, Normals etc
	uint32_t         m_offset;                   // The offset in the VBO for current Vertex Attribute
	uint32_t         m_no_of_components;         // Vertices are 3 (x,y,z) TexCoords are 2 (s,t) component
	uint32_t         m_size_of_attribute;        // Size of a Attributes is NoOfComponents * float
	float32_t *      m_vbodata;                  // Data for this Attribute
};
}        // namespace ror

#include "rorvertex_attributes.hh"
