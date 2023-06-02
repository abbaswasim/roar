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
#include <cstddef>
#include <cstdint>
#include <sys/wait.h>

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

	auto &setting = ror::settings();

	this->m_show_anchors  = setting.m_gui.m_show_anchors;
	this->m_show_gizmo    = setting.m_gui.m_show_gizmo;
	this->m_show_overlays = setting.m_gui.m_show_overlays;
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

		this->m_event_system->unsubscribe(keyboard_question_mark_click, this->m_demo_windown_callback);
	}
}

void Gui::init_upload(rhi::Device &a_device, ror::EventSystem &a_event_system)
{
	this->m_device = &a_device;

	this->m_event_system = &a_event_system;

	auto &setting = ror::settings();

	ImGuiIO &io        = ImGui::GetIO();
	io.FontGlobalScale = setting.m_gui.m_font_scale;        //  0.45f;        // For font blender_droid at size 32 using font scale .45 makes it look nicer

	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Incase there are indices which doesn't fit in uint16_t
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

	// Setup style
	if (setting.m_gui.m_theme == ror::Settings::GuiTheme::dark)
		ImGui::StyleColorsDark();
	else if (setting.m_gui.m_theme == ror::Settings::GuiTheme::light)
		ImGui::StyleColorsLight();
	else        // Assumes classic otherwise
		ImGui::StyleColorsClassic();

	ImGui::GetStyle().ScaleAllSizes(setting.m_gui.m_scale);        // If you want to make things bigger, should probably be DPI aware

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
		font_cfg.SizePixels           = setting.m_gui.m_font_size;
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
		font_cfg.SizePixels = setting.m_gui.m_font_size;
		this->m_fonts.emplace_back(io.Fonts->AddFontDefault(&font_cfg));
	}

	// Lets enable the default font from the setting
	this->m_current_font = setting.m_gui.m_default_font;
	if (this->m_fonts.size() > this->m_current_font && this->m_fonts[this->m_current_font] != nullptr)
		this->m_default_font = this->m_fonts[this->m_current_font];

	uint8_t *pixels;
	int      width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	size_t   size       = static_cast<size_t>(width) * static_cast<size_t>(height) * 4;
	uint8_t *new_pixels = new uint8_t[size];        // Need a copy because texture will own the pixels from now on
	std::memcpy(new_pixels, pixels, size);

	rhi::fill_texture_from_memory(new_pixels, static_cast<uint32_t>(width), static_cast<uint32_t>(height), 4, this->m_texture_image, "imgui_font_texture");

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
	auto vs_shader = rhi::build_shader(a_device, "gui.glsl.vert");
	auto fs_shader = rhi::build_shader(a_device, "gui.glsl.frag");

	this->m_shader_program.upload(a_device, vs_shader, fs_shader, this->m_vertex_descriptor, rhi::BlendMode::blend, rhi::PrimitiveTopology::triangles, "gui_pso", true, false, true);

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

void Gui::draw_test_windows(ror::OrbitCamera &a_camera, ror::Vector4f &a_dimensions)
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

	rhi::Device &a_device = *this->m_device;

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

void Gui::render(const ror::Renderer &a_renderer, rhi::RenderCommandEncoder &a_encoder, ror::OrbitCamera &a_camera)
{
	auto dimensions = a_renderer.dimensions();
	this->draw_test_windows(a_camera, dimensions);

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
