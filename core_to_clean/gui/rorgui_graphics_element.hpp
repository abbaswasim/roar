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
#include "math/rorvector4.h"
#include "opengl/rorogl_vertex_buffer.h"
#include "renderer/rorrenderer.h"
#include "rorgui_element.h"
#include INCLUDERENDERER

namespace ror
{
class ROAR_ENGINE_ITEM cguigraphics_element : public ror::cguielement
{
  public:
	FORCE_INLINE cguigraphics_element(void);
	virtual ~cguigraphics_element(void);

	FORCE_INLINE bool initialize(uint32_t a_turn);
	FORCE_INLINE bool update(float32_t a_delta_time);
	FORCE_INLINE TextureObject *get_texture_object() const;
	FORCE_INLINE void           set_texture_object(TextureObject *a_texture_object);
	FORCE_INLINE Vector4f get_texture_offset() const;
	FORCE_INLINE void     set_texture_offset(Vector4f &a_texture_offset);
	FORCE_INLINE bool     get_scalable_texture() const;
	FORCE_INLINE void     set_scalable_texture(bool a_scalable_texture);
	FORCE_INLINE OGLVertexBuffer **get_vertex_buffer() const;
	//FORCE_INLINE void				SetVertexBuffer(COGLVertexBuffer *a_VBO);
	FORCE_INLINE void set(cguielement *a_parent, std::string a_text, Vector3i &a_position, Vector4i &a_non_client_area, bool a_visible, bool a_ground, Vector4f *a_background, Vector3f *a_foreground, Vector2i &a_bounds, IFunctor *a_call_back, EElementType a_element_type, int32_t a_font_index, EAlignment a_alighment, TextureObject *a_texture_object, Vector4f a_texture_offset, bool a_scaleable);
	FORCE_INLINE bool get_draw_background() const;
	FORCE_INLINE void set_draw_background(bool a_draw_background);
	FORCE_INLINE void toggle_background();
	FORCE_INLINE uint32_t get_no_of_vertex_buffers() const;
	FORCE_INLINE void     set_no_of_vertex_buffers(uint32_t a_no_of_vbos);
	FORCE_INLINE uint32_t get_current_vertex_buffer() const;
	FORCE_INLINE void     set_current_vertex_buffer(uint32_t a_active_vbo);
	FORCE_INLINE void     increment_current_vertex_buffer();

	virtual void render();
	virtual void global_initialize();

  protected:
	FORCE_INLINE void fill_vertex_buffer(uint32_t a_index, uint32_t a_number_of_vertices, uint32_t a_number_of_attributes, float32_t *a_points, float32_t *a_tex_coords);

	ror::OGLVertexBuffer **m_vertex_buffer;         // VBO For drawing the GUI Element
	ror::TextureObject *   m_texture_object;        // The texture attached with GUI Element, Element don't take ownership of m_TextureObject
	ror::Vector4f          m_texture_offset;        // Texture Coordinates of the GUI Texture Element
	                                                // (The texture for this Element Might be part of Big Texture)
	                                                // x,y,z,w (x = xMin,y = yMin,z = xMax,w = yMax) in Pixels
	bool     m_scalable;                            // Can the Texture be used for the whole Element
	bool     m_draw_background;                     // Background drawing switch
	uint32_t m_no_of_vertex_buffers;                // Number of VBOs in the element
	uint32_t m_current_vertex_buffer;               // Active VBOs in the element
};
}        // namespace ror

#include "rorgui_graphics_element.hh"
