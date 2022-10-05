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

#include "configuration/rorconfiguration.hpp"
#include "configuration/rorsettings_configuration.hpp"
#include "foundation/rorsystem.hpp"
#include "foundation/rortypes.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rortypes.hpp"
#include <string>

namespace ror
{

class ROAR_ENGINE_ITEM Settings final
{
  public:
	FORCE_INLINE           Settings(const Settings &a_other)      = delete;         //! Copy constructor
	FORCE_INLINE           Settings(Settings &&a_other) noexcept  = delete;         //! Move constructor
	FORCE_INLINE Settings &operator=(const Settings &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Settings &operator=(Settings &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE ~Settings() noexcept                             = default;        //! Destructor

	FORCE_INLINE Settings()
	{
		auto setting = ror::get_settings();        // This is config settings that will load settings.json, then we cache it in Settings object

		this->m_roar_title      = setting.get<std::string>("title");
		this->m_roar_cache      = setting.get<std::string>("roar_cache");
		this->m_default_scene   = setting.get<std::string>("default_scene");
		this->m_buffers_format  = setting.get<std::string>("buffers_format");
		this->m_working_dir     = setting.get<std::string>("working_dir");
		this->m_renderer_config = setting.get<std::string>("renderer_config");

		this->m_zoom_speed  = setting.get<float32_t>("zoom_speed");
		this->m_depth_clear = setting.get<float32_t>("depth");

		this->m_unit              = setting.get<uint32_t>("unit");
		this->m_buffer_increment  = setting.get<uint32_t>("buffer_increment");
		this->m_multisample_count = setting.get<uint32_t>("multisample_count");

		this->m_clean_on_boot        = setting.get<bool>("clean_on_boot");
		this->m_visualise_mipmaps    = setting.get<bool>("visualise_mipmaps");
		this->m_vertical_sync        = setting.get<bool>("vsync");
		this->m_window_transparent   = setting.get<bool>("window_transparent");
		this->m_window_premultiplied = setting.get<bool>("window_premultiplied");
		this->m_window_prerotated    = setting.get<bool>("window_prerotated");
		this->m_fullscreen           = setting.get<bool>("fullscreen");
		this->m_resizable            = setting.get<bool>("resizable");
		this->m_force_rgba_textures  = setting.get<bool>("force_rgba_textures");

		auto bgc = setting.get<std::vector<float32_t>>("background");
		if (bgc.size() >= 4)
			this->m_background_color = ror::Vector4f(bgc[0], bgc[1], bgc[2], bgc[3]);

		auto alc = setting.get<std::vector<float32_t>>("ambient_light_color");
		if (alc.size() >= 4)
			this->m_ambient_light_color = ror::Vector4f(alc[0], alc[1], alc[2], alc[3]);

		this->m_fog_enabled = setting.get<bool>("fog:enabled");

		auto foc = setting.get<std::vector<float32_t>>("fog:color");
		if (foc.size() >= 4)
			this->m_fog_color = ror::Vector4f(foc[0], foc[1], foc[2], foc[3]);

		this->m_fog_start = setting.get<float32_t>("fog:start");
		this->m_fog_end   = setting.get<float32_t>("fog:end");

		{
			auto x = setting.get<uint32_t>("window:x");
			auto y = setting.get<uint32_t>("window:y");
			auto w = setting.get<uint32_t>("window:width");
			auto h = setting.get<uint32_t>("window:height");

			if (x > 0 && y > 0 && w > 0 && h > 0)
				this->m_window_dimensions = ror::Vector4i(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(w), static_cast<int32_t>(h));
		}

		this->m_viewport = this->m_window_dimensions;

		auto color_format         = setting.get<std::string>("window:color_format");
		auto depth_stencil_format = setting.get<std::string>("window:depth_stencil_format");

		if (color_format != "")
			this->m_pixel_format = rhi::string_to_pixel_format(color_format);

		if (depth_stencil_format != "")
			this->m_depth_stencil_format = rhi::string_to_pixel_format(depth_stencil_format);

		this->m_window_buffer_count = setting.get<uint32_t>("window:buffer_count");

		{
			auto x = setting.get<uint32_t>("viewport:x");
			auto y = setting.get<uint32_t>("viewport:y");
			auto w = setting.get<uint32_t>("viewport:width");
			auto h = setting.get<uint32_t>("viewport:height");        // Height can be negative

			if (x > 0 && w > 0 && h > 0)
				this->m_viewport = ror::Vector4i(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(w), static_cast<int32_t>(h));
		}

		this->m_metal.version_major            = setting.get<uint32_t>("metal:version_major");
		this->m_metal.version_minor            = setting.get<uint32_t>("metal:version_minor");
		this->m_metal.argument_buffers         = setting.get<bool>("metal:argument_buffers");
		this->m_metal.indirect_command_buffers = setting.get<bool>("metal:indirect_command_buffers");

		this->m_vulkan.version_major = setting.get<uint32_t>("vulkan:version_major");
		this->m_vulkan.version_minor = setting.get<uint32_t>("vulkan:version_minor");

		this->m_print_generated_shaders = setting.get<bool>("print_generated_shaders");
		this->m_write_generated_shaders = setting.get<bool>("write_generated_shaders");

		this->m_clean_dirs = setting.get<std::vector<std::string>>("clean_dirs");
	}

	std::string m_roar_title{};
	std::string m_roar_cache{};
	std::string m_default_scene{};
	std::string m_buffers_format{};
	std::string m_working_dir{};
	std::string m_renderer_config{"renderer.json"};

	std::vector<std::string> m_clean_dirs{};        //! Directories to clean on boot

	float32_t m_zoom_speed{20.0f};
	float32_t m_fog_start{0.0f};
	float32_t m_fog_end{500.0f};
	float32_t m_depth_clear{1.0f};

	uint32_t m_unit{1};        //! 1 == meter, 1000 == km etc, to use the unit multiply it with your quantities
	uint32_t m_buffer_increment{1};
	uint32_t m_multisample_count{8};

	bool m_clean_on_boot{false};
	bool m_visualise_mipmaps{false};
	bool m_vertical_sync{false};
	bool m_window_transparent{false};
	bool m_window_premultiplied{false};
	bool m_window_prerotated{false};
	bool m_fullscreen{false};
	bool m_resizable{false};
	bool m_fog_enabled{false};
	bool m_force_rgba_textures{false};
	bool m_print_generated_shaders{false};
	bool m_write_generated_shaders{false};

	ror::Vector4f m_background_color{0.19f, 0.04f, 0.14f, 1.0f};
	ror::Vector4f m_ambient_light_color{0.2f, 0.2f, 0.2f, 1.0f};
	ror::Vector4f m_fog_color{0.5f, 0.5f, 0.5f, 1.0f};

	ror::Vector4i m_window_dimensions{0, 0, 1024, 768};
	ror::Vector4i m_viewport{0, 0, 1024, 768};
	uint32_t      m_window_buffer_count{3};

	rhi::PixelFormat m_pixel_format{rhi::PixelFormat::r8g8b8a8_uint32_norm};
	rhi::PixelFormat m_depth_stencil_format{rhi::PixelFormat::depth24_norm_stencil8_uint32};

	struct Options
	{
		uint32_t version_major{3};
		uint32_t version_minor{0};
	};

	struct MetalOptions : public Options
	{
		bool argument_buffers{true};
		bool indirect_command_buffers{false};
	};

	struct VulkanOptions : public Options
	{};

	VulkanOptions m_vulkan{};
	MetalOptions  m_metal{};

  protected:
  private:
};

FORCE_INLINE Settings &settings() noexcept
{
	static Settings sc{};        // Global settings container
	return sc;
}
}        // namespace ror
