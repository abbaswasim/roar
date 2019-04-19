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

#include "appearance/rortexture_object.h"
#include "interfaces/roriframe_buffer.h"
#include "rorogl_render_buffer.h"
#include "scene_graph/rorgeometry_node.h"

namespace ror
{
class ROAR_ENGINE_ITEM OGLFrameBuffer : public ror::IFrameBuffer
{
  public:
	enum EAttachment
	{
		rGL_COLOR_ATTACHMENT0  = 0x8ce0,
		rGL_COLOR_ATTACHMENT1  = 0x8ce1,
		rGL_COLOR_ATTACHMENT2  = 0x8ce2,
		rGL_COLOR_ATTACHMENT3  = 0x8ce3,
		rGL_COLOR_ATTACHMENT4  = 0x8ce4,
		rGL_COLOR_ATTACHMENT5  = 0x8ce5,
		rGL_COLOR_ATTACHMENT6  = 0x8ce6,
		rGL_COLOR_ATTACHMENT7  = 0x8ce7,
		rGL_COLOR_ATTACHMENT8  = 0x8ce8,
		rGL_COLOR_ATTACHMENT9  = 0x8ce9,
		rGL_COLOR_ATTACHMENT10 = 0x8cea,
		rGL_COLOR_ATTACHMENT11 = 0x8ceb,
		rGL_COLOR_ATTACHMENT12 = 0x8cec,
		rGL_COLOR_ATTACHMENT13 = 0x8ced,
		rGL_COLOR_ATTACHMENT14 = 0x8cee,
		rGL_COLOR_ATTACHMENT15 = 0x8cef,
		rGL_COLOR_ATTACHEMENT_MAX,
		rGL_ATTACH_DEPTH   = 0x8d00,
		rGL_ATTACH_STENCIL = 0x8d20,
		rGL_ATTACHEMENT_MAX
	};

	FORCE_INLINE OGLFrameBuffer(void);
	FORCE_INLINE OGLFrameBuffer(const OGLFrameBuffer *a_frame_buffer);
	~OGLFrameBuffer(void);

	void pure_virtual_function(){};

	FORCE_INLINE void bind_buffer(bool a_binding);
	FORCE_INLINE uint32_t get_buffer_id() const;
	FORCE_INLINE void     set_buffer_id(uint32_t a_buffer_id);
	FORCE_INLINE void     set_read_buffer(const ror::OGLFrameBuffer::EAttachment a_color_attachment);
	FORCE_INLINE void     set_draw_buffer(const ror::OGLFrameBuffer::EAttachment a_color_attachment);
	FORCE_INLINE void     set_draw_buffers(const uint32_t a_size, const ror::OGLFrameBuffer::EAttachment *a_color_attachment);
	FORCE_INLINE void     load_frame_buffer();
	FORCE_INLINE const GeometryNode *get_geometry_node() const;
	FORCE_INLINE void                set_geometry_node(const ror::GeometryNode *a_geometry);
	FORCE_INLINE void                attach_texture_object(ror::TextureObject *a_texture_image, OGLFrameBuffer::EAttachment a_attachement = rGL_COLOR_ATTACHMENT0);
	FORCE_INLINE void                attach_render_buffer(ror::OGLRenderBuffer *a_render_buffer, EAttachment a_attachment);
	FORCE_INLINE void                start_render();
	FORCE_INLINE void                end_render();

  private:
	uint32_t                 m_buffer_id;            // The Frame buffer ID
	int32_t                  m_viewport[4];          // Buffer for ViewPort
	const ror::GeometryNode *m_geometry_node;        // Geometry Reference
	                                                 //static EAttachment					 m_NextColorAttachment;		// Color Attachment Points
};
}        // namespace ror

#include "rorogl_frame_buffer.inl"
