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

#include "camera/rorcamera.hpp"
#include "configuration/rorconfiguration.hpp"
#include "core/foundation/rorcrtp.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rordynamic_mesh.hpp"
#include "graphics/rorenvironment.hpp"
#include "math/rorsegments.hpp"
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

struct DebugData
{
	std::vector<Lines3f> m_camera_fustrums{};                        //! A line list for each camera, this is just CPU side data
	std::vector<Lines3f> m_camera_cascades{};                        //! A line list for each frustum cascade, this is just CPU side data
	DynamicMesh          m_shadow_cascades{};                        //! A few quads to draw the shadow cascades on
	DynamicMesh          m_frustums[4];                              //! Extends of the 4 cascades
	int32_t              m_colored_lines_pso{-1};                    //! A generic PSO that can be used to render colored lines
	int32_t              m_textured_quads_pso{-1};                   //! A generic PSO that can be used to render textured quads
	int32_t              m_shadow_map_textured_quads_pso{-1};        //! A generic PSO that can be used to render textured quads
	int32_t              m_default_sampler{-1};                      //! Index of the default sampler all textured quds will use
	int32_t              m_shadow_texture{-1};                       //! Index of the shadow map texture that I want to display on one quad
};

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
	void                             upload(rhi::Device &a_device, rhi::BuffersPack &a_buffer_pack);
	void                             upload_frame_graphs(rhi::Device &a_device);
	void                             upload_environments(rhi::Device &a_device);
	std::vector<rhi::RenderpassType> render_pass_types() const;
	void                             deferred_buffer_upload(rhi::Device &a_device, ror::Scene &a_scene);
	void                             dimensions(const ror::Vector4f &a_dimensions, rhi::Device &a_device);
	void                             set_modifier_events(ror::EventSystem &a_event_system);
	void                             reset_sets_bindings();
	void                             set_render_mode(uint32_t a_render_mode);
	int32_t                          brdf_integration_lut_index(std::string a_name = "brdf_integration_lut");
	void                             push_shader_update_candidate(std::string a_shader);
	void                             push_shader_record(rhi::Shader &a_shader, int32_t a_shader_id);
	void                             push_dependent_shader_record(rhi::Shader &a_shader, int32_t a_shader_id, std::string a_name);
	void                             get_final_pass_subpass(rhi::Renderpass **a_pass, rhi::Rendersubpass **a_subpass) const;
	void                             update_per_view_uniform(const ror::Matrix4f &a_view, const ror::Matrix4f &a_projection, const ror::Vector4ui &a_viewport, const ror::Vector3f &a_eye);

	// void                             add_shader_buffer(std::string a_name, rhi::ShaderInput &&a_shader_buffer);

	// clang-format off
	FORCE_INLINE constexpr auto &buffers()                      noexcept { return this->m_buffers;                 }
	FORCE_INLINE constexpr auto &program(size_t a_index)  const          { return this->m_programs[a_index];       }
	FORCE_INLINE constexpr auto &shaders()                const noexcept { return this->m_shaders;                 }
	FORCE_INLINE constexpr auto &shaders()                      noexcept { return this->m_shaders;                 }
	FORCE_INLINE constexpr auto &programs()               const noexcept { return this->m_programs;                }
	FORCE_INLINE constexpr auto &programs()                     noexcept { return this->m_programs;                }
	FORCE_INLINE constexpr auto &images()                 const noexcept { return this->m_images;                  }
	FORCE_INLINE constexpr auto &images()                       noexcept { return this->m_images;                  }
	FORCE_INLINE constexpr auto &samplers()               const noexcept { return this->m_samplers;                }
	FORCE_INLINE constexpr auto &samplers()                     noexcept { return this->m_samplers;                }
	FORCE_INLINE constexpr auto &textures()               const noexcept { return this->m_textures;                }
	FORCE_INLINE constexpr auto &textures()                     noexcept { return this->m_textures;                }
	FORCE_INLINE constexpr auto &environments()           const noexcept { return this->m_environments;            }
	FORCE_INLINE constexpr auto &buffers()                const noexcept { return this->m_buffers;                 }
	FORCE_INLINE constexpr auto &input_render_targets()   const noexcept { return this->m_input_render_targets;    }
	FORCE_INLINE constexpr auto &input_render_buffers()   const noexcept { return this->m_input_render_buffers;    }
	FORCE_INLINE constexpr auto &render_state()           const noexcept { return this->m_render_state;            }
	FORCE_INLINE constexpr auto &dimensions()             const noexcept { return this->m_dimensions;              }
	FORCE_INLINE constexpr auto &viewport()               const noexcept { return this->m_viewport;                }
	FORCE_INLINE constexpr auto &frame_graphs()           const noexcept { return this->m_frame_graphs;            }
	FORCE_INLINE constexpr auto &current_frame_graph()    const noexcept { return *this->m_current_frame_graph;    }
	FORCE_INLINE constexpr auto &dynamic_meshes()         const noexcept { return this->m_dynamic_meshes;          }
	FORCE_INLINE constexpr auto canonical_cube()          const noexcept { return this->m_canonical_cube;          }

	FORCE_INLINE           auto shader_buffer(const std::string& a_name)                           const           { return this->m_buffers_mapping.at(a_name);   }
	FORCE_INLINE           auto &current_environment()                                             const noexcept  { return this->m_environments[static_cast_safe<uint32_t>(this->m_current_environment)];}

	FORCE_INLINE constexpr void shaders(const std::vector<rhi::Shader>            &a_shaders)             noexcept { this->m_shaders = a_shaders;                         }
	FORCE_INLINE constexpr void programs(const std::vector<rhi::Program>          &a_programs)            noexcept { this->m_programs = a_programs;                       }
	FORCE_INLINE constexpr void viewport(const ror::Vector4i                      &a_viewport)            noexcept { this->m_viewport = a_viewport;                       }
	FORCE_INLINE constexpr void frame_graphs(const FrameGraph                     &a_frame_graphs)        noexcept { this->m_frame_graphs = a_frame_graphs;               }
	FORCE_INLINE constexpr void current_frame_graph(std::vector<rhi::Renderpass>  *a_current_frame_graph) noexcept { this->m_current_frame_graph = a_current_frame_graph; }
	// clang-format on

	using InputRenderTargets = std::vector<rhi::RenderTarget, rhi::BufferAllocator<rhi::RenderTarget>>;
	using InputBufferTargets = std::vector<rhi::RenderBuffer, rhi::BufferAllocator<rhi::RenderBuffer>>;
	using ShaderCallbackMap  = std::unordered_map<std::string, std::function<void(std::string &, ror::Renderer &)>>;
	using ShaderBufferMap    = std::unordered_map<std::string, rhi::ShaderBuffer *>;

	rhi::TextureImage *m_skybox_hdr_patch_ti{nullptr};
	rhi::TextureImage *m_skybox_ldr_patch_ti{nullptr};
	rhi::TextureImage *m_irradiance_patch_ti{nullptr};
	rhi::TextureImage *m_radiance_patch_ti{nullptr};

	void                upload_debug_geometry(const rhi::Device &a_device, ror::EventSystem &a_event_system, ror::Scene &a_scene);
	void                update_frustums_geometry(const ror::OrbitCamera &a_camera);
	const rhi::Texture *get_shadow_texture() const;

  protected:
  private:
	declare_translation_unit_vtable();

	std::vector<rhi::RenderpassType> render_pass_types(const std::vector<rhi::Renderpass> &a_pass) const;
	const rhi::RenderTarget         *find_rendertarget_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index);
	const rhi::RenderBuffer         *find_renderbuffer_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index);
	void                             generate_shader_buffers_mapping();
	void                             generate_shader_callbacks_mapping();

	void upload_remaining_textures(rhi::Device &a_device);
	void upload_samplers(rhi::Device &a_device);

	void     install_input_handlers(ror::EventSystem &a_event_system);
	void     uninstall_input_handlers(ror::EventSystem &a_event_system);
	void     update_shader(rhi::Device &a_device, std::string &a_shader);
	void     patch_shader(rhi::Shader &a_shader, std::string &a_shader_name);
	uint32_t environment_visualize_mode(uint32_t a_environment_index);
	void     create_environment_mesh(rhi::Device &a_device);
	void     create_grid_mesh(const rhi::Device &a_device, ror::EventSystem &a_event_system);
	void     load_programs();
	void     load_debug_data();
	void     load_frame_graphs();
	void     load_textures();
	void     load_samplers();
	void     load_environments();
	void     load_buffers();
	void     load_buffer_templates();
	void     setup_references();
	void     setup_final_pass();
	void     setup_shadow_pass();

	std::vector<rhi::Shader>                m_shaders{};                                      //! All the global shaders
	std::vector<rhi::Program>               m_programs{};                                     //! All the global shader programs
	std::vector<rhi::TextureImage>          m_images{};                                       //! All the texture images some render passes might want to write into
	std::vector<rhi::TextureSampler>        m_samplers{};                                     //! All samplers in the renderer, usually should only have a mipmapped and non-mipmapped sampler, but could have more
	std::vector<rhi::Texture>               m_textures{};                                     //! All textures the renderer has loaded. This is only used to associate samplers with images
	std::vector<ror::IBLEnvironment>        m_environments{};                                 //! All the IBL environments, referring to textures within m_textures
	std::vector<rhi::ShaderBuffer>          m_buffers{};                                      //! All the buffers some render passes might want to write into TODO: this should be a vector or (per-pass * per-frame) count
	ror::Vector4f                           m_dimensions{1024.0f, 768.0f, 1.0f, 1.0f};        //! Dimensions of the renderer framebuffers, if not provided will use from window, overriden by renderer.json, z, w are scaling factors
	ror::Vector4i                           m_viewport{0, 0, 1024, 768};                      //! Viewport to use to render into the render targets, RTs can override it
	FrameGraph                              m_frame_graphs{};                                 //! Frame graph for all techniques like forward, deferred etc
	std::vector<rhi::Renderpass>           *m_current_frame_graph{nullptr};                   //! Non-owning raw pointer alias that Points to the active technique in the framegraphs
	rhi::Renderstate<rhi::RenderstateDepth> m_render_state{};                                 //! Almost all the render state that the renderer requires will be stored here, currently only uses depth state
	InputRenderTargets                      m_input_render_targets{};                         //! Render targets that are not directly associated with any render pass but required to be filled in before rendering starts
	InputBufferTargets                      m_input_render_buffers{};                         //! Render buffers that are not directly associated with any render pass but required to be filled in before rendering starts
	ShaderBufferMap                         m_buffers_mapping{};                              //! All the Shader buffers in m_buffers are now name accessible, TODO: this should be a vector or (per-pass * per-frame) count
	ShaderCallbackMap                       m_callbacks_mapping{};                            //! All the callbacks that can be used to patch or do something else to shaders
	std::vector<ror::DynamicMesh *>         m_dynamic_meshes{};                               //! Non-Owning pointers to all the dynamic meshes created in the renderer should be rendererd at the end, mostly has cubemap mes
	ror::DynamicMesh                        m_cube_map_mesh{};                                //! Single instance of a cubemap mesh used by all environments
	DebugData                               m_debug_data{};                                   //! Other debug data like shadow map quads and camera frustums
	int32_t                                 m_current_environment{-1};                        //! Which of the available environments should we use
	int32_t                                 m_grid_id{-1};                                    //! Reference to the grid for easy access
	uint32_t                                m_canonical_cube{};                               //! Canonical cube for debugging purposes
	uint32_t                                m_final_pass{};                                   //! Index of the final pass in the render pass chains in framegraph
	int32_t                                 m_shadow_pass{-1};                                //! Index of the shadow pass in the render pass chains in framegraph
	EventCallback                           m_semi_colon_key_callback{};                      //! Semi colon key call back to enable disable the grid
};
}        // namespace ror

#include "renderer/rorrenderer.hh"
