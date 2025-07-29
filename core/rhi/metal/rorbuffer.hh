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

#include "foundation/rormacros.hpp"
#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/metal/rorbuffer.hpp"
#include "rhi/metal/rordevice.hpp"
#include "rhi/metal/rormetal_common.hpp"
#include "rhi/rortypes.hpp"
#include <Metal/MTLRenderCommandEncoder.hpp>
#include <cassert>

namespace rhi
{

void BufferMetal::release()
{
	if (this->m_buffer)
		this->m_buffer->release();
}

FORCE_INLINE void BufferMetal::init(const rhi::Device &a_device, size_t a_size_in_bytes, rhi::ResourceStorageOption a_mode)
{
	if (a_size_in_bytes == 0)
		ror::log_warn("Creating a buffer of size {}", a_size_in_bytes);

	assert(this->m_buffer == nullptr && "Calling init on already created buffer");

	this->storage_mode(a_mode);

	MTL::Device *device = a_device.platform_device();
	this->m_buffer      = device->newBuffer(std::max(1ul, a_size_in_bytes), rhi::to_metal_resource_option(this->storage_mode()));
}

FORCE_INLINE void BufferMetal::init(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes, rhi::ResourceStorageOption a_mode)
{
	this->init(a_device, a_size_in_bytes, a_mode);
	this->upload(a_device, a_data_pointer, a_size_in_bytes);
}

FORCE_INLINE constexpr void BufferMetal::unmap(std::uintptr_t a_offset, std::uintptr_t a_length) noexcept
{
	if (this->storage_mode() == rhi::ResourceStorageOption::managed)
	{
		this->m_buffer->didModifyRange(NS::Range::Make(a_offset, a_length));        // only valid for Managed mode, shared and other doesn't require this
	}
	else if (this->storage_mode() == rhi::ResourceStorageOption::shared)
	{}        // Do nothing already unmapped
	else
		ror::log_critical("Can't unmap private or memory less buffer");
}

FORCE_INLINE constexpr auto BufferMetal::map() noexcept
{
	if (this->storage_mode() == rhi::ResourceStorageOption::shared ||
	    this->storage_mode() == rhi::ResourceStorageOption::managed)
	{
		return static_cast<uint8_t *>(this->m_buffer->contents());
	}
	else
	{
		ror::log_critical("Can't map private or memory less buffer");
		return static_cast<uint8_t *>(nullptr);
	}
}

FORCE_INLINE constexpr void BufferMetal::unmap() noexcept
{
	this->unmap(0, this->m_buffer->length());
}

FORCE_INLINE void BufferMetal::resize(const rhi::Device &a_device, size_t a_length)
{
	if (this->m_buffer->length() > a_length)
		return;
	else
	{
		// Lets adjust the size
		this->m_buffer->autorelease();        // Showing interest for this to be released somewhere in the future, unlike immidiate release()
		this->m_buffer = nullptr;
		this->init(a_device, a_length + (a_length / 4), this->m_storage_mode);        // Append an extra quarter of what is required for future
	}
}

void BufferMetal::upload(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes)
{
	/*
	For buffers in the device address space, align the offset to the data type consumed by the vertex function (which is always less than or equal to 16 bytes).
	For buffers in the constant address space, align the offset to 256 bytes in macOS. In iOS, align the offset to the maximum of either the data type consumed
	by the vertex function, or 4 bytes. A 16-byte alignment is safe in iOS if you don't need to consider the data type.
	*/

	this->upload(a_device, a_data_pointer, 0, a_size_in_bytes);
}

void BufferMetal::upload(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_offset, size_t a_length)
{
	// Some sanity checks first
	assert(this->m_buffer && "Called upload on uninitialized buffer, call init() first");
	assert(a_offset <= a_length);
	assert(a_offset <= this->m_buffer->length());
	assert(this->m_buffer->length() >= a_length && "Not enough space in the buffer being copied into");
	assert(this->m_buffer->length() >= a_offset + a_length && "Not enough space in the buffer being copied into");

	if (this->storage_mode() == rhi::ResourceStorageOption::exclusive)
	{
		MTL::Device             *device               = a_device.platform_device();
		MTL::CommandQueue       *queue                = a_device.platform_queue();
		MTL::Buffer             *source_buffer        = device->newBuffer(a_data_pointer + a_offset, a_length, MTL::ResourceStorageModeShared);
		MTL::CommandBuffer      *command_buffer       = queue->commandBuffer();
		MTL::BlitCommandEncoder *blit_command_encoder = command_buffer->blitCommandEncoder();

		blit_command_encoder->copyFromBuffer(source_buffer, a_offset, this->m_buffer, a_offset, a_length);
		blit_command_encoder->endEncoding();

		command_buffer->addCompletedHandler([this](MTL::CommandBuffer *) { this->ready(true); });
		command_buffer->commit();
	}
	else if (this->storage_mode() == rhi::ResourceStorageOption::shared ||
	         this->storage_mode() == rhi::ResourceStorageOption::managed)
	{
		std::memcpy(this->map() + a_offset, a_data_pointer, a_length);
		this->unmap(a_offset, a_length);
		this->ready(true);
	}
	else
	{
		assert(this->storage_mode() == rhi::ResourceStorageOption::memory_less);
		assert(0 && "Can't upload into a memory less buffer");
	}
}

void BufferMetal::upload(const uint8_t *a_data_pointer, size_t a_offset, size_t a_length)
{
	// Some sanity checks first
	assert(a_offset < this->m_buffer->length());
	assert(this->m_buffer->length() >= a_length && "Not enough space in the buffer being copied into");
	assert(this->m_buffer->length() >= a_offset + a_length && "Not enough space in the buffer being copied into");

	assert(this->storage_mode() != rhi::ResourceStorageOption::exclusive && "Can't update private/exclusive buffer data, its expensive, if really needed call upload(a_device) instead");
	assert(this->storage_mode() != rhi::ResourceStorageOption::memory_less && "Can't update memory less buffer data");

	std::memcpy(this->map() + a_offset, a_data_pointer, a_length);
	this->unmap(a_offset, a_length);
}

FORCE_INLINE constexpr size_t BufferMetal::size_in_bytes() const noexcept
{
	if (this->m_buffer)
		return ror::static_cast_safe<size_t>(this->m_buffer->length());
	else
		return static_cast<size_t>(0);
}

}        // namespace rhi
