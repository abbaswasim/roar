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

#include "appearance/rortexture_image.h"
#include "appearance/rortexture_object.h"
#include "foundation/rorsingleton.h"
#include "interfaces/roritexture_manager.h"

namespace ror
{
class ROAR_ENGINE_ITEM OGLTextureManager : public Singleton<OGLTextureManager>, public ror::ITextureManager
{
  public:
	FORCE_INLINE bool update_texture(uint32_t a_id);
	FORCE_INLINE uint32_t get_texture_size(uint32_t a_id);
	FORCE_INLINE uint32_t get_all_textures_size();
	FORCE_INLINE void     bind_texture(uint32_t a_id);

	TextureObject *create_texture(std::string a_file_name);
	TextureObject *create_texture(std::string a_file_name, TextureObject::ETextureTarget a_target, uint32_t a_bpp, uint32_t a_width, uint32_t a_height);
	void           release_texture(uint32_t a_id);
	void           release_all_textures();

	TextureObject *set_gltexture_parameters(TextureImage *a_texture_image);

  private:
	FORCE_INLINE OGLTextureManager(void);
	~OGLTextureManager(void);

	typedef std::unordered_map<std::string, TextureObject *> texture_objects_map;

	texture_objects_map *m_textures;        // Texture Store

	// Make CSingleton<COGLTextureManager> as friend
	friend class Singleton<OGLTextureManager>;
};
}        // namespace ror

#include "rorogl_texture_manager.inl"
