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
	FORCE_INLINE VertexAttribute &operator=(const VertexAttribute &a_other) = default;                  //! Copy assignment operator
	FORCE_INLINE VertexAttribute &operator=(VertexAttribute &&a_other) noexcept = default;              //! Move assignment operator
	FORCE_INLINE ~VertexAttribute() noexcept                                    = default;              //! Destructor

	/**
	 * Use this version of the constructor if you want VertexDescriptor to take care of
	 * Creating valid buffers and allocate location, offset and binding for you
	 * If in doubt if the attribute represented is valid or not call complete() to check
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
								 uint64_t            a_buffer_offset,
								 uint32_t            a_binding,
								 uint32_t            a_buffer_index,
								 rhi::BufferSemantic a_semantics = rhi::BufferSemantic::vertex_position,
								 rhi::VertexFormat   a_format    = rhi::VertexFormat::float32_3) :
		m_location(a_location),
		m_offset(a_offset),
		m_buffer_offset(a_buffer_offset),
		m_binding(a_binding),
		m_buffer_index(a_buffer_index),
		m_semantics(a_semantics),
		m_format(a_format),
		m_complete(true)
	{}

	// clang-format off
	FORCE_INLINE uint32_t            location()         const { return this->m_location;         }
	FORCE_INLINE uint32_t            offset()           const { return this->m_offset;           }
	FORCE_INLINE uint64_t            buffer_offset()    const { return this->m_buffer_offset;    }
	FORCE_INLINE uint32_t            binding()          const { return this->m_binding;          }
	FORCE_INLINE uint32_t            buffer_index()     const { return this->m_buffer_index;     }
	FORCE_INLINE rhi::VertexFormat   format()           const { return this->m_format;           }
	FORCE_INLINE rhi::BufferSemantic semantics()        const { return this->m_semantics;        }
	FORCE_INLINE bool                complete()         const { return this->m_complete;         }

	FORCE_INLINE void   location(uint32_t a_location)              noexcept { this->m_location         = a_location;     }
	FORCE_INLINE void   offset(uint32_t a_offset)                  noexcept { this->m_offset           = a_offset;       }
	FORCE_INLINE void   buffer_offset(uint64_t a_offset)           noexcept { this->m_buffer_offset    = a_offset;       }
	FORCE_INLINE void   binding(uint32_t a_binding)                noexcept { this->m_binding          = a_binding;      }
	FORCE_INLINE void   buffer_index(uint32_t a_index)             noexcept { this->m_buffer_index     = a_index;        }
	FORCE_INLINE void   format(rhi::VertexFormat a_format)         noexcept { this->m_format           = a_format;       }
	FORCE_INLINE void   semantics(rhi::BufferSemantic a_semantics) noexcept { this->m_semantics        = a_semantics;    }
	FORCE_INLINE void   complete(bool a_complete)                  noexcept { this->m_complete         = a_complete;     }
	// clang-format on

  private:
	uint32_t            m_location{0};                                            //! Use in shader like "layout(location = m_location)", decided by layout
	uint32_t            m_offset{0};                                              //! Offset of this attribute relative to other attributes
	uint64_t            m_buffer_offset{0};                                       //! Offset of this attribute in buffer buffer_index from the start, decided by layout
	uint32_t            m_binding{0};                                             //! Which layout binding describes this attribute
	uint32_t            m_buffer_index{0};                                        //! Which buffer do I live in? this is the buffer_pack index, destination buffer index
	rhi::BufferSemantic m_semantics{rhi::BufferSemantic::vertex_position};        //! Whats the type of attribute? Position, UV, Normal etc
	rhi::VertexFormat   m_format{rhi::VertexFormat::float32_3};                   //! float/int vec2/vec3/vec4 normalized/not packed/unpacked
	bool                m_complete{false};                                        //! Whether the vertex attribute is complete and valid, worked out by layout walk
};

static_assert(std::is_trivially_copyable_v<VertexAttribute>, "VertexAttribute is not trivially copyable");
static_assert(std::is_standard_layout_v<VertexAttribute>, "VertexAttribute is not standard layout");

}        // namespace rhi
