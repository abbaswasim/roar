
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
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorrenderstate.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rorshader_input.hpp"
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
	FORCE_INLINE           Renderer(const Renderer &a_other)      = delete;         //! Copy constructor
	FORCE_INLINE           Renderer(Renderer &&a_other) noexcept  = delete;         //! Move constructor
	FORCE_INLINE Renderer &operator=(const Renderer &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Renderer &operator=(Renderer &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~Renderer() noexcept override            = default;        //! Destructor

	FORCE_INLINE                     Renderer();        //! Default constructor
	void                             load_specific();
	void                             upload(rhi::Device &);
	std::vector<rhi::RenderpassType> render_pass_types() const;
	std::vector<rhi::RenderpassType> all_render_pass_types() const;

	// clang-format off
	FORCE_INLINE constexpr auto& shaders()                const noexcept { return this->m_shaders;                 }
	FORCE_INLINE constexpr auto& programs()               const noexcept { return this->m_programs;                }
	FORCE_INLINE constexpr auto& textures()               const noexcept { return this->m_textures;                }
	FORCE_INLINE constexpr auto& buffers()                const noexcept { return this->m_buffers;                 }
	FORCE_INLINE constexpr auto& input_render_targets()   const noexcept { return this->m_input_render_targets;    }
	FORCE_INLINE constexpr auto& input_render_buffers()   const noexcept { return this->m_input_render_buffers;    }
	FORCE_INLINE constexpr auto& render_state()           const noexcept { return this->m_render_state;            }
	FORCE_INLINE constexpr auto& dimensions()             const noexcept { return this->m_dimensions;              }
	FORCE_INLINE constexpr auto& viewport()               const noexcept { return this->m_viewport;                }
	FORCE_INLINE constexpr auto& frame_graphs()           const noexcept { return this->m_frame_graphs;            }
	FORCE_INLINE constexpr auto& current_frame_graph()    const noexcept { return *this->m_current_frame_graph;    }
	FORCE_INLINE constexpr auto& shader_buffers()         const noexcept { return this->m_global_shader_buffers;   }

	FORCE_INLINE           auto& shader_buffer(const std::string& a_name) { return this->m_global_shader_buffers.at(a_name);   }

	FORCE_INLINE constexpr void shaders(const std::vector<rhi::Shader>            &a_shaders)             noexcept { this->m_shaders = a_shaders;                         }
	FORCE_INLINE constexpr void programs(const std::vector<rhi::Program>          &a_programs)            noexcept { this->m_programs = a_programs;                       }
	FORCE_INLINE constexpr void dimensions(const ror::Vector2ui                   &a_dimensions)          noexcept { this->m_dimensions = a_dimensions;                   }
	FORCE_INLINE constexpr void viewport(const ror::Vector4i                      &a_viewport)            noexcept { this->m_viewport = a_viewport;                       }
	FORCE_INLINE constexpr void frame_graphs(const FrameGraph                     &a_frame_graphs)        noexcept { this->m_frame_graphs = a_frame_graphs;               }
	FORCE_INLINE constexpr void current_frame_graph(std::vector<rhi::Renderpass>  *a_current_frame_graph) noexcept { this->m_current_frame_graph = a_current_frame_graph; }
	// clang-format on

	using InputRenderTargets = std::vector<rhi::RenderTarget, rhi::BufferAllocator<rhi::RenderTarget>>;
	using InputBufferTargets = std::vector<rhi::RenderBuffer, rhi::BufferAllocator<rhi::RenderBuffer>>;
	using ShaderBufferMap    = std::unordered_map<std::string, rhi::ShaderBuffer*>;

  protected:
  private:
	declare_translation_unit_vtable();

	std::vector<rhi::RenderpassType>                render_pass_types(const std::vector<rhi::Renderpass> &a_pass) const;
	std::reference_wrapper<const rhi::RenderTarget> find_rendertarget_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index);
	std::reference_wrapper<const rhi::RenderBuffer> find_renderbuffer_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index);

	void load_programs();
	void load_frame_graphs();
	void load_textures();
	void load_buffers();
	void setup_references();

	std::vector<rhi::Shader>              m_shaders{};                           //! All the global shaders
	std::vector<rhi::Program>             m_programs{};                          //! All the global shader programs
	std::vector<rhi::TextureImage>        m_textures{};                          //! All the textures some render passes might want to write into
	std::vector<rhi::Buffer<rhi::Static>> m_buffers{};                           //! All the buffers some render passes might want to write into
	ror::Vector2ui                        m_dimensions{1024, 768};               //! Dimensions of the renderer framebuffers, if not provided will use from window, overriden by renderer.json
	ror::Vector4i                         m_viewport{0, 0, 1024, 768};           //! Viewport to use to render into the render targets, RTs can override it
	FrameGraph                            m_frame_graphs{};                      //! Frame graph for all techniques like forward, deferred etc
	std::vector<rhi::Renderpass>         *m_current_frame_graph{nullptr};        //! Non-owning raw pointer alias that Points to the active technique in the framegraphs
	rhi::Renderstate                      m_render_state{};                      //! Almost all the render state that the renderer requires will be stored here
	InputRenderTargets                    m_input_render_targets{};              //! Render targets that are not directly associated with any render pass but required to be filled in before rendering starts
	InputBufferTargets                    m_input_render_buffers{};              //! Render buffers that are not directly associated with any render pass but required to be filled in before rendering starts
	ShaderBufferMap                       m_global_shader_buffers{};             //! All the global Shader buffers (UBO/SSBO) that can be used in different passes
};

}        // namespace ror

#include "renderer/rorrenderer.hh"
