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
#include "core/foundation/rorcrtp.hpp"
#include "event_system/rorevent_system.hpp"
#include "foundation/rorjobsystem.hpp"
#include "foundation/rormacros.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "profiling/rortimer.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rorcommand_buffer.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorshader_buffer.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <functional>
#include <vector>

namespace ror
{
class Scene;
class Renderer;
}        // namespace ror

namespace rhi
{

enum class RenderOutputType
{
	color,
	resolve,
	depth,
	buffer
};

class RenderOutput
{
  public:
	FORCE_INLINE               RenderOutput(const RenderOutput &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               RenderOutput(RenderOutput &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderOutput &operator=(const RenderOutput &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE RenderOutput &operator=(RenderOutput &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderOutput() noexcept                            = default;        //! Destructor

	FORCE_INLINE RenderOutput(uint32_t a_target, LoadAction a_load_action, StoreAction a_store_action, RenderOutputType a_type) :
	    m_target_index(a_target), m_load_action(a_load_action), m_store_action(a_store_action), m_type(a_type)
	{}

	uint32_t         m_target_index{};                              //! Could be a render_target or render_buffer to write into
	LoadAction       m_load_action{LoadAction::dont_care};          //! What to do at load time
	StoreAction      m_store_action{StoreAction::dont_care};        //! What to do at store time
	RenderOutputType m_type{RenderOutputType::color};               //! Type of the render target / render buffer. It could be a resolve RT or depth RT

  protected:
	FORCE_INLINE RenderOutput() = delete;        //! Default constructor
  private:
	declare_translation_unit_vtable();
};

class RenderTarget final : public RenderOutput
{
  public:
	using TextureReference = std::reference_wrapper<rhi::TextureImage>;

	FORCE_INLINE               RenderTarget()                                = delete;         //! Default constructor
	FORCE_INLINE               RenderTarget(const RenderTarget &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               RenderTarget(RenderTarget &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderTarget &operator=(const RenderTarget &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE RenderTarget &operator=(RenderTarget &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~RenderTarget() noexcept override                           = default;        //! Destructor

	FORCE_INLINE RenderTarget(uint32_t a_target, rhi::TextureImage &a_target_reference, LoadAction a_load_action, StoreAction a_store_action, RenderOutputType a_type) :
	    RenderOutput(a_target, a_load_action, a_store_action, a_type), m_target_reference(a_target_reference)
	{}

	TextureReference m_target_reference;        //! Reference to target texture

  protected:
  private:
	declare_translation_unit_vtable() override;
};

define_type_to_shader_semantics(RenderTarget)
{
	return rhi::BufferSemantic::custom;
}

class RenderBuffer final : public RenderOutput
{
  public:
	using ShaderBufferReference = std::reference_wrapper<ShaderBuffer>;

	FORCE_INLINE               RenderBuffer()                                = delete;         //! Default constructor
	FORCE_INLINE               RenderBuffer(const RenderBuffer &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               RenderBuffer(RenderBuffer &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderBuffer &operator=(const RenderBuffer &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE RenderBuffer &operator=(RenderBuffer &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~RenderBuffer() noexcept override                           = default;        //! Destructor

	FORCE_INLINE RenderBuffer(uint32_t a_target, ShaderBuffer &a_target_reference, LoadAction a_load_action, StoreAction a_store_action, RenderOutputType a_type) :
	    RenderOutput(a_target, a_load_action, a_store_action, a_type), m_target_reference(a_target_reference)
	{}

	ShaderBufferReference m_target_reference;        //! Reference to buffer

  protected:
  private:
	declare_translation_unit_vtable() override;
};

define_type_to_shader_semantics(RenderBuffer)
{
	return rhi::BufferSemantic::custom;
}

template <class _type>
struct RenderOutputRef
{
	const _type     *m_render_output{nullptr};                 //! Non-Owning pointer to the reference object, could be a render_texture or render_buffer
	uint32_t         m_index{0};                               //! Index of the render output in the array of outputs (temporary untill we find a reference)
	rhi::ShaderStage m_stage{rhi::ShaderStage::vertex};        //! Which stage(s) it will be bound to

	FORCE_INLINE RenderOutputRef()
	{}

	FORCE_INLINE RenderOutputRef(_type *a_output, uint32_t a_index, rhi::ShaderStage a_stage) :
	    m_render_output(a_output), m_index(a_index), m_stage(a_stage)
	{}
};

class Rendersubpass final
{
  public:
	using Rendersubpasses = std::vector<std::reference_wrapper<const Rendersubpass>>;
	using RenderTargets   = std::vector<RenderOutputRef<RenderTarget>>;
	using BufferTargets   = std::vector<RenderOutputRef<RenderBuffer>>;

	FORCE_INLINE                Rendersubpass()                                 = default;        //! Default constructor
	FORCE_INLINE                Rendersubpass(const Rendersubpass &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                Rendersubpass(Rendersubpass &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Rendersubpass &operator=(const Rendersubpass &a_other)         = default;        //! Copy assignment operator
	FORCE_INLINE Rendersubpass &operator=(Rendersubpass &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE ~Rendersubpass() noexcept                                      = default;        //! Destructor

	// clang-format off
	FORCE_INLINE constexpr auto &name()                   const noexcept { return this->m_name;                  }
	FORCE_INLINE constexpr auto  technique()              const noexcept { return this->m_technique;             }
	FORCE_INLINE constexpr auto  type()                   const noexcept { return this->m_type;                  }
	FORCE_INLINE constexpr auto  state()                  const noexcept { return this->m_state;                 }
	FORCE_INLINE constexpr auto &input_attachments()      const noexcept { return this->m_input_attachments;     }
	FORCE_INLINE constexpr auto &rendered_inputs()        const noexcept { return this->m_rendered_inputs;       }
	FORCE_INLINE constexpr auto &buffer_inputs()          const noexcept { return this->m_buffer_inputs;         }
	FORCE_INLINE constexpr auto &render_targets()               noexcept { return this->m_render_targets;        }
	FORCE_INLINE constexpr auto &input_attachments()            noexcept { return this->m_input_attachments;     }
	FORCE_INLINE constexpr auto &rendered_inputs()              noexcept { return this->m_rendered_inputs;       }
	FORCE_INLINE constexpr auto &buffer_inputs()                noexcept { return this->m_buffer_inputs;         }
	FORCE_INLINE constexpr auto  program_id()             const noexcept { return this->m_program_id;            }
	FORCE_INLINE constexpr auto  debug_output()           const noexcept { return this->m_debug_output;          }
	FORCE_INLINE constexpr auto  has_depth()              const noexcept { return this->m_has_depth;             }
	FORCE_INLINE constexpr auto  cull_mode()              const noexcept { return this->m_cull_mode;             }

	FORCE_INLINE constexpr void name(const std::string& a_name)                                            noexcept { this->m_name = a_name;                             }
	FORCE_INLINE constexpr void technique(rhi::RenderpassTechnique a_technique)                            noexcept { this->m_technique = a_technique;                   }
	FORCE_INLINE constexpr void type(rhi::RenderpassType a_type)                                           noexcept { this->m_type = a_type;                             }
	FORCE_INLINE constexpr void state(rhi::RenderpassState a_state)                                        noexcept { this->m_state = a_state;                           }
	FORCE_INLINE constexpr void render_targets(const std::vector<uint32_t> &a_render_targets)              noexcept { this->m_render_targets = a_render_targets;         }
	FORCE_INLINE constexpr void input_attachments(const RenderTargets &a_input_attachments)                noexcept { this->m_input_attachments = a_input_attachments;   }
	FORCE_INLINE constexpr void rendered_inputs(const RenderTargets &a_rendered_inputs)                    noexcept { this->m_rendered_inputs = a_rendered_inputs;       }
	FORCE_INLINE constexpr void buffer_inputs(const BufferTargets &a_buffer_inputs)                        noexcept { this->m_buffer_inputs = a_buffer_inputs;           }
	FORCE_INLINE constexpr void program_id(int32_t a_program_id)                                           noexcept { this->m_program_id = a_program_id;                 }
	FORCE_INLINE constexpr void debug_output(bool a_debug_output)                                          noexcept { this->m_debug_output = a_debug_output;             }
	FORCE_INLINE constexpr void has_depth(bool a_has_depth)                                                noexcept { this->m_has_depth = a_has_depth;                   }
	FORCE_INLINE constexpr void cull_mode(const rhi::PrimitiveCullMode a_cull_mode)                        noexcept { this->m_cull_mode = a_cull_mode;                   }
	// clang-format on

	void setup(rhi::RenderCommandEncoder &a_command_buffer, ror::Renderer &a_renderer);
	void setup(rhi::ComputeCommandEncoder &a_command_encoder, ror::Renderer &a_renderer);
	bool has_depth_attachment(const std::vector<RenderTarget> &a_renderpass_render_targets, const std::vector<uint32_t> &a_subpas_render_targets);

  protected:
  private:
	template <class _type>
	FORCE_INLINE void bind_render_inputs(_type &a_encoder);
	template <class _type>
	FORCE_INLINE void bind_input_attachments(_type &a_encoder);
	template <class _type>
	FORCE_INLINE void bind_buffer_inputs(_type &a_encoder);
	// FORCE_INLINE void bind_render_targets(rhi::ComputeCommandEncoder &a_encoder, rhi::Renderpass &a_render_pass); // NOTE: To enable if needed

	std::string              m_name{};                                               //! Debug name of this render pass
	rhi::RenderpassTechnique m_technique{rhi::RenderpassTechnique::fragment};        //! Will this render pass be excuted in fragment or compute
	rhi::RenderpassType      m_type{rhi::RenderpassType::main};                      //! Is it a shadow, reflection etc or main pass etc
	rhi::RenderpassState     m_state{rhi::RenderpassState::transient};               //! Do I need to pre-run this once or required every frame
	std::vector<uint32_t>    m_render_targets{};                                     //! Indices of outputs in Renderpass render targets NOTE: in renderer.json all the indices are pointing to renderpass::render_targets[]
	RenderTargets            m_input_attachments{};                                  //! References to input attachments from other subpasses, different from m_render_inputs
	RenderTargets            m_rendered_inputs{};                                    //! References to texture outputs from other attachments that can be sampled by this subpass as a texture, like shadow map inputs
	BufferTargets            m_buffer_inputs{};                                      //! References to buffer outputs from other attachments that can be read by this subpass as buffer inputs
	int32_t                  m_program_id{-1};                                       //! A program id that could be used to execute this pass or will use the content PSOs
	bool                     m_debug_output{false};                                  //! Whether debug output is required
	bool                     m_has_depth{false};                                     //! Whether there is a depth buffer required and attached
	rhi::PrimitiveCullMode   m_cull_mode{rhi::PrimitiveCullMode::back};              //! Subpass specific cull mode, otherwise Renderpass one is used
};

template <class _type>
class RenderpassCrtp : public ror::Crtp<_type, RenderpassCrtp>
{
  public:
	using Renderpasses = std::vector<std::reference_wrapper<RenderpassCrtp>>;

	FORCE_INLINE                 RenderpassCrtp()                                  = default;        //! Default constructor
	FORCE_INLINE                 RenderpassCrtp(const RenderpassCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                 RenderpassCrtp(RenderpassCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderpassCrtp &operator=(const RenderpassCrtp &a_other)          = default;        //! Copy assignment operator
	FORCE_INLINE RenderpassCrtp &operator=(RenderpassCrtp &&a_other) noexcept      = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderpassCrtp() noexcept override                       = default;        //! Destructor

	// clang-format off
	FORCE_INLINE constexpr auto &subpasses()                 noexcept { return this->m_subpasses;          }
	FORCE_INLINE constexpr auto &subpasses()           const noexcept { return this->m_subpasses;          }
	FORCE_INLINE constexpr auto &render_targets()      const noexcept { return this->m_render_targets;     }
	FORCE_INLINE constexpr auto &render_buffers()      const noexcept { return this->m_render_buffers;     }
	FORCE_INLINE constexpr auto  dimensions()          const noexcept { return this->m_dimensions;         }
	FORCE_INLINE constexpr auto  cull_mode()           const noexcept { return this->m_cull_mode;          }
	FORCE_INLINE constexpr auto &parent_ids()          const noexcept { return this->m_parent_ids;         }
	FORCE_INLINE constexpr auto &parents()             const noexcept { return this->m_parents;            }
	FORCE_INLINE constexpr auto  background()          const noexcept { return this->m_background;         }
	FORCE_INLINE constexpr auto  enabled()             const noexcept { return this->m_enabled;            }

	FORCE_INLINE constexpr void subpasses(const std::vector<Rendersubpass>& a_passes)                    noexcept { this->m_subpasses = a_passes;                 }
	FORCE_INLINE constexpr void dimensions(ror::Vector2ui a_dimensions)                                  noexcept { this->m_dimensions = a_dimensions;            }
	FORCE_INLINE constexpr void parent_ids(const std::vector<uint32_t>& a_parents)                       noexcept { this->m_parent_ids = a_parents;               }
	FORCE_INLINE constexpr void cull_mode(const rhi::PrimitiveCullMode a_cull_mode)                      noexcept { this->m_cull_mode = a_cull_mode;              }
	FORCE_INLINE constexpr void parents(const Renderpasses& a_parents)                                   noexcept { this->m_parents = a_parents;                  }
	FORCE_INLINE constexpr void render_targets(const std::vector<RenderTarget> &a_render_targets)        noexcept { this->m_render_targets = a_render_targets;    }
	FORCE_INLINE constexpr void render_buffers(const std::vector<RenderBuffer> &a_render_buffers)        noexcept { this->m_render_buffers = a_render_buffers;    }
	FORCE_INLINE constexpr void background(ror::Vector4f a_color)                                        noexcept { this->m_background = a_color;                 }
	FORCE_INLINE constexpr void enabled(bool a_enabled)                                                  noexcept { this->m_enabled = a_enabled;                  }
	FORCE_INLINE constexpr void upload()                                                                 noexcept { this->underlying().upload();                  }

	FORCE_INLINE constexpr auto render_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index)   noexcept { return this->underlying().render_encoder(a_command_buffer, a_index); }
	FORCE_INLINE constexpr auto compute_encoder(rhi::CommandBuffer &a_command_buffer, uint32_t a_index)  noexcept { return this->underlying().compute_encoder(a_command_buffer, a_index);}
	FORCE_INLINE constexpr auto platform_renderpass(uint32_t a_index)                                    noexcept { return this->underlying().platform_renderpass(a_index);              }
	FORCE_INLINE constexpr auto platform_renderpass_count()                                              noexcept { return this->underlying().platform_renderpass_count();               }
	FORCE_INLINE constexpr void make_final_pass(rhi::Swapchain a_surface, uint32_t a_index)              noexcept { this->underlying().make_final_pass(a_surface, a_index);              }
	// clang-format on

	FORCE_INLINE void setup(rhi::RenderCommandEncoder &a_command_encoder);
	FORCE_INLINE void setup(rhi::ComputeCommandEncoder &a_command_encoder);

	FORCE_INLINE void execute(rhi::CommandBuffer &a_command_buffer, ror::Scene &a_scene, rhi::Swapchain a_surface,
	                          ror::JobSystem &a_job_system, ror::EventSystem &a_event_system, rhi::BuffersPack &a_buffer_pack,
	                          rhi::Device &a_device, ror::Timer &a_timer, ror::Renderer &a_renderer);

  protected:
  private:
	void bind_render_targets(rhi::ComputeCommandEncoder &a_command_encoder);
	void bind_render_buffers(rhi::ComputeCommandEncoder &a_command_encoder);

	std::vector<Rendersubpass> m_subpasses{};                                    //! All the subpasses in this render pass
	std::vector<RenderTarget>  m_render_targets{};                               //! Output attachments (images), NOTE: in rorrenderer.json, these render_targets::index is index in textures[]
	std::vector<RenderBuffer>  m_render_buffers{};                               //! Output attachments (buffers), NOTE: in rorrenderer.json, these render_buffers::index is index in buffers[]
	ror::Vector4f              m_background{0.14f, 0.14f, 0.14f, 1.0f};          //! Background color of this render pass we will use to clear it with
	ror::Vector2ui             m_dimensions{1024, 768};                          //! Dimensions for this renderpass if provided will override frame graph dimensions
	std::vector<uint32_t>      m_parent_ids{};                                   //! All passes that need to complete before this can run
	Renderpasses               m_parents{};                                      //! All passes that need to complete before this can run
	rhi::PrimitiveCullMode     m_cull_mode{rhi::PrimitiveCullMode::back};        //! Default cull-mode for this render pass, default is back
	bool                       m_enabled{true};                                  //! Enabled by default unless a "disabled":true is found in the config
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rorrenderpass.hh"
