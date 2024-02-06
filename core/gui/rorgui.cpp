// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2023
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

#include "event_system/rorevent_handles.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rortypes.hpp"
#include "foundation/rorutilities.hpp"
#include "gui/rorgizmo.hpp"
#include "math/rormatrix4.hpp"
#include "math/rormatrix4_functions.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector3.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rorlog.hpp"
#include "profiling/rortimer.hpp"
#include "renderer/rorrenderer.hpp"
#include "resources/rorresource.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/rorvertex_attribute.hpp"
#include "rhi/rorvertex_description.hpp"
#include "rhi/rorvertex_layout.hpp"
#include "rorgui.hpp"
#include "rorroar_font.hpp"
#include "settings/rorsettings.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>

namespace ror
{

Gui::Gui()
{
	ImGuiContext *context = ImGui::CreateContext();
	ImGui::SetCurrentContext(context);

	ImGuiIO &io = ImGui::GetIO();

	this->m_move_callback = [&io](ror::Event &e) {
		if (io.WantCaptureMouse)
			e.m_live = false;

		if (e.is_compatible<ror::Vector2d>())
		{
			auto vec2   = std::any_cast<ror::Vector2d>(e.m_payload);
			io.MousePos = ImVec2{static_cast<float32_t>(vec2.x), static_cast<float32_t>(vec2.y)};
		}
	};

	this->m_left_down_callback = [&io](ror::Event &e) {
		if (io.WantCaptureMouse)
			e.m_live = false;

		io.AddMouseButtonEvent(imgui_keys_left, true);
	};
	this->m_left_up_callback = [&io](ror::Event &e) {
		if (io.WantCaptureMouse)
			e.m_live = false;

		io.AddMouseButtonEvent(imgui_keys_left, false);
	};

	this->m_right_down_callback = [&io](ror::Event &e) {
		if (io.WantCaptureMouse)
			e.m_live = false;

		io.AddMouseButtonEvent(imgui_keys_right, true);
	};

	this->m_right_up_callback = [&io](ror::Event &e) {
		if (io.WantCaptureMouse)
			e.m_live = false;

		io.AddMouseButtonEvent(imgui_keys_right, false);
	};

	this->m_mouse_wheel_callback = [&io](ror::Event &e) {
		if (io.WantCaptureMouse)
			e.m_live = false;

		if (e.is_compatible<ror::Vector2d>())
		{
			auto vec2 = std::any_cast<ror::Vector2d>(e.m_payload);

			io.AddMouseWheelEvent(static_cast<float32_t>(vec2.x), static_cast<float32_t>(vec2.y));
		}
		else
		{
			assert(0 && "Incompatible payload with imgui scroll callback");
		}
	};

	this->m_tab_key_callback = [&io](ror::Event &e) {
		if (io.WantCaptureKeyboard)
			e.m_live = false;

		auto &setting           = ror::settings();
		setting.m_gui.m_visible = !setting.m_gui.m_visible;
	};

	this->m_demo_windown_callback = [&io, this](ror::Event &e) {
		if (io.WantCaptureKeyboard)
			e.m_live = false;

		this->m_show_demo_window = !this->m_show_demo_window;
	};

	this->m_gizmo_toggle_callback = [&io, this](ror::Event &e) {
		if (io.WantCaptureKeyboard)
			e.m_live = false;

		this->m_show_gizmo = !this->m_show_gizmo;
	};

	this->m_overlays_toggle_callback = [&io, this](ror::Event &e) {
		if (io.WantCaptureKeyboard)
			e.m_live = false;

		this->m_show_overlays = !this->m_show_overlays;
	};

	this->m_anchors_toggle_callback = [&io, this](ror::Event &e) {
		if (io.WantCaptureKeyboard)
			e.m_live = false;

		this->m_show_anchors = !this->m_show_anchors;
	};

	this->m_settings_toggle_callback = [&io, this](ror::Event &e) {
		if (io.WantCaptureKeyboard)
			e.m_live = false;

		this->m_show_settings = !this->m_show_settings;
	};

	this->m_debug_toggle_callback = [&io, this](ror::Event &e) {
		if (io.WantCaptureKeyboard)
			e.m_live = false;

		this->m_show_debug = !this->m_show_debug;
	};

	auto &setting = ror::settings();

	this->m_show_anchors  = setting.m_gui.m_show_anchors;
	this->m_show_gizmo    = setting.m_gui.m_show_gizmo;
	this->m_show_overlays = setting.m_gui.m_show_overlays;
	this->m_show_settings = setting.m_gui.m_show_settings;
	this->m_show_debug    = setting.m_gui.m_show_debug;
}

Gui::~Gui() noexcept        //! Destructor
{
	ImGui::DestroyContext();
}

void Gui::install_input_handlers()
{
	if (this->m_event_system)
	{
		this->m_event_system->subscribe_early(mouse_scroll, this->m_mouse_wheel_callback);

		this->m_event_system->subscribe_early(mouse_move, this->m_move_callback);
		this->m_event_system->subscribe_early(mouse_left_mouse_down, this->m_left_down_callback);
		this->m_event_system->subscribe_early(mouse_left_mouse_click, this->m_left_up_callback);
		this->m_event_system->subscribe_early(mouse_right_mouse_down, this->m_right_down_callback);
		this->m_event_system->subscribe_early(mouse_right_mouse_click, this->m_right_up_callback);

		this->m_event_system->subscribe(keyboard_tab_click, this->m_tab_key_callback);

		this->m_event_system->subscribe(keyboard_g_click, this->m_gizmo_toggle_callback);
		this->m_event_system->subscribe(keyboard_o_click, this->m_overlays_toggle_callback);
		this->m_event_system->subscribe(keyboard_a_click, this->m_anchors_toggle_callback);
		this->m_event_system->subscribe(keyboard_s_click, this->m_settings_toggle_callback);
		this->m_event_system->subscribe(keyboard_d_click, this->m_debug_toggle_callback);

		this->m_event_system->subscribe(keyboard_question_mark_click, this->m_demo_windown_callback);
	}
}

void Gui::uninstall_input_handlers()
{
	if (this->m_event_system)
	{
		this->m_event_system->unsubscribe(mouse_scroll, this->m_mouse_wheel_callback);

		this->m_event_system->unsubscribe(mouse_move, this->m_move_callback);
		this->m_event_system->unsubscribe(mouse_left_mouse_down, this->m_left_down_callback);
		this->m_event_system->unsubscribe(mouse_left_mouse_click, this->m_left_up_callback);
		this->m_event_system->unsubscribe(mouse_right_mouse_down, this->m_right_down_callback);
		this->m_event_system->unsubscribe(mouse_right_mouse_click, this->m_right_up_callback);

		this->m_event_system->unsubscribe(keyboard_tab_click, this->m_tab_key_callback);

		this->m_event_system->unsubscribe(keyboard_g_click, this->m_gizmo_toggle_callback);
		this->m_event_system->unsubscribe(keyboard_o_click, this->m_overlays_toggle_callback);
		this->m_event_system->unsubscribe(keyboard_a_click, this->m_anchors_toggle_callback);
		this->m_event_system->unsubscribe(keyboard_s_click, this->m_settings_toggle_callback);
		this->m_event_system->unsubscribe(keyboard_d_click, this->m_debug_toggle_callback);

		this->m_event_system->unsubscribe(keyboard_question_mark_click, this->m_demo_windown_callback);
	}
}

uint8_t *generate_shadow_pixels(uint8_t *pixels, int iwidth, int iheight)
{
	auto width  = static_cast<uint32_t>(iwidth);
	auto height = static_cast<uint32_t>(iheight);

	// First lets make a copy
	uint32_t bytes{width * height * 4};
	int32_t  ibytes{iwidth * iheight * 4};
	uint8_t *pixels_shadow = new uint8_t[bytes];
	memcpy(pixels_shadow, pixels, bytes);

	// Clear the buffers' green and blue components
	for (uint32_t i = 0; i < bytes; i += 4)
	{
		pixels_shadow[i + 1] = 0;        // green
		pixels_shadow[i + 2] = 0;        // blue
	}

	// Create shadow at offset
	auto offsetf = ImGui::GetIO().Fonts->TexGlyphShadowOffset;
	auto offset  = ror::Vector2i{static_cast<int>(offsetf.x), static_cast<int>(offsetf.y)};

	for (int y = offset.y; y < iheight; y++)
	{
		for (int x = offset.x; x < iwidth; x++)
		{
			int32_t current_index      = static_cast<int32_t>(((x * 4) + (iwidth * 4 * y)));
			int32_t write_shadow_index = static_cast<int32_t>((((x + offset.x) * 4) + (iwidth * 4 * (y + offset.y))));

			if (current_index >= 0 && write_shadow_index >= 0 &&
			    current_index + 3 < ibytes && write_shadow_index + 3 < ibytes)
			{
				uint8_t current_pixel        = pixels[current_index + 3];
				uint8_t current_pixel_shadow = pixels[write_shadow_index + 3];

				// Only write shadow pixels into empty areas
				if (current_pixel != 0 && current_pixel_shadow == 0)
					pixels_shadow[write_shadow_index + 2] = pixels[current_index + 3];
			}
		}
	}

	// Makes an outline if needed
	// for (int i = 0; i < iheight; i++)
	// {
	// 	for (int j = 0; j < iwidth; j++)
	// 	{
	// 		// Also make outline
	// 		int outline_kernel[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};

	// 		int  current_index = ((j * 4) + (iwidth * 4 * i));
	// 		unsigned char current_pixel = pixels_shadow[current_index + 3];

	// 		if (current_pixel == 0)
	// 		{
	// 			int value = 0;
	// 			// Find outline
	// 			for (int y = 0, oy = -1; y < 3; y++, oy++)
	// 			{
	// 				for (int x = 0, ox = -1; x < 3; x++, ox++)
	// 				{
	// 					int ix = j + ox;
	// 					int iy = i + oy;

	// 					if ((ix < iwidth * 4 && ix >= 0) && (iy < iheight * 4 && iy >= 0))
	// 					{
	// 						float kvalu = outline_kernel[x][y];
	// 						int   woi   = ((ix * 4) + (iwidth * 4 * iy));
	// 						value += static_cast<uint8_t>(pixels[woi + 3] * kvalu);
	// 					}
	// 				}
	// 			}
	// 			// Found a color around
	// 			pixels_shadow[current_index + 1] = static_cast<uint8_t>(value);
	// 		}
	// 	}
	// }

	//  Adds blurr if needed, doesn't blur much at the moment, needs fixing
	// for (int i = 0; i < iheight; i++)
	// {
	// 	for (int j = 0; j < iwidth; j++)
	// 	{
	// 		int current_index = ((j * 4) + (iwidth * 4 * i));
	// 		//            unsigned char current_pixel = pixels_shadow[current_index + 3];

	// 		float blurr_kernel[3][3] = {{0.0625, 0.125, 0.0625}, {0.125, 0.25, 0.125}, {0.0625, 0.125, 0.0625}};

	// 		// Blur it
	// 		//            if (current_pixel != 0)
	// 		{
	// 			float value = 0;
	// 			for (int y = 0, oy = -1; y < 3; y++, oy++)
	// 			{
	// 				for (int x = 0, ox = -1; x < 3; x++, ox++)
	// 				{
	// 					int ix = j + ox;
	// 					int iy = i + oy;

	// 					if ((ix < iwidth * 4 && ix >= 0) && (iy < iheight * 4 && iy >= 0))
	// 					{
	// 						float kvalu = blurr_kernel[x][y];
	// 						int   woi   = ((ix * 4) + (iwidth * 4 * iy));
	// 						value += pixels_shadow[woi + 1] * kvalu;
	// 					}
	// 				}
	// 			}
	// 			// Found a color around
	// 			pixels_shadow[current_index + 1] = static_cast<uint8_t>(value);
	// 		}
	// 	}
	// }

	auto &setting = ror::settings();
	if (setting.m_gui.m_dump_texture)
	{
		auto &gui_texture = resource("imgui_texture.ppm", ResourceSemantic::textures, ResourceAction::create);

		std::string w{std::to_string(width)};
		std::string h{std::to_string(height)};

		std::vector<uint8_t> data{};
		data.reserve(width * height * 3 + 17);        // 17 is the header size assuming texture size of 9999 max
		data.push_back('P');
		data.push_back('6');
		data.push_back('\n');
		for (auto &wc : w)
			data.push_back(static_cast<uint8_t>(wc));
		data.push_back(' ');
		for (auto &hc : h)
			data.push_back(static_cast<uint8_t>(hc));
		data.push_back('\n');
		data.push_back('2');
		data.push_back('5');
		data.push_back('5');
		data.push_back('\n');

		for (int i = 0; i < iheight; i++)
		{
			for (int j = 0; j < iwidth; j++)
			{
				for (int k = 1; k < 4; ++k)
					data.push_back(pixels_shadow[((j * 4) + (iwidth * 4 * i)) + k] % 256);
			}
		}

		gui_texture.update({data.begin(), data.end()}, true, false, true);        // Force updating the Resource because I am sure no one else is using it
		gui_texture.flush();
	}

	return pixels_shadow;
}

// Based on https://github.com/ocornut/imgui/issues/707#issuecomment-468798935
void set_imgui_style()
{
	ImGuiStyle &style  = ImGui::GetStyle();
	ImVec4     *colors = style.Colors;

	/// 0 = FLAT APPEARENCE
	/// 1 = MORE "3D" LOOK
	int is3D = 0;

	colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_ChildBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_WindowBg]              = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_PopupBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_Border]                = ImVec4(0.32f, 0.32f, 0.32f, 0.71f);
	colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_FrameBg]               = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
	colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
	colors[ImGuiCol_FrameBgActive]         = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
	colors[ImGuiCol_TitleBg]               = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
	colors[ImGuiCol_TitleBgActive]         = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
	colors[ImGuiCol_MenuBarBg]             = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
	colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_CheckMark]             = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
	colors[ImGuiCol_SliderGrab]            = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
	colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
	colors[ImGuiCol_Button]                = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
	colors[ImGuiCol_ButtonHovered]         = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
	colors[ImGuiCol_ButtonActive]          = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
	colors[ImGuiCol_Header]                = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_HeaderHovered]         = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_HeaderActive]          = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
	colors[ImGuiCol_Separator]             = ImVec4(0.50, 0.50, 0.50, 0.137f);
	colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
	colors[ImGuiCol_SeparatorActive]       = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
	colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
	colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_Tab]                   = ImVec4(0.45f, 0.45f, 0.45f, 0.80f);
	colors[ImGuiCol_TabHovered]            = ImVec4(0.73f, 0.73f, 0.73f, 0.80f);
	colors[ImGuiCol_TabActive]             = ImVec4(0.54f, 0.54f, 0.54f, 1.0f);
	colors[ImGuiCol_ResizeGrip]            = ImVec4(0.51f, 0.51f, 0.51f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.12f, 0.12f, 0.12f, 0.67f);
	colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.20f, 0.20f, 0.20f, 0.95f);

	style.PopupRounding = 3;

	style.WindowPadding = ImVec2(4, 4);
	style.FramePadding  = ImVec2(6, 4);
	style.ItemSpacing   = ImVec2(6, 2);

	style.ScrollbarSize = 18;

	style.WindowBorderSize = 1;
	style.ChildBorderSize  = 1;
	style.PopupBorderSize  = 1;
	style.FrameBorderSize  = static_cast<float32_t>(is3D);

	style.WindowRounding    = 3;
	style.ChildRounding     = 3;
	style.FrameRounding     = 3;
	style.ScrollbarRounding = 2;
	style.GrabRounding      = 3;

#ifdef IMGUI_HAS_DOCK
	style.TabBorderSize = is3D;
	style.TabRounding   = 3;

	colors[ImGuiCol_DockingEmptyBg]     = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
	colors[ImGuiCol_Tab]                = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
	colors[ImGuiCol_TabActive]          = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused]       = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
	colors[ImGuiCol_DockingPreview]     = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding              = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
#endif
}

void Gui::init_upload(const rhi::Device &a_device, const ror::Renderer &a_renderer, ror::EventSystem &a_event_system)
{
	this->m_device = &a_device;

	this->m_event_system = &a_event_system;

	auto &setting = ror::settings();

	ImGuiIO &io        = ImGui::GetIO();
	io.FontGlobalScale = setting.m_gui.m_font_scale;        //  0.45f;        // For font blender_droid at size 32 using font scale .45 makes it look nicer

	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Set gly shadows offset
	io.Fonts->TexGlyphShadowOffset = ImVec2(5, 3);

	// Incase there are indices which doesn't fit in uint16_t
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	// Setup style
	if (setting.m_gui.m_theme == ror::Settings::GuiTheme::dark)
		ImGui::StyleColorsDark();
	else if (setting.m_gui.m_theme == ror::Settings::GuiTheme::light)
		ImGui::StyleColorsLight();
	else        // Assumes classic otherwise
		ImGui::StyleColorsClassic();

	ImGui::GetStyle().ScaleAllSizes(static_cast<float32_t>(setting.m_gui.m_scale));        // If you want to make things bigger, should probably be DPI aware

	set_imgui_style();

	// Not using the ImFontGlyphRangesBuilder, instead hard coding this in here
	static ImWchar roar_icons_range[]{ROAR_ICON_RANGE_START, ROAR_ICON_RANGE_END, 0};

	for (auto &font : setting.m_gui.m_fonts)
	{
		auto &font_resource  = ror::load_resource(font, ror::ResourceSemantic::fonts);
		auto  resource_data  = font_resource.data();
		auto  font_data_size = static_cast_safe<int32_t>(resource_data.size());

		// Hate this horrible const cast but imgui won't accept const void * even if it won't own it
		void *font_data = const_cast<void *>(reinterpret_cast<const void *>(resource_data.data()));

		ImFontConfig font_cfg;
		font_cfg.SizePixels           = static_cast<float32_t>(setting.m_gui.m_font_size);
		font_cfg.FontDataOwnedByAtlas = false;                                        // So imgui doesn't delete the resource data from underneath us
		std::memcpy(font_cfg.Name, font.c_str(), std::min(40ul, font.size()));        // 40 Hard limit on ImGui font name

		ImFont  *imfont{nullptr};
		ImWchar *glyph_ranges{nullptr};

		if (font_data)
		{
			if (font == "roar_icons.ttf")        // If we are loading roar_icons.ttf font use specified range only
				glyph_ranges = roar_icons_range;

			if (this->m_fonts.size() > 0)        // If we have any fonts lets append to those, NOTE: I think this doesn't work if there are overlapping fonts, check...
				font_cfg.MergeMode = true;

			imfont = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 0, &font_cfg, glyph_ranges);

			if (imfont == nullptr)
				ror::log_warn("Can't create a font from the loaded font resource {}", font.c_str());
		}

		this->m_fonts.push_back(imfont);        // I might be inserting a nullptr here but thats ok otherwise my default_font index will be wrong
	}

	// If there were no fonts provided lets use the default font at least
	if (this->m_fonts.empty())
	{
		ImFontConfig font_cfg;
		font_cfg.SizePixels = static_cast<float32_t>(setting.m_gui.m_font_size);
		this->m_fonts.emplace_back(io.Fonts->AddFontDefault(&font_cfg));
	}

	// Lets enable the default font from the setting
	this->m_current_font = setting.m_gui.m_default_font;
	if (this->m_fonts.size() > this->m_current_font && this->m_fonts[this->m_current_font] != nullptr)
		this->m_default_font = this->m_fonts[this->m_current_font];

	uint8_t *pixels;
	int      width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	auto new_pixels = generate_shadow_pixels(pixels, width, height);

	rhi::fill_texture_from_memory(new_pixels, static_cast<uint32_t>(width), static_cast<uint32_t>(height), 4, this->m_texture_image, false, "imgui_font_texture");

	this->m_texture_image.upload(a_device);
	io.Fonts->SetTexID(reinterpret_cast<ImTextureID>(&this->m_texture_image));
	this->m_texture_sampler.upload(a_device);

	this->install_input_handlers();

	this->m_shader_buffer.add_entry("orthographic_projection", rhi::Format::float32_4x4);
	this->m_shader_buffer.upload(a_device, rhi::ResourceStorageOption::managed);

	// Create vertex descriptor
	rhi::VertexAttribute vap{0, 0, 1, 0, 0, 0, rhi::BufferSemantic::vertex_position, rhi::VertexFormat::float32_2};        // location, offset, count, buffer_offset, binding, buffer_index, semantic, format
	rhi::VertexLayout    vlp{0, 20};                                                                                       // binding, stride, rate, multiplier, function
	rhi::VertexAttribute vat{1, 8, 1, 0, 0, 0, rhi::BufferSemantic::vertex_texture_coord_0, rhi::VertexFormat::float32_2};
	rhi::VertexLayout    vlt{0, 20};
	rhi::VertexAttribute vac{2, 16, 1, 0, 0, 0, rhi::BufferSemantic::vertex_color_0, rhi::VertexFormat::uint8_4_norm};
	rhi::VertexLayout    vlc{0, 20};

	std::vector<rhi::VertexAttribute> vattribs{vap, vat, vac};
	std::vector<rhi::VertexLayout>    vlayouts{vlp, vlt, vlc};

	this->m_vertex_descriptor = rhi::VertexDescriptor{vattribs, vlayouts};

	// Create shader program
	auto vs_shader = rhi::build_shader<rhi::Shader>(a_device, "gui.glsl.vert");
	auto fs_shader = rhi::build_shader<rhi::Shader>(a_device, "gui.glsl.frag");

	// Needs final pass stuff
	rhi::Renderpass    *pass{nullptr};
	rhi::Rendersubpass *subpass{nullptr};

	a_renderer.get_final_pass_subpass(&pass, &subpass);

	// Only requires the following descriptors
	// layout(set = 0, binding = 0) uniform highp sampler2D base_color_sampler; set and binding needs changing according to API
	// layout(std140, set = 0, binding = 3) uniform gui_per_frame_uniform; name and set and binding needs changing according to API

	this->m_shader_program.build_descriptor(a_device, a_renderer, &this->m_shader_buffer, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
											&this->m_texture_image, &this->m_texture_sampler, nullptr, false, false);
	this->m_shader_program.upload(a_device, *pass, *subpass, vs_shader, fs_shader, this->m_vertex_descriptor, rhi::BlendMode::blend, rhi::PrimitiveTopology::triangles, "gui_pso", true, false, true);

	this->m_vertex_buffer.init(a_device, setting.m_gui.m_vertex_buffer_size);        // By default in shared mode
	this->m_index_buffer.init(a_device, setting.m_gui.m_index_buffer_size);          // By default in shared mode

	// auto p0 = ror::Vector4f{2.0f, 2.0f, 0.0f, 1.0f};
	// auto p1 = ror::Vector4f{2.5f, 2.5f, 0.0f, 1.0f};
	// auto p2 = ror::Vector4f{7.5f, 2.5f, 0.0f, 1.0f};
	// auto p3 = ror::Vector4f{1.0f, 1.0f, 0.0f, 1.0f};
	// auto p4 = ror::Vector4f{5.0f, 1.0f, 0.0f, 1.0f};
	// auto p5 = ror::Vector4f{9.0f, 2.5f, 0.0f, 1.0f};
	// auto p6 = ror::Vector4f{7.5f, 1.0f, 0.0f, 1.0f};
	// auto p7 = ror::Vector4f{9.0f, 1.0f, 0.0f, 1.0f};
	// auto p8 = ror::Vector4f{8.0f, -1.0f, 0.0f, 1.0f};
	// auto p9 = ror::Vector4f{8.5f, -1.0f, 0.0f, 1.0f};

	// Anchors::Anchor a0{p0, 4.3f};
	// Anchors::Anchor a1{p1, p0, p2, Anchors::AnchorType::bezier};
	// Anchors::Anchor a2{p1, p3, p4};
	// Anchors::Anchor a3{p2, p6, p7, p5};
	// Anchors::Anchor a4{p6, p8, p9, p7};

	// m_anchors.push_anchor(a0);
	// m_anchors.push_anchor(a1);
	// m_anchors.push_anchor(a2);
	// m_anchors.push_anchor(a3);
	// m_anchors.push_anchor(a4);

	this->m_gizmo.init(this->m_default_font, ror::Vector4f{0.0f, 0.0f, 0.0f, 1.0f});
	this->m_overlays.init(this->m_default_font);
}

static void show_debug_overlay(bool &a_show_debug)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
	                                ImGuiWindowFlags_AlwaysAutoResize |
	                                ImGuiWindowFlags_NoSavedSettings |
	                                ImGuiWindowFlags_NoFocusOnAppearing |
	                                ImGuiWindowFlags_NoInputs |
	                                ImGuiWindowFlags_NoNav;
	{
		const float          PAD       = 10.0f;
		const ImGuiViewport *viewport  = ImGui::GetMainViewport();
		ImVec2               work_pos  = viewport->WorkPos;        // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2               work_size = viewport->WorkSize;
		ImVec2               window_pos, window_pos_pivot;
		window_pos.x       = (work_pos.x + work_size.x - PAD);
		window_pos.y       = (work_pos.y + work_size.y - PAD);
		window_pos_pivot.x = 1.0f;
		window_pos_pivot.y = 1.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}

	static FrameCounter fps;

	ImGui::SetNextWindowBgAlpha(0.35f);        // Transparent background
	if (ImGui::Begin("Roar Debug...", &a_show_debug, window_flags))
	{
		ImGui::Text("Roar Debug:");
		ImGui::Separator();
		ImGui::Text("Framerate: (%1.f)", fps.fps(1000));
	}
	ImGui::End();
}

static void show_multi_render_view_titles(bool &a_show_render_titles)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground |
	                                ImGuiWindowFlags_NoDecoration |
	                                ImGuiWindowFlags_AlwaysAutoResize |
	                                ImGuiWindowFlags_NoSavedSettings |
	                                ImGuiWindowFlags_NoFocusOnAppearing |
	                                ImGuiWindowFlags_NoInputs |
	                                ImGuiWindowFlags_NoNav;

	const float          padding  = 5.0f;
	const ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImVec2               work_pos = viewport->WorkPos;        // Use work area to avoid menu-bar/task-bar, if any!
	ImVec2               window_pos, window_pos_pivot;
	window_pos.x       = (work_pos.x + padding);
	window_pos.y       = (work_pos.y + padding);
	window_pos_pivot.x = 0.0f;
	window_pos_pivot.y = 0.0f;
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	window_flags |= ImGuiWindowFlags_NoMove;

	// also defined same in render_modes.glsl.frag, need to be kept in sync
	const uint32_t  segments_count{6};
	const float32_t skew{0.5};
	const float32_t segments_size{viewport->WorkSize.x / (segments_count + 2)};

	float32_t segments[segments_count];

	// Make segments
	for (uint32_t i = 0u; i < segments_count; ++i)
		segments[i] = segments_size * (static_cast<float32_t>(i) + 1) + segments_size * skew;

	const std::string titles[segments_count] = {"Occlusion", "Normal", "Roughness", "Metallic", "Base Color", "Emissive"};

	ImGui::SetNextWindowBgAlpha(0.0f);        // Fully transparent background
	if (ImGui::Begin("Multi-Render View", &a_show_render_titles, window_flags))
	{
		for (uint32_t i = 0u; i < segments_count; ++i)
		{
			auto s = ImGui::CalcTextSize(titles[i].c_str());
			ImGui::SetCursorScreenPos(ImVec2{segments[i] + ((segments_size - s.x) / 2), window_pos.y});
			ImGui::Text("%s", titles[i].c_str());
		}
	}
	ImGui::End();
}

static bool rendering_mode_selector(uint32_t &a_render_mode_index, bool &a_show_render_titles)
{
	bool return_state{false};

	if (ImGui::TreeNode("Render mode selector"))
	{
		int selected = 0;
		for (auto &render_mode : ror::Settings().m_gui.m_render_modes)
		{
			if (render_mode.c_str()[0] != '-')
			{
				ImGui::TreePush(render_mode.c_str() + 1);
				if (ImGui::Selectable(render_mode.c_str()))
				{
					a_render_mode_index = static_cast_safe<uint32_t>(selected);
					return_state        = true;

					if (render_mode == "Multi-Render View")
						a_show_render_titles = true;
					else
						a_show_render_titles = false;
				}
				ImGui::TreePop();
			}
			else
			{
				ImGui::Separator();
				ImGui::Text("%s", render_mode.c_str() + 1);
			}

			selected++;
		}
		ImGui::TreePop();
	}

	// int mode_radio_index = 0;
	// for (auto &render_mode : ror::Settings().m_gui.m_render_modes)
	// {
	// 	if (ImGui::RadioButton(render_mode.c_str(), &mode_index, mode_radio_index++))
	// 	{
	// 		a_render_mode_index = static_cast_safe<uint32_t>(mode_index);

	// 		return true;
	// 	}
	// }

	// auto      &rendermodes = render_modes();
	// if (ImGui::Combo("Render style##Selector", &mode_index, rendermodes.c_str()))
	// {
	// 	a_render_mode_index = static_cast_safe<uint32_t>(mode_index);

	// 	return true;
	// }

	return return_state;
}

static void show_settings(bool &a_show_settings, ror::EventSystem &a_event_system, bool &a_show_render_titles)
{
	ImGui::Begin("Roar settings...", &a_show_settings);

	uint32_t render_mode_index{0};
	if (rendering_mode_selector(render_mode_index, a_show_render_titles))
	{
		auto render_mode_handle = create_event_handle(EventType::renderer);
		a_event_system.notify({render_mode_handle, true, render_mode_index});
	}

	ImGui::End();
}

void Gui::draw_test_windows(ror::OrbitCamera &a_camera, ror::Vector4f &a_dimensions, ror::EventSystem &a_event_system)
{
	auto &setting = ror::settings();

	// Lets render GUI
	// static bool show_another_window{true};
	// static float clear_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

	ImGuiIO &io = ImGui::GetIO();

	io.DisplaySize             = ImVec2(a_dimensions.x / a_dimensions.z, a_dimensions.y / a_dimensions.w);
	io.DisplayFramebufferScale = ImVec2(a_dimensions.z, a_dimensions.w);

	ImGui::NewFrame();
	static ror::Matrix4f result{};

	auto font_id = setting.m_generic_numbers[0];

	if (font_id >= 0 && font_id < static_cast_safe<int32_t>(this->m_fonts.size()))
		this->m_default_font = this->m_fonts[static_cast<size_t>(font_id)];

	ImGui::PushFont(this->m_default_font);

	if (this->m_show_demo_window)
		ImGui::ShowDemoWindow(&this->m_show_demo_window);

	if (this->m_show_settings)
		show_settings(this->m_show_settings, a_event_system, this->m_show_render_titles);

	if (this->m_show_debug)
		show_debug_overlay(this->m_show_debug);

	if (this->m_show_render_titles)
		show_multi_render_view_titles(this->m_show_render_titles);

	{
		// ImGui::Begin("Hello, world!");           // Create a window called "Hello, world!" and append into it.
		// ImGui::Text("Transform");                // Display some text (you can use a format strings too)
		// ImGui::Text("Location X");
		// ImGui::Text("Rotation X");
		// ImGui::Text("Scale X");
		// ImGui::Text("Collection");
		// ImGui::Text("Instancing");
		// ImGui::End();

		auto drawlist = imgui_draw_list();
		auto dis      = io.DisplaySize;

		auto          mpos  = ImGui::GetMousePos();
		auto          mcpos = io.MouseClickedPos[0];        // Left click only
		ror::Vector2f mouse_position{mpos.x, mpos.y};
		ror::Vector2f left_mouse_position{mcpos.x, mcpos.y};
		ror::Vector4f view_port{0.0f, 0.0f, dis.x, dis.y};

		auto view       = a_camera.view();
		auto projection = a_camera.projection();

		auto view_projection = projection * view;

		if (this->m_show_anchors)
		{
			this->m_anchors.new_frame(ImGui::IsMouseClicked(0), ImGui::IsMouseReleased(0), mouse_position, left_mouse_position);
			this->m_anchors.draw(drawlist, view_projection, view_port, ImGui::IsMouseClicked(0));
		}

		if (this->m_show_gizmo)
			this->m_gizmo.draw(view_projection, view_port);
		if (this->m_show_overlays)
			this->m_overlays.draw(view_projection, view_port);
	}

	// // 3. Show another simple window.
	// if (show_another_window)
	// {
	// 	ImGui::Begin("Another Window", &show_another_window);        // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	// 	ImGui::Text("Hello from another window!");

	// 	// a_drawlist->AddText(nullptr, color == imgui_black ? 13.0f : 12.0f, ImVec2{x - text_offset, y - 5.0f}, color, names[j - 1]);
	// 	ImGui::PushFont(this->m_fonts[this->m_fonts.size() - 2]);
	// 	if (ImGui::Button(ROAR_ICON_CAMERA "Close Me"))
	// 		show_another_window = false;
	// 	ImGui::PopFont();
	// 	ImGui::End();
	// }

	ImGui::PopFont();

	ImGui::EndFrame();
	ImGui::Render();
}

void Gui::upload_draw_data(ImDrawData *a_draw_data)
{
	assert(this->m_device);

	const rhi::Device &a_device = *this->m_device;

	if (!a_draw_data)
		return;

	// Size in bytes of the vertex and index buffers required
	size_t vertex_buffer_size = static_cast<size_t>(a_draw_data->TotalVtxCount) * sizeof(ImDrawVert);
	size_t index_buffer_size  = static_cast<size_t>(a_draw_data->TotalIdxCount) * sizeof(ImDrawIdx);

	this->m_vertex_buffer.resize(a_device, vertex_buffer_size);        // Makes sure its atleast as big as vertex_buffer_size (it could be bigger)
	this->m_index_buffer.resize(a_device, index_buffer_size);          // Makes sure its atleast as big as index_buffer_size (it could be bigger)

	// NOTE: I am not just calling upload on this because that would require multiple map/unmap sequences, or create holding buffers which cost extra memory
	ImDrawVert *vertex_buffer = reinterpret_cast<ImDrawVert *>(this->m_vertex_buffer.map());
	ImDrawIdx  *index_buffer  = reinterpret_cast<ImDrawIdx *>(this->m_index_buffer.map());

	for (int n = 0; n < a_draw_data->CmdListsCount; n++)
	{
		const ImDrawList *cmd_list = a_draw_data->CmdLists[n];

		std::memcpy(reinterpret_cast<void *>(vertex_buffer), cmd_list->VtxBuffer.Data, static_cast<size_t>(cmd_list->VtxBuffer.Size) * sizeof(ImDrawVert));
		std::memcpy(reinterpret_cast<void *>(index_buffer), cmd_list->IdxBuffer.Data, static_cast<size_t>(cmd_list->IdxBuffer.Size) * sizeof(ImDrawIdx));

		vertex_buffer += cmd_list->VtxBuffer.Size;
		index_buffer += cmd_list->IdxBuffer.Size;
	}

	this->m_vertex_buffer.unmap();
	this->m_index_buffer.unmap();

	this->m_vertex_buffer.ready(true);
	this->m_index_buffer.ready(true);
}

void Gui::setup_render_state(rhi::RenderCommandEncoder &a_encoder, const ror::Renderer &a_renderer, ImDrawData *a_draw_data)
{
	a_encoder.cull_mode(rhi::PrimitiveCullMode::none);                                             // No face culling
	a_encoder.front_facing_winding(rhi::PrimitiveWinding::clockwise);                              // What ImGui requires
	a_encoder.vertex_buffer(this->m_vertex_buffer, 0, 0);                                          // Position at index 0 offset 0 because its provided in attribute
	a_encoder.vertex_buffer(this->m_vertex_buffer, 0, 1);                                          // UV at index 1 offset 0 because its provided in attribute
	a_encoder.vertex_buffer(this->m_vertex_buffer, 0, 2);                                          // Color at index 2 offset 0 because its provided in attribute
	a_encoder.render_pipeline_state(this->m_shader_program);                                       // Include alpha blend state
	a_encoder.fragment_texture(this->m_texture_image, 0);                                          // Could be overriden by render later
	a_encoder.fragment_sampler(this->m_texture_sampler, 0);                                        // The only default sampler that should be bilinear according to ImGui requirements
	a_encoder.depth_stencil_state(a_renderer.render_state().depth_state_always_no_write());        // No depth test as we are rendering painter style quads

	// Setup viewport, orthographic projection matrix
	const float32_t viewport_near{0.0f};
	const float32_t viewport_far{1.0f};

	float32_t fb_width  = a_draw_data->DisplaySize.x * a_draw_data->FramebufferScale.x;
	float32_t fb_height = a_draw_data->DisplaySize.y * a_draw_data->FramebufferScale.y;

	a_encoder.viewport({0.0f, 0.0f, fb_width, fb_height},
	                   {viewport_near, viewport_far});

	float L = a_draw_data->DisplayPos.x;
	float R = a_draw_data->DisplayPos.x + a_draw_data->DisplaySize.x;
	float T = a_draw_data->DisplayPos.y;
	float B = a_draw_data->DisplayPos.y + a_draw_data->DisplaySize.y;
	float N = viewport_near;
	float F = viewport_far;

	auto ortho_projection_matrix = ror::make_ortho(L, R, B, T, N, F);
	ortho_projection_matrix.m_values[14] *= -1;        // For some reason that I don't yet understand this is what ImGui requires, this probably has something to do with handedness and NDC from 0 - 1 mapping

	this->m_shader_buffer.buffer_map();
	this->m_shader_buffer.update("orthographic_projection", &ortho_projection_matrix);
	this->m_shader_buffer.buffer_unmap();
	this->m_shader_buffer.buffer_bind(a_encoder, rhi::ShaderStage::vertex);
}

void Gui::render(const ror::Renderer &a_renderer, rhi::RenderCommandEncoder &a_encoder, ror::OrbitCamera &a_camera, ror::EventSystem &a_event_system)
{
	auto dimensions = a_renderer.dimensions();
	this->draw_test_windows(a_camera, dimensions, a_event_system);

	ImDrawData *draw_data = ImGui::GetDrawData();
	if (!draw_data)
		return;

	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	float32_t fb_width  = draw_data->DisplaySize.x * draw_data->FramebufferScale.x;
	float32_t fb_height = draw_data->DisplaySize.y * draw_data->FramebufferScale.y;

	if (fb_width <= 0.0f || fb_height <= 0.0f || draw_data->CmdListsCount == 0)
		return;

	ImGuiIO &io = ImGui::GetIO();

	// FIXME: Add proper time
	io.DeltaTime = 1.0f / 60.0f;        // set the time elapsed since the previous frame (in seconds)

	this->upload_draw_data(draw_data);
	this->setup_render_state(a_encoder, a_renderer, draw_data);

	std::vector<std::vector<uint8_t>> vertexBuffer{};
	std::vector<std::vector<uint8_t>> indexBuffer{};

	// Will project scissor/clipping rectangles into framebuffer space
	ImVec2 clip_off   = draw_data->DisplayPos;              // (0,0) unless using multi-viewports
	ImVec2 clip_scale = draw_data->FramebufferScale;        // (1,1) unless using retina display which are often (2,2)

	size_t vertexBufferOffset = 0;
	size_t indexBufferOffset  = 0;

	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList *cmd_list = draw_data->CmdLists[n];

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[cmd_i];

			if (pcmd->UserCallback)
			{
				if (pcmd->UserCallback != ImDrawCallback_ResetRenderState)        // NOTE: Not using reset render state
					pcmd->UserCallback(cmd_list, pcmd);
				else
				{
					assert(0 && "Not using imgui callback reset functionality yet");
				}
			}
			else
			{
				// Project scissor/clipping rectangles into framebuffer space
				ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
				ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);

				// Clamp to viewport as setScissorRect() won't accept values that are off bounds
				clip_min.x = std::max(0.0f, clip_min.x);
				clip_min.y = std::max(0.0f, clip_min.y);

				clip_max.x = std::min(fb_width, clip_max.x);
				clip_max.y = std::min(fb_height, clip_max.y);

				if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y || pcmd->ElemCount == 0)        // drawIndexedPrimitives() validation doesn't accept this
					continue;

				// Set scissor for this draw-call
				a_encoder.scissor({static_cast<uint32_t>(clip_min.x),
				                   static_cast<uint32_t>(clip_min.y),
				                   static_cast<uint32_t>(clip_max.x - clip_min.x),
				                   static_cast<uint32_t>(clip_max.y - clip_min.y)});

				// Bind texture, this might be redundant if there is only ever going be one texture
				if (ImTextureID tex_id = pcmd->GetTexID())
					a_encoder.fragment_texture(*reinterpret_cast<rhi::TextureImage *>(tex_id), 0);

				a_encoder.vertex_buffer_offset(vertexBufferOffset + pcmd->VtxOffset * sizeof(ImDrawVert), 0);
				a_encoder.vertex_buffer_offset(vertexBufferOffset + pcmd->VtxOffset * sizeof(ImDrawVert), 1);
				a_encoder.vertex_buffer_offset(vertexBufferOffset + pcmd->VtxOffset * sizeof(ImDrawVert), 2);
				a_encoder.draw_indexed_primitives(rhi::PrimitiveTopology::triangles, pcmd->ElemCount, rhi::Format::uint16_1, this->m_index_buffer, indexBufferOffset + pcmd->IdxOffset * sizeof(ImDrawIdx));
			}
		}

		vertexBufferOffset += static_cast<size_t>(cmd_list->VtxBuffer.Size) * sizeof(ImDrawVert);
		indexBufferOffset += static_cast<size_t>(cmd_list->IdxBuffer.Size) * sizeof(ImDrawIdx);
	}
}

void Gui::push_anchor(Anchors::Anchor a_anchor)
{
	this->m_anchors.push_anchor(std::move(a_anchor));
}

size_t Gui::anchors_count()
{
	return this->m_anchors.anchors_count();
}

Anchors::Anchor &Gui::anchor(size_t a_index)
{
	return this->m_anchors.anchor(a_index);
}

bool Gui::anchor_moving(size_t a_index)
{
	return this->m_anchors.moving(a_index);
}

}        // namespace ror
