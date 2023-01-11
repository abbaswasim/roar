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
#include "resources/rorresource.hpp"
#include "rhi/crtp_interfaces/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <memory>

namespace rhi
{
// FORCE_INLINE TextureAPIHandle TextureImage::handle() const noexcept
// {
//	return this->m_handle;
// }

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::width() const noexcept
{
	return this->m_mips[0].m_width;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::height() const noexcept
{
	return this->m_mips[0].m_height;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::depth() const noexcept
{
	return this->m_mips[0].m_depth;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::bytes_per_pixel() const noexcept
{
	return this->m_bytes_per_pixel;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::format() const noexcept
{
	return this->m_format;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::usage() const noexcept
{
	return this->m_usage;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::levels() const noexcept
{
	return ror::static_cast_safe<uint32_t>(this->m_mips.size());
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::target() const noexcept
{
	return this->m_target;
}

// FORCE_INLINE void TextureImageCrtp<_type>::handle(TextureHandle a_handle) noexcept
// {
//	this->m_handle = a_handle.m_handle;
// }

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::width(uint32_t a_width) noexcept
{
	this->m_mips[0].m_width = a_width;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::height(uint32_t a_height) noexcept
{
	this->m_mips[0].m_height = a_height;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::depth(uint32_t a_depth) noexcept
{
	this->m_mips[0].m_depth = a_depth;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::bytes_per_pixel(uint32_t a_bytes_per_pixel) noexcept
{
	this->m_bytes_per_pixel = a_bytes_per_pixel;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::format(rhi::PixelFormat a_format) noexcept
{
	this->m_format = a_format;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::usage(rhi::TextureUsage a_usage) noexcept
{
	this->m_usage = a_usage;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::target(TextureTarget a_target) noexcept
{
	this->m_target = a_target;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::allocate(uint64_t a_size)
{
	this->m_size = a_size;
	auto *ptr    = new uint8_t[this->m_size];
	this->m_data.reset(ptr);
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::upload(rhi::Device *a_device)
{
	this->underlying().upload(a_device);
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::ready(bool a_ready) noexcept
{
	this->m_ready = a_ready;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::ready() const noexcept
{
	return this->m_ready;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::name() const noexcept
{
	return this->m_name;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::name(std::string a_name) noexcept
{
	this->m_name = a_name;
}

template <class _type>
FORCE_INLINE auto *TextureImageCrtp<_type>::data() const noexcept
{
	return this->m_data.get();
}

template <class _type>
FORCE_INLINE auto *TextureImageCrtp<_type>::data() noexcept
{
	return this->m_data.get();
}

template <class _type>
FORCE_INLINE auto &TextureImageCrtp<_type>::mips() noexcept
{
	return this->m_mips;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::size() const noexcept
{
	return this->m_size;
}

template <class _type>
// Now TextureImageCrtp own the a_data pointer
FORCE_INLINE void TextureImageCrtp<_type>::reset(uint8_t *a_data, uint64_t a_size) noexcept
{
	this->m_size = a_size;
	this->m_data.reset(a_data);
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::push_empty_mip() noexcept
{
	this->m_mips.emplace_back();
}

template <class _type>
FORCE_INLINE constexpr rhi::TextureFilter TextureSamplerCrtp<_type>::mag_filter() const noexcept
{
	return this->m_mag_filter;
}
template <class _type>
FORCE_INLINE constexpr rhi::TextureFilter TextureSamplerCrtp<_type>::min_filter() const noexcept
{
	return this->m_min_filter;
}
template <class _type>
FORCE_INLINE constexpr rhi::TextureMipFilter TextureSamplerCrtp<_type>::mip_mode() const noexcept
{
	return this->m_mip_mode;
}
template <class _type>
FORCE_INLINE constexpr rhi::TextureAddressMode TextureSamplerCrtp<_type>::wrap_s() const noexcept
{
	return this->m_wrap_s;
}
template <class _type>
FORCE_INLINE constexpr rhi::TextureAddressMode TextureSamplerCrtp<_type>::wrap_t() const noexcept
{
	return this->m_wrap_t;
}
template <class _type>
FORCE_INLINE constexpr rhi::TextureAddressMode TextureSamplerCrtp<_type>::wrap_u() const noexcept
{
	return this->m_wrap_u;
}

template <class _type>
FORCE_INLINE constexpr rhi::TextureBorder TextureSamplerCrtp<_type>::border_color() const noexcept
{
	return this->m_border_color;
}

template <class _type>
FORCE_INLINE constexpr void TextureSamplerCrtp<_type>::mag_filter(rhi::TextureFilter a_filter) noexcept
{
	this->m_mag_filter = a_filter;
}
template <class _type>
FORCE_INLINE constexpr void TextureSamplerCrtp<_type>::min_filter(rhi::TextureFilter a_filter) noexcept
{
	this->m_min_filter = a_filter;
}
template <class _type>
FORCE_INLINE constexpr void TextureSamplerCrtp<_type>::mip_mode(rhi::TextureMipFilter a_mode) noexcept
{
	this->m_mip_mode = a_mode;
}
template <class _type>
FORCE_INLINE constexpr void TextureSamplerCrtp<_type>::wrap_s(rhi::TextureAddressMode a_wrap) noexcept
{
	this->m_wrap_s = a_wrap;
}
template <class _type>
FORCE_INLINE constexpr void TextureSamplerCrtp<_type>::wrap_t(rhi::TextureAddressMode a_wrap) noexcept
{
	this->m_wrap_t = a_wrap;
}
template <class _type>
FORCE_INLINE constexpr void TextureSamplerCrtp<_type>::wrap_u(rhi::TextureAddressMode a_wrap) noexcept
{
	this->m_wrap_u = a_wrap;
}

template <class _type>
FORCE_INLINE constexpr void TextureSamplerCrtp<_type>::border_color(rhi::TextureBorder a_border) noexcept
{
	this->m_border_color = a_border;
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

}        // namespace rhi
