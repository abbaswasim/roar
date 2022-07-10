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

#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rorutilities.hpp"
#include "rhi/crtp_interfaces/rortexture.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rortypes.hpp"
#include <memory>

namespace rhi
{
// FORCE_INLINE TextureAPIHandle TextureImage::handle() const noexcept
// {
//	return this->m_handle;
// }

template<class _type>
FORCE_INLINE uint32_t TextureImageCrtp<_type>::width() const noexcept
{
	return this->m_mips[0].m_width;
}

template<class _type>
FORCE_INLINE uint32_t TextureImageCrtp<_type>::height() const noexcept
{
	return this->m_mips[0].m_height;
}

template<class _type>
FORCE_INLINE uint32_t TextureImageCrtp<_type>::depth() const noexcept
{
	return this->m_mips[0].m_depth;
}

template<class _type>
FORCE_INLINE rhi::PixelFormat TextureImageCrtp<_type>::format() const noexcept
{
	return this->m_format;
}

template<class _type>
FORCE_INLINE uint32_t TextureImageCrtp<_type>::levels() const noexcept
{
	return ror::static_cast_safe<uint32_t>(this->m_mips.size());
}

template<class _type>
FORCE_INLINE TextureTarget TextureImageCrtp<_type>::target() const noexcept
{
	return this->m_target;
}

// FORCE_INLINE void TextureImageCrtp<_type>::handle(TextureHandle a_handle) noexcept
// {
//	this->m_handle = a_handle.m_handle;
// }

template<class _type>
FORCE_INLINE void TextureImageCrtp<_type>::width(uint32_t a_width) noexcept
{
	this->m_mips[0].m_width = a_width;
}

template<class _type>
FORCE_INLINE void TextureImageCrtp<_type>::height(uint32_t a_height) noexcept
{
	this->m_mips[0].m_height = a_height;
}

template<class _type>
FORCE_INLINE void TextureImageCrtp<_type>::depth(uint32_t a_depth) noexcept
{
	this->m_mips[0].m_depth = a_depth;
}

template<class _type>
FORCE_INLINE void TextureImageCrtp<_type>::format(rhi::PixelFormat a_format) noexcept
{
	this->m_format = a_format;
}

template<class _type>
FORCE_INLINE void TextureImageCrtp<_type>::target(TextureTarget a_target) noexcept
{
	this->m_target = a_target;
}

template<class _type>
FORCE_INLINE void TextureImageCrtp<_type>::allocate(uint64_t a_size)
{
	this->m_size = a_size;
	auto *ptr    = new uint8_t[this->m_size];
	this->m_data.reset(ptr);
}

template<class _type>
FORCE_INLINE void TextureImageCrtp<_type>::upload()
{
	this->underlying().upload();
}

template<class _type>
FORCE_INLINE uint8_t *TextureImageCrtp<_type>::data() const noexcept
{
	return this->m_data.get();
}

template<class _type>
FORCE_INLINE std::vector<typename TextureImageCrtp<_type>::Mipmap> &TextureImageCrtp<_type>::mips() noexcept
{
	return this->m_mips;
}

template<class _type>
FORCE_INLINE uint64_t TextureImageCrtp<_type>::size() const noexcept
{
	return this->m_size;
}

template<class _type>
// Now TextureImageCrtp own the a_data pointer
FORCE_INLINE void TextureImageCrtp<_type>::reset(uint8_t *a_data, uint64_t a_size) noexcept
{
	this->m_size = a_size;
	this->m_data.reset(a_data);
}

template<class _type>
FORCE_INLINE void TextureImageCrtp<_type>::push_empty_mip() noexcept
{
	this->m_mips.emplace_back();
}

template<class _type>
rhi::TextureFilter TextureSamplerCrtp<_type>::mag_filter()
{
	return this->m_mag_filter;
}
template<class _type>
rhi::TextureFilter TextureSamplerCrtp<_type>::min_filter()
{
	return this->m_min_filter;
}
template<class _type>
rhi::TextureFilter TextureSamplerCrtp<_type>::mip_mode()
{
	return this->m_mip_mode;
}
template<class _type>
rhi::TextureAddressMode TextureSamplerCrtp<_type>::wrap_s()
{
	return this->m_wrap_s;
}
template<class _type>
rhi::TextureAddressMode TextureSamplerCrtp<_type>::wrap_t()
{
	return this->m_wrap_t;
}
template<class _type>
rhi::TextureAddressMode TextureSamplerCrtp<_type>::wrap_u()
{
	return this->m_wrap_u;
}

template<class _type>
void TextureSamplerCrtp<_type>::mag_filter(rhi::TextureFilter a_filter)
{
	this->m_mag_filter = a_filter;
}
template<class _type>
void TextureSamplerCrtp<_type>::min_filter(rhi::TextureFilter a_filter)
{
	this->m_min_filter = a_filter;
}
template<class _type>
void TextureSamplerCrtp<_type>::mip_mode(rhi::TextureFilter a_mode)
{
	this->m_mip_mode = a_mode;
}
template<class _type>
void TextureSamplerCrtp<_type>::wrap_s(rhi::TextureAddressMode a_wrap)
{
	this->m_wrap_s = a_wrap;
}
template<class _type>
void TextureSamplerCrtp<_type>::wrap_t(rhi::TextureAddressMode a_wrap)
{
	this->m_wrap_t = a_wrap;
}
template<class _type>
void TextureSamplerCrtp<_type>::wrap_u(rhi::TextureAddressMode a_wrap)
{
	this->m_wrap_u = a_wrap;
}

template <class _type>
rhi::TextureImageHandle TextureCrtp<_type>::texture_image()
{
	return this->m_texture_image;
}
template <class _type>
rhi::TextureSamplerHandle TextureCrtp<_type>::texture_sampler()
{
	return this->m_texture_sampler;
}
template <class _type>
void TextureCrtp<_type>::texture_image(rhi::TextureImageHandle a_handle)
{
	this->m_texture_image = a_handle;
}
template <class _type>
void TextureCrtp<_type>::texture_sampler(rhi::TextureSamplerHandle a_handle)
{
	this->m_texture_sampler = a_handle;
}

}        // namespace ror
