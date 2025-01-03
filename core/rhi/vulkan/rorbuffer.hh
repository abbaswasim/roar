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

#include "profiling/rorlog.hpp"
#include "rhi/vulkan/rorbuffer.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <cstring>

namespace rhi
{
void BufferVulkan::release()
{
	vk_destroy_buffer(this->m_device, this->m_buffer);
}

FORCE_INLINE void BufferVulkan::init(const rhi::Device &a_device, size_t a_size_in_bytes, rhi::ResourceStorageOption a_mode)
{
	if (a_size_in_bytes == 0)
		ror::log_warn("Creating a buffer of size {}", a_size_in_bytes);

	assert(this->m_buffer == nullptr && "Calling init on already created buffer");

	this->buffer_size(a_size_in_bytes);
	this->storage_mode(a_mode);

	this->m_device = a_device.platform_device();

	// TODO: Abstract these out
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT |
	                           VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

	VkSharingMode                     mode = VK_SHARING_MODE_EXCLUSIVE;                                                                                                                         // VK_SHARING_MODE_CONCURRENT,
	std::vector<uint32_t>             queue_family_indices{a_device.platform_graphics_queue_index(), a_device.platform_compute_queue_index(), a_device.platform_transfer_queue_index()};        // TODO: is it too many?
	VkPhysicalDeviceMemoryProperties2 memory_properties = a_device.memory_properties();
	VkMemoryPropertyFlags             properties        = rhi::to_vulkan_resource_option(a_mode);

	vk_create_buffer_with_memory(this->m_device, this->m_buffer, std::max(1ul, a_size_in_bytes), usage, mode, queue_family_indices, this->m_memory, memory_properties.memoryProperties, properties);
}

FORCE_INLINE void BufferVulkan::init(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes, rhi::ResourceStorageOption a_mode)
{
	this->init(a_device, a_size_in_bytes, a_mode);
	this->upload(a_device, a_data_pointer, a_size_in_bytes);
}

FORCE_INLINE constexpr auto BufferVulkan::map() noexcept
{
	if (this->storage_mode() == rhi::ResourceStorageOption::shared ||
	    this->storage_mode() == rhi::ResourceStorageOption::managed)
	{
		return vk_map_memory(this->m_device, this->m_memory);
	}
	else
	{
		ror::log_critical("Can't map private or memory less buffer");
		return static_cast<uint8_t *>(nullptr);
	}
}

FORCE_INLINE constexpr void BufferVulkan::unmap() noexcept
{
	if (this->storage_mode() == rhi::ResourceStorageOption::shared ||
	    this->storage_mode() == rhi::ResourceStorageOption::managed)
	{
		vk_unmap_memory(this->m_device, this->m_memory);
	}
	else
	{
		ror::log_critical("Can't unmap private or memory less buffer");
	}
}

FORCE_INLINE void BufferVulkan::resize(const rhi::Device &a_device, size_t a_length)
{
	if (this->buffer_size() > a_length)
		return;
	else
	{
		// Lets adjust the size
		vk_destroy_buffer(a_device.platform_device(), this->m_buffer);
		ror::log_critical("Destroying buffer that might be in use, need to find a way to add it to some list that will be deleted later");
		this->init(a_device, a_length + (a_length / 4), this->m_storage_mode);        // Append an extra quarter of what is required for future
	}
}

void BufferVulkan::upload(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_size_in_bytes)
{
	/*
	For buffers in the device address space, align the offset to the data type consumed by the vertex function (which is always less than or equal to 16 bytes).
	For buffers in the constant address space, align the offset to 256 bytes in macOS. In iOS, align the offset to the maximum of either the data type consumed
	by the vertex function, or 4 bytes. A 16-byte alignment is safe in iOS if you don't need to consider the data type.
	*/

	this->upload(a_device, a_data_pointer, 0, a_size_in_bytes);
}

void BufferVulkan::upload(const rhi::Device &a_device, const uint8_t *a_data_pointer, size_t a_offset, size_t a_length)
{
	// Some sanity checks first
	assert(this->m_buffer && "Called upload on uninitialized buffer, call init() first");
	assert(a_offset <= a_length);
	assert(a_offset <= this->buffer_size());
	assert(this->buffer_size() >= a_length && "Not enough space in the buffer being copied into");
	assert(this->buffer_size() >= a_offset + a_length && "Not enough space in the buffer being copied into");

	if (this->storage_mode() == rhi::ResourceStorageOption::exclusive)
	{
		VkDevice                          device               = a_device.platform_device();
		VkQueue                           transfer_queue       = a_device.platform_transfer_queue();
		std::mutex                       &transfer_queue_mutex = a_device.platform_transfer_queue_mutex();
		VkCommandPool                     command_pool         = vk_create_command_pools(device, a_device.platform_transfer_queue_index(), VK_COMMAND_POOL_CREATE_TRANSIENT_BIT);
		VkBuffer                          staging_buffer{nullptr};
		VkDeviceMemory                    staging_memory{nullptr};
		VkBufferCopy                      buffer_buffer_copy_region{};
		VkBufferUsageFlags                usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		VkSharingMode                     mode  = VK_SHARING_MODE_CONCURRENT;        // VK_SHARING_MODE_EXCLUSIVE,
		std::vector<uint32_t>             queue_family_indices{a_device.platform_transfer_queue_index()};
		VkPhysicalDeviceMemoryProperties2 memory_properties = a_device.memory_properties();
		VkMemoryPropertyFlags             properties        = rhi::to_vulkan_resource_option(rhi::ResourceStorageOption::managed);

		// Create buffer and copy my data to it
		vk_create_buffer_with_memory(this->m_device, this->m_buffer, a_length, usage, mode, queue_family_indices, staging_memory, memory_properties.memoryProperties, properties);
		auto *data = vk_map_memory(device, staging_memory);
		memcpy(data, a_data_pointer, a_length);
		vk_unmap_memory(device, staging_memory);

		buffer_buffer_copy_region.srcOffset = a_offset;        // Optional
		buffer_buffer_copy_region.dstOffset = a_offset;        // Optional
		buffer_buffer_copy_region.size      = a_length;

		VkCommandBuffer staging_command_buffer = vk_begin_single_use_command_buffer(device, command_pool);

		// Now blit it into the GPU resident buffer
		vkCmdCopyBuffer(staging_command_buffer, staging_buffer, this->m_buffer, 1, &buffer_buffer_copy_region);

		// NOTE: there is possibility of waiting for fence while these uploads happen but it doesn't work in this architecture, thats why this command does waitIdle
		vk_end_single_use_command_buffer_and_wait(device, staging_command_buffer, transfer_queue, command_pool, &transfer_queue_mutex);

		vk_destroy_memory(device, staging_memory);
		vk_destroy_buffer(device, staging_buffer);
		vk_destroy_command_pools(device, command_pool);

		this->ready(true);
	}
	else if (this->storage_mode() == rhi::ResourceStorageOption::shared ||
	         this->storage_mode() == rhi::ResourceStorageOption::managed)
	{
		std::memcpy(this->map() + a_offset, a_data_pointer, a_length);
		this->unmap();
		this->ready(true);
	}
	else
	{
		assert(this->storage_mode() == rhi::ResourceStorageOption::memory_less);
		assert(0 && "Can't upload into a memory less buffer");
	}
}

void BufferVulkan::upload(const uint8_t *a_data_pointer, size_t a_offset, size_t a_length)
{
	// Some sanity checks first
	assert(a_offset < this->buffer_size());
	assert(this->buffer_size() >= a_length && "Not enough space in the buffer being copied into");
	assert(this->buffer_size() >= a_offset + a_length && "Not enough space in the buffer being copied into");

	assert(this->storage_mode() != rhi::ResourceStorageOption::exclusive && "Can't update private/exclusive buffer data, its expensive, if really needed call upload(a_device) instead");
	assert(this->storage_mode() != rhi::ResourceStorageOption::memory_less && "Can't update memory less buffer data");

	std::memcpy(this->map() + a_offset, a_data_pointer, a_length);
	this->unmap();
}

}        // namespace rhi
