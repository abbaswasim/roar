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
FORCE_INLINE OGLFrameBuffer::OGLFrameBuffer(void) :
    m_buffer_id(0),
    m_geometry_node(nullptr)
{}

FORCE_INLINE OGLFrameBuffer::OGLFrameBuffer(const OGLFrameBuffer *a_frame_buffer) :
    m_buffer_id(a_frame_buffer->m_buffer_id),
    m_geometry_node(a_frame_buffer->m_geometry_node)
{}

FORCE_INLINE uint32_t OGLFrameBuffer::get_buffer_id() const
{
	return this->m_buffer_id;
}

FORCE_INLINE void OGLFrameBuffer::set_buffer_id(uint32_t a_buffer_id)
{
	this->m_buffer_id = a_buffer_id;
}

FORCE_INLINE void OGLFrameBuffer::load_frame_buffer()
{
	glGenFramebuffersEXT(1, &this->m_buffer_id);
}

FORCE_INLINE void OGLFrameBuffer::bind_buffer(bool a_binding)
{
	if (a_binding)
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->m_buffer_id);
	else
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

FORCE_INLINE void OGLFrameBuffer::set_read_buffer(const EAttachment a_color_attachment)
{
	glReadBuffer(a_color_attachment);
}

FORCE_INLINE void OGLFrameBuffer::set_draw_buffer(const EAttachment a_color_attachment)
{
	glDrawBuffer(a_color_attachment);
}

FORCE_INLINE const GeometryNode *OGLFrameBuffer::get_geometry_node() const
{
	return this->m_geometry_node;
}

FORCE_INLINE void OGLFrameBuffer::set_geometry_node(const ror::GeometryNode *a_geometry_node)
{
	this->m_geometry_node = a_geometry_node;
}

FORCE_INLINE void OGLFrameBuffer::set_draw_buffers(const uint32_t a_size, const EAttachment *a_color_attachment)
{
	//		rGLEnum *Attachments[a_Size];
	//		glDrawBuffers(a_Size, a_ColorAttachment);
}

FORCE_INLINE void OGLFrameBuffer::start_render()
{
	glGetIntegerv(GL_VIEWPORT, this->m_viewport);
	glViewport(0, 0,
	           this->m_geometry_node->get_texture_object()->get_texture_width(),
	           this->m_geometry_node->get_texture_object()->get_texture_height());
	this->bind_buffer(true);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

FORCE_INLINE void OGLFrameBuffer::end_render()
{
	this->m_geometry_node->get_texture_object()->update_mip_maps();
	glViewport(this->m_viewport[0], this->m_viewport[1], this->m_viewport[2], this->m_viewport[3]);
	this->bind_buffer(false);
}

FORCE_INLINE void OGLFrameBuffer::attach_texture_object(ror::TextureObject *a_texture_object, OGLFrameBuffer::EAttachment a_attachement)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->m_buffer_id);

	if (a_attachement < OGLFrameBuffer::rGL_COLOR_ATTACHEMENT_MAX)
	{
		if (TextureObject::rGL_TEXTURE_2D == a_texture_object->get_texture_target())
		{
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, a_attachement, GL_TEXTURE_2D, a_texture_object->get_texture_id(), 0);
		}
		else if (TextureObject::rGL_TEXTURE_1D == a_texture_object->get_texture_target())
		{
			glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, a_attachement, GL_TEXTURE_1D, a_texture_object->get_texture_id(), 0);
		}
		else if (TextureObject::rGL_TEXTURE_3D == a_texture_object->get_texture_target())
		{
			glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, a_attachement, GL_TEXTURE_3D, a_texture_object->get_texture_id(), 0, 0);
		}

		//this->m_NextColorAttachment = (EAttachment)((uint32_t)this->m_NextColorAttachment + 1);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

FORCE_INLINE void OGLFrameBuffer::attach_render_buffer(ror::OGLRenderBuffer *a_render_buffer, EAttachment a_attachment)
{
	a_render_buffer->bind_buffer(true);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->m_buffer_id);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, a_attachment, GL_RENDERBUFFER_EXT, a_render_buffer->get_buffer_id());
	//a_RenderBuffer->BindBuffer(false);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

}        // namespace ror
