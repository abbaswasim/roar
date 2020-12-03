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
FORCE_INLINE OGLIndexBuffer::OGLIndexBuffer(void) :
    m_buffer_id(0)
{}

FORCE_INLINE OGLIndexBuffer::OGLIndexBuffer(const OGLIndexBuffer *a_index_buffer) :
    m_buffer_id(a_index_buffer->m_buffer_id)
{}

FORCE_INLINE uint32_t OGLIndexBuffer::get_buffer_id() const
{
	return this->m_buffer_id;
}

FORCE_INLINE void OGLIndexBuffer::set_buffer_id(uint32_t a_buffer_id)
{
	this->m_buffer_id = a_buffer_id;
}

FORCE_INLINE void OGLIndexBuffer::load_index_buffer()
{
	assert(this->m_attributes && "Attributes is Null");
	glGenBuffers(1, &this->m_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_buffer_id);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_size_of_ibo, nullptr, GL_STATIC_DRAW);
	for (uint32_t i = 0; i < this->m_no_of_groups_in_ibo; i++)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, this->m_attributes[i].get_offset(),
		                this->m_attributes[i].get_no_of_attribute() * sizeof(rUsShort),
		                this->m_attributes[i].get_ibodata());
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);        // Now make it passive
}

FORCE_INLINE void OGLIndexBuffer::release_index_buffer()
{
	glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &this->m_buffer_id);
}

FORCE_INLINE void OGLIndexBuffer::bind_buffer(bool a_binding)
{
	if (a_binding)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_buffer_id);
	else
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

FORCE_INLINE void OGLIndexBuffer::draw_elements(uint32_t a_group_number)
{
	assert(a_group_number < this->m_no_of_groups_in_ibo && "There are no triangle groups in the buffer");
	glDrawRangeElements(GL_TRIANGLES, this->m_attributes[a_group_number].get_indices_minimum(),
	                    this->m_attributes[a_group_number].get_indices_maximum(), this->m_attributes[a_group_number].get_no_of_attribute(),
	                    GL_UNSIGNED_SHORT, BUFFER_OFFSET(this->m_attributes[a_group_number].get_offset()));
}

}        // namespace ror
