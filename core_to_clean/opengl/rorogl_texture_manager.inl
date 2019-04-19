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
FORCE_INLINE OGLTextureManager::OGLTextureManager(void)
{
	m_textures = new texture_objects_map;
}

FORCE_INLINE bool OGLTextureManager::update_texture(uint32_t a_id)
{
	return true;
}

FORCE_INLINE uint32_t OGLTextureManager::get_texture_size(uint32_t a_id)
{
	return 0;
}

FORCE_INLINE uint32_t OGLTextureManager::get_all_textures_size()
{
	return 0;
}

FORCE_INLINE void OGLTextureManager::bind_texture(uint32_t a_id)
{
	if (0 == a_id)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}

	texture_objects_map::iterator iter;
	for (iter = m_textures->begin(); iter != m_textures->end(); ++iter)
	{
		if (a_id == iter->second->get_texture_id())
		{
			iter->second->bind_texture();
			return;
		}
	}
}

}        // namespace ror
