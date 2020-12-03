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

namespace ror
{
FORCE_INLINE VertexAttributes::VertexAttributes(void) :
    m_vertex_attribute(rVERTICES),
    m_offset(0),
    m_no_of_components(3),
    m_size_of_attribute(sizeof(float32_t) * m_no_of_components),
    m_vbodata(nullptr)
{}

FORCE_INLINE VertexAttributes::~VertexAttributes(void)
{
	this->m_vbodata = nullptr;
	//rDELETEARRAY (m_VBOData);
}

FORCE_INLINE VertexAttributes::EVertexAttribute VertexAttributes::get_vertex_attribute() const
{
	return this->m_vertex_attribute;
}

FORCE_INLINE void VertexAttributes::set_vertex_attribute(EVertexAttribute a_vertex_attribute)
{
	this->m_vertex_attribute = a_vertex_attribute;
}

FORCE_INLINE uint32_t VertexAttributes::get_offset() const
{
	return this->m_offset;
}

FORCE_INLINE void VertexAttributes::set_offset(uint32_t a_offset)
{
	this->m_offset = a_offset;
}

FORCE_INLINE uint32_t VertexAttributes::get_no_of_components() const
{
	return this->m_no_of_components;
}

FORCE_INLINE void VertexAttributes::set_no_of_components(uint32_t a_no_of_component)
{
	this->m_no_of_components  = a_no_of_component;
	this->m_size_of_attribute = this->m_no_of_components * sizeof(float32_t);
}

FORCE_INLINE uint32_t VertexAttributes::get_size_of_attribute() const
{
	return this->m_size_of_attribute;
}

FORCE_INLINE void VertexAttributes::set_size_of_attribute(uint32_t &a_size_of_attribute)
{
	this->m_size_of_attribute = a_size_of_attribute;
}

FORCE_INLINE float32_t *VertexAttributes::get_vbodata() const
{
	return this->m_vbodata;
}

FORCE_INLINE void VertexAttributes::set_vbodata(float32_t *a_vbodata)
{
	//rDELETEARRAY (m_VBOData);
	this->m_vbodata = a_vbodata;
}

}        // namespace ror
