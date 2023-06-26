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

#include "rhi/crtp_interfaces/rorbuffer.hpp"
#include "rhi/rorrhi_macros.hpp"

#include "rhi/vulkan/rorvulkan_common.hpp"

namespace rhi
{

class DeviceVulkan;
using Device = DeviceVulkan;

declare_rhi_render_type(RenderCommandEncoder);
declare_rhi_render_type(ComputeCommandEncoder);

class ROAR_ENGINE_ITEM BufferVulkan
{
  public:
	FORCE_INLINE               BufferVulkan()                                = default;        //! Default constructor
	FORCE_INLINE               BufferVulkan(const BufferVulkan &a_other)     = default;        //! Copy constructor
	FORCE_INLINE               BufferVulkan(BufferVulkan &&a_other) noexcept = default;        //! Move constructor
	FORCE_INLINE BufferVulkan &operator=(const BufferVulkan &a_other)        = default;        //! Copy assignment operator
	FORCE_INLINE BufferVulkan &operator=(BufferVulkan &&a_other) noexcept    = default;        //! Move assignment operator
	FORCE_INLINE virtual ~BufferVulkan() noexcept                            = default;        //! Destructor

	[[nodiscard]] FORCE_INLINE constexpr auto map() noexcept;

	FORCE_INLINE void           release();
	FORCE_INLINE void           resize(rhi::Device &a_device, size_t a_length);
	FORCE_INLINE void           init(rhi::Device &a_device, size_t a_size_in_bytes, rhi::ResourceStorageOption = rhi::ResourceStorageOption::shared);
	FORCE_INLINE void           init(rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes, rhi::ResourceStorageOption a_mode = rhi::ResourceStorageOption::shared);
	FORCE_INLINE void           upload(rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes);
	FORCE_INLINE void           upload(rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_offset, size_t a_length);
	FORCE_INLINE void           upload(const uint8_t *a_data_pointer, size_t a_offset, size_t a_length);
	FORCE_INLINE constexpr void unmap() noexcept;
	FORCE_INLINE constexpr void unmap(std::uintptr_t a_from, std::uintptr_t a_to) noexcept;

	void bind(rhi::RenderCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uintptr_t a_offset, uint32_t a_index) noexcept;
	void bind(rhi::ComputeCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uintptr_t a_offset, uint32_t a_index) noexcept;

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

	VkBuffer                  *m_buffer{nullptr};                                         //! API handle to buffer
	rhi::ResourceStorageOption m_storage_mode{rhi::ResourceStorageOption::shared};        //! Storage mode of the buffer, shared means no synchronisation is required
	bool                       m_ready{false};                                            //! Keeps track of whether the buffer is ready to be used or not
};

/**
 * This type of buffer has both a CPU side copy and a GPU uploaded buffer
 * Ideally the CPU side copy is also kept up to date with every update to the GPU
 * This might not be required for some GPU only side buffer for that case
 * Use BufferVulkan/BufferVulkan instead
 */
template <typename _type = Static>
class ROAR_ENGINE_ITEM BufferHybrid : public BufferCrtp<_type>, public BufferVulkan
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
		BufferVulkan::init(a_device, this->data().data(), this->filled_size(), rhi::ResourceStorageOption::managed);        // TODO: Fix me
	}

	FORCE_INLINE void upload_partial(size_t a_offset, size_t a_length)
	{
		// Since both the BufferCrtp and BufferVulkan are of the same size, I use one offeset here and one inside BufferVulkan::upload
		BufferVulkan::upload(this->data().data() + a_offset, a_offset, a_length);
	}

  protected:
  private:
};

// Template deduction guide CATD for Buffer static
template <class _type>
BufferHybrid(_type) -> BufferHybrid<Static>;

// I am doing this so the rest of the system can use the CRTP implementation directly as "Buffer"
using Buffer = BufferVulkan;

}        // namespace rhi

#include "rhi/vulkan/rorbuffer.hh"
