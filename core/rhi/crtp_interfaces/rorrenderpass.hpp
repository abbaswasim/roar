
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
#include "foundation/rormacros.hpp"
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rorbuffer.hpp"
#include "rhi/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include <functional>
#include <vector>

namespace rhi
{
class RenderOutput
{
  public:
	FORCE_INLINE               RenderOutput(const RenderOutput &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               RenderOutput(RenderOutput &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderOutput &operator=(const RenderOutput &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE RenderOutput &operator=(RenderOutput &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderOutput() noexcept                            = default;        //! Destructor

	FORCE_INLINE RenderOutput(uint32_t a_target, LoadAction a_load_action, StoreAction a_store_action) :
	    m_target_index(a_target), m_load_action(a_load_action), m_store_action(a_store_action)
	{}

	uint32_t    m_target_index{};                              //! Could be a render_target or render_buffer to write into
	LoadAction  m_load_action{LoadAction::dont_care};          //! What to do at load time
	StoreAction m_store_action{StoreAction::dont_care};        //! What to do at store time

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

	FORCE_INLINE RenderTarget(uint32_t a_target, rhi::TextureImage &a_target_reference, LoadAction a_load_action, StoreAction a_store_action) :
	    RenderOutput(a_target, a_load_action, a_store_action), m_target_reference(a_target_reference)
	{}

	TextureReference m_target_reference;        //! Reference to target texture

  protected:
  private:
	declare_translation_unit_vtable() override;
};

class RenderBuffer final : public RenderOutput
{
  public:
	using BufferReference = std::reference_wrapper<rhi::Buffer<rhi::Static>>;

	FORCE_INLINE               RenderBuffer()                                = delete;         //! Default constructor
	FORCE_INLINE               RenderBuffer(const RenderBuffer &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               RenderBuffer(RenderBuffer &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderBuffer &operator=(const RenderBuffer &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE RenderBuffer &operator=(RenderBuffer &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~RenderBuffer() noexcept override                           = default;        //! Destructor

	FORCE_INLINE RenderBuffer(uint32_t a_target, rhi::Buffer<rhi::Static> &a_target_reference, LoadAction a_load_action, StoreAction a_store_action) :
	    RenderOutput(a_target, a_load_action, a_store_action), m_target_reference(a_target_reference)
	{}

	BufferReference m_target_reference;        //! Reference to buffer

  protected:
  private:
	declare_translation_unit_vtable() override;
};

class Rendersubpass final
{
  public:
	using Rendersubpasses = std::vector<std::reference_wrapper<const Rendersubpass>>;
	using RenderTargets   = std::vector<std::reference_wrapper<const RenderTarget>>;

	FORCE_INLINE                Rendersubpass()                                 = default;        //! Default constructor
	FORCE_INLINE                Rendersubpass(const Rendersubpass &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                Rendersubpass(Rendersubpass &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Rendersubpass &operator=(const Rendersubpass &a_other)         = default;        //! Copy assignment operator
	FORCE_INLINE Rendersubpass &operator=(Rendersubpass &&a_other) noexcept     = default;        //! Move assignment operator
	FORCE_INLINE ~Rendersubpass() noexcept                                      = default;        //! Destructor

	// clang-format off
	FORCE_INLINE constexpr auto &name()                   const noexcept { return this->m_name;                  }
	FORCE_INLINE constexpr auto technique()               const noexcept { return this->m_technique;             }
	FORCE_INLINE constexpr auto type()                    const noexcept { return this->m_type;                  }
	FORCE_INLINE constexpr auto state()                   const noexcept { return this->m_state;                 }
	FORCE_INLINE constexpr auto &input_attachment_ids()   const noexcept { return this->m_input_attachment_ids;  }
	FORCE_INLINE constexpr auto &rendered_input_ids()     const noexcept { return this->m_rendered_input_ids;    }
	FORCE_INLINE constexpr auto &input_attachments()      const noexcept { return this->m_input_attachments;     }
	FORCE_INLINE constexpr auto &rendered_inputs()        const noexcept { return this->m_rendered_inputs;       }
	FORCE_INLINE constexpr auto program_id()              const noexcept { return this->m_program_id;            }
	FORCE_INLINE constexpr auto debug_output()            const noexcept { return this->m_debug_output;          }
	FORCE_INLINE constexpr auto has_depth()               const noexcept { return this->m_has_depth;             }

	FORCE_INLINE constexpr void name(const std::string& a_name)                                            noexcept { this->m_name = a_name;                             }
	FORCE_INLINE constexpr void technique(rhi::RenderpassTechnique a_technique)                            noexcept { this->m_technique = a_technique;                   }
	FORCE_INLINE constexpr void type(rhi::RenderpassType a_type)                                           noexcept { this->m_type = a_type;                             }
	FORCE_INLINE constexpr void state(rhi::RenderpassState a_state)                                        noexcept { this->m_state = a_state;                           }
	FORCE_INLINE constexpr void input_attachment_ids(const std::vector<uint32_t> &a_input_attachments)     noexcept { this->m_input_attachment_ids = a_input_attachments;}
	FORCE_INLINE constexpr void rendered_input_ids(const std::vector<uint32_t> &a_rendered_inputs)         noexcept { this->m_rendered_input_ids = a_rendered_inputs;    }
	FORCE_INLINE constexpr void input_attachments(const Rendersubpasses &a_input_attachments)              noexcept { this->m_input_attachments = a_input_attachments;   }
	FORCE_INLINE constexpr void rendered_inputs(const RenderTargets &a_rendered_inputs)                    noexcept { this->m_rendered_inputs = a_rendered_inputs;       }
	FORCE_INLINE constexpr void program_id(int32_t a_program_id)                                           noexcept { this->m_program_id = a_program_id;                 }
	FORCE_INLINE constexpr void debug_output(bool a_debug_output)                                          noexcept { this->m_debug_output = a_debug_output;             }
	FORCE_INLINE constexpr void has_depth(bool a_has_depth)                                                noexcept { this->m_has_depth = a_has_depth;                   }
	// clang-format on

  protected:
  private:
	std::string              m_name{};                                               //! Debug name of this render pass
	rhi::RenderpassTechnique m_technique{rhi::RenderpassTechnique::fragment};        //! Will this render pass be excuted in fragment or compute
	rhi::RenderpassType      m_type{rhi::RenderpassType::main};                      //! Is it a shadow, reflection etc or main pass etc
	rhi::RenderpassState     m_state{rhi::RenderpassState::transient};               //! Do I need to pre-run this once or required every frame
	std::vector<uint32_t>    m_input_attachment_ids{};                               //! Input attachments from other subpasses, different from m_render_inputs
	std::vector<uint32_t>    m_rendered_input_ids{};                                 //! Outputs from other attachments that can be sampled by this subpass as a texture, like shadow map
	Rendersubpasses          m_input_attachments{};                                  //! Refereces to input attachments from other subpasses, different from m_render_inputs
	RenderTargets            m_rendered_inputs{};                                    //! References to outputs from other attachments that can be sampled by this subpass as a texture, like shadow map
	int32_t                  m_program_id{};                                         //! A program id that could be used to execute this pass or will use the content PSOs
	bool                     m_debug_output{false};                                  //! Whether debug output is required
	bool                     m_has_depth{false};                                     //! Whether there is a depth buffer required and attached
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
	FORCE_INLINE ~RenderpassCrtp() noexcept override                               = default;        //! Destructor

	// clang-format off
	FORCE_INLINE constexpr auto &subpasses()                 noexcept { return this->m_subpasses;          }
	FORCE_INLINE constexpr auto &subpasses()           const noexcept { return this->m_subpasses;          }
	FORCE_INLINE constexpr auto &render_targets()      const noexcept { return this->m_render_targets;     }
	FORCE_INLINE constexpr auto &render_buffers()      const noexcept { return this->m_render_buffers;     }
	FORCE_INLINE constexpr auto  dimensions()          const noexcept { return this->m_dimensions;         }
	FORCE_INLINE constexpr auto  viewport()            const noexcept { return this->m_viewport;           }
	FORCE_INLINE constexpr auto &parent_ids()          const noexcept { return this->m_parent_ids;         }
	FORCE_INLINE constexpr auto &parents()             const noexcept { return this->m_parents;            }
	FORCE_INLINE constexpr auto  background()          const noexcept { return this->m_background;         }

	FORCE_INLINE constexpr void subpasses(const std::vector<Rendersubpass>& a_passes)                    { this->m_subpasses = a_passes;                 }
	FORCE_INLINE constexpr void dimensions(ror::Vector2ui a_dimensions)                                  { this->m_dimensions = a_dimensions;            }
	FORCE_INLINE constexpr void viewport(ror::Vector4i a_viewport)                                       { this->m_viewport = a_viewport;                }
	FORCE_INLINE constexpr void parent_ids(const std::vector<uint32_t>& a_parents)                       { this->m_parent_ids = a_parents;               }
	FORCE_INLINE constexpr void parents(const Renderpasses& a_parents)                                   { this->m_parents = a_parents;                  }
	FORCE_INLINE constexpr void render_targets(const std::vector<RenderTarget> &a_render_targets)        { this->m_render_targets = a_render_targets;    }
	FORCE_INLINE constexpr void render_buffers(const std::vector<RenderBuffer> &a_render_buffers)        { this->m_render_buffers = a_render_buffers;    }
	FORCE_INLINE constexpr void background(ror::Vector4f a_color)                                        { this->m_background = a_color;                 }
	FORCE_INLINE constexpr void upload()                                                                 { this->underlying().upload();                  }
	// clang-format on

  protected:
  private:
	std::vector<Rendersubpass> m_subpasses{};                                  //! All the subpasses in this render pass
	std::vector<RenderTarget>  m_render_targets{};                             //! Output attachments (images)
	std::vector<RenderBuffer>  m_render_buffers{};                             //! Output attachments (buffers)
	ror::Vector4f              m_background{0.14f, 0.14f, 0.14f, 1.0f};        //! Background color of this render pass we will use to clear it with
	ror::Vector2ui             m_dimensions{1024, 768};                        //! Dimensions for this renderpass if provided will override frame graph dimensions
	ror::Vector4i              m_viewport{0, 0, 1024, 768};                    //! Viewport for this renderpass if provided will override frame graph viewport
	std::vector<uint32_t>      m_parent_ids{};                                 //! All passes that need to complete before this can run
	Renderpasses               m_parents{};                                    //! All passes that need to complete before this can run
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rorrenderpass.hh"
