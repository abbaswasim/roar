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
#include "camera/rorshadow_data.hpp"
#include "configuration/rorconfiguration.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "foundation/rorutilities.hpp"
#include "graphics/rordynamic_mesh.hpp"
#include "graphics/rorenvironment.hpp"
#include "graphics/rornode.hpp"
#include "profiling/rortimer.hpp"
#include "renderer/rorframes.hpp"
#include "renderer/rorframes_count.hpp"
#include "renderer/rorpicking.hpp"
#include "rhi/rorbuffer_allocator.hpp"
#include "rhi/rorrenderpass.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"
#include <cassert>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace ror
{
class Scene;

struct DebugData
{
	using LinesSoup = std::unordered_map<size_t, DynamicMesh>;

	LinesSoup   m_lines_soup{};                             //! A list of things that can be updated per frame by keeping its ID
	DynamicMesh m_shadow_cascades{};                        //! A few quads to draw the shadow cascades on
	DynamicMesh m_frustums[4];                              //! Extends of the 4 cascades
	int32_t     m_colored_lines_pso{-1};                    //! A generic PSO that can be used to render colored lines
	int32_t     m_textured_quads_pso{-1};                   //! A generic PSO that can be used to render textured quads
	int32_t     m_shadow_map_textured_quads_pso{-1};        //! A generic PSO that can be used to render textured quads
	int32_t     m_default_sampler{-1};                      //! Index of the default sampler all textured quds will use
	int32_t     m_shadow_texture{-1};                       //! Index of the shadow map texture that I want to display on one quad
};

class Renderer final : public Configuration<Renderer>
{
  public:
	using FrameGraph    = std::unordered_map<std::string, std::vector<rhi::Renderpass>>;
	using RenderpassVec = std::vector<rhi::RenderpassType>;

	FORCE_INLINE           Renderer(const Renderer &a_other)      = delete;         //! Copy constructor
	FORCE_INLINE           Renderer(Renderer &&a_other) noexcept  = delete;         //! Move constructor
	FORCE_INLINE Renderer &operator=(const Renderer &a_other)     = delete;         //! Copy assignment operator
	FORCE_INLINE Renderer &operator=(Renderer &&a_other) noexcept = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~Renderer() noexcept override            = default;        //! Destructor

	FORCE_INLINE  Renderer();        //! Default constructor
	void          render(ror::Scene &a_scene, ror::JobSystem &a_job_system, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack, rhi::Device &a_device, ror::Timer &a_timer);
	void          load_specific();
	void          upload(rhi::Device &a_device, ror::Scene &a_scene, ror::EventSystem &a_event_system, const ror::Vector4f &a_dimensions, rhi::BuffersPack &a_buffer_pack);
	void          upload_frame_graphs(rhi::Device &a_device);
	void          upload_environments(rhi::Device &a_device);
	void          cycle_environment();
	RenderpassVec render_pass_types() const;
	void          scene_buffers_upload(rhi::Device &a_device, ror::Scene &a_scene);
	void          dimensions(const ror::Vector4f &a_dimensions, rhi::Device &a_device);
	void          set_modifier_events(ror::EventSystem &a_event_system);
	void          reset_sets_bindings();
	void          set_render_mode(uint32_t a_render_mode);
	int32_t       brdf_integration_lut_index(std::string a_name = "brdf_integration_lut");
	void          push_shader_update_candidate(std::string a_shader);
	void          push_shader_record(rhi::Shader &a_shader, int32_t a_shader_id);
	void          push_dependent_shader_record(rhi::Shader &a_shader, int32_t a_shader_id, std::string a_name);
	void          get_final_pass_subpass(rhi::Renderpass **a_pass, rhi::Rendersubpass **a_subpass) const;
	void          update_per_view_uniform(const ror::Matrix4f &a_view, const ror::Matrix4f &a_projection, const ror::Vector4ui &a_viewport, const ror::Vector3f &a_eye);
	void          update_cascade_per_view_uniform(uint32_t a_cascade_index);

	// clang-format off
	FORCE_INLINE constexpr auto &buffers()                      noexcept { return this->m_buffers;                         }
	FORCE_INLINE constexpr auto &program(size_t a_index)  const          { return this->m_programs[a_index];               }
	FORCE_INLINE constexpr auto &shaders()                const noexcept { return this->m_shaders;                         }
	FORCE_INLINE constexpr auto &shaders()                      noexcept { return this->m_shaders;                         }
	FORCE_INLINE constexpr auto &programs()               const noexcept { return this->m_programs;                        }
	FORCE_INLINE constexpr auto &programs()                     noexcept { return this->m_programs;                        }
	FORCE_INLINE constexpr auto &images()                 const noexcept { return this->m_images;                          }
	FORCE_INLINE constexpr auto &images()                       noexcept { return this->m_images;                          }
	FORCE_INLINE constexpr auto &samplers()               const noexcept { return this->m_samplers;                        }
	FORCE_INLINE constexpr auto &samplers()                     noexcept { return this->m_samplers;                        }
	FORCE_INLINE constexpr auto &textures()               const noexcept { return this->m_textures;                        }
	FORCE_INLINE constexpr auto &textures()                     noexcept { return this->m_textures;                        }
	FORCE_INLINE constexpr auto &environments()           const noexcept { return this->m_environments;                    }
	FORCE_INLINE constexpr auto &buffers()                const noexcept { return this->m_buffers;                         }
	FORCE_INLINE constexpr auto &input_render_targets()   const noexcept { return this->m_input_render_targets;            }
	FORCE_INLINE constexpr auto &input_render_buffers()   const noexcept { return this->m_input_render_buffers;            }
	FORCE_INLINE constexpr auto &render_state()           const noexcept { return this->m_render_state;                    }
	FORCE_INLINE constexpr auto &dimensions()             const noexcept { return this->m_dimensions;                      }
	FORCE_INLINE constexpr auto &viewport()               const noexcept { return this->m_viewport;                        }
	FORCE_INLINE constexpr auto &frame_graphs()           const noexcept { return this->m_frame_graphs;                    }
	FORCE_INLINE constexpr auto &current_frame_graph()    const noexcept { return *this->m_current_frame_graph;            }
	FORCE_INLINE constexpr auto &shadow_cascade_cache()   const noexcept { return this->m_shadow_cascade_cache;            }
	FORCE_INLINE constexpr auto &shadow_cascades_data()   const noexcept { return this->m_shadow_cascades_data;            }
	FORCE_INLINE constexpr auto &pickables()              const noexcept { return this->m_pickables;                       }
	FORCE_INLINE constexpr auto &pickables()                    noexcept { return this->m_pickables;                       }
	FORCE_INLINE constexpr auto pick_hits()               const noexcept { return this->m_pickables.hits();                }
	FORCE_INLINE constexpr auto &pick_target()            const noexcept { return this->m_pickables.target();              }
	FORCE_INLINE constexpr auto &pick_target()                  noexcept { return this->m_pickables.target();              }
	FORCE_INLINE constexpr auto &dynamic_meshes()         const noexcept { return this->m_dynamic_meshes;                  }
	FORCE_INLINE constexpr auto current_frame_index()     const noexcept { return this->m_frames.current_frame_index();    }
	FORCE_INLINE constexpr auto canonical_cube()          const noexcept { return this->m_canonical_cube;                  }
	FORCE_INLINE constexpr auto frames_count()            const noexcept { return this->m_frames.max_frames();             }
	FORCE_INLINE constexpr auto renderpass_index()        const noexcept { return this->m_renderpass_index;                }
	FORCE_INLINE constexpr auto subpass_index()           const noexcept { return this->m_subpass_index;                   }
	FORCE_INLINE constexpr auto renderpasses_count()      const noexcept { assert(this->m_renderpasses_count);
		                                                                         return this->m_renderpasses_count;        }
	FORCE_INLINE constexpr auto shaodwpasses_count()      const noexcept { return this->m_shadowpasses_count;              }
	FORCE_INLINE constexpr auto subpasses_count()         const noexcept { assert(this->m_subpasses_count);
		                                                                         return this->m_subpasses_count;           }

	FORCE_INLINE constexpr auto &vertex_descriptors(size_t a_index)                                const noexcept  { return this->m_vertex_descriptors[a_index];                                            }
	FORCE_INLINE constexpr auto &vertex_descriptors()                                              const noexcept  { return this->m_vertex_descriptors;                                                     }
	FORCE_INLINE constexpr auto &vertex_descriptors()                                                    noexcept  { return this->m_vertex_descriptors;                                                     }
	FORCE_INLINE           auto &current_environment()                                             const noexcept  { return this->m_environments[static_cast_safe<uint32_t>(this->m_current_environment)];  }
	FORCE_INLINE           auto &current_environment()                                                   noexcept  { return this->m_environments[static_cast_safe<uint32_t>(this->m_current_environment)];  }

	FORCE_INLINE constexpr void shaders(const std::vector<rhi::Shader>            &a_shaders)             noexcept { this->m_shaders = a_shaders;                                                            }
	FORCE_INLINE constexpr void programs(const std::vector<rhi::Program>          &a_programs)            noexcept { this->m_programs = a_programs;                                                          }
	FORCE_INLINE constexpr void viewport(const ror::Vector4i                      &a_viewport)            noexcept { this->m_viewport = a_viewport;                                                          }
	FORCE_INLINE constexpr void frame_graphs(const FrameGraph                     &a_frame_graphs)        noexcept { this->m_frame_graphs = a_frame_graphs;                                                  }
	FORCE_INLINE constexpr void renderpass_index(size_t                            a_pass_index)          noexcept { this->m_renderpass_index = a_pass_index;                                                }
	FORCE_INLINE constexpr void subpass_index(size_t                               a_pass_index)          noexcept { this->m_subpass_index = a_pass_index;                                                   }
	FORCE_INLINE constexpr void next_renderpass()                                                         noexcept { this->m_renderpass_index = (this->m_renderpass_index + 1) % this->m_renderpasses_count; }
	FORCE_INLINE constexpr void next_subpass()                                                            noexcept { this->m_subpass_index = (this->m_subpass_index + 1) % this->m_subpasses_count;          }
	FORCE_INLINE constexpr void signal_pick()                                                             noexcept { this->m_pick_queue = ror::max_frames_in_flight;                                         }
	// clang-format on

	FORCE_INLINE constexpr auto picking_available() noexcept;
	FORCE_INLINE auto           calculate_shader_buffer_index(rhi::ShaderBufferFrequency a_frequency) const;
	FORCE_INLINE auto           calculate_shader_buffer_index(const rhi::ShaderBuffer &a_shader_buffer) const;
	FORCE_INLINE auto           shader_buffer(const std::string &a_name) const;

	using InputRenderTargets = std::vector<rhi::RenderTarget, rhi::BufferAllocator<rhi::RenderTarget>>;
	using InputBufferTargets = std::vector<rhi::RenderBuffer, rhi::BufferAllocator<rhi::RenderBuffer>>;
	using ShaderCallbackMap  = std::unordered_map<std::string, std::function<void(std::string &, ror::Renderer &)>>;
	using ShaderBuffers      = std::vector<std::vector<rhi::ShaderBuffer>>;
	using ShaderBufferMap    = std::unordered_map<std::string, std::vector<rhi::ShaderBuffer> *>;

	rhi::TextureImage *m_skybox_hdr_patch_ti{nullptr};
	rhi::TextureImage *m_skybox_ldr_patch_ti{nullptr};
	rhi::TextureImage *m_irradiance_patch_ti{nullptr};
	rhi::TextureImage *m_radiance_patch_ti{nullptr};

	void                     toggle_selection(ror::NodeData &a_node);
	void                     upload_debug_geometry(const rhi::Device &a_device, ror::EventSystem &a_event_system);
	void                     update_frustums_geometry(const ror::OrbitCamera &a_camera);
	const rhi::Texture      *get_render_target_texture(const rhi::Renderpass &a_pass, const rhi::Rendersubpass &a_subpass, uint32_t a_render_target_index) const;
	const rhi::Texture      *get_pass_texture(rhi::RenderpassType a_subpass_type, uint32_t a_render_target_index = 0) const;
	const rhi::Texture      *get_shadow_texture(size_t a_cascade_index) const;
	const rhi::TextureImage *get_pass_image(rhi::RenderpassType a_pass_type, uint32_t a_render_target_index = 0) const;
	const rhi::TextureImage *get_shadow_image(size_t a_cascade_index) const;
	rhi::Renderpass         *get_pass(rhi::RenderpassType a_subpass_type) const;

	std::pair<rhi::Renderpass *, rhi::Rendersubpass *> get_pass_subpass(rhi::RenderpassType a_subpass_type) const;

  protected:
  private:
	// clang-format off
	FORCE_INLINE constexpr auto shader_buffer_vector(const std::string &a_name) const          { return this->m_buffers_mapping.at(a_name);              }
	// clang-format on

	std::vector<rhi::RenderpassType> render_pass_types(const std::vector<rhi::Renderpass> &a_pass) const;
	const rhi::RenderTarget         *find_rendertarget_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index);
	const rhi::RenderBuffer         *find_renderbuffer_reference(const std::vector<rhi::Renderpass> &a_renderpasses, uint32_t a_index);
	const rhi::Renderpass           &get_shadow_pass(uint32_t a_cascade_index = 0);        // The first cascade render pass is a good representation of all the passes, they are mostly the same
	void                             generate_shader_buffers_mapping();
	void                             generate_shader_callbacks_mapping();

	void     upload_remaining_textures(rhi::Device &a_device);
	void     upload_samplers(rhi::Device &a_device);
	void     install_input_handlers(ror::EventSystem &a_event_system);
	void     uninstall_input_handlers(ror::EventSystem &a_event_system);
	void     update_shader(rhi::Device &a_device, std::string &a_shader);
	void     patch_shader(rhi::Shader &a_shader, std::string &a_shader_name);
	uint32_t environment_visualize_mode(uint32_t a_environment_index);
	void     create_environment_mesh(rhi::Device &a_device);
	void     create_grid_mesh(const rhi::Device &a_device, ror::EventSystem &a_event_system);
	void     load_programs();
	void     load_vertex_descriptors(rhi::BuffersPack &a_buffer_pack);
	void     load_debug_data();
	void     load_frame_graphs();
	void     load_textures();
	void     load_samplers();
	void     load_environments();
	void     load_buffers();
	void     load_buffer_templates();
	void     setup_references();
	void     setup_final_pass();
	void     setup_shadow_passes();
	void     setup_shader_buffers();
	void     update_shadow_cascades(ror::Scene &a_scene);
	void     update_click_function(ror::Scene &a_scene, ror::Vector2f &a_mouse_position);
	void     draw_ui();

	ror::Frames                             m_frames{};                                       //! All the frames in flight
	ror::Picking                            m_pickables{};                                    //! All the pickable nodes witin the scene for the a single pick
	std::vector<rhi::Shader>                m_shaders{};                                      //! All the global shaders
	std::vector<rhi::Program>               m_programs{};                                     //! All the global shader programs
	std::vector<uint32_t>                   m_program_vertex_desriptor_ids{};                 //! All the global shader programs descriptor indices in the vertex descriptors array
	std::vector<rhi::VertexDescriptor>      m_vertex_descriptors{};                           //! All the global vertex descriptors used by the programs, instead of relying on default ones
	std::vector<rhi::TextureImage>          m_images{};                                       //! All the texture images some render passes might want to write into
	std::vector<rhi::TextureSampler>        m_samplers{};                                     //! All samplers in the renderer, usually should only have a mipmapped and non-mipmapped sampler, but could have more
	std::vector<rhi::Texture>               m_textures{};                                     //! All textures the renderer has loaded. This is only used to associate samplers with images
	std::vector<ror::IBLEnvironment>        m_environments{};                                 //! All the IBL environments, referring to textures within m_textures
	ShaderBuffers                           m_buffers{};                                      //! All the buffers some render passes might want to write into. This could be max (per-pass * per-frame) count if this buffer type requires it
	ror::Vector4f                           m_dimensions{1024.0f, 768.0f, 1.0f, 1.0f};        //! Dimensions of the renderer framebuffers, if not provided will use from window, overriden by renderer.json, z, w are scaling factors
	ror::Vector4i                           m_viewport{0, 0, 1024, 768};                      //! Viewport to use to render into the render targets, RTs can override it
	FrameGraph                              m_frame_graphs{};                                 //! Frame graph for all techniques like forward, deferred etc
	std::vector<rhi::Renderpass>           *m_current_frame_graph{nullptr};                   //! Non-owning raw pointer alias that Points to the active technique in the framegraphs
	rhi::Renderstate<rhi::RenderstateDepth> m_render_state{};                                 //! Almost all the render state that the renderer requires will be stored here, currently only uses depth state
	InputRenderTargets                      m_input_render_targets{};                         //! Render targets that are not directly associated with any render pass but required to be filled in before rendering starts
	InputBufferTargets                      m_input_render_buffers{};                         //! Render buffers that are not directly associated with any render pass but required to be filled in before rendering starts
	ShaderBufferMap                         m_buffers_mapping{};                              //! All the Shader buffers in m_buffers are now name accessible, TODO: this should be a vector or (per-pass * per-frame) count
	ShaderCallbackMap                       m_callbacks_mapping{};                            //! All the callbacks that can be used to patch or do something else to shaders
	std::vector<ror::DynamicMesh *>         m_dynamic_meshes{};                               //! Non-Owning pointers to all the dynamic meshes created in the renderer should be rendererd at the end, mostly has cubemap meshes etc
	ror::DynamicMesh                        m_cube_map_mesh{};                                //! Single instance of a cubemap mesh used by all environments
	DebugData                               m_debug_data{};                                   //! Other debug data like shadow map quads and camera frustums
	ror::Light::CascadeData                 m_shadow_cascade_cache{};                         //! Cache of shadow data for current scene's current camera and current directional shadow light
	ror::ShadowData<ror::cascade_count>     m_shadow_cascades_data{};                         //! Cache of shadow data for all cascades that is used to build shadow_cascades_data for the directional light
	int32_t                                 m_current_environment{-1};                        //! Which of the available environments should we use
	int32_t                                 m_grid_id{-1};                                    //! Reference to the grid for easy access
	uint32_t                                m_canonical_cube{};                               //! Canonical cube for debugging purposes
	int32_t                                 m_pick_queue{-1};                                 //! This is a queue of frames to wait before a pick pulling is valid, -1 means no picking signaled
	uint32_t                                m_final_pass{};                                   //! Index of the final pass in the render pass chains in framegraph
	uint32_t                                m_shadowpasses_count{0};                          //! Total amount of shadow passes in the current framegraph
	const rhi::TextureImage                *m_shadow_image{nullptr};                          //! Cached copy of the image used as render target for shadow maps
	const rhi::TextureImage                *m_pick_image{nullptr};                            //! Cached copy of the image used as render target for picking
	size_t                                  m_shadow_window_index{0};                         //! Index of the gui windows used to render the shadow image into
	size_t                                  m_picking_window_index{0};                        //! Index of the gui windows used to render the picking render target image into
	size_t                                  m_renderpasses_count{0};                          //! Total amount of renderpasses in the current framegraph
	size_t                                  m_subpasses_count{0};                             //! Total amount of subpasses in the current framegraph
	size_t                                  m_renderpass_index{0};                            //! Which renderpass are we currently rendering from the frame graph. This is state
	size_t                                  m_subpass_index{0};                               //! Which subpass are we currently rendering from the frame graph. This is state
	int32_t                                 m_shadow_passes[ror::cascade_count];              //! Indices of the shadow passes in the render pass chains in framegraph
	ror::Scene                             *m_scene{nullptr};                                 //! Non-owning alias to the currently rendered scene (main/global scene)
	EventCallback                           m_semi_colon_key_callback{};                      //! Semi colon key call back to enable disable the grid
	EventCallback                           m_click_callback{};                               //! Click lambda function that will be used to subscribe and unsubsribe this camera with event system for mouse click events
	std::function<void(IBLEnvironment &)>   m_environment_upload{};                           //! Environment upload lambda used by environment cycle menthod
	ror::NodeData                          *m_selected_node{nullptr};                         //! If not nullptr will contain the last selected node when picking
};
}        // namespace ror

#include "renderer/rorrenderer.hh"
