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
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorhandles.hpp"
#include "rhi/rortypes.hpp"
#include <filesystem>
#include <vector>

namespace rhi
{
class ROAR_ENGINE_ITEM TextureImage final
{
  public:
	FORCE_INLINE               TextureImage()                                = default;        //! Default constructor
	FORCE_INLINE               TextureImage(const TextureImage &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE               TextureImage(TextureImage &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TextureImage &operator=(const TextureImage &a_other) = delete;                //! Copy assignment operator
	FORCE_INLINE TextureImage &operator=(TextureImage &&a_other) noexcept = default;           //! Move assignment operator
	FORCE_INLINE ~TextureImage() noexcept                                 = default;           //! Destructor

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

	struct Mipmap
	{
		uint32_t m_width{0};         // Width of this mipmap
		uint32_t m_height{0};        // Height of this mipmap
		uint32_t m_depth{1};         // Depth of this mipmap, always 1 unless 3D volume texture
		uint64_t m_offset{0};        // Offset in the data array inside the TexturImagee
	};

	FORCE_INLINE TextureAPIHandle handle() const noexcept;
	FORCE_INLINE uint32_t         width() const noexcept;
	FORCE_INLINE uint32_t         height() const noexcept;
	FORCE_INLINE PixelFormat      format() const noexcept;
	FORCE_INLINE uint32_t         levels() const noexcept;
	FORCE_INLINE TextureTarget    target() const noexcept;

	FORCE_INLINE void handle(TextureHandle) noexcept;
	FORCE_INLINE void width(uint32_t) noexcept;
	FORCE_INLINE void height(uint32_t) noexcept;
	FORCE_INLINE void format(PixelFormat) noexcept;
	FORCE_INLINE void target(TextureTarget) noexcept;

  protected:
  private:
	FORCE_INLINE void allocate(uint64_t a_size);

	TextureAPIHandle     m_handle{-1};                                            // Texture id used by the Renderer APIs
	uint64_t             m_size{0};                                               // Size of all mipmaps combined in bytes
	TextureTarget        m_target{TextureTarget::texture_2D};                     // Can be 1D, 2D or 3D etc texture
	PixelFormat          m_format{PixelFormat::r8g8b8a8_uint32_norm_srgb};        // Pixel format of the texture
	std::vector<uint8_t> m_data{};                                                // All mipmaps data, NOTE: This doesn't have to be BufferAllocated
	std::vector<Mipmap>  m_mips{};                                                // Will have at least one level, base texture width and height are mip[0] width/height, NOTE: This doesn't have to be BufferAllocated
};

// static_assert(std::is_trivially_copyable_v<TextureImage>, "TextureImage is not trivially copyable");
static_assert(std::is_standard_layout_v<TextureImage>, "TextureImage is not standard layout");

class ROAR_ENGINE_ITEM TextureSampler final
{
  public:
	FORCE_INLINE                 TextureSampler()                                  = default;        //! Default constructor
	FORCE_INLINE                 TextureSampler(const TextureSampler &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                 TextureSampler(TextureSampler &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE TextureSampler &operator=(const TextureSampler &a_other) = default;                 //! Copy assignment operator
	FORCE_INLINE TextureSampler &operator=(TextureSampler &&a_other) noexcept = default;             //! Move assignment operator
	FORCE_INLINE ~TextureSampler() noexcept                                   = default;             //! Destructor

	TextureFilter m_mag_filter{TextureFilter::linear};                      // Magnification filter
	TextureFilter m_min_filter{TextureFilter::linear_mipmap_linear};        // Minification filter
	TextureWrap   m_wrap_s{TextureWrap::repeat};                            // Wrapping mode in X direction
	TextureWrap   m_wrap_t{TextureWrap::repeat};                            // Wrapping mode in Y direction
	TextureWrap   m_wrap_u{TextureWrap::repeat};                            // Wrapping mode in Z direction
};

static_assert(std::is_trivially_copyable_v<TextureSampler>, "TextureSampler is not trivially copyable");
static_assert(std::is_standard_layout_v<TextureSampler>, "TextureSampler is not standard layout");

class ROAR_ENGINE_ITEM Texture final
{
  public:
	FORCE_INLINE          Texture()                           = default;          //! Default constructor
	FORCE_INLINE          Texture(const Texture &a_other)     = default;          //! Copy constructor
	FORCE_INLINE          Texture(Texture &&a_other) noexcept = default;          //! Move constructor
	FORCE_INLINE Texture &operator=(const Texture &a_other) = default;            //! Copy assignment operator
	FORCE_INLINE Texture &operator=(Texture &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE ~Texture() noexcept                            = default;        //! Destructor

	TextureImageHandle   m_texture_image{-1};         // TextureImage handle, -1 means invalid handle which is possible
	TextureSamplerHandle m_texture_sampler{0};        // TextureSampler handle, 0 means default sampler
};

static_assert(std::is_trivially_copyable_v<Texture>, "Texture is not trivially copyable");
static_assert(std::is_standard_layout_v<Texture>, "Texture is not standard layout");

// Define type to semantic for buffers allocator
define_type_to_shader_semantics(TextureImage)
{
	return BufferSemantic::texture_image_data;
}

define_type_to_shader_semantics(TextureSampler)
{
	return BufferSemantic::texture_sampler_data;
}

define_type_to_shader_semantics(Texture)
{
	return BufferSemantic::texture_data;
}

}        // namespace rhi

#include "rortexture.hh"
