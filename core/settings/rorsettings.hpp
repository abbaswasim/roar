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
#include <_types/_uint32_t.h>
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

		this->m_roar_cache     = setting.get<std::string>("title");
		this->m_roar_cache     = setting.get<std::string>("roar_cache");
		this->m_default_scene  = setting.get<std::string>("default_scene");
		this->m_buffers_format = setting.get<std::string>("buffers_format");
		this->m_working_dir    = setting.get<std::string>("working_dir");

		this->m_zoom_speed = setting.get<float32_t>("zoom_speed");

		this->m_unit              = setting.get<uint32_t>("unit");
		this->m_buffer_increment  = setting.get<uint32_t>("buffer_increment");
		this->m_multisample_count = setting.get<uint32_t>("multisample_count");
		this->m_window_bpp        = setting.get<uint32_t>("window:bpp");

		this->m_visualise_mipmaps    = setting.get<bool>("visualise_mipmaps");
		this->m_vertical_sync        = setting.get<bool>("vsync");
		this->m_window_transparent   = setting.get<bool>("window_transparent");
		this->m_window_premultiplied = setting.get<bool>("window_premultiplied");
		this->m_window_prerotated    = setting.get<bool>("window_prerotated");
		this->m_fullscreen           = setting.get<bool>("fullscreen");
		this->m_resizable            = setting.get<bool>("resizable");

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
				this->m_window_dimensions = ror::Vector4ui(x, y, w, h);
		}

		auto r = setting.get<uint32_t>("window:layout:r");
		auto g = setting.get<uint32_t>("window:layout:g");
		auto b = setting.get<uint32_t>("window:layout:b");
		auto a = setting.get<uint32_t>("window:layout:a");
		auto d = setting.get<uint32_t>("window:layout:d");
		auto s = setting.get<uint32_t>("window:layout:s");

		if (r > 0 && g > 0 && b > 0 && a > 0)
			this->m_window_rgba_sizes = ror::Vector4ui(r, g, b, a);

		if (d > 0 && s > 0)
			this->m_window_depth_stencil_sizes = ror::Vector2ui(d, s);

		{
			auto x = setting.get<uint32_t>("viewport:x");
			auto y = setting.get<uint32_t>("viewport:y");        // Y can be negative
			auto w = setting.get<uint32_t>("viewport:width");
			auto h = setting.get<uint32_t>("viewport:height");

			if (x > 0 && w > 0 && h > 0)
				this->m_window_dimensions = ror::Vector4ui(x, y, w, h);
		}
	}

	std::string m_roar_title{};
	std::string m_roar_cache{};
	std::string m_default_scene{};
	std::string m_buffers_format{};
	std::string m_working_dir{};

	float32_t m_zoom_speed{20.0f};
	float32_t m_fog_start{0.0f};
	float32_t m_fog_end{500.0f};

	uint32_t m_unit{1};        //! 1 == meter, 1000 == km etc, to use the unit multiply it with your quantities
	uint32_t m_buffer_increment{1};
	uint32_t m_multisample_count{8};
	uint32_t m_window_bpp{72};        //! R8G8B8A8 + D24 + S8

	bool m_visualise_mipmaps{false};
	bool m_vertical_sync{false};
	bool m_window_transparent{false};
	bool m_window_premultiplied{false};
	bool m_window_prerotated{false};
	bool m_fullscreen{false};
	bool m_resizable{false};
	bool m_fog_enabled{false};

	ror::Vector4f m_background_color{0.19f, 0.04f, 0.14f, 1.0f};
	ror::Vector4f m_ambient_light_color{0.2f, 0.2f, 0.2f, 1.0f};
	ror::Vector4f m_fog_color{0.5f, 0.5f, 0.5f, 1.0f};

	ror::Vector4ui m_window_dimensions{0, 0, 1024, 768};
	ror::Vector4ui m_window_rgba_sizes{8, 8, 8, 8};
	ror::Vector4ui m_viewpoert{0, 0, 1024, 768};
	ror::Vector2ui m_window_depth_stencil_sizes{24, 8};

  protected:
  private:
};

FORCE_INLINE Settings &settings()
{
	static Settings sc{};        // Global settings container
	return sc;
}
}        // namespace ror
