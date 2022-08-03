
// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2022
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
#include "core/foundation/rorcrtp.hpp"
#include "foundation/rormacros.hpp"
#include "math/rorvector2.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ror
{
class Renderer final : public Configuration<Renderer>
{
  public:
	using FrameGraph = std::unordered_map<std::string, std::vector<rhi::Renderpass>>;

	// FORCE_INLINE           Renderer()                             = default;        //! Default constructor
	FORCE_INLINE           Renderer(const Renderer &a_other)      = default;        //! Copy constructor
	FORCE_INLINE           Renderer(Renderer &&a_other) noexcept  = default;        //! Move constructor
	FORCE_INLINE Renderer &operator=(const Renderer &a_other)     = default;        //! Copy assignment operator
	FORCE_INLINE Renderer &operator=(Renderer &&a_other) noexcept = default;        //! Move assignment operator
	FORCE_INLINE virtual ~Renderer() noexcept override            = default;        //! Destructor

	FORCE_INLINE                     Renderer();        //! Default constructor
	void                             load_specific();
	void                             upload(rhi::Device &);
	std::vector<rhi::RenderpassType> render_passes();
	std::vector<rhi::RenderpassType> all_render_passes();

	// clang-format off
	auto& shaders()                       { return this->m_shaders; }
	auto& programs()                      { return this->m_programs; }
	auto& render_targets()                { return this->m_render_targets; }
	auto& render_buffers()                { return this->m_render_buffers; }
	auto& dimensions()                    { return this->m_dimensions; }
	auto& viewport()                      { return this->m_viewport; }
	auto& frame_graphs()                  { return this->m_frame_graphs; }
	auto& current_frame_graph()           { return *this->m_current_frame_graph; }

	void shaders(const std::vector<rhi::Shader>                      &a_shaders)                { this->m_shaders = a_shaders; }
	void programs(const std::vector<rhi::Program>                    &a_programs)               { this->m_programs = a_programs; }
	void dimensions(const ror::Vector2ui                             &a_dimensions)             { this->m_dimensions = a_dimensions; }
	void viewport(const ror::Vector4i                                &a_viewport)               { this->m_viewport = a_viewport; }
	void frame_graphs(const FrameGraph                               &a_frame_graphs)           { this->m_frame_graphs = a_frame_graphs; }
	void current_frame_graph(std::vector<rhi::Renderpass>            *a_current_frame_graph)    { this->m_current_frame_graph = a_current_frame_graph; }
	// clang-format on

  protected:
  private:
	declare_translation_unit_vtable();

	FORCE_INLINE std::vector<rhi::RenderpassType> render_passes(std::vector<rhi::Renderpass>& a_pass);

	void load_programs();
	void load_frame_graphs();
	void load_render_targets();
	void load_render_buffers();
	void setup_references();

	std::vector<rhi::Shader>              m_shaders{};                           //! All the global shaders
	std::vector<rhi::Program>             m_programs{};                          //! All the global shader programs
	std::vector<rhi::TextureImage>        m_render_targets{};                    //! All the render_targets some render passes might want to write into
	std::vector<rhi::Buffer<rhi::Static>> m_render_buffers{};                    //! All the render_buffers some render passes might want to write into
	ror::Vector2ui                        m_dimensions{1024, 768};               //! Dimensions of the renderer, if not provided will use from window
	ror::Vector4i                         m_viewport{0, 0, 1024, 768};           //! Viewport to use to render into the render targets, RTs can override it
	FrameGraph                            m_frame_graphs{};                      //! Frame graph for all techniques like forward, deferred etc
	std::vector<rhi::Renderpass>         *m_current_frame_graph{nullptr};        //! Non-owning raw pointer alias that Points to the active technique in the framegraphs
};

}        // namespace ror

#include "renderer/rorrenderer.hh"
