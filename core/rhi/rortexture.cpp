// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021-2023
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

#include "foundation/rorsystem.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "settings/rorsettings.hpp"
#include <cassert>
#include <cstddef>
#include <cstring>

#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace rhi
{

ror::Vector4f reinhard_tone_mapping(ror::Vector4f color)
{
	return color / (color + ror::Vector4f(1.0f));
}

static void _stbi_write_function(void *a_context, void *a_data, int a_size)
{
	auto *ptr = reinterpret_cast<std::pair<std::vector<uint8_t> *, size_t> *>(a_context);

	assert(ptr->second + static_cast<size_t>(a_size) <= ptr->first->size());

	memcpy(ptr->first->data() + ptr->second, a_data, static_cast<size_t>(a_size));

	ptr->second += static_cast<size_t>(a_size);
}

void write_hdr(const std::filesystem::path &a_path, uint32_t a_width, uint32_t a_height, float32_t *a_pixel_data, bool a_overwrite)
{
	auto &output_texture = ror::resource(a_path, ror::ResourceSemantic::textures, ror::ResourceAction::make);

	if (a_overwrite)
		output_texture.remove();

	int32_t comp = 4;

	std::vector<uint8_t> tga_data{};
	tga_data.resize(a_width * a_height * static_cast<uint32_t>(comp) * 3);        // We only write out RGB data for HDR
	std::pair<std::vector<uint8_t> *, size_t> ptr_offset = std::make_pair(&tga_data, 0);

	void *context = reinterpret_cast<void *>(&ptr_offset);

	stbi_flip_vertically_on_write(true);
	auto res = stbi_write_hdr_to_func(_stbi_write_function, context, static_cast<int32_t>(a_width), static_cast<int32_t>(a_height), comp, a_pixel_data);
	stbi_flip_vertically_on_write(false);

	if (res == 0)
		ror::log_critical("Couldn't write file {}", a_path.c_str());

	output_texture.update({tga_data.begin(), tga_data.end()}, true, false, true);        // Force updating the Resource because I am sure no one else is using it
	output_texture.flush();
}

void write_tga(const std::filesystem::path &a_path, uint32_t a_width, uint32_t a_height, uint8_t *a_pixel_data, bool a_overwrite)
{
	auto &output_texture = ror::resource(a_path, ror::ResourceSemantic::textures, ror::ResourceAction::make);

	if (a_overwrite)
		output_texture.remove();

	int32_t comp = 4;

	std::vector<uint8_t> tga_data{};
	tga_data.resize(a_width * a_height * static_cast<uint32_t>(comp));
	std::pair<std::vector<uint8_t> *, size_t> ptr_offset = std::make_pair(&tga_data, 0);

	void *context = reinterpret_cast<void *>(&ptr_offset);

	auto res = stbi_write_tga_to_func(_stbi_write_function, context, static_cast<int32_t>(a_width), static_cast<int32_t>(a_height), comp, a_pixel_data);

	if (res == 0)
		ror::log_critical("Couldn't write file {}", a_path.c_str());

	output_texture.update({tga_data.begin(), tga_data.end()}, true, false, true);        // Force updating the Resource because I am sure no one else is using it
	output_texture.flush();
}

void write_ppm(const std::filesystem::path &a_path, uint32_t a_width, uint32_t a_height, uint8_t *a_pixel_data)
{
	auto &output_texture = ror::resource(a_path, ror::ResourceSemantic::textures, ror::ResourceAction::make);

	std::string w{std::to_string(a_width)};
	std::string h{std::to_string(a_height)};

	std::vector<uint8_t> ppm_data{};
	ppm_data.reserve(a_width * a_height * 3 + 17);        // 17 is the header size assuming texture size of 9999 max
	ppm_data.push_back('P');
	ppm_data.push_back('6');
	ppm_data.push_back('\n');
	for (auto &wc : w)
		ppm_data.push_back(static_cast<uint8_t>(wc));
	ppm_data.push_back(' ');
	for (auto &hc : h)
		ppm_data.push_back(static_cast<uint8_t>(hc));
	ppm_data.push_back('\n');
	ppm_data.push_back('2');
	ppm_data.push_back('5');
	ppm_data.push_back('5');
	ppm_data.push_back('\n');

	for (uint32_t i = 0; i < a_height; i++)
	{
		for (uint32_t j = 0; j < a_width; j++)
		{
			for (uint32_t k = 0; k < 3; ++k)        // Ignoring 4th component Alpha
				ppm_data.push_back(a_pixel_data[((j * 4) + (a_width * 4 * i)) + k] % 256);
		}
	}

	output_texture.update({ppm_data.begin(), ppm_data.end()}, true, false, true);        // Force updating the Resource because I am sure no one else is using it
	output_texture.flush();
}

void write_ppm(const std::filesystem::path &a_path, uint32_t a_width, uint32_t a_height, std::vector<uint8_t> &a_data)
{
	assert(a_data.size() == (a_width * a_height * 4) && "Can only write RGBA ppm images");

	uint8_t *pixel_data = a_data.data();
	write_ppm(a_path, a_width, a_height, pixel_data);
}

void fill_texture_from_memory(uint8_t *a_data, uint32_t a_width, uint32_t a_height, uint32_t a_bytes_per_pixel, rhi::TextureImage &a_texture, bool a_is_hdr, const std::string &a_name)
{
	uint32_t expected_bpp = a_is_hdr ? 16 : 4;
	(void) expected_bpp;

	assert(a_bytes_per_pixel == expected_bpp && "Only 4/16 bytes per pixel supported at the moment");        // Don't want to deal with anything other than 16 and 4 bytes per pixel

	a_texture.push_empty_mip();

	// By default consider everything sRGB.
	// glTF base color and emissive needs to be sRGB because of that everything else needs to be explicitly set to linear
	if (!a_is_hdr)
	{
		if (ror::settings().m_force_linear_textures)
			a_texture.format(rhi::PixelFormat::r8g8b8a8_uint32_norm);
		else
			a_texture.format(rhi::PixelFormat::r8g8b8a8_uint32_norm_srgb);
	}

	a_texture.reset(a_data, static_cast<uint64_t>(a_width * a_height * a_bytes_per_pixel));        // a_texture now owns the new_data pointer
	a_texture.width(a_width);
	a_texture.height(a_height);
	a_texture.depth(1u);
	a_texture.bytes_per_pixel(a_bytes_per_pixel);
	a_texture.name(a_name);
	a_texture.usage(rhi::TextureUsage::shader_read);
	a_texture.hdr(a_is_hdr);
	a_texture.setup();
}

void read_texture_from_memory(const uint8_t *a_data, size_t a_data_size, rhi::TextureImage &a_texture, bool a_is_hdr, const std::string &a_name)
{
	// If req_comp is ever changed from 4 to bpp, make sure ORM[H] texture is swizzled accordingly or separated
	int32_t w = 0, h = 0, bpp = 0, req_comp = 4;

	auto &setting = ror::settings();

	if (!setting.m_force_rgba_textures)
		ror::log_critical("Reading 4 component textures but force_rgba_textures isn't set in settings.json");

	uint8_t *new_data{nullptr};

	stbi_set_flip_vertically_on_load(a_is_hdr);

	if (setting.m_force_ldr_textures)
		a_is_hdr = false;

	if (a_is_hdr)
	{
		// Sanity check, checking hdr is always 3 but we are expanding it using stbi_image
		if constexpr (ror::get_build() == ror::BuildType::build_debug)
		{
			int32_t x = 0, y = 0, comp = 4;
			int     res = stbi_info_from_memory(a_data, ror::static_cast_safe<int32_t>(a_data_size), &x, &y, &comp);
			assert(res && "stbi_info_from_memory failed");
			(void) res;

			assert(comp == 3 && "HDR format must have 3 components");
		}

		// TODO: Use the following available hdr formats in metal
		// PixelFormatRG11B10Float = 92,
		// PixelFormatRGB9E5Float = 93,

		// Currently due to how stb image can only convert each channel to 32-bit float, I have to use the following 128bit format
		// r32g32b32a32_float128         = PlatformPixelFormatRGBA32Float == PixelFormatRGBA32Float = 125,

		// Following are used to control stbi hdr to ldr conversion if any ldr image is loaded via the 'f' interface, but this is not relevant for me
		// stbi_hdr_to_ldr_gamma(2.2f);
		// stbi_hdr_to_ldr_scale(1.0f);

		auto *new_float_data = stbi_loadf_from_memory(a_data, ror::static_cast_safe<int32_t>(a_data_size), &w, &h, &bpp, req_comp);        // Final argument = 0 means get real bpp
		new_data             = reinterpret_cast<uint8_t *>(new_float_data);
	}
	else
	{
		new_data = stbi_load_from_memory(a_data, ror::static_cast_safe<int32_t>(a_data_size), &w, &h, &bpp, req_comp);        // Final argument = 0 means get real bpp
	}

	stbi_set_flip_vertically_on_load(false);

	// Incase testing of the image is requird if its loaded correctly or not
	// write_ppm("image_loaded_data.ppm", static_cast<uint32_t>(w), static_cast<uint32_t>(h), a_is_hdr ? reinterpre_cast<float32_t*>(new_data) : new_data);

	// This will now consume the new_data pointer so no need to clean it up
	fill_texture_from_memory(new_data, static_cast<uint32_t>(w), static_cast<uint32_t>(h), static_cast<uint32_t>(req_comp * (a_is_hdr ? 4 : 1)), a_texture, a_is_hdr, a_name);
}

TextureImage make_texture(rhi::Device &a_device, rhi::PixelFormat a_format, uint32_t a_width, uint32_t a_height, rhi::TextureTarget a_target,
						  rhi::TextureUsage a_usage, rhi::TextureMipGenMode a_gen_mode, bool a_mipmapped, bool a_is_hdr)
{
	rhi::TextureImage texture_image{};

	texture_image.format(a_format);
	texture_image.mip_gen_mode(a_gen_mode);
	texture_image.mipmapped(a_mipmapped);
	texture_image.target(a_target);
	texture_image.push_empty_mip();
	texture_image.width(a_width);
	texture_image.height(a_height);
	texture_image.usage(a_usage);
	texture_image.hdr(a_is_hdr);

	auto size = texture_image.setup();

	texture_image.allocate(size);
	texture_image.upload(a_device);        // TODO: Move out since it doesn't make much sense, or receive a data ptr

	return texture_image;
}

}        // namespace rhi
