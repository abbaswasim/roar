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

#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace rhi
{

ror::Vector4f reinhard_tone_mapping(ror::Vector4f color)
{
	return color / (color + ror::Vector4f(1.0f));
}

void write_ppm(std::filesystem::path a_path, uint32_t a_width, uint32_t a_height, std::vector<uint8_t> &a_data)
{
	(void) a_data;
	auto    &output_texture = ror::resource(a_path, ror::ResourceSemantic::textures, ror::ResourceAction::create);
	uint8_t *pixel_data     = a_data.data();

	assert(a_data.size() == (a_width * a_height * 4) && "Can only write RGBA ppm images");

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
				ppm_data.push_back(pixel_data[((j * 4) + (a_width * 4 * i)) + k] % 256);
		}
	}

	output_texture.update({ppm_data.begin(), ppm_data.end()}, true, false, true);        // Force updating the Resource because I am sure no one else is using it
	output_texture.flush();
}

void write_ppm(std::filesystem::path a_path, uint32_t a_width, uint32_t a_height, std::vector<float32_t> &a_data)
{
	size_t               size = a_width * a_height * 4u;
	std::vector<uint8_t> ldr_data;
	ldr_data.resize(size);

	for (size_t i = 0; i < size; i += 4)
	{
		ror::Vector4f v;

		v.x = a_data[i + 0];
		v.y = a_data[i + 1];
		v.z = a_data[i + 2];
		v.w = a_data[i + 3];

		auto t = reinhard_tone_mapping(v);

		ldr_data[i + 0] = static_cast<uint8_t>(t.x * 255.0f);
		ldr_data[i + 1] = static_cast<uint8_t>(t.y * 255.0f);
		ldr_data[i + 2] = static_cast<uint8_t>(t.z * 255.0f);
		ldr_data[i + 3] = static_cast<uint8_t>(t.w * 255.0f);
	}

	write_ppm(a_path, a_width, a_height, ldr_data);
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

TextureImage make_texture(rhi::Device &a_device, rhi::PixelFormat a_format, uint32_t a_width, uint32_t a_height,
                          rhi::TextureTarget a_target, rhi::TextureUsage a_usage, bool a_mipmapped)
{
	rhi::TextureImage texture_image{};

	texture_image.format(a_format);
	texture_image.mipmapped(a_mipmapped);
	texture_image.target(a_target);
	texture_image.push_empty_mip();
	texture_image.width(a_width);
	texture_image.height(a_height);
	texture_image.usage(a_usage);

	auto size = texture_image.setup();

	texture_image.allocate(size);
	texture_image.upload(a_device);

	return texture_image;
}

}        // namespace rhi
