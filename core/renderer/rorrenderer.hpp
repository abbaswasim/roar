
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

	FORCE_INLINE Renderer();        //! Default constructor
	void         load_specific();

  protected:
  private:
	declare_translation_unit_vtable();

	void load_programs();
	void load_frame_graphs();
	void load_render_targets();
	void load_render_buffers();

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
