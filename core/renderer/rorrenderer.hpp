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
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "math/rorvector2.hpp"
#include "profiling/rortimer.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorprogram.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorrenderstate.hpp"
#include "rhi/rorshader.hpp"
#include "rhi/rorshader_buffer.hpp"
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
class Scene;

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
	void                             render(ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer);
	void                             load_specific();
	void                             upload(rhi::Device &, rhi::BuffersPack &);
	void                             upload_frame_graphs(rhi::Device &);
	std::vector<rhi::RenderpassType> render_pass_types() const;
	std::vector<rhi::RenderpassType> all_render_pass_types() const;
	void                             generate_shader_buffers_mapping();
	void                             deferred_buffer_upload(rhi::Device &a_device, ror::Scene &a_scene);
	void                             dimensions(const ror::Vector4f &a_dimensions, rhi::Device &a_device);
	// void                             add_shader_buffer(std::string a_name, rhi::ShaderInput &&a_shader_buffer);

	// clang-format off
	FORCE_INLINE constexpr auto &buffers()                      noexcept { return this->m_buffers;                 }
	FORCE_INLINE constexpr auto &program(size_t a_index)  const          { return this->m_programs[a_index];       }
	FORCE_INLINE constexpr auto &shaders()                const noexcept { return this->m_shaders;                 }
	FORCE_INLINE constexpr auto &programs()               const noexcept { return this->m_programs;                }
	FORCE_INLINE constexpr auto &textures()               const noexcept { return this->m_textures;                }
	FORCE_INLINE constexpr auto &buffers()                const noexcept { return this->m_buffers;                 }
	FORCE_INLINE constexpr auto &input_render_targets()   const noexcept { return this->m_input_render_targets;    }
	FORCE_INLINE constexpr auto &input_render_buffers()   const noexcept { return this->m_input_render_buffers;    }
	FORCE_INLINE constexpr auto &render_state()           const noexcept { return this->m_render_state;            }
	FORCE_INLINE constexpr auto &dimensions()             const noexcept { return this->m_dimensions;              }
	FORCE_INLINE constexpr auto &viewport()               const noexcept { return this->m_viewport;                }
	FORCE_INLINE constexpr auto &frame_graphs()           const noexcept { return this->m_frame_graphs;            }
	FORCE_INLINE constexpr auto &current_frame_graph()    const noexcept { return *this->m_current_frame_graph;    }

	FORCE_INLINE           auto shader_buffer(const std::string& a_name)                           const           { return this->m_buffers_mapping.at(a_name);   }

	FORCE_INLINE constexpr void shaders(const std::vector<rhi::Shader>            &a_shaders)             noexcept { this->m_shaders = a_shaders;                         }
	FORCE_INLINE constexpr void programs(const std::vector<rhi::Program>          &a_programs)            noexcept { this->m_programs = a_programs;                       }
	FORCE_INLINE constexpr void viewport(const ror::Vector4i                      &a_viewport)            noexcept { this->m_viewport = a_viewport;                       }
	FORCE_INLINE constexpr void frame_graphs(const FrameGraph                     &a_frame_graphs)        noexcept { this->m_frame_graphs = a_frame_graphs;               }
	FORCE_INLINE constexpr void current_frame_graph(std::vector<rhi::Renderpass>  *a_current_frame_graph) noexcept { this->m_current_frame_graph = a_current_frame_graph; }
	// clang-format on

	using InputRenderTargets = std::vector<rhi::RenderTarget, rhi::BufferAllocator<rhi::RenderTarget>>;
	using InputBufferTargets = std::vector<rhi::RenderBuffer, rhi::BufferAllocator<rhi::RenderBuffer>>;
	using ShaderBufferMap    = std::unordered_map<std::string, rhi::ShaderBuffer *>;

  protected:
  private:
	declare_translation_unit_vtable();

	std::vector<rhi::RenderpassType> render_pass_types(const std::vector<rhi::Renderpass> &a_pass) const;
	const rhi::RenderTarget         *find_rendertarget_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index);
	const rhi::RenderBuffer         *find_renderbuffer_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index);

	void load_programs();
	void load_frame_graphs();
	void load_textures();
	void load_buffers();
	void load_buffer_templates();
	void setup_references();

	std::vector<rhi::Shader>       m_shaders{};                                      //! All the global shaders
	std::vector<rhi::Program>      m_programs{};                                     //! All the global shader programs
	std::vector<rhi::TextureImage> m_textures{};                                     //! All the textures some render passes might want to write into
	std::vector<rhi::ShaderBuffer> m_buffers{};                                      //! All the buffers some render passes might want to write into
	ror::Vector4f                  m_dimensions{1024.0f, 768.0f, 1.0f, 1.0f};        //! Dimensions of the renderer framebuffers, if not provided will use from window, overriden by renderer.json, z, w are scaling factors
	ror::Vector4i                  m_viewport{0, 0, 1024, 768};                      //! Viewport to use to render into the render targets, RTs can override it
	FrameGraph                     m_frame_graphs{};                                 //! Frame graph for all techniques like forward, deferred etc
	std::vector<rhi::Renderpass>  *m_current_frame_graph{nullptr};                   //! Non-owning raw pointer alias that Points to the active technique in the framegraphs
	rhi::Renderstate               m_render_state{};                                 //! Almost all the render state that the renderer requires will be stored here
	InputRenderTargets             m_input_render_targets{};                         //! Render targets that are not directly associated with any render pass but required to be filled in before rendering starts
	InputBufferTargets             m_input_render_buffers{};                         //! Render buffers that are not directly associated with any render pass but required to be filled in before rendering starts
	ShaderBufferMap                m_buffers_mapping{};                              //! All the Shader buffers in m_buffers are now name accessible
};

}        // namespace ror

#include "renderer/rorrenderer.hh"
