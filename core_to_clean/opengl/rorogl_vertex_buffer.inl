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
FORCE_INLINE OGLVertexBuffer::OGLVertexBuffer(void) :
    m_buffer_id(0)
{}

FORCE_INLINE OGLVertexBuffer::OGLVertexBuffer(const OGLVertexBuffer *a_vertex_buffer) :
    m_buffer_id(a_vertex_buffer->m_buffer_id)
{}

FORCE_INLINE uint32_t OGLVertexBuffer::get_buffer_id() const
{
	return this->m_buffer_id;
}

FORCE_INLINE void OGLVertexBuffer::set_buffer_id(uint32_t a_buffer_id)
{
	this->m_buffer_id = a_buffer_id;
}

FORCE_INLINE void OGLVertexBuffer::load_vertex_buffer()
{
	assert(this->m_attributes && "Attributes are null");
	glGenBuffers(1, &this->m_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer_id);

	glBufferData(GL_ARRAY_BUFFER, this->m_size_of_vbo, nullptr, GL_STATIC_DRAW);
	uint32_t current_offset = 0;
	for (uint32_t i = 0; i < this->m_no_of_attributes; i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, current_offset, this->m_attributes[i].get_size_of_attribute() * this->m_total_no_of_attributes,
		                this->m_attributes[i].get_vbodata());
		this->m_attributes[i].set_offset(current_offset);

		// Now Specify where the Data is
		if (ror::VertexAttributes::rVERTICES == this->m_attributes[i].get_vertex_attribute())
		{
			glVertexPointer(this->m_attributes[i].get_no_of_components(), GL_FLOAT, 0, BUFFER_OFFSET(current_offset));
		}
		else if (ror::VertexAttributes::rNORMALS == this->m_attributes[i].get_vertex_attribute())
		{
			glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(current_offset));
		}
		else if (ror::VertexAttributes::rTEXCOORDS == this->m_attributes[i].get_vertex_attribute())
		{
			glTexCoordPointer(this->m_attributes[i].get_no_of_components(), GL_FLOAT, 0, BUFFER_OFFSET(current_offset));
		}
		/*
			// Now Specify where the Data is
			if (i == 0)
			{
				glVertexPointer(this->m_Attributes[0].GetNoOfComponents(), GL_FLOAT, 0, BUFFER_OFFSET(CurrentOffset));
			}
			else if (i == 1)
			{
				glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(CurrentOffset));
			}
			else if (i == 2)
			{
				glTexCoordPointer(this->m_Attributes[2].GetNoOfComponents(), GL_FLOAT, 0, BUFFER_OFFSET(CurrentOffset));
			}
			*/
		current_offset += this->m_attributes[i].get_size_of_attribute() * this->m_total_no_of_attributes;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);        // Now make it passive
}

FORCE_INLINE void OGLVertexBuffer::update_vertex_buffer()
{
	assert(this->m_attributes && "Attributes are null");

	glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer_id);

	// Only the First Two Attributes are Updated which are Vertices and Normals
	for (uint32_t i = 0; i < this->m_no_of_attributes - 1; i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, this->m_attributes[i].get_offset(), m_attributes[i].get_size_of_attribute() * this->m_total_no_of_attributes,
		                this->m_attributes[i].get_vbodata());
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);        // Now make it passive
}

FORCE_INLINE void OGLVertexBuffer::reload_vertex_buffer()
{
	assert(this->m_attributes && "Attributes are null");

	glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer_id);

	for (uint32_t i = 0; i < this->m_no_of_attributes; i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, this->m_attributes[i].get_offset(), m_attributes[i].get_size_of_attribute() * this->m_total_no_of_attributes,
		                this->m_attributes[i].get_vbodata());
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);        // Now make it passive
}

FORCE_INLINE void OGLVertexBuffer::release_vertex_buffer()
{
	glDeleteBuffers(GL_ARRAY_BUFFER, &this->m_buffer_id);
}

FORCE_INLINE void OGLVertexBuffer::bind_buffer(bool a_binding)
{
	if (a_binding)
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->m_buffer_id);
		for (uint32_t i = 0; i < this->m_no_of_attributes; i++)
		{
			ror::VertexAttributes::EVertexAttribute current_vertix_attribute = this->m_attributes[i].get_vertex_attribute();

			if (ror::VertexAttributes::rVERTICES == current_vertix_attribute)
			{
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(this->m_attributes[0].get_no_of_components(), GL_FLOAT, 0, BUFFER_OFFSET(this->m_attributes[i].get_offset()));
			}
			else if (ror::VertexAttributes::rNORMALS == current_vertix_attribute)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(this->m_attributes[i].get_offset()));
			}
			else if (ror::VertexAttributes::rTEXCOORDS == current_vertix_attribute)
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(this->m_attributes[i].get_no_of_components(), GL_FLOAT, 0, BUFFER_OFFSET(this->m_attributes[i].get_offset()));
			}
		}
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		for (uint32_t i = 0; i < this->m_no_of_attributes; i++)
		{
			ror::VertexAttributes::EVertexAttribute current_vertix_attribute = this->m_attributes[i].get_vertex_attribute();
			if (ror::VertexAttributes::rVERTICES == current_vertix_attribute)
				glDisableClientState(GL_VERTEX_ARRAY);
			else if (ror::VertexAttributes::rNORMALS == current_vertix_attribute)
				glDisableClientState(GL_NORMAL_ARRAY);
			else if (ror::VertexAttributes::rTEXCOORDS == current_vertix_attribute)
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
}

FORCE_INLINE void OGLVertexBuffer::draw_arrays(rGLEnum a_primtives)
{
	glDrawArrays(a_primtives, 0, this->m_total_no_of_attributes);
}

}        // namespace ror
