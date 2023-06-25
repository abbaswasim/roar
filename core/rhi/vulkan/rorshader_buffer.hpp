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
#include "rhi/rordevice.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rorshader_buffer_template.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/vulkan/rorbuffer.hpp"
#include "rhi/vulkan/rortypes.hpp"

#include <cstddef>

namespace rhi
{
class ShaderBufferVulkan : public ShaderBufferCrtp<ShaderBufferVulkan>, public BufferVulkan
{
  public:
	FORCE_INLINE                     ShaderBufferVulkan()                                      = delete;         //! Default constructor
	FORCE_INLINE                     ShaderBufferVulkan(const ShaderBufferVulkan &a_other)     = delete;         //! Copy constructor
	FORCE_INLINE                     ShaderBufferVulkan(ShaderBufferVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE ShaderBufferVulkan &operator=(const ShaderBufferVulkan &a_other)              = delete;         //! Copy assignment operator
	FORCE_INLINE ShaderBufferVulkan &operator=(ShaderBufferVulkan &&a_other) noexcept          = delete;         //! Move assignment operator
	FORCE_INLINE virtual ~ShaderBufferVulkan() noexcept override                               = default;        //! Destructor

	template <class... _types>
	FORCE_INLINE ShaderBufferVulkan(std::string a_buffer_name, rhi::ShaderBufferType a_type, rhi::Layout a_layout, uint32_t a_set, uint32_t a_binding, _types... a_others) :
	    ShaderBufferCrtp(a_buffer_name, a_type, a_layout, a_set, a_binding, a_others...)
	{}

	FORCE_INLINE ShaderBufferVulkan(std::string a_buffer_name, rhi::ShaderBufferType a_type, rhi::Layout a_layout, uint32_t a_set, uint32_t a_binding) :
	    ShaderBufferCrtp(a_buffer_name, a_type, a_layout, a_set, a_binding)
	{}

	declare_translation_unit_vtable() override;

	// clang-format off
	template<typename _encoder_type>
	FORCE_INLINE constexpr void  buffer_bind(_encoder_type& a_encoder, rhi::ShaderStage a_stage)                        { this->bind(a_encoder, a_stage, this->offset(), this->binding());  }
	template<typename _encoder_type>
	FORCE_INLINE constexpr void  buffer_bind(_encoder_type& a_encoder, rhi::ShaderStage a_stage, uintptr_t a_binding)   { this->bind(a_encoder, a_stage, this->offset(), a_binding);        }
	FORCE_INLINE constexpr void  buffer_unmap()                                                         noexcept        { this->unmap(); this->m_mapped_address = nullptr;                  }
	FORCE_INLINE constexpr void  buffer_map()                                                           noexcept        { this->m_mapped_address = this->map();                             }
	FORCE_INLINE constexpr void  buffer_init(rhi::Device& a_device, uint32_t a_size, rhi::ResourceStorageOption a_mode) { this->init(a_device, a_size, a_mode);                             }
	// clang-format on

	FORCE_INLINE void buffer_copy(const uint8_t *a_data, size_t a_offset, size_t a_length)
	{
		assert(this->m_mapped_address && "Need to map the shader buffer first before copy is called");
		assert(a_data && "Data is null need to provide valid data before copy is called");

		std::memcpy(this->m_mapped_address + a_offset, a_data, a_length);
	}

	// Forbids using uploads from BufferVulkan
	using ShaderBufferCrtp<ShaderBufferVulkan>::upload;

  protected:
  private:
	uint8_t *m_mapped_address{nullptr};        //! For batch copy the buffer will be mapped once in here and unmapped once
};

using ShaderBuffer = ShaderBufferVulkan;

}        // namespace rhi

#include "rhi/vulkan/rorshader_buffer.hh"
