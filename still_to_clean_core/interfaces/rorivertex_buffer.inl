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
FORCE_INLINE IVertexBuffer::IVertexBuffer(void) :
    m_attributes(nullptr),
    m_no_of_attributes(3),
    m_total_no_of_attributes(0),
    m_size_of_attribute(0),
    m_size_of_vbo(0),
    m_vbodata(nullptr)
{}

FORCE_INLINE IVertexBuffer::IVertexBuffer(const IVertexBuffer *a_vertex_buffer) :
    m_attributes(a_vertex_buffer->m_attributes),
    m_no_of_attributes(a_vertex_buffer->m_no_of_attributes),
    m_total_no_of_attributes(a_vertex_buffer->m_total_no_of_attributes),
    m_size_of_attribute(a_vertex_buffer->m_size_of_attribute),
    m_size_of_vbo(a_vertex_buffer->m_size_of_vbo),
    m_vbodata(a_vertex_buffer->m_vbodata)
{}

FORCE_INLINE VertexAttributes *IVertexBuffer::get_attributes() const
{
	return this->m_attributes;
}

FORCE_INLINE void IVertexBuffer::set_attributes(VertexAttributes *a_attributes)
{
	rDELETEARRAY(this->m_attributes);
	this->m_attributes = a_attributes;
}

FORCE_INLINE uint32_t IVertexBuffer::get_no_of_attributes() const
{
	return this->m_no_of_attributes;
}

FORCE_INLINE void IVertexBuffer::set_no_of_attributes(uint32_t a_no_of_attributes)
{
	this->m_no_of_attributes = a_no_of_attributes;
}

FORCE_INLINE uint32_t IVertexBuffer::get_total_no_of_attributes() const
{
	return this->m_total_no_of_attributes;
}

FORCE_INLINE void IVertexBuffer::set_total_no_of_attributes(uint32_t a_total_no_of_attributes)
{
	this->m_total_no_of_attributes = a_total_no_of_attributes;
}

FORCE_INLINE uint32_t IVertexBuffer::get_size_of_attribute() const
{
	return this->m_size_of_attribute;
}

FORCE_INLINE void IVertexBuffer::set_size_of_attribute(uint32_t a_size_of_attributes)
{
	this->m_size_of_attribute = a_size_of_attributes;
}

FORCE_INLINE uint32_t IVertexBuffer::get_size_of_vbo() const
{
	return this->m_size_of_vbo;
}

FORCE_INLINE void IVertexBuffer::set_size_of_vbo(uint32_t a_size_of_vbo)
{
	this->m_size_of_vbo = a_size_of_vbo;
}

FORCE_INLINE float32_t *IVertexBuffer::get_vbodata() const
{
	return this->m_vbodata;
}

FORCE_INLINE void IVertexBuffer::set_vbodata(float32_t *a_vbodata)
{
	rDELETEARRAY(this->m_vbodata);
	this->m_vbodata = a_vbodata;
}

FORCE_INLINE void *IVertexBuffer::map_buffer(rGLEnum a_mapping)
{
	return glMapBuffer(GL_ARRAY_BUFFER, a_mapping);
}

FORCE_INLINE void IVertexBuffer::un_map_buffer()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

}        // namespace ror
