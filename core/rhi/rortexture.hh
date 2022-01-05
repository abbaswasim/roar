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

#include "rhi/rortexture.hpp"

namespace rhi
{
FORCE_INLINE TextureAPIHandle TextureImage::handle() const noexcept
{
	return this->m_handle;
}

FORCE_INLINE uint32_t TextureImage::width() const noexcept
{
	return this->m_mips[0].m_width;
}

FORCE_INLINE uint32_t TextureImage::height() const noexcept
{
	return this->m_mips[0].m_height;
}

FORCE_INLINE PixelFormat TextureImage::format() const noexcept
{
	return this->m_format;
}

FORCE_INLINE uint32_t TextureImage::levels() const noexcept
{
	return ror::static_cast_safe<uint32_t>(this->m_mips.size());
}
FORCE_INLINE TextureImage::TextureTarget TextureImage::target() const noexcept
{
	return this->m_target;
}

FORCE_INLINE void TextureImage::handle(TextureHandle a_handle) noexcept
{
	this->m_handle = a_handle.m_handle;
}

FORCE_INLINE void TextureImage::width(uint32_t a_width) noexcept
{
	this->m_mips[0].m_width = a_width;
}

FORCE_INLINE void TextureImage::height(uint32_t a_height) noexcept
{
	this->m_mips[0].m_height = a_height;
}

FORCE_INLINE void TextureImage::format(PixelFormat a_format) noexcept
{
	this->m_format = a_format;
}

FORCE_INLINE void TextureImage::target(TextureTarget a_target) noexcept
{
	this->m_target = a_target;
}

FORCE_INLINE void TextureImage::allocate(uint64_t a_size)
{
	this->m_size = a_size;
	this->m_data.resize(this->m_size);
}

}        // namespace rhi
