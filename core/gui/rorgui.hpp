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

#pragma once

#include "camera/rorcamera.hpp"
#include "event_system/rorevent_system.hpp"
#include "renderer/rorrenderer.hpp"
#include "rhi/rordevice.hpp"
#include "roranchor.hpp"
#include "rorgizmo.hpp"

#include <vector>

namespace ror
{

class ROAR_ENGINE_ITEM Gui final
{
  public:
	FORCE_INLINE      Gui(const Gui &a_other)           = delete;        //! Copy constructor
	FORCE_INLINE      Gui(Gui &&a_other) noexcept       = delete;        //! Move constructor
	FORCE_INLINE Gui &operator=(const Gui &a_other)     = delete;        //! Copy assignment operator
	FORCE_INLINE Gui &operator=(Gui &&a_other) noexcept = delete;        //! Move assignment operator

	Gui();                  //! Default constructor
	~Gui() noexcept;        //! Destructor

	void init_upload(rhi::Device &a_device, ror::EventSystem &a_event_system);
	void render(const ror::Renderer &a_renderer, rhi::RenderCommandEncoder &a_encoder, ror::OrbitCamera &a_camera);

	enum imgui_keys
	{
		imgui_keys_left,
		imgui_keys_right,
		imgui_keys_middle
	};

  protected:
  private:
	void             install_input_handlers();
	void             uninstall_input_handlers();
	void             draw_test_windows(ror::OrbitCamera &a_camera, ror::Vector4f &a_dimensions);
	void             setup_render_state(rhi::RenderCommandEncoder &a_encoder, const ror::Renderer &a_renderer, ImDrawData *a_draw_data);
	void             push_anchor(Anchors::Anchor a_anchor);
	size_t           anchors_count();
	Anchors::Anchor &anchor(size_t a_index);
	bool             anchor_moving(size_t a_index);
	void             upload_draw_data(ImDrawData *a_draw_data);

	Anchors m_anchors{};
	Gizmo   m_gizmo{};

	std::vector<ImFont *> m_fonts{};                       //! All the fonts provided in gui fonts list
	uint32_t              m_current_font{0};               //! Index of the default pointer
	ImFont               *m_default_font{nullptr};         //! Non-Owning pointer to a default pointer
	ror::EventSystem     *m_event_system{nullptr};         //! Non-owning pointer to the event system
	EventCallback         m_move_callback{};               //! Mouse move function that will be used to subscribe and unsubscribe this imgui with event system
	EventCallback         m_left_down_callback{};          //! Mouse down click function that will be used to subscribe and unsubscribe this imgui with event system
	EventCallback         m_left_up_callback{};            //! Mouse up function that will be used to subscribe and unsubscribe this imgui with event system
	EventCallback         m_right_down_callback{};         //! Same but for right click
	EventCallback         m_right_up_callback{};           //! Same but for right click
	EventCallback         m_mouse_wheel_callback{};        //! Mouse wheel event to scroll up
	EventCallback         m_tab_key_callback{};            //! Tab key call back to enable disable the UI

	// Render data
	rhi::Device          *m_device{nullptr};               //! Non-Owning pointer to a device that is used to initiliazed this gui
	rhi::TextureImage     m_texture_image{};               //! Texture image, should probably be a list of these at some point
	rhi::TextureSampler   m_texture_sampler{};             //! Texture sampler, the default sampler used to render the UI
	rhi::Program          m_shader_program{-1, -1};        //! Program in undefined/uinitialized state
	rhi::VertexDescriptor m_vertex_descriptor{};           //! The vertex descriptor of the UI, that is defined by xy, uv, c of ImGUI vertex buffer
	rhi::Buffer           m_vertex_buffer{};               //! Vertex buffer with interleaved data of xy,uv,c
	rhi::Buffer           m_index_buffer{};                //! Index buffer with uint16_t type

	rhi::ShaderBuffer m_shader_buffer{"gui_per_frame_uniform", rhi::ShaderBufferType::ubo, rhi::Layout::std140, 0, 3};        // this 3 needs to match the one in gui.glsl.vert
};

FORCE_INLINE Gui &gui() noexcept
{
	static Gui ui{};        // Global gui container
	return ui;
}

}        // namespace ror
