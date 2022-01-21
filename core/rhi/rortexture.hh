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
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <memory>

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

FORCE_INLINE uint32_t TextureImage::depth() const noexcept
{
	return this->m_mips[0].m_depth;
}

FORCE_INLINE PixelFormat TextureImage::format() const noexcept
{
	return this->m_format;
}

FORCE_INLINE uint32_t TextureImage::levels() const noexcept
{
	return ror::static_cast_safe<uint32_t>(this->m_mips.size());
}
FORCE_INLINE TextureTarget TextureImage::target() const noexcept
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

FORCE_INLINE void TextureImage::depth(uint32_t a_depth) noexcept
{
	this->m_mips[0].m_depth = a_depth;
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
	auto *ptr    = new uint8_t[this->m_size];
	this->m_data.reset(ptr);
}

FORCE_INLINE uint8_t *TextureImage::data() const noexcept
{
	return this->m_data.get();
}
FORCE_INLINE std::vector<TextureImage::Mipmap> &TextureImage::mips() noexcept
{
	return this->m_mips;
}

// Now TextureImage own the a_data pointer
FORCE_INLINE void TextureImage::reset(uint8_t *a_data, uint64_t a_size) noexcept
{
	this->m_size = a_size;
	this->m_data.reset(a_data);
}

FORCE_INLINE void TextureImage::push_empty_mip() noexcept
{
	this->m_mips.emplace_back();
}

static void read_texture_from_file_stb(ror::Resource &a_texture_resource, TextureImage &a_texture)
{
	int32_t     w = 0, h = 0, bpp = 0;
	const auto &resource_data = a_texture_resource.data();
	auto       *a_data        = stbi_load_from_memory(resource_data.data(), ror::static_cast_safe<int32_t>(resource_data.size()), &w, &h, &bpp, 0);        // Final argument = 0 means get real bpp

	a_texture.push_empty_mip();
	a_texture.format(PixelFormat::r8g8b8a8_uint32_norm_srgb);           // TODO: How do I read this via STB?
	a_texture.reset(a_data, static_cast<uint64_t>(w * h * bpp));        // a_texture now owns the a_data pointer returned by stbi
	a_texture.width(static_cast<uint32_t>(w));
	a_texture.height(static_cast<uint32_t>(h));
	a_texture.depth(static_cast<uint32_t>(bpp));
}

static PixelFormat basis_to_format(basist::transcoder_texture_format a_fmt)
{
	switch (a_fmt)
	{
		case basist::transcoder_texture_format::cTFASTC_4x4:
			return PixelFormat::astc_4x4_srgb_block;
		case basist::transcoder_texture_format::cTFBC7_RGBA:
			return PixelFormat::bc7_srgb_block;
		case basist::transcoder_texture_format::cTFRGBA32:
			return PixelFormat::r8g8b8a8_uint32_norm_srgb;
		case basist::transcoder_texture_format::cTFETC1_RGB:
		case basist::transcoder_texture_format::cTFETC2_RGBA:
		case basist::transcoder_texture_format::cTFBC1_RGB:
		case basist::transcoder_texture_format::cTFBC3_RGBA:
		case basist::transcoder_texture_format::cTFBC4_R:
		case basist::transcoder_texture_format::cTFBC5_RG:
		case basist::transcoder_texture_format::cTFPVRTC1_4_RGB:
		case basist::transcoder_texture_format::cTFPVRTC1_4_RGBA:
		case basist::transcoder_texture_format::cTFATC_RGB:
		case basist::transcoder_texture_format::cTFATC_RGBA:
		case basist::transcoder_texture_format::cTFFXT1_RGB:
		case basist::transcoder_texture_format::cTFPVRTC2_4_RGB:
		case basist::transcoder_texture_format::cTFPVRTC2_4_RGBA:
		case basist::transcoder_texture_format::cTFETC2_EAC_R11:
		case basist::transcoder_texture_format::cTFETC2_EAC_RG11:
		case basist::transcoder_texture_format::cTFRGB565:
		case basist::transcoder_texture_format::cTFBGR565:
		case basist::transcoder_texture_format::cTFRGBA4444:
		case basist::transcoder_texture_format::cTFBC7_ALT:
		case basist::transcoder_texture_format::cTFTotalTextureFormats:
			ror::log_critical("Format not supported yet!, add support before continuing.");
			return PixelFormat::r8g8b8a8_uint32_norm_srgb;
	}

	return PixelFormat::r8g8b8a8_uint32_norm_srgb;
}

static void read_texture_basis_universal(ror::Resource &a_texture_resource, TextureImage &a_texture)
{
	// FIXME: Should only be called once per execution, check if this could be used in MT environment
	basist::basisu_transcoder_init();

	const std::vector<uint8_t> &ktx2_file_data = a_texture_resource.data();

	// Should be done for each transcode
	basist::etc1_global_selector_codebook sel_codebook(basist::g_global_selector_cb_size, basist::g_global_selector_cb);
	basist::ktx2_transcoder               dec(&sel_codebook);

	if (!dec.init(ktx2_file_data.data(), ror::static_cast_safe<uint32_t>(ktx2_file_data.size())))
	{
		ror::log_critical("Basis transcode init failed.");
		return;
	}

	if (!dec.start_transcoding())
	{
		ror::log_critical("Basis start_transcoding failed.");
		return;
	}

	ror::log_info("Resolution: {}x{}", dec.get_width(), dec.get_height());
	ror::log_info("Mipmap Levels: {}", dec.get_levels());
	ror::log_info("Texture Array Size (layers): {}", dec.get_layers());
	ror::log_info("Total Faces: {} (%s)", dec.get_faces(), (dec.get_faces() == 6) ? "CUBEMAP" : "2D");
	ror::log_info("Is Texture Video: {}", dec.is_video());

	const bool is_etc1s = dec.get_format() == basist::basis_tex_format::cETC1S;
	ror::log_info("Supercompression Format: %s", is_etc1s ? "ETC1S" : "UASTC");

	ror::log_info("Supercompression Scheme: ");
	switch (dec.get_header().m_supercompression_scheme)
	{
		case basist::KTX2_SS_NONE:
			ror::log_info("\t \t NONE");
			break;
		case basist::KTX2_SS_BASISLZ:
			ror::log_info("\t \t BASISLZ");
			break;
		case basist::KTX2_SS_ZSTANDARD:
			ror::log_info("\t \t ZSTANDARD");
			break;
		default:
			ror::log_info("\t \t Invalid/unknown/unsupported");
			return;
	}

	ror::log_info("Has Alpha: {}", static_cast<uint32_t>(dec.get_has_alpha()));
	ror::log_info("Levels:");

	for (uint32_t i = 0; i < dec.get_levels(); i++)
	{
		ror::log_info("{}. Offset: {}, Length: {}, Uncompressed Length: {}",
					  i, static_cast<long long unsigned int>(dec.get_level_index()[i].m_byte_offset),
					  static_cast<long long unsigned int>(dec.get_level_index()[i].m_byte_length),
					  static_cast<long long unsigned int>(dec.get_level_index()[i].m_uncompressed_byte_length));
	}

	// TODO: This needs abstracting out
	basist::transcoder_texture_format tex_fmt = basist::transcoder_texture_format::cTFBC7_RGBA;
	// basist::transcoder_texture_format tex_fmt = basist::transcoder_texture_format::cTFRGBA32;

	// Use this for your format
	bool compressed = !basist::basis_transcoder_format_is_uncompressed(tex_fmt);

	if (!basis_is_format_supported(tex_fmt, dec.get_format()))
	{
		// Error not supported transcoder format
		ror::log_critical("Requested transcoded format not supported {}", basis_to_format(tex_fmt));
		return;
	}

	const uint32_t total_layers       = std::max(1u, dec.get_layers());
	uint64_t       mips_size          = 0;
	uint64_t       decoded_block_size = basisu::get_qwords_per_block(basist::basis_get_basisu_texture_format(tex_fmt)) * sizeof(uint64_t);

	for (uint32_t level_index = 0; level_index < dec.get_levels(); level_index++)
	{
		for (uint32_t layer_index = 0; layer_index < total_layers; layer_index++)
		{
			for (uint32_t face_index = 0; face_index < dec.get_faces(); face_index++)
			{
				basist::ktx2_image_level_info level_info;

				if (!dec.get_image_level_info(level_info, level_index, layer_index, face_index))
				{
					ror::log_critical("Failed retrieving image level information ({}, {}, {})!\n", layer_index, level_index, face_index);
					return;
				}

				if (compressed)
					mips_size += decoded_block_size * level_info.m_total_blocks;
				else
					mips_size += level_info.m_orig_height * level_info.m_orig_width * 4;        // FIXME: Only works for RGBA32 uncompressed format
			}
		}
	}

	a_texture.allocate(mips_size);
	a_texture.format(basis_to_format(tex_fmt));

	uint8_t *decoded_data = a_texture.data();
	uint64_t mip_offset   = 0;
	auto    &texture_mips = a_texture.mips();

	for (uint32_t level_index = 0; level_index < dec.get_levels(); level_index++)
	{
		for (uint32_t layer_index = 0; layer_index < total_layers; layer_index++)
		{
			for (uint32_t face_index = 0; face_index < dec.get_faces(); face_index++)
			{
				basist::ktx2_image_level_info level_info;

				if (!dec.get_image_level_info(level_info, level_index, layer_index, face_index))
				{
					ror::log_critical("Failed retrieving image level information {}, {}, {}", layer_index, level_index, face_index);
					return;
				}

				uint32_t decode_flags = 0;
				uint64_t decode_size  = level_info.m_orig_height * level_info.m_orig_width * 4;        // FIXME: Only works for RGBA

				if (compressed)
					decode_size = decoded_block_size * level_info.m_total_blocks;

				if (!dec.transcode_image_level(level_index, layer_index, face_index, decoded_data + mip_offset, (compressed ? level_info.m_total_blocks : static_cast<uint32_t>(decode_size)), tex_fmt, decode_flags))
				{
					ror::log_critical("Failed transcoding image level {}, {}, {}, {}", layer_index, level_index, face_index, tex_fmt);
					return;
				}

				TextureImage::Mipmap mip;
				mip.m_width  = level_info.m_orig_width;
				mip.m_height = level_info.m_orig_height;
				mip.m_offset = mip_offset;

				texture_mips.emplace_back(mip);

				if (ror::get_build() == ror::BuildType::build_debug)
				{
					if (ror::get_settings().get<bool>("visualise_mipmaps"))
					{
						const std::vector<ror::Vector3f> colors{{1.0f, 0.0f, 0.0f},
																{0.0f, 1.0f, 0.0f},
																{0.0f, 0.0f, 1.0f},
																{1.0f, 1.0f, 0.0f},
																{1.0f, 0.0f, 1.0f},
																{0.0f, 1.0f, 1.0f},
																{0.0f, 0.0f, 0.0f},
																{1.0f, 1.0f, 1.0f},
																{1.0f, 0.0f, 0.0f},
																{0.0f, 0.0f, 1.0f}};

						for (size_t i = 0; i < decode_size; i += 4)        // FIXME: Only works for RGBA
						{
							uint8_t cs[3];

							cs[0] = ror::static_cast_safe<uint8_t>(colors[level_index % 10].x * 255);
							cs[1] = ror::static_cast_safe<uint8_t>(colors[level_index % 10].y * 255);
							cs[2] = ror::static_cast_safe<uint8_t>(colors[level_index % 10].z * 255);

							decoded_data[mip_offset + i + 0] = cs[0];
							decoded_data[mip_offset + i + 1] = cs[1];
							decoded_data[mip_offset + i + 2] = cs[2];
						}
					}
				}

				mip_offset += decode_size;
			}
		}
	}
}

template <TextureTarget _target>
FORCE_INLINE TextureImage read_texture_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels)
{
	TextureImage texture;

	auto &texture_resource = ror::load_resource(a_absolute_file_name, ror::ResourceSemantic::textures);

	// assumes ktx file is basisu ktx. TODO: Should also add support for normal ktx files
	if (a_absolute_file_name.extension() == ".ktx2")
		read_texture_basis_universal(texture_resource, texture);
	else if (a_separate_channels)
		assert(0 && "Seprate channels not implemented yet");
	else
		read_texture_from_file_stb(texture_resource, texture);

	return texture;
}

FORCE_INLINE TextureImage read_texture_1d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels)
{
	return read_texture_from_file<TextureTarget::texture_1D>(a_absolute_file_name, a_separate_channels);
}

FORCE_INLINE TextureImage read_texture_2d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels)
{
	return read_texture_from_file<TextureTarget::texture_2D>(a_absolute_file_name, a_separate_channels);
}

FORCE_INLINE TextureImage read_texture_3d_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels)
{
	return read_texture_from_file<TextureTarget::texture_3D>(a_absolute_file_name, a_separate_channels);
}

FORCE_INLINE TextureImage read_texture_cube_from_file(const std::filesystem::path &a_absolute_file_name, bool a_separate_channels)
{
	return read_texture_from_file<TextureTarget::texture_cube>(a_absolute_file_name, a_separate_channels);
}

}        // namespace rhi
