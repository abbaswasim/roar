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
	auto layers = is_texture_array(this->m_target) ? this->m_mips[0].m_depth : 1u;
	auto faces  = is_texture_cubemap(this->m_target) ? 6u : 1u;

	return ror::static_cast_safe<uint32_t>(this->m_mips.size() / (layers * faces));
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
	this->m_bytes_per_pixel = pixel_format_to_bytes(a_format);
	this->m_format          = a_format;
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
FORCE_INLINE void TextureImageCrtp<_type>::allocate()
{
	size_t size{rhi::calculate_texture_size(this->width(), this->height(), this->depth(), this->format(),
	                                        this->mipmapped(), rhi::is_texture_cubemap(this->target()), rhi::is_texture_array(this->target()))};
	this->allocate(size);
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::allocate(uint64_t a_size)
{
	assert(this->m_size == 0 && "Already allocated, should free first");
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
FORCE_INLINE void TextureImageCrtp<_type>::mipmapped(bool a_mipmapped) noexcept
{
	this->m_mipmapped = a_mipmapped;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::mip_gen_mode(rhi::TextureMipGenMode a_gen_mode) noexcept
{
	this->m_mip_gen_mode = a_gen_mode;
}

template <class _type>
FORCE_INLINE void TextureImageCrtp<_type>::hdr(bool a_hdr) noexcept
{
	this->m_hdr = a_hdr;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::ready() const noexcept
{
	return this->m_ready;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::mipmapped() const noexcept
{
	return this->m_mipmapped;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::mip_gen_mode() const noexcept
{
	return this->m_mip_gen_mode;
}

template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::hdr() const noexcept
{
	return this->m_hdr;
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
FORCE_INLINE const auto &TextureImageCrtp<_type>::mips() const noexcept
{
	return this->m_mips;
}

template <class _type>
FORCE_INLINE auto &TextureImageCrtp<_type>::mips() noexcept
{
	return this->m_mips;
}

template <class _type>
FORCE_INLINE auto &TextureImageCrtp<_type>::mip(size_t a_index) const noexcept
{
	assert(a_index < this->m_mips.size() && "Out of bound index for mipmap levels");

	return this->m_mips[a_index];
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
	if (this->m_mips.empty())
		this->m_mips.emplace_back();
}

// Need to be called once at least an empty mip is setup and has width and height, and format is set
template <class _type>
FORCE_INLINE auto TextureImageCrtp<_type>::setup() noexcept
{
	assert(this->m_mips.size() && "No mips available");

	uint32_t width{this->width()};
	uint32_t height{this->height()};
	uint32_t depth{this->depth()};
	uint32_t faces{is_texture_cubemap(this->m_target) ? 6u : 1u};
	uint32_t layers{is_texture_array(this->m_target) ? depth : 1u};

	uint32_t mip_levels_count{this->m_mipmapped ? calculate_texture_mip_levels(width, height, is_texture_array(this->m_target) ? 1 : depth) : 1};

	this->m_mips.resize(mip_levels_count * faces * layers);

	size_t size{0};
	size_t level_index{0};

	for (size_t layer = 0; layer < layers; ++layer)
	{
		for (size_t face = 0; face < faces; ++face)
		{
			for (size_t level = 0; level < mip_levels_count; ++level)
			{
				uint32_t mip_width  = std::max(1u, width >> level);
				uint32_t mip_height = std::max(1u, height >> level);
				uint32_t mip_depth  = std::max(1u, depth >> level);

				auto &miplevel    = this->m_mips[level_index];
				miplevel.m_width  = mip_width;
				miplevel.m_height = mip_height;
				miplevel.m_depth  = mip_depth;
				miplevel.m_offset = size * this->m_bytes_per_pixel;

				miplevel.m_size = mip_width * mip_height * mip_depth;
				size += miplevel.m_size;
				miplevel.m_size *= this->m_bytes_per_pixel;

				++level_index;
			}
		}
	}

	// NOTE: We are not setting m_size here because thats suppose to be set when loaded with the actual data size
	return size * this->m_bytes_per_pixel;
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
rhi::TextureImageHandle TextureCrtp<_type>::texture_image() const
{
	return this->m_texture_image;
}
template <class _type>
rhi::TextureSamplerHandle TextureCrtp<_type>::texture_sampler() const
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

template <class _type>
void TextureImageCrtp<_type>::verify_sizes(bool a_is_cube) const noexcept
{
	auto &last_mip  = this->mips().back();
	auto &first_mip = this->mips()[0];

	assert(this->data());
	if (this->mip_gen_mode() == rhi::TextureMipGenMode::manual && this->mipmapped())
	{
		assert(this->size() == ((last_mip.m_width * last_mip.m_height * last_mip.m_depth * this->bytes_per_pixel()) + last_mip.m_offset) && "Image size doesn't match the expected texture size");
	}
	else
	{
		if (a_is_cube)
		{
			assert(this->size() == ((last_mip.m_width * last_mip.m_height * last_mip.m_depth * this->bytes_per_pixel()) + last_mip.m_offset) && "Image size doesn't match the expected texture size");
		}
		else
		{
			assert(this->size() == (first_mip.m_width * first_mip.m_height * first_mip.m_depth * this->bytes_per_pixel()) && "Image size doesn't match the expected texture size");
		}
	}

	(void) last_mip;
	(void) first_mip;
	(void) a_is_cube;
}

}        // namespace rhi
