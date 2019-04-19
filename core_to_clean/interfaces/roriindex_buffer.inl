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
FORCE_INLINE IIndexBuffer::IIndexBuffer(void) :
    m_attributes(nullptr),
    m_no_of_groups_in_ibo(1),
    m_size_of_ibo(0),
    m_ibodata(nullptr)
{}

FORCE_INLINE IIndexBuffer::IIndexBuffer(const IIndexBuffer *a_index_buffer) :
    m_attributes(a_index_buffer->m_attributes),
    m_no_of_groups_in_ibo(a_index_buffer->m_no_of_groups_in_ibo),
    m_size_of_ibo(a_index_buffer->m_size_of_ibo),
    m_ibodata(a_index_buffer->m_ibodata)
{}

FORCE_INLINE IndexAttributes *IIndexBuffer::get_attributes() const
{
	return m_attributes;
}

FORCE_INLINE void IIndexBuffer::set_attributes(IndexAttributes *a_attributes)
{
	rDELETEARRAY(m_attributes);
	m_attributes = a_attributes;
}

FORCE_INLINE uint32_t IIndexBuffer::get_no_of_groups_in_ibo() const
{
	return m_no_of_groups_in_ibo;
}

FORCE_INLINE void IIndexBuffer::set_no_of_groups_in_ibo(uint32_t a_no_of_groups_in_ibo)
{
	m_no_of_groups_in_ibo = a_no_of_groups_in_ibo;
}

FORCE_INLINE uint32_t IIndexBuffer::get_size_of_ibo() const
{
	return m_size_of_ibo;
}

FORCE_INLINE void IIndexBuffer::set_size_of_ibo(uint32_t a_size_of_ibo)
{
	m_size_of_ibo = a_size_of_ibo;
}

FORCE_INLINE rUsShort *IIndexBuffer::get_ibodata() const
{
	return m_ibodata;
}

FORCE_INLINE void IIndexBuffer::set_ibodata(rUsShort *a_ibodata)
{
	m_ibodata = a_ibodata;
}

FORCE_INLINE void *IIndexBuffer::map_buffer(rGLEnum a_mapping)
{
	return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, a_mapping);
}

FORCE_INLINE void IIndexBuffer::un_map_buffer()
{
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

}        // namespace ror
