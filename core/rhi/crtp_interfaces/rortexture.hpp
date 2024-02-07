// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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

#include "foundation/rorcrtp.hpp"
#include "foundation/rormacros.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rortypes.hpp"
#include <any>
#include <atomic>
#include <filesystem>
#include <memory>
#include <utility>
#include <vector>

// Global function to create rhi::TextureImage
#include "basisu_transcoder.h"

namespace rhi
{

uint32_t calculate_texture_mip_levels(uint32_t a_width, uint32_t a_height, uint32_t a_depth);
size_t   calculate_texture_size(uint32_t a_width, uint32_t a_height, uint32_t a_depth, rhi::PixelFormat a_format, bool a_mipmapped = false, bool a_cubemap = false, bool a_array = false);

template <class _type>
class ROAR_ENGINE_ITEM TextureImageCrtp : public ror::Crtp<_type, TextureImageCrtp>
{
  public:
	FORCE_INLINE                   TextureImageCrtp(const TextureImageCrtp &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                   TextureImageCrtp(TextureImageCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TextureImageCrtp &operator=(const TextureImageCrtp &a_other)            = delete;         //! Copy assignment operator
	FORCE_INLINE TextureImageCrtp &operator=(TextureImageCrtp &&a_other) noexcept        = default;        //! Move assignment operator
	FORCE_INLINE virtual ~TextureImageCrtp() noexcept override                           = default;        //! Destructor

	struct Mipmap
	{
		uint32_t m_width{0};         // Width of this mipmap
		uint32_t m_height{0};        // Height of this mipmap
		uint32_t m_depth{1};         // Depth of this mipmap, always 1 unless 3D volume texture
		uint64_t m_offset{0};        // Offset in the data array inside the TexturImage
		uint64_t m_size{0};          // Size of this mipmap in bytes
	};

	FORCE_INLINE auto  handle() const noexcept;
	FORCE_INLINE auto  width() const noexcept;
	FORCE_INLINE auto  height() const noexcept;
	FORCE_INLINE auto  depth() const noexcept;
	FORCE_INLINE auto  bytes_per_pixel() const noexcept;
	FORCE_INLINE auto  format() const noexcept;
	FORCE_INLINE auto  usage() const noexcept;
	FORCE_INLINE auto  levels() const noexcept;
	FORCE_INLINE auto  target() const noexcept;
	FORCE_INLINE auto *data() const noexcept;
	FORCE_INLINE auto *data() noexcept;
	FORCE_INLINE auto &mips() noexcept;
	FORCE_INLINE auto &mip(size_t a_index) const noexcept;
	FORCE_INLINE auto  size() const noexcept;
	FORCE_INLINE auto  ready() const noexcept;
	FORCE_INLINE auto  mipmapped() const noexcept;
	FORCE_INLINE auto  mip_gen_mode() const noexcept;
	FORCE_INLINE auto  hdr() const noexcept;
	FORCE_INLINE auto  name() const noexcept;
	FORCE_INLINE auto  setup() noexcept;

	FORCE_INLINE const auto &mips() const noexcept;

	FORCE_INLINE void handle(rhi::TextureHandle) noexcept;
	FORCE_INLINE void width(uint32_t) noexcept;
	FORCE_INLINE void height(uint32_t) noexcept;
	FORCE_INLINE void depth(uint32_t) noexcept;
	FORCE_INLINE void bytes_per_pixel(uint32_t) noexcept;
	FORCE_INLINE void format(rhi::PixelFormat) noexcept;
	FORCE_INLINE void usage(rhi::TextureUsage) noexcept;
	FORCE_INLINE void target(TextureTarget) noexcept;
	FORCE_INLINE void reset(uint8_t *, uint64_t) noexcept;
	FORCE_INLINE void push_empty_mip() noexcept;
	FORCE_INLINE void allocate();
	FORCE_INLINE void allocate(uint64_t a_size);
	FORCE_INLINE void upload(rhi::Device *a_device);
	FORCE_INLINE void ready(bool) noexcept;
	FORCE_INLINE void mipmapped(bool) noexcept;
	FORCE_INLINE void mip_gen_mode(rhi::TextureMipGenMode) noexcept;
	FORCE_INLINE void hdr(bool) noexcept;
	FORCE_INLINE void name(std::string) noexcept;
	FORCE_INLINE void verify_sizes(bool a_is_cube) const noexcept;

  protected:
	FORCE_INLINE TextureImageCrtp() = default;        //! Default constructor
  private:
	bool                       m_ready{false};                                               //! True when the texture is uploaded to the GPU and ready to be used
	bool                       m_mipmapped{false};                                           //! True when the texture image is suppose to be used as mipmapped as well
	bool                       m_hdr{false};                                                 //! True when the texture HDR format most probalby 3 components
	uint32_t                   m_bytes_per_pixel{4};                                         //! Bytes per pixel
	uint64_t                   m_size{0};                                                    //! Size of the first mipmap in bytes (not all mipmaps).
	TextureTarget              m_target{TextureTarget::texture_2D};                          //! Can be 1D, 2D or 3D etc texture
	TextureMipGenMode          m_mip_gen_mode{TextureMipGenMode::manual};                    //! If mipmapped, how are these mipmappes generated, manually provided or generated using API
	rhi::PixelFormat           m_format{rhi::PixelFormat::r8g8b8a8_uint32_norm_srgb};        //! Pixel format of the texture
	rhi::TextureUsage          m_usage{rhi::TextureUsage::shader_read};                      //! What the texture is used for, by default just a read (sample) usage, can be ORed with other usage
	std::unique_ptr<uint8_t[]> m_data{};                                                     //! All mipmaps data
	std::vector<Mipmap>        m_mips{};                                                     //! Will have at least one level, base texture width and height are mip[0] width/height
	std::string                m_name{};                                                     //! Name of the texture, usually given for debugging purposes or the file name
};

// static_assert(std::is_trivially_copyable_v<TextureImage>, "TextureImage is not trivially copyable");
// static_assert(std::is_standard_layout_v<TextureImage>, "TextureImage is not standard layout");

template <class _type>
class ROAR_ENGINE_ITEM TextureSamplerCrtp : public ror::Crtp<_type, TextureSamplerCrtp>
{
  public:
	FORCE_INLINE                     TextureSamplerCrtp(const TextureSamplerCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                     TextureSamplerCrtp(TextureSamplerCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TextureSamplerCrtp &operator=(const TextureSamplerCrtp &a_other)              = default;        //! Copy assignment operator
	FORCE_INLINE TextureSamplerCrtp &operator=(TextureSamplerCrtp &&a_other) noexcept          = default;        //! Move assignment operator
	FORCE_INLINE virtual ~TextureSamplerCrtp() noexcept override                               = default;        //! Destructor

	FORCE_INLINE constexpr rhi::TextureFilter      mag_filter() const noexcept;
	FORCE_INLINE constexpr rhi::TextureFilter      min_filter() const noexcept;
	FORCE_INLINE constexpr rhi::TextureMipFilter   mip_mode() const noexcept;
	FORCE_INLINE constexpr rhi::TextureAddressMode wrap_s() const noexcept;
	FORCE_INLINE constexpr rhi::TextureAddressMode wrap_t() const noexcept;
	FORCE_INLINE constexpr rhi::TextureAddressMode wrap_u() const noexcept;
	FORCE_INLINE constexpr rhi::TextureBorder      border_color() const noexcept;
	FORCE_INLINE constexpr void                    mag_filter(rhi::TextureFilter a_filter) noexcept;
	FORCE_INLINE constexpr void                    min_filter(rhi::TextureFilter a_filter) noexcept;
	FORCE_INLINE constexpr void                    mip_mode(rhi::TextureMipFilter a_mode) noexcept;
	FORCE_INLINE constexpr void                    wrap_s(rhi::TextureAddressMode a_wrap) noexcept;
	FORCE_INLINE constexpr void                    wrap_t(rhi::TextureAddressMode a_wrap) noexcept;
	FORCE_INLINE constexpr void                    wrap_u(rhi::TextureAddressMode a_wrap) noexcept;
	FORCE_INLINE constexpr void                    border_color(rhi::TextureBorder a_color) noexcept;

  protected:
	FORCE_INLINE TextureSamplerCrtp() = default;        //! Default constructor
  private:
	rhi::TextureFilter      m_mag_filter{rhi::TextureFilter::linear};                //! Magnification filter
	rhi::TextureFilter      m_min_filter{rhi::TextureFilter::linear};                //! Minification filter
	rhi::TextureMipFilter   m_mip_mode{rhi::TextureMipFilter::not_mipmapped};        //! Mipmapping mode
	rhi::TextureAddressMode m_wrap_s{rhi::TextureAddressMode::clamp_to_edge};        //! Wrapping mode in X direction
	rhi::TextureAddressMode m_wrap_t{rhi::TextureAddressMode::clamp_to_edge};        //! Wrapping mode in Y direction
	rhi::TextureAddressMode m_wrap_u{rhi::TextureAddressMode::clamp_to_edge};        //! Wrapping mode in Z direction
	rhi::TextureBorder      m_border_color{rhi::TextureBorder::opaque};              //! Border color by default is opaque black
};

// static_assert(std::is_trivially_copyable_v<TextureSampler>, "TextureSampler is not trivially copyable");
// static_assert(std::is_standard_layout_v<TextureSampler>, "TextureSampler is not standard layout");

template <class _type>
class ROAR_ENGINE_ITEM TextureCrtp : public ror::Crtp<_type, TextureCrtp>
{
  public:
	FORCE_INLINE              TextureCrtp(const TextureCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              TextureCrtp(TextureCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TextureCrtp &operator=(const TextureCrtp &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE TextureCrtp &operator=(TextureCrtp &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~TextureCrtp() noexcept override                 = default;        //! Destructor

	FORCE_INLINE TextureCrtp(rhi::TextureImageHandle a_image_handle, rhi::TextureSamplerHandle a_sampler_handle) :
	    m_texture_image(a_image_handle), m_texture_sampler(a_sampler_handle)
	{}

	rhi::TextureImageHandle   texture_image();
	rhi::TextureSamplerHandle texture_sampler();
	rhi::TextureImageHandle   texture_image() const;
	rhi::TextureSamplerHandle texture_sampler() const;
	void                      texture_image(rhi::TextureImageHandle a_handle);
	void                      texture_sampler(rhi::TextureSamplerHandle a_handle);

  protected:
	FORCE_INLINE TextureCrtp() = default;        //! Default constructor
  private:
	rhi::TextureImageHandle   m_texture_image{-1};         //! TextureImage handle, -1 means invalid handle which is possible
	rhi::TextureSamplerHandle m_texture_sampler{0};        //! TextureSampler handle, 0 means default sampler
};

// static_assert(std::is_trivially_copyable_v<Texture>, "Texture is not trivially copyable");
// static_assert(std::is_standard_layout_v<Texture>, "Texture is not standard layout");

}        // namespace rhi

#include "rortexture.hh"
