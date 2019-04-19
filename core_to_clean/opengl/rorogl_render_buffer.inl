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
FORCE_INLINE OGLRenderBuffer::OGLRenderBuffer(void) :
    m_buffer_id(0),
    m_width(0),
    m_height(0),
    m_internal_format(rGL_DEPTH_COMPONENT)
{}

FORCE_INLINE OGLRenderBuffer::OGLRenderBuffer(const OGLRenderBuffer *a_render_buffer) :
    m_buffer_id(a_render_buffer->m_buffer_id),
    m_width(a_render_buffer->m_width),
    m_height(a_render_buffer->m_height),
    m_internal_format(a_render_buffer->m_internal_format)
{}

FORCE_INLINE uint32_t OGLRenderBuffer::get_buffer_id() const
{
	return this->m_buffer_id;
}

FORCE_INLINE void OGLRenderBuffer::set_buffer_id(uint32_t a_buffer_id)
{
	this->m_buffer_id = a_buffer_id;
}

FORCE_INLINE void OGLRenderBuffer::set_internal_format(EInternalFormat a_internal_format)
{
	m_internal_format = a_internal_format;
}

FORCE_INLINE ror::OGLRenderBuffer::EInternalFormat OGLRenderBuffer::get_internal_format() const
{
	return m_internal_format;
}

FORCE_INLINE void OGLRenderBuffer::load_render_buffer(EInternalFormat a_internal_format, uint32_t a_width, uint32_t a_height)
{
	//assert(this->m_Attributes && "Attributes are null");
	this->m_internal_format = a_internal_format;
	this->m_width           = a_width;
	this->m_height          = a_height;

	glGenRenderbuffersEXT(1, &this->m_buffer_id);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->m_buffer_id);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, this->m_internal_format, this->m_width, this->m_height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

FORCE_INLINE void OGLRenderBuffer::bind_buffer(bool a_binding)
{
	if (a_binding)
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->m_buffer_id);
	else
		glBindFramebufferEXT(GL_RENDERBUFFER_EXT, 0);
}

}        // namespace ror
