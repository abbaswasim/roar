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
class ROAR_ENGINE_ITEM TextureObject
{
  public:
	enum ETextureTarget
	{
		rGL_TEXTURE_1D = 0x0de0,
		rGL_TEXTURE_2D = 0x0de1,
		rGL_TEXTURE_3D,
		rTEXTURE_TARGET_MAX
	};

	FORCE_INLINE TextureObject(void);
	FORCE_INLINE TextureObject(uint32_t a_id, uint32_t a_texture_size, uint32_t a_reference_count, ETextureTarget a_texture_target);
	FORCE_INLINE TextureObject(TextureObject &a_other);
	~TextureObject(void);

	FORCE_INLINE uint32_t get_texture_id() const;
	FORCE_INLINE void     set_texture_id(uint32_t &a_texture_id);
	FORCE_INLINE uint32_t get_texture_size() const;
	FORCE_INLINE void     set_texture_size(uint32_t a_texture_id);
	FORCE_INLINE uint32_t get_reference_count() const;
	FORCE_INLINE void     set_reference_count(uint32_t &a_texture_id);
	FORCE_INLINE uint32_t increment_reference_count();
	FORCE_INLINE uint32_t decrement_reference_count();
	FORCE_INLINE ETextureTarget get_texture_target() const;
	FORCE_INLINE void           set_texture_target(ETextureTarget a_texture_target);
	//		FORCE_INLINE CTextureImage*	GetTextureImageFile() const;
	//		FORCE_INLINE void			SetTextureImageFile(CoreEngine::CTextureImage * a_TextureImageFile);
	FORCE_INLINE void update_mip_maps() const;
	FORCE_INLINE uint32_t get_texture_width() const;
	FORCE_INLINE void     set_texture_width(uint32_t a_texture_width);
	FORCE_INLINE uint32_t get_texture_height() const;
	FORCE_INLINE void     set_texture_height(uint32_t a_texture_height);

	// OpenGL Specific Texture Binding
	FORCE_INLINE void bind_texture(bool a_binding = true) const;

	FORCE_INLINE bool is_texture_resident();
	FORCE_INLINE void set_texture_priority(float32_t a_priority);

  private:
	uint32_t       m_texture_id;             // Texture ID used by The Renderer
	uint32_t       m_texture_size;           // Texture Data size in Bytes
	uint32_t       m_texture_width;          // Texture Width used by The Texture
	uint32_t       m_texture_height;         // Texture Height used by The Texture
	uint32_t       m_reference_count;        // How many times this Texture is Used
	ETextureTarget m_texture_target;         // Can be 1D, 2D or 3D texture
};
}        // namespace ror

#include "rortexture_object.hh"
