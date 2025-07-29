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

#include "foundation/rormacros.hpp"
#include "rhi/crtp_interfaces/rorbuffer.hpp"
#include "rhi/rorrhi_macros.hpp"
#include "rhi/rortypes.hpp"
#include <Metal/MTLBuffer.hpp>
#include <Metal/MTLComputeCommandEncoder.hpp>
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <cstddef>

namespace rhi
{

declare_rhi_render_type(RenderCommandEncoder);
declare_rhi_render_type(ComputeCommandEncoder);

class ROAR_ENGINE_ITEM BufferMetal
{
  public:
	FORCE_INLINE              BufferMetal()                               = default;        //! Default constructor
	FORCE_INLINE              BufferMetal(const BufferMetal &a_other)     = default;        //! Copy constructor
	FORCE_INLINE              BufferMetal(BufferMetal &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE BufferMetal &operator=(const BufferMetal &a_other)       = default;        //! Copy assignment operator
	FORCE_INLINE BufferMetal &operator=(BufferMetal &&a_other) noexcept   = default;        //! Move assignment operator
	FORCE_INLINE virtual ~BufferMetal() noexcept                          = default;        //! Destructor

	[[nodiscard]] FORCE_INLINE constexpr auto map() noexcept;

	FORCE_INLINE void           release();
	FORCE_INLINE void           resize(const rhi::Device &a_device, size_t a_length);
	FORCE_INLINE void           init(const rhi::Device &a_device, size_t a_size_in_bytes, rhi::ResourceStorageOption = rhi::ResourceStorageOption::shared);
	FORCE_INLINE void           init(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes, rhi::ResourceStorageOption a_mode = rhi::ResourceStorageOption::shared);
	FORCE_INLINE void           upload(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes);
	FORCE_INLINE void           upload(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_offset, size_t a_length);
	FORCE_INLINE void           upload(const uint8_t *a_data_pointer, size_t a_offset, size_t a_length);
	FORCE_INLINE constexpr void unmap() noexcept;

	void bind(rhi::RenderCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uintptr_t a_offset, uint32_t a_index) noexcept;
	void bind(rhi::ComputeCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uintptr_t a_offset, uint32_t a_index) noexcept;

	// FORCE_INLINE constexpr auto size()               const noexcept { return this->m_buffer->length(); }

	// clang-format off
	FORCE_INLINE constexpr auto ready()              const noexcept { return this->m_ready;            }
	FORCE_INLINE constexpr auto platform_buffer()    const noexcept { return this->m_buffer;           }
	FORCE_INLINE constexpr auto storage_mode()       const noexcept { return this->m_storage_mode;     }
	FORCE_INLINE constexpr size_t size_in_bytes()    const noexcept;

	FORCE_INLINE constexpr void ready(bool a_ready)                        noexcept { this->m_ready = a_ready;       }
	FORCE_INLINE           void storage_mode(rhi::ResourceStorageOption a_mode)     { this->m_storage_mode = a_mode; }
	// clang-format on

  protected:
  private:
	declare_translation_unit_vtable();
	FORCE_INLINE constexpr void unmap(std::uintptr_t a_from, std::uintptr_t a_to) noexcept;

	MTL::Buffer               *m_buffer{nullptr};                                         //! API handle to buffer
	rhi::ResourceStorageOption m_storage_mode{rhi::ResourceStorageOption::shared};        //! Storage mode of the buffer, shared means no synchronisation is required
	bool                       m_ready{false};                                            //! Keeps track of whether the buffer is ready to be used or not
};

// Template deduction guide CATD for Buffer static
template <class _type>
BufferHybrid(_type) -> BufferHybrid<Static>;

// I am doing this so the rest of the system can use the CRTP implementation directly as "Buffer"
using Buffer = BufferMetal;

}        // namespace rhi

#include "rhi/metal/rorbuffer.hh"
