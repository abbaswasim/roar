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
#include <type_traits>

namespace rhi
{
/**
 * Description of a geometry vertex
 * Prefered to be used via VertexDescriptor not directly
 */
class ROAR_ENGINE_ITEM VertexAttribute final
{
  public:
	FORCE_INLINE                  VertexAttribute(const VertexAttribute &a_other)     = default;        //! Copy constructor
	FORCE_INLINE                  VertexAttribute(VertexAttribute &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE VertexAttribute &operator=(const VertexAttribute &a_other)           = default;        //! Copy assignment operator
	FORCE_INLINE VertexAttribute &operator=(VertexAttribute &&a_other) noexcept       = default;        //! Move assignment operator
	FORCE_INLINE ~VertexAttribute() noexcept                                          = default;        //! Destructor

	/**
	 * Use this version of the constructor if you want VertexDescriptor to take care of
	 * Creating valid buffers and allocate location, offset and binding for you
	 * This makes VertexAttribute more aware and clever than dumb data blob
	 */
	FORCE_INLINE VertexAttribute(rhi::BufferSemantic a_semantics = rhi::BufferSemantic::vertex_position,
	                             rhi::VertexFormat   a_format    = rhi::VertexFormat::float32_3) :
	    m_semantics(a_semantics),
	    m_format(a_format)
	{}        //! Default constructor as well

	/**
	 * Use this version of the constructor if you want to take care of creating buffers yourself
	 * At the time of use make sure a_binding is valid
	 */
	FORCE_INLINE VertexAttribute(uint32_t            a_location,
	                             uint32_t            a_offset,
	                             uint32_t            a_count,
	                             size_t              a_buffer_offset,
	                             uint32_t            a_binding,
	                             uint32_t            a_buffer_index,
	                             rhi::BufferSemantic a_semantics = rhi::BufferSemantic::vertex_position,
	                             rhi::VertexFormat   a_format    = rhi::VertexFormat::float32_3) :
	    m_location(a_location),
	    m_offset(a_offset),
	    m_count(a_count),
	    m_buffer_offset(a_buffer_offset),
	    m_binding(a_binding),
	    m_buffer_index(a_buffer_index),
	    m_semantics(a_semantics),
	    m_format(a_format)
	{}

	// clang-format off
	FORCE_INLINE constexpr auto location()         const noexcept { return this->m_location;         }
	FORCE_INLINE constexpr auto offset()           const noexcept { return this->m_offset;           }
	FORCE_INLINE constexpr auto count()            const noexcept { return this->m_count;            }
	FORCE_INLINE constexpr auto buffer_offset()    const noexcept { return this->m_buffer_offset;    }
	FORCE_INLINE constexpr auto binding()          const noexcept { return this->m_binding;          }
	FORCE_INLINE constexpr auto buffer_index()     const noexcept { return this->m_buffer_index;     }
	FORCE_INLINE constexpr auto semantics()        const noexcept { return this->m_semantics;        }
	FORCE_INLINE constexpr auto format()           const noexcept { return this->m_format;           }

	FORCE_INLINE void   location(uint32_t a_location)              noexcept { this->m_location         = a_location;     }
	FORCE_INLINE void   offset(uint32_t a_offset)                  noexcept { this->m_offset           = a_offset;       }
	FORCE_INLINE void   count(uint32_t a_count)                    noexcept { this->m_count            = a_count;        }
	FORCE_INLINE void   buffer_offset(size_t a_offset)             noexcept { this->m_buffer_offset    = a_offset;       }
	FORCE_INLINE void   binding(uint32_t a_binding)                noexcept { this->m_binding          = a_binding;      }
	FORCE_INLINE void   buffer_index(uint32_t a_index)             noexcept { this->m_buffer_index     = a_index;        }
	FORCE_INLINE void   semantics(rhi::BufferSemantic a_semantics) noexcept { this->m_semantics        = a_semantics;    }
	FORCE_INLINE void   format(rhi::VertexFormat a_format)         noexcept { this->m_format           = a_format;       }
	// clang-format on

  private:
	uint32_t            m_location{0};                                            //! Use in shader like "layout(location = m_location)", decided by layout
	uint32_t            m_offset{0};                                              //! Offset of this attribute relative to other attributes
	uint32_t            m_count{0};                                               //! How many of VertexFormat count elements are there in this attribute
	size_t              m_buffer_offset{0};                                       //! Offset of this attribute in buffer buffer_index from the start, decided by layout
	uint32_t            m_binding{0};                                             //! Copy of the binding in its corresponding layout binding that describes this attribute
	uint32_t            m_buffer_index{0};                                        //! Which buffer do I live in? this is the buffer_pack index, destination buffer index
	rhi::BufferSemantic m_semantics{rhi::BufferSemantic::vertex_position};        //! Whats the type of attribute? Position, UV, Normal etc
	rhi::VertexFormat   m_format{rhi::VertexFormat::float32_3};                   //! float/int vec2/vec3/vec4 normalized/not packed/unpacked
};

static_assert(std::is_trivially_copyable_v<VertexAttribute>, "VertexAttribute is not trivially copyable");
static_assert(std::is_standard_layout_v<VertexAttribute>, "VertexAttribute is not standard layout");

}        // namespace rhi
