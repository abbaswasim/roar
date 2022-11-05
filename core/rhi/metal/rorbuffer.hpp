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
class DeviceMetal;
using Device = DeviceMetal;

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
	FORCE_INLINE void           init(rhi::Device &a_device, size_t a_size_in_bytes, rhi::ResourceStorageOption = rhi::ResourceStorageOption::shared);
	FORCE_INLINE void           init(rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes, rhi::ResourceStorageOption a_mode = rhi::ResourceStorageOption::shared);
	FORCE_INLINE void           upload(rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes);
	FORCE_INLINE void           upload(rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_offset, size_t a_length);
	FORCE_INLINE constexpr void unmap() noexcept;
	FORCE_INLINE constexpr void unmap(std::uintptr_t a_from, std::uintptr_t a_to) noexcept;
	FORCE_INLINE constexpr void bind(MTL::RenderCommandEncoder *a_cmd_encoder, rhi::ShaderType a_shader_stage, uint32_t a_index, uint32_t a_offset = 0) const noexcept;
	FORCE_INLINE constexpr void bind(MTL::ComputeCommandEncoder *a_cmd_encoder, rhi::ShaderType a_shader_stage, uint32_t a_index, uint32_t a_offset = 0) const noexcept;

	FORCE_INLINE void upload(const uint8_t *a_data_pointer, size_t a_offset, size_t a_length);
	// FORCE_INLINE constexpr auto size()               const noexcept { return this->m_buffer->length(); }

	// clang-format off
	FORCE_INLINE constexpr auto ready()              const noexcept { return this->m_ready;            }
	FORCE_INLINE constexpr auto platform_buffer()    const noexcept { return this->m_buffer;           }
	FORCE_INLINE constexpr auto storage_mode()       const noexcept { return this->m_storage_mode;     }

	FORCE_INLINE constexpr void ready(bool a_ready)                        noexcept { this->m_ready = a_ready;       }
	FORCE_INLINE           void storage_mode(rhi::ResourceStorageOption a_mode)     { this->m_storage_mode = a_mode; }
	// clang-format on

  protected:
  private:
	declare_translation_unit_vtable();

	MTL::Buffer               *m_buffer{nullptr};                                         //! API handle to buffer
	rhi::ResourceStorageOption m_storage_mode{rhi::ResourceStorageOption::shared};        //! Storage mode of the buffer, shared means no synchronisation is required
	bool                       m_ready{false};                                            //! Keeps track of whether the buffer is ready to be used or not
};

/**
 * This type of buffer has both a CPU side copy and a GPU uploaded buffer
 * Ideally the CPU side copy is also kept up to date with every update to the GPU
 * This might not be required for some GPU only side buffer for that case
 * Use BufferMetal/BufferVulkan instead
 */
template <typename _type = Static>
class ROAR_ENGINE_ITEM BufferHybrid : public BufferCrtp<_type>, public BufferMetal
{
  public:
	FORCE_INLINE               BufferHybrid()                                = default;        //! Default constructor
	FORCE_INLINE               BufferHybrid(const BufferHybrid &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               BufferHybrid(BufferHybrid &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE BufferHybrid &operator=(const BufferHybrid &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE BufferHybrid &operator=(BufferHybrid &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE virtual ~BufferHybrid() noexcept override                   = default;        //! Destructor

	FORCE_INLINE void upload(rhi::Device &a_device)
	{
		// Does both init and upload at the same time
		BufferMetal::init(a_device, this->data().data(), this->filled_size(), rhi::ResourceStorageOption::managed);        // TODO: Fix me
	}

  protected:
  private:
};

// Template deduction guide CATD for Buffer static
template <class _type>
BufferHybrid(_type) -> BufferHybrid<Static>;

// I am doing this so the rest of the system can use the CRTP implementation directly as "Buffer"
template <typename _type>
using Buffer = BufferHybrid<_type>;

}        // namespace rhi

#include "rhi/metal/rorbuffer.hh"
