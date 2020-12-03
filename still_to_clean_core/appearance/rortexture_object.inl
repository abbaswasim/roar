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
FORCE_INLINE TextureObject::TextureObject(void) :
    m_texture_id(0),
    m_reference_count(1),
    m_texture_size(0),
    m_texture_target(rGL_TEXTURE_2D),
    m_texture_width(0),
    m_texture_height(0)
{}

FORCE_INLINE TextureObject::TextureObject(uint32_t a_id, uint32_t a_texture_size, uint32_t a_reference_count, ETextureTarget a_texture_target) :
    m_texture_id(a_id),
    m_reference_count(a_reference_count),
    m_texture_size(a_texture_size),
    m_texture_target(a_texture_target),
    m_texture_width(0),
    m_texture_height(0)
{}

FORCE_INLINE TextureObject::TextureObject(TextureObject &a_other) :
    m_texture_id(a_other.m_texture_id),
    m_reference_count(a_other.m_reference_count),
    m_texture_size(a_other.m_texture_size),
    m_texture_target(a_other.m_texture_target),
    m_texture_width(0),
    m_texture_height(0)
{}

FORCE_INLINE uint32_t TextureObject::get_texture_id() const
{
	return m_texture_id;
}

FORCE_INLINE void TextureObject::set_texture_id(uint32_t &a_texture_id)
{
	m_texture_id = a_texture_id;
}

FORCE_INLINE uint32_t TextureObject::get_texture_size() const
{
	return m_texture_size;
}

FORCE_INLINE void TextureObject::set_texture_size(uint32_t a_texture_id)
{
	m_texture_size = a_texture_id;
}

FORCE_INLINE uint32_t TextureObject::get_texture_width() const
{
	return m_texture_width;
}

FORCE_INLINE void TextureObject::set_texture_width(uint32_t a_texture_width)
{
	m_texture_width = a_texture_width;
}

FORCE_INLINE uint32_t TextureObject::get_texture_height() const
{
	return m_texture_height;
}

FORCE_INLINE void TextureObject::set_texture_height(uint32_t a_texture_height)
{
	m_texture_height = a_texture_height;
}

FORCE_INLINE uint32_t TextureObject::increment_reference_count()
{
	return ++m_reference_count;
}

FORCE_INLINE uint32_t TextureObject::decrement_reference_count()
{
	return --m_reference_count;
}

FORCE_INLINE uint32_t TextureObject::get_reference_count() const
{
	return m_reference_count;
}

FORCE_INLINE void TextureObject::set_reference_count(uint32_t &a_reference_count)
{
	m_reference_count = a_reference_count;
}

FORCE_INLINE TextureObject::ETextureTarget TextureObject::get_texture_target() const
{
	return m_texture_target;
}

FORCE_INLINE void TextureObject::set_texture_target(TextureObject::ETextureTarget a_texture_target)
{
	m_texture_target = a_texture_target;
}

FORCE_INLINE void TextureObject::bind_texture(bool a_binding) const
{
	if (true == a_binding)
	{
		glBindTexture(this->m_texture_target, this->m_texture_id);
	}
	else
	{
		glBindTexture(this->m_texture_target, 0);
	}
	/*
		else if(ETextureTarget == rGL_TEXTURE_3D) // Not Supported Yet
		glBindTexture(GL_TEXTURE_3D_EXT,m_TextureID);
		*/
}

FORCE_INLINE void TextureObject::update_mip_maps() const
{
	glActiveTexture(GL_TEXTURE0);
	this->bind_texture();
	glGenerateMipmapEXT(this->m_texture_target);
	this->bind_texture(false);
	glActiveTexture(GL_TEXTURE1);
}

FORCE_INLINE bool TextureObject::is_texture_resident()
{
	uint32_t  textures[] = {m_texture_id};
	GLboolean status[1];
	return (glAreTexturesResident(1, textures, status) ? true : false);
}

FORCE_INLINE void TextureObject::set_texture_priority(float32_t a_priority)
{
	uint32_t  textures[] = {m_texture_id};
	float32_t priority[] = {a_priority};
	glPrioritizeTextures(1, textures, priority);
}

}        // namespace ror
