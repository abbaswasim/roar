// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2021
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

#include "rhi/crtp_interfaces/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/vulkan/rorcompute_command_encoder.hpp"
#include "rhi/vulkan/rordevice.hpp"
#include "rhi/vulkan/rorrender_command_encoder.hpp"
#include "rhi/vulkan/rortexture.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "vulkan/vulkan_core.h"

#include <cassert>

// namespace rhi
// {

/*
VkBuffer create_buffer(VkDevice a_device, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, std::vector<uint32_t> a_share_indices)
{
    // TODO: Change default behaviour of sharing between transfer and graphics only
    // std::vector<uint32_t> indicies{graphics_queue_index, transfer_queue_index};

    VkBufferCreateInfo buffer_info{};

    buffer_info.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.pNext                 = nullptr;
    buffer_info.flags                 = 0;
    buffer_info.size                  = a_size;                // example: 1024 * 1024 * 2;        // 2kb
    buffer_info.usage                 = a_usage;               // example: VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
    buffer_info.sharingMode           = a_sharing_mode;        // examples: VK_SHARING_MODE_CONCURRENT, VK_SHARING_MODE_EXCLUSIVE; this has performance implications, not all resources are shared either
    buffer_info.queueFamilyIndexCount = ror::static_cast_safe<uint32_t>(a_share_indices.size());
    buffer_info.pQueueFamilyIndices   = a_share_indices.data();

    VkBuffer buffer;
    VkResult result = vkCreateBuffer(a_device, &buffer_info, vk::VkAllocator, &buffer);

    assert(result == VK_SUCCESS && "Failed to create vulkan buffer!");
    assert(buffer);

    return buffer;
}

uint32_t find_memory_type(VkPhysicalDevice a_physical_device, uint32_t a_type_filter, VkMemoryPropertyFlags a_properties)
{
    VkPhysicalDeviceMemoryProperties memory_properties{};
    vkGetPhysicalDeviceMemoryProperties(a_physical_device, &memory_properties);

    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
    {
        if (a_type_filter & (1 << i) && ((memory_properties.memoryTypes[i].propertyFlags & a_properties) == a_properties))
        {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type!");
}

VkDeviceMemory allocate_bind_buffer_memory(VkDevice a_device, VkBuffer a_buffer, VkMemoryPropertyFlags a_properties = (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
{
    VkMemoryRequirements buffer_mem_req{};
    vkGetBufferMemoryRequirements(a_device, a_buffer, &buffer_mem_req);

    VkPhysicalDevice physical_device{};
    VkDeviceMemory   buffer_memory;

    VkMemoryAllocateInfo allocation_info{};
    allocation_info.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocation_info.allocationSize  = buffer_mem_req.size;
    allocation_info.memoryTypeIndex = find_memory_type(physical_device, buffer_mem_req.memoryTypeBits, a_properties);
    allocation_info.pNext           = nullptr;

    VkResult result = vkAllocateMemory(a_device, &allocation_info, vk::VkAllocator, &buffer_memory);
    assert(result == VK_SUCCESS && "Failed to allocate vulkan buffer memory!");
    assert(buffer_memory);

    result = vkBindBufferMemory(a_device, a_buffer, buffer_memory, 0);
    assert(result == VK_SUCCESS && "Failed to bind vulkan buffer memory!");

    return buffer_memory;
}

std::pair<VkBuffer, VkDeviceMemory> create_buffer_pre_allocate(VkDevice a_device, size_t a_size, VkBufferUsageFlags a_usage, VkSharingMode a_sharing_mode, std::vector<uint32_t> a_share_indices,
                                                               VkMemoryPropertyFlags a_properties = (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
{
    auto staging_buffer        = create_buffer(a_device, a_size, a_usage, a_sharing_mode, a_share_indices);
    auto staging_buffer_memory = allocate_bind_buffer_memory(a_device, staging_buffer, a_properties);

    return std::make_pair(staging_buffer, staging_buffer_memory);
}
auto begin_single_use_cmd_buffer()
{
    VkCommandBufferAllocateInfo command_buffer_allocate_info{};
    command_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    command_buffer_allocate_info.pNext              = nullptr;
    command_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    command_buffer_allocate_info.commandPool        = this->m_transfer_command_pool;
    command_buffer_allocate_info.commandBufferCount = 1;

    VkCommandBuffer staging_command_buffer;
    VkResult        result = vkAllocateCommandBuffers(this->m_device, &command_buffer_allocate_info, &staging_command_buffer);
    assert(result == VK_SUCCESS);

    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(staging_command_buffer, &command_buffer_begin_info);

    return staging_command_buffer;
}

void end_single_use_cmd_buffer(VkCommandBuffer a_command_buffer)
{
    vkEndCommandBuffer(a_command_buffer);

    VkSubmitInfo staging_submit_info{};
    staging_submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    staging_submit_info.commandBufferCount = 1;
    staging_submit_info.pCommandBuffers    = &a_command_buffer;

    vkQueueSubmit(this->m_transfer_queue, 1, &staging_submit_info, VK_NULL_HANDLE);
    vkQueueWaitIdle(this->m_transfer_queue);        // TODO: Should be improved in the future

    vkFreeCommandBuffers(this->m_device, this->m_transfer_command_pool, 1, &a_command_buffer);
}

void copy_from_staging_buffers_to_buffers(std::vector<std::pair<VkBuffer, size_t>> &a_source, std::vector<VkBuffer> &a_destination)
{
    VkCommandBuffer staging_command_buffer = begin_single_use_cmd_buffer();

    if (a_source.size() != a_destination.size())
        ror::log_critical("Copying from different size a_source to a_destination, something won't be copied correctly");

    // TODO: Could be done in one go
    for (size_t i = 0; i < a_source.size(); ++i)
    {
        VkBufferCopy buffer_image_copy_region{};
        buffer_image_copy_region.srcOffset = 0;        // Optional
        buffer_image_copy_region.dstOffset = 0;        // Optional
        buffer_image_copy_region.size      = a_source[i].second;

        vkCmdCopyBuffer(staging_command_buffer, a_source[i].first, a_destination[i], 1, &buffer_image_copy_region);
    }

    end_single_use_cmd_buffer(staging_command_buffer);
}

void create_texture()
{
    rhi::TextureImage texture = rhi::read_texture_from_file<TextureTarget::texture_2D>("./assets/astroboy/astro_boy_uastc.ktx2", false);

    VkDevice       device{};
    VkBuffer       staging_buffer{};
    VkDeviceMemory staging_buffer_memory{};

    staging_buffer        = create_buffer(device, texture.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_CONCURRENT, {0, 1});
    staging_buffer_memory = allocate_bind_buffer_memory(device, staging_buffer);
    (void) staging_buffer_memory;

    uint8_t *texture_data;
    vkMapMemory(device, staging_buffer_memory, 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void **>(&texture_data));
    memcpy(texture_data, texture.data(), texture.size());
    vkUnmapMemory(device, staging_buffer_memory);

    auto texture_image        = create_image(texture.width(), texture.height(), texture.format(), VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, texture.mips().size());
    auto texture_image_memory = allocate_bind_image_memory(texture_image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    auto texture_image_view   = create_image_view(texture_image, texture.format(), VK_IMAGE_ASPECT_COLOR_BIT, texture.mips().size());

    std::vector<VkImage>  texture_images{texture_image};
    std::vector<VkBuffer> source_textures{staging_buffer};

    transition_image_layout(texture_image, texture.format(), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, texture.mips().size());
    copy_from_staging_buffers_to_images(source_textures, texture_images, texture);
    transition_image_layout(texture_image, texture.format(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, texture.mips().size());
    create_texture_sampler(static_cast<float32_t>(texture.mips().size()));

    // Cleanup staging buffers
    vkDestroyBuffer(device, staging_buffer, vk::VkAllocator);
    vkFreeMemory(device, staging_buffer_memory, vk::VkAllocator);

    staging_buffer        = nullptr;
    staging_buffer_memory = nullptr;
}
*/

namespace rhi
{

FORCE_INLINE constexpr VkImageType to_vulkan_texture_target(rhi::TextureTarget a_target)
{
	switch (a_target)
	{
		case rhi::TextureTarget::texture_1D:
			return VkImageType::VK_IMAGE_TYPE_1D;        // MTL::TextureType::TextureType1D;
		case rhi::TextureTarget::texture_2D:
			return VkImageType::VK_IMAGE_TYPE_2D;
		case rhi::TextureTarget::texture_3D:
			return VkImageType::VK_IMAGE_TYPE_3D;
		case rhi::TextureTarget::texture_1D_array:
			break;
			// 	return MTL::TextureType::TextureType1DArray;
		case rhi::TextureTarget::texture_2D_array:
			break;
			// return MTL::TextureType::TextureType2DArray;
		case rhi::TextureTarget::texture_2D_MS:
			break;
			// return MTL::TextureType::TextureType2DMultisample;
		case rhi::TextureTarget::texture_cube:
			break;
			// return MTL::TextureType::TextureTypeCube;
		case rhi::TextureTarget::texture_cube_array:
			break;
			// return MTL::TextureType::TextureTypeCubeArray;
		case rhi::TextureTarget::texture_2D_MS_array:
			break;
			// return MTL::TextureType::TextureType2DMultisampleArray;
	}

	return VkImageType::VK_IMAGE_TYPE_2D;
}

FORCE_INLINE constexpr VkFilter to_vulkan_texture_filter(rhi::TextureFilter a_filter)
{
	return static_cast<VkFilter>(a_filter);
}

FORCE_INLINE constexpr VkFilter to_vulkan_texture_mip_filter(rhi::TextureMipFilter a_filter)
{
	return static_cast<VkFilter>(a_filter);
}

FORCE_INLINE constexpr VkBorderColor to_vulkan_texture_border(rhi::TextureBorder a_color)
{
	return static_cast<VkBorderColor>(a_color);
}

FORCE_INLINE constexpr VkSamplerAddressMode to_vulkan_texture_address_mode(rhi::TextureAddressMode a_mode)
{
	return static_cast<VkSamplerAddressMode>(a_mode);
}

void TextureImageVulkan::bind(rhi::RenderCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) noexcept
{
	if (a_shader_stage == rhi::ShaderStage::fragment || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.fragment_texture(*this, a_index);

	if (a_shader_stage == rhi::ShaderStage::vertex || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.vertex_texture(*this, a_index);

	if (a_shader_stage == rhi::ShaderStage::tile)
		a_command_encoder.tile_texture(*this, a_index);
}

void TextureImageVulkan::bind(rhi::ComputeCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) noexcept
{
	if (a_shader_stage == rhi::ShaderStage::compute || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.texture(*this, a_index);
	else
	{
		assert(0 && "Can't bind texture image to this shader stage");
	}
}

void TextureSamplerVulkan::bind(rhi::RenderCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) noexcept
{
	if (a_shader_stage == rhi::ShaderStage::fragment || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.fragment_sampler(*this, a_index);

	if (a_shader_stage == rhi::ShaderStage::vertex || a_shader_stage == rhi::ShaderStage::vertex_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.vertex_sampler(*this, a_index);

	if (a_shader_stage == rhi::ShaderStage::tile)
		a_command_encoder.tile_sampler(*this, a_index);
}

void TextureSamplerVulkan::bind(rhi::ComputeCommandEncoder &a_command_encoder, rhi::ShaderStage a_shader_stage, uint32_t a_index) noexcept
{
	if (a_shader_stage == rhi::ShaderStage::compute || a_shader_stage == rhi::ShaderStage::compute_vertex || a_shader_stage == rhi::ShaderStage::compute_fragment || a_shader_stage == rhi::ShaderStage::compute_vertex_fragment)
		a_command_encoder.sampler(*this, a_index);
	else
	{
		assert(0 && "Can't bind texture image to this shader stage");
	}
}

void TextureImageVulkan::upload(rhi::Device &a_device)
{
	(void) a_device;
	// MTL::Device            *device             = a_device.platform_device();
	// MTL::TextureDescriptor *texture_descriptor = MTL::TextureDescriptor::alloc()->init();
	// auto                    tex_bpp            = this->bytes_per_pixel();
	// uint32_t                bytes_per_row      = tex_bpp * this->width();
	// MTL::Origin             texture_origin{0, 0, 0};
	// MTL::Size               size{this->width(), this->height(), this->depth()};

	// assert(device);

	if (this->width() == 0 || this->height() == 0)
	{
		ror::log_critical("Uploading a texture of zero width or height texture name = {}, width x height=({}, {})",
		                  this->name().c_str(), this->width(), this->height());
		return;
	}

	// texture_descriptor->setWidth(this->width());
	// texture_descriptor->setHeight(this->height());
	// texture_descriptor->setPixelFormat(to_vulkan_pixelformat(this->format()));
	// texture_descriptor->setTextureType(to_vulkan_texture_target(this->target()));
	// texture_descriptor->setMipmapLevelCount(this->mips().size());
	// texture_descriptor->setUsage(MTL::TextureUsageUnknown);

	bool needs_upload = true;

	// if (this->usage() == rhi::TextureUsage::shader_read || this->usage() == rhi::TextureUsage::render_target_read)
	// 	texture_descriptor->setUsage(MTL::TextureUsageShaderRead);

	if (this->usage() == rhi::TextureUsage::render_target || this->usage() == rhi::TextureUsage::render_target_read)
	{
		// texture_descriptor->setUsage(texture_descriptor->usage() | MTL::TextureUsageRenderTarget);
		needs_upload = false;
	}

	if (this->usage() == rhi::TextureUsage::shader_write)
	{
		// texture_descriptor->setUsage(texture_descriptor->usage() | MTL::TextureUsageShaderWrite);
		needs_upload = false;
	}

	// texture_descriptor->setStorageMode(MTL::StorageModePrivate);

	// if (this->m_texture)
		// this->m_texture->release();

	// this->m_texture = device->newTexture(texture_descriptor);

	if (needs_upload)
	{
		assert(this->data());
		assert(this->size() == this->width() * this->height() * this->bytes_per_pixel());

		// MTL::CommandQueue       *queue                = a_device.platform_queue();
		// MTL::Buffer             *source_buffer        = device->newBuffer(this->data(), this->size(), MTL::ResourceStorageModeShared);
		// MTL::CommandBuffer      *command_buffer       = queue->commandBuffer();
		// MTL::BlitCommandEncoder *blit_command_encoder = command_buffer->blitCommandEncoder();

		// assert(queue);

		// blit_command_encoder->copyFromBuffer(source_buffer, 0, bytes_per_row, this->width() * this->height() * this->bytes_per_pixel(), size, this->m_texture, 0, 0, texture_origin);
		// blit_command_encoder->endEncoding();

		// command_buffer->addCompletedHandler([this, source_buffer](MTL::CommandBuffer *) {this->ready(true); source_buffer->release(); });
		// command_buffer->commit();
	}
	else
		this->ready(true);

	// texture_descriptor->release();
}

void TextureSamplerVulkan::upload(rhi::Device &a_device)
{
	(void) a_device;
	// MTL::Device            *device             = a_device.platform_device();
	// MTL::SamplerDescriptor *sampler_descriptor = MTL::SamplerDescriptor::alloc()->init();

	// sampler_descriptor->setSAddressMode(to_vulkan_texture_address_mode(this->wrap_s()));
	// sampler_descriptor->setTAddressMode(to_vulkan_texture_address_mode(this->wrap_t()));
	// sampler_descriptor->setRAddressMode(to_vulkan_texture_address_mode(this->wrap_u()));
	// sampler_descriptor->setMinFilter(to_vulkan_texture_filter(this->min_filter()));
	// sampler_descriptor->setMagFilter(to_vulkan_texture_filter(this->mag_filter()));
	// sampler_descriptor->setMipFilter(to_vulkan_texture_mip_filter(this->mip_mode()));
	// sampler_descriptor->setBorderColor(to_vulkan_texture_border(this->border_color()));

	// this->m_sampler = device->newSamplerState(sampler_descriptor);

	// sampler_descriptor->release();
}

define_translation_unit_vtable(TextureImageVulkan)
{}

define_translation_unit_vtable(TextureSamplerVulkan)
{}

define_translation_unit_vtable(TextureVulkan)
{}

}        // namespace rhi
