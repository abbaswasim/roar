
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
#include "math/rorvector2.hpp"
#include "math/rorvector4.hpp"
#include "rhi/rortypes.hpp"

namespace rhi
{
template <class _type>
class Attachment final
{
  public:
	FORCE_INLINE             Attachment()                              = default;        //! Default constructor
	FORCE_INLINE             Attachment(const Attachment &a_other)     = default;        //! Copy constructor
	FORCE_INLINE             Attachment(Attachment &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE Attachment &operator=(const Attachment &a_other)      = default;        //! Copy assignment operator
	FORCE_INLINE Attachment &operator=(Attachment &&a_other) noexcept  = default;        //! Move assignment operator
	FORCE_INLINE ~Attachment() noexcept                                = default;        //! Destructor

	uint32_t m_target_index{};                  //! Could be a render_target or render_buffer to write into
	_type    m_action{_type::dont_care};        //! What to do at load time or store time

  protected:
  private:
};

using InputAttachment  = Attachment<LoadAction>;
using OutputAttachment = Attachment<StoreAction>;

template <class _type>
class RenderpassCrtp : public ror::Crtp<_type, RenderpassCrtp>
{
  public:
	FORCE_INLINE                 RenderpassCrtp(const RenderpassCrtp &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                 RenderpassCrtp(RenderpassCrtp &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE RenderpassCrtp &operator=(const RenderpassCrtp &a_other)          = default;        //! Copy assignment operator
	FORCE_INLINE RenderpassCrtp &operator=(RenderpassCrtp &&a_other) noexcept      = default;        //! Move assignment operator
	FORCE_INLINE virtual ~RenderpassCrtp() noexcept override                       = default;        //! Destructor

	// clang-format off
	FORCE_INLINE auto name()                  { return this->m_name;               }
	FORCE_INLINE auto technique()             { return this->m_technique;          }
	FORCE_INLINE auto type()                  { return this->m_type;               }
	FORCE_INLINE auto state()                 { return this->m_state;              }
	FORCE_INLINE auto input_attachments()     { return this->m_input_attachments;  }
	FORCE_INLINE auto output_attachments()    { return this->m_output_attachments; }
	FORCE_INLINE auto dimensions()            { return this->m_dimensions;         }
	FORCE_INLINE auto viewport()              { return this->m_viewport;           }
	FORCE_INLINE auto parents()               { return this->m_parents;            }
	FORCE_INLINE auto program_id()            { return this->m_program_id;         }
	FORCE_INLINE auto debug_output()          { return this->m_debug_output;       }

	FORCE_INLINE void name(const std::string& a_name)                                                 { this->m_name = a_name;                             }
	FORCE_INLINE void technique(rhi::RenderpassTechnique a_technique)                                 { this->m_technique = a_technique;                   }
	FORCE_INLINE void type(rhi::RenderpassType a_type)                                                { this->m_type = a_type;                             }
	FORCE_INLINE void state(rhi::RenderpassState a_state)                                             { this->m_state = a_state;                           }
	FORCE_INLINE void input_attachments(const std::vector<InputAttachment> &a_input_attachments)      { this->m_input_attachments = a_input_attachments;   }
	FORCE_INLINE void output_attachments(const std::vector<OutputAttachment> &a_output_attachments)   { this->m_output_attachments = a_output_attachments; }
	FORCE_INLINE void dimensions(ror::Vector2ui a_dimensions)                                         { this->m_dimensions = a_dimensions;                 }
	FORCE_INLINE void viewport(ror::Vector4i a_viewport)                                              { this->m_viewport = a_viewport;                     }
	FORCE_INLINE void parents(const std::vector<uint32_t>& a_parents)                                 { this->m_parents = a_parents;                       }
	FORCE_INLINE void program_id(int32_t a_program_id)                                                { this->m_program_id = a_program_id;                 }
	FORCE_INLINE void debug_output(bool a_debug_output)                                               { this->m_debug_output = a_debug_output;             }
	// clang-format on

  protected:
	FORCE_INLINE RenderpassCrtp() = default;        //! Default constructor
  private:
	std::string                   m_name{};                                               //! Debug name of this render pass
	rhi::RenderpassTechnique      m_technique{rhi::RenderpassTechnique::fragment};        //! Will this render pass be excuted in fragment or compute
	rhi::RenderpassType           m_type{rhi::RenderpassType::main};                      //! Is it a shadow, reflection etc or main pass etc
	rhi::RenderpassState          m_state{rhi::RenderpassState::transient};               //! Do I need to pre-run this once or required every frame
	std::vector<InputAttachment>  m_input_attachments{};                                  //! Input attachments
	std::vector<OutputAttachment> m_output_attachments{};                                 //! Output attachments
	ror::Vector2ui                m_dimensions{};                                         //! Dimensions for this renderpass if provided will override frame graph dimensions
	ror::Vector4i                 m_viewport{};                                           //! Viewport for this renderpass if provided will override frame graph viewport
	std::vector<uint32_t>         m_parents{};                                            //! All passes that need to complete before this can run
	int32_t                       m_program_id{};                                         //! A program id that could be used to execute this pass or will use the content PSOs
	bool                          m_debug_output{false};                                  //! Whether debug output is required
};

}        // namespace rhi

#include "rhi/crtp_interfaces/rorrenderpass.hh"
