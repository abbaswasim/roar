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
#include "rhi/vulkan/rortexture.hpp"

namespace rhi
{

void TextureImageVulkan::upload(std::any a_device)
{
	(void) a_device;
	ror::log_critical("Uploading texture to Vulkan");
}

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

void TextureSamplerVulkan::upload(rhi::Device &a_device)
{
	(void) a_device;	
}

define_translation_unit_vtable(TextureImageVulkan)
{}

define_translation_unit_vtable(TextureSamplerVulkan)
{}

define_translation_unit_vtable(TextureVulkan)
{}

}        // namespace rhi
