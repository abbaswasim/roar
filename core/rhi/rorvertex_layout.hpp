// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2008-2021
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

#include "rhi/rorbuffers_format.hpp"

namespace rhi
{
class ROAR_ENGINE_ITEM VertexLayout final
{
  public:
	FORCE_INLINE               VertexLayout()                                = default;        //! Default constructor
	FORCE_INLINE               VertexLayout(const VertexLayout &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               VertexLayout(VertexLayout &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE VertexLayout &operator=(const VertexLayout &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE VertexLayout &operator=(VertexLayout &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE ~VertexLayout() noexcept                                    = default;        //! Destructor

	/**
	 * Use this version of the constructor if you want VertexDescriptor to take care of
	 * Creating valid buffers and allocate stride for you
	 * If in doubt if the layout represented is valid or not call complete() to check
	 * This makes VertexLayout more aware and clever than dumb data blob
	 */
	FORCE_INLINE VertexLayout(uint32_t a_binding, uint32_t a_rate, rhi::StepFunction a_function) :
	    m_binding(a_binding),
	    m_rate(a_rate),
	    m_function(a_function)
	{}

	/**
	 * Use this version of the constructor if you want to take care of creating stride yourself
	 * At the time of use layout will be considered complete and valid
	 */
	FORCE_INLINE VertexLayout(uint32_t a_binding, uint32_t a_stride, uint32_t a_rate = 1, uint32_t a_multiplier = 1, rhi::StepFunction a_function = rhi::StepFunction::vertex) :
	    m_binding(a_binding),
	    m_stride(a_stride),
	    m_rate(a_rate),
	    m_format_multiplier(a_multiplier),
	    m_function(a_function)
	{}

	// clang-format off
	FORCE_INLINE uint32_t           binding()               const { return this->m_binding;              }
	FORCE_INLINE uint32_t           stride()                const { return this->m_stride;               }
	FORCE_INLINE uint32_t           rate()                  const { return this->m_rate;                 }          // Make sure to read the lower 16bits only for rate
	FORCE_INLINE uint32_t           format_multiplier()     const { return this->m_format_multiplier;    }          // Make sure to read the higher 16bits only for rate
	FORCE_INLINE rhi::StepFunction  step_function()         const { return this->m_function;             }

	FORCE_INLINE void   binding(uint32_t a_binding)                     noexcept { this->m_binding            = a_binding;           }
	FORCE_INLINE void   stride(uint32_t a_stride)                       noexcept { this->m_stride             = a_stride;            }
	FORCE_INLINE void   rate(uint32_t a_rate)                           noexcept { this->m_rate               = a_rate;              }
	FORCE_INLINE void   format_multiplier(uint32_t a_multiplier)        noexcept { this->m_format_multiplier  = a_multiplier;        }
	FORCE_INLINE void   step_function(rhi::StepFunction a_function)     noexcept { this->m_function           = a_function;          }
	// clang-format on

  private:
	uint32_t          m_binding{0};                                 //! Binding in shaders set by VertexDescriptor
	uint32_t          m_stride{0};                                  //! Stride from attribute to attribute, set by VertexDescriptor
	uint32_t          m_rate{1};                                    //! Contain rate at which attributes are read. Default 1, only use when m_function is instance
	uint32_t          m_format_multiplier{1};                       //! Defined for custom formats and arrays
	rhi::StepFunction m_function{rhi::StepFunction::vertex};        //! Only valid for anything that's not vertex attribute for example per instance data
};

static_assert(std::is_trivially_copyable_v<VertexLayout>, "VertexLayout is not trivially copyable");
static_assert(std::is_standard_layout_v<VertexLayout>, "VertexLayout is not standard layout");

}        // namespace rhi
