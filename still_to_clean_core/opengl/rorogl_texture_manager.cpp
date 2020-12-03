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

#include "rorogl_texture_manager.h"

namespace ror
{
OGLTextureManager::~OGLTextureManager(void)
{
	rDELETE(this->m_textures);
}

TextureObject *OGLTextureManager::create_texture(std::string a_file_name)
{
	// Before trying to create a new texture check if its already in the textures loaded
	texture_objects_map::iterator iter = this->m_textures->find(a_file_name);
	if (iter != this->m_textures->end())
	{
		iter->second->increment_reference_count();        //m_ReferenceCount++;
		return iter->second /*->GetTextureID()*/;
	}
	// Otherwise we load it from disk
	TextureImage *texture_image_file = rNew TextureImage();

	if (!texture_image_file->load_image_file(a_file_name))
		return 0;

	TextureObject *new_texture_object = this->set_gltexture_parameters(texture_image_file);
	new_texture_object->set_texture_size(texture_image_file->get_bpp() * texture_image_file->get_height() * texture_image_file->get_width());

	this->m_textures->insert(std::make_pair(std::string(a_file_name), new_texture_object));

	new_texture_object->set_texture_width(texture_image_file->get_width());
	new_texture_object->set_texture_height(texture_image_file->get_height());

	rDELETE(texture_image_file);
	return new_texture_object /*->GetTextureID()*/;
}

TextureObject *OGLTextureManager::create_texture(std::string a_file_name, TextureObject::ETextureTarget a_target, uint32_t a_bpp, uint32_t a_width, uint32_t a_height)
{
	// The Filename must be unique any dummy name
	rUsChar **pixel_data             = new rUsChar *;        //new rUsChar[a_BPP * a_Width * a_Height];			// Pixels Data
	*pixel_data                      = nullptr;
	TextureImage *texture_image_file = rNew TextureImage(a_bpp, a_width, a_height, pixel_data);

	TextureObject *new_texture_object = this->set_gltexture_parameters(texture_image_file);
	new_texture_object->set_texture_size(texture_image_file->get_bpp() * texture_image_file->get_height() * texture_image_file->get_width());

	this->m_textures->insert(std::make_pair(std::string(a_file_name), new_texture_object));

	new_texture_object->set_texture_width(texture_image_file->get_width());
	new_texture_object->set_texture_height(texture_image_file->get_height());

	rDELETE(texture_image_file);
	return new_texture_object /*->GetTextureID()*/;
}

TextureObject *OGLTextureManager::set_gltexture_parameters(TextureImage *a_texture_image)
{
	TextureObject *new_texture_object = rNew TextureObject();
	uint32_t                                 temp_id;
	glGenTextures(1, &temp_id);

	new_texture_object->set_texture_id(temp_id);

	TextureObject::ETextureTarget t_target = new_texture_object->get_texture_target();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(t_target, temp_id);

	glTexParameteri(t_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(t_target, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(t_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(t_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(t_target, GL_GENERATE_MIPMAP, GL_TRUE);

	//		glTexParameterf(tTarget, GL_TEXTURE_MIN_LOD, -10.0f);
	//		glTexParameterf(tTarget, GL_TEXTURE_MAX_LOD, 10.0f);
	//		glTexParameterf(tTarget, GL_TEXTURE_LOD_BIAS, 3.0f);

#ifdef USE_ANISOTPIC_FILTERING
	float32_t max_anisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropy);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropy);
#endif

#ifdef _DEBUG
	assert(a_texture_image->get_width() == a_texture_image->get_height());
#endif
	if (a_texture_image->get_bpp() == 4)
	{
		glTexImage2D(t_target, 0, GL_RGBA8, a_texture_image->get_width(), a_texture_image->get_height(), 0,
		             GL_BGRA_EXT, GL_UNSIGNED_BYTE, *a_texture_image->get_pixel_data());
	}
	else if (a_texture_image->get_bpp() == 3)
	{
		glTexImage2D(t_target, 0, GL_RGBA8, a_texture_image->get_width(), a_texture_image->get_height(), 0,
		             GL_BGR, GL_UNSIGNED_BYTE, *a_texture_image->get_pixel_data());
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	return new_texture_object;
}

void OGLTextureManager::release_texture(uint32_t a_id)
{
	texture_objects_map::iterator iter;
	for (iter = this->m_textures->begin(); iter != this->m_textures->end(); ++iter)
	{
		if (a_id == iter->second->get_texture_id())
		{
			if (iter->second->decrement_reference_count() == 0)
			{
				uint32_t temp_texture_id = iter->second->get_texture_id();
				glDeleteTextures(1, &temp_texture_id);
				rDELETE(iter->second);
				this->m_textures->erase(iter);
			}
			return;
		}
	}
}

void OGLTextureManager::release_all_textures()
{
	texture_objects_map::iterator iter = this->m_textures->begin();
	for (iter = this->m_textures->begin(); iter != this->m_textures->end(); ++iter)
	{
		uint32_t temp_texture_id = iter->second->get_texture_id();
		glDeleteTextures(1, &temp_texture_id);
		rDELETE(iter->second);
	}
	this->m_textures->clear();
}

}        // namespace ror
