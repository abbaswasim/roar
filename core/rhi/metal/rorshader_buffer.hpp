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

#include "foundation/rormacros.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rorshader_buffer.hpp"
#include "rhi/metal/rorbuffer.hpp"
#include "rhi/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"

#include <Metal/MTLBuffer.hpp>
#include <cstddef>

namespace rhi
{
class ShaderBufferMetal : public ShaderBufferCrtp<ShaderBufferMetal>, public BufferMetal<>        // Defaul _Static type BufferMetal base class
{
  public:
	FORCE_INLINE                    ShaderBufferMetal()                                     = default;        //! Default constructor
	FORCE_INLINE                    ShaderBufferMetal(const ShaderBufferMetal &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                    ShaderBufferMetal(ShaderBufferMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderBufferMetal &operator=(const ShaderBufferMetal &a_other)             = delete;         //! Copy assignment operator
	FORCE_INLINE ShaderBufferMetal &operator=(ShaderBufferMetal &&a_other) noexcept         = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~ShaderBufferMetal() noexcept override                             = default;        //! Destructor

	template <class... _types>
	FORCE_INLINE ShaderBufferMetal(std::string a_buffer_name, rhi::ShaderBufferType a_type, rhi::Layout a_layout, uint32_t a_set, uint32_t a_binding, _types... a_others) :
	    ShaderBufferCrtp(a_buffer_name, a_type, a_layout, a_set, a_binding, a_others...)
	{}

	FORCE_INLINE ShaderBufferMetal(std::string a_buffer_name, rhi::ShaderBufferType a_type, rhi::Layout a_layout, uint32_t a_set, uint32_t a_binding) :
	    ShaderBufferCrtp(a_buffer_name, a_type, a_layout, a_set, a_binding)
	{}

	declare_translation_unit_vtable() override;

	// clang-format off
	FORCE_INLINE constexpr void  buffer_allocate(ptrdiff_t a_size)                                               { this->size(a_size); this->offset(a_size);  }
	FORCE_INLINE constexpr void  buffer_unmap()                                                         noexcept { this->unmap();                             }
	FORCE_INLINE constexpr auto  buffer_map()                                                           noexcept { return this->map();                        }
	FORCE_INLINE constexpr auto &buffer_data()                                                          noexcept { return this->data();                       }
	FORCE_INLINE constexpr void  buffer_init(rhi::Device& a_device, uint32_t a_size)                             { this->init(a_device, a_size);              }
	FORCE_INLINE constexpr void  buffer_copy(const uint8_t *a_data, size_t a_size, ptrdiff_t a_offset)  noexcept { this->copy(a_data, a_size, a_offset);      }
	FORCE_INLINE constexpr void  buffer_update()                                                        noexcept { this->reupload();                          }
	// clang-format on

  protected:
  private:
};

using ShaderBuffer = ShaderBufferMetal;

}        // namespace rhi

#include "rhi/metal/rorshader_buffer.hh"
