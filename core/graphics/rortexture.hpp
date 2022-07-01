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

#pragma once

#include "foundation/rormacros.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rortypes.hpp"
#include <atomic>
#include <filesystem>
#include <memory>
#include <utility>
#include <vector>

// Global function to create rhi::TextureImage
#include "transcoder/basisu_transcoder.h"

namespace ror
{

enum class TextureTarget
{
	texture_1D,
	texture_2D,
	texture_3D,
	texture_cube,
	texture_2D_MS,
	texture_2D_MS_array,
	texture_1D_array,
	texture_2D_array,
	texture_cube_array
};

class ROAR_ENGINE_ITEM TextureImage final
{
  public:
	FORCE_INLINE               TextureImage()                                = default;        //! Default constructor
	FORCE_INLINE               TextureImage(const TextureImage &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE               TextureImage(TextureImage &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TextureImage &operator=(const TextureImage &a_other)        = delete;         //! Copy assignment operator
	FORCE_INLINE TextureImage &operator=(TextureImage &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~TextureImage() noexcept                                    = default;        //! Destructor

	struct Mipmap
	{
		uint32_t m_width{0};         // Width of this mipmap
		uint32_t m_height{0};        // Height of this mipmap
		uint32_t m_depth{1};         // Depth of this mipmap, always 1 unless 3D volume texture
		uint64_t m_offset{0};        // Offset in the data array inside the TexturImage
	};

	FORCE_INLINE rhi::TextureAPIHandle handle() const noexcept;
	FORCE_INLINE uint32_t              width() const noexcept;
	FORCE_INLINE uint32_t              height() const noexcept;
	FORCE_INLINE uint32_t              depth() const noexcept;
	FORCE_INLINE rhi::PixelFormat format() const noexcept;
	FORCE_INLINE uint32_t         levels() const noexcept;
	FORCE_INLINE TextureTarget    target() const noexcept;
	FORCE_INLINE uint8_t         *data() const noexcept;
	FORCE_INLINE std::vector<Mipmap> &mips() noexcept;

	FORCE_INLINE void handle(rhi::TextureHandle) noexcept;
	FORCE_INLINE void width(uint32_t) noexcept;
	FORCE_INLINE void height(uint32_t) noexcept;
	FORCE_INLINE void depth(uint32_t) noexcept;
	FORCE_INLINE void format(rhi::PixelFormat) noexcept;
	FORCE_INLINE void target(TextureTarget) noexcept;
	FORCE_INLINE void reset(uint8_t *, uint64_t) noexcept;
	FORCE_INLINE void push_empty_mip() noexcept;
	FORCE_INLINE void allocate(uint64_t a_size);

  protected:
  private:
	// TextureAPIHandle           m_handle{-1};                                            // Texture id used by the Renderer APIs
	uint64_t                   m_size{0};                                                    // Size of all mipmaps combined in bytes
	TextureTarget              m_target{TextureTarget::texture_2D};                          // Can be 1D, 2D or 3D etc texture
	rhi::PixelFormat           m_format{rhi::PixelFormat::r8g8b8a8_uint32_norm_srgb};        // Pixel format of the texture
	std::unique_ptr<uint8_t[]> m_data{};                                                     // All mipmaps data
	std::vector<Mipmap>        m_mips{};                                                     // Will have at least one level, base texture width and height are mip[0] width/height, NOTE: This doesn't have to be BufferAllocated
};

// static_assert(std::is_trivially_copyable_v<TextureImage>, "TextureImage is not trivially copyable");
static_assert(std::is_standard_layout_v<TextureImage>, "TextureImage is not standard layout");

class ROAR_ENGINE_ITEM TextureSampler final
{
  public:
	FORCE_INLINE                 TextureSampler()                                  = default;        //! Default constructor
	FORCE_INLINE                 TextureSampler(const TextureSampler &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                 TextureSampler(TextureSampler &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TextureSampler &operator=(const TextureSampler &a_other)          = default;        //! Copy assignment operator
	FORCE_INLINE TextureSampler &operator=(TextureSampler &&a_other) noexcept      = default;        //! Move assignment operator
	FORCE_INLINE ~TextureSampler() noexcept                                        = default;        //! Destructor

	rhi::TextureFilter      mag_filter();
	rhi::TextureFilter      min_filter();
	rhi::TextureFilter      mip_mode();
	rhi::TextureAddressMode wrap_s();
	rhi::TextureAddressMode wrap_t();
	rhi::TextureAddressMode wrap_u();
	void                    mag_filter(rhi::TextureFilter a_filter);
	void                    min_filter(rhi::TextureFilter a_filter);
	void                    mip_mode(rhi::TextureFilter a_mode);
	void                    wrap_s(rhi::TextureAddressMode a_wrap);
	void                    wrap_t(rhi::TextureAddressMode a_wrap);
	void                    wrap_u(rhi::TextureAddressMode a_wrap);

  private:
	rhi::TextureFilter      m_mag_filter{rhi::TextureFilter::linear};         // Magnification filter
	rhi::TextureFilter      m_min_filter{rhi::TextureFilter::linear};         // Minification filter
	rhi::TextureFilter      m_mip_mode{rhi::TextureFilter::linear};           // Mipmapping mode
	rhi::TextureAddressMode m_wrap_s{rhi::TextureAddressMode::repeat};        // Wrapping mode in X direction
	rhi::TextureAddressMode m_wrap_t{rhi::TextureAddressMode::repeat};        // Wrapping mode in Y direction
	rhi::TextureAddressMode m_wrap_u{rhi::TextureAddressMode::repeat};        // Wrapping mode in Z direction
};

static_assert(std::is_trivially_copyable_v<TextureSampler>, "TextureSampler is not trivially copyable");
static_assert(std::is_standard_layout_v<TextureSampler>, "TextureSampler is not standard layout");

class ROAR_ENGINE_ITEM Texture final
{
  public:
	FORCE_INLINE          Texture()                             = default;        //! Default constructor
	FORCE_INLINE          Texture(const Texture &a_other)       = default;        //! Copy constructor
	FORCE_INLINE          Texture(Texture &&a_other) noexcept   = default;        //! Move constructor
	FORCE_INLINE Texture &operator=(const Texture &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Texture &operator=(Texture &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Texture() noexcept                            = default;        //! Destructor

	rhi::TextureImageHandle   texture_image();
	rhi::TextureSamplerHandle texture_sampler();
	void                      texture_image(rhi::TextureImageHandle a_handle);
	void                      texture_sampler(rhi::TextureSamplerHandle a_handle);

  private:
	rhi::TextureImageHandle   m_texture_image{-1};         // TextureImage handle, -1 means invalid handle which is possible
	rhi::TextureSamplerHandle m_texture_sampler{0};        // TextureSampler handle, 0 means default sampler
};

static_assert(std::is_trivially_copyable_v<Texture>, "Texture is not trivially copyable");
static_assert(std::is_standard_layout_v<Texture>, "Texture is not standard layout");

void              read_texture_from_memory(const uint8_t *a_data, size_t a_data_size, TextureImage &a_texture);
FORCE_INLINE void read_texture_from_resource(ror::Resource &a_texture_resource, TextureImage &a_texture);

/**
 * @brief      Generic image loader that will invoke the right decoder based on extension from absolute path
 * @details    Read a texture via absolute file name via resource loading mechanism.
 *             Internally it might use stb or basisu or ktx to load different types of texture images
 * @param      a_absolute_file_name As the name suggests, an absolute file name of the texture to load
 * @return     TextureImage that contains all the mipmaps of the texture and all pixel data
 */
template <TextureTarget _target>
FORCE_INLINE TextureImage read_texture_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);

FORCE_INLINE TextureImage read_texture_1d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);
FORCE_INLINE TextureImage read_texture_2d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);
FORCE_INLINE TextureImage read_texture_3d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);
FORCE_INLINE TextureImage read_texture_cube_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels = false);

}        // namespace ror

namespace rhi
{

// Define type to semantic for buffers allocator
define_type_to_shader_semantics(ror::TextureImage)
{
	return rhi::BufferSemantic::texture_image_data;
}

define_type_to_shader_semantics(ror::TextureSampler)
{
	return rhi::BufferSemantic::texture_sampler_data;
}

define_type_to_shader_semantics(ror::Texture)
{
	return rhi::BufferSemantic::texture_data;
}

}        // namespace rhi

#include "rortexture.hh"
