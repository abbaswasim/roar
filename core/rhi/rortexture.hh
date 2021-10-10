// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "foundation/rormacros.hpp"
#include "rhi/rortexture.hpp"

namespace rhi
{
FORCE_INLINE TextureHandle Texture::handle() const noexcept
{
	return this->m_handle;
}

FORCE_INLINE uint32_t Texture::width() const noexcept
{
	return this->m_mips[0].m_width;
}

FORCE_INLINE uint32_t Texture::height() const noexcept
{
	return this->m_mips[0].m_height;
}

FORCE_INLINE PixelFormat Texture::format() const noexcept
{
	return this->m_format;
}

FORCE_INLINE uint32_t Texture::levels() const noexcept
{
	return ror::static_cast_safe<uint32_t>(this->m_mips.size());
}
FORCE_INLINE Texture::TextureTarget Texture::target() const noexcept
{
	return this->m_target;
}

FORCE_INLINE void Texture::handle(TextureHandle a_handle) noexcept
{
	this->m_handle = a_handle;
}

FORCE_INLINE void Texture::width(uint32_t a_width) noexcept
{
	this->m_mips[0].m_width = a_width;
}

FORCE_INLINE void Texture::height(uint32_t a_height) noexcept
{
	this->m_mips[0].m_height = a_height;
}

FORCE_INLINE void Texture::format(PixelFormat a_format) noexcept
{
	this->m_format = a_format;
}

FORCE_INLINE void Texture::target(TextureTarget a_target) noexcept
{
	this->m_target = a_target;
}

FORCE_INLINE void Texture::allocate(uint64_t a_size)
{
	this->m_size = a_size;
	this->m_data.resize(this->m_size);
}

}        // namespace rhi
