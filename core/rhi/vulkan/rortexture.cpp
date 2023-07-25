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

#include "foundation/rorutilities.hpp"
#include "profiling/rorlog.hpp"
#include "rhi/crtp_interfaces/rortexture.hpp"
#include "rhi/rortypes.hpp"
#include "rhi/vulkan/rorcompute_command_encoder.hpp"
#include "rhi/vulkan/rordevice.hpp"
#include "rhi/vulkan/rorrender_command_encoder.hpp"
#include "rhi/vulkan/rortexture.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"
#include <cassert>

namespace rhi
{

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

std::vector<VkBufferImageCopy> mipmaps_to_buffer_image_copy_regions(TextureImageVulkan &a_texture)
{
	std::vector<VkBufferImageCopy> buffer_image_copy_regions{};

	for (uint32_t j = 0; j < a_texture.mips().size(); j++)
	{
		VkBufferImageCopy buffer_image_copy_region{};

		buffer_image_copy_region.bufferRowLength   = 0;
		buffer_image_copy_region.bufferImageHeight = 0;

		buffer_image_copy_region.bufferOffset                    = a_texture.mips()[j].m_offset;
		buffer_image_copy_region.imageSubresource.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		buffer_image_copy_region.imageSubresource.mipLevel       = j;
		buffer_image_copy_region.imageSubresource.baseArrayLayer = 0;
		buffer_image_copy_region.imageSubresource.layerCount     = 1;
		buffer_image_copy_region.imageOffset                     = {0, 0, 0};
		buffer_image_copy_region.imageExtent                     = {a_texture.mips()[j].m_width, a_texture.mips()[j].m_height, 1};

		buffer_image_copy_regions.push_back(buffer_image_copy_region);
	}

	return buffer_image_copy_regions;
}

void TextureImageVulkan::upload(rhi::Device &a_device)
{
	if (this->width() == 0 || this->height() == 0)
	{
		ror::log_critical("Uploading a texture of zero width or height texture name = {}, width x height=({}, {})",
		                  this->name().c_str(), this->width(), this->height());
		return;
	}

	auto                              device               = a_device.platform_device();
	auto                              transfer_queue       = a_device.platform_transfer_queue();
	std::mutex                       &transfer_queue_mutex = a_device.platform_transfer_queue_mutex();
	VkBufferUsageFlags                usage                = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VkSharingMode                     mode                 = VK_SHARING_MODE_EXCLUSIVE;        // This is queue sharing mode not CPU vs GPU
	std::vector<uint32_t>             queue_family_indices{a_device.platform_graphics_queue_index(), a_device.platform_transfer_queue_index()};
	VkPhysicalDeviceMemoryProperties2 memory_properties = a_device.memory_properties();
	VkMemoryPropertyFlags             properties        = rhi::to_vulkan_resource_option(rhi::ResourceStorageOption::exclusive);
	VkBuffer                          staging_buffer{};
	VkDeviceMemory                    staging_memory{};
	auto                              copy_regions = mipmaps_to_buffer_image_copy_regions(*this);
	auto                              is_depth     = is_pixel_format_depth_format(this->format());

	VkImageUsageFlags usage_flags{};
	VkImageLayout     final_layout{};

	bool needs_upload = true;

	if (this->usage() == rhi::TextureUsage::shader_read)
	{
		final_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		usage_flags  = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
	}

	if (this->usage() == rhi::TextureUsage::render_target || this->usage() == rhi::TextureUsage::shader_write || this->usage() == rhi::TextureUsage::render_target_read)
	{
		// Not using these in any way at the moment
		// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL = 2,
		// VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL = 3,
		// VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL = 4,
		final_layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Not using these in any way at the moment
		// VK_IMAGE_USAGE_TRANSFER_SRC_BIT             = 0x00000001,
		// VK_IMAGE_USAGE_TRANSFER_DST_BIT             = 0x00000002,
		// VK_IMAGE_USAGE_STORAGE_BIT                  = 0x00000008,
		// VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT = 0x00000020,

		ror::log_warn("Using render target mode, but using too many flags");
		usage_flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;            // Not sure which one is correct
		usage_flags |= VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;        // Should I also be adding these?

		if (is_depth)
			usage_flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		else
			usage_flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;        // Not sure which one is correct

		needs_upload = false;
	}

	// TODO: Convert target to VK_IMAGE_TYPE_2D by using to_vulkan_texture_target(this->target()) at the moment it returns imageviewtype
	if (this->m_image == nullptr && this->m_image_memory == nullptr)
		vk_create_image_with_memory(device, this->m_image, this->width(), this->height(), this->depth(), to_vulkan_pixelformat(this->format()),
		                            ror::static_cast_safe<uint32_t>(this->mips().size()), usage_flags,
		                            this->m_image_memory, memory_properties.memoryProperties, properties,
		                            VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL);
	else
		ror::log_warn("Recreating image skipping");

	VkImageAspectFlags aspect_flags{VK_IMAGE_ASPECT_COLOR_BIT};

	if (is_depth)
		aspect_flags = VK_IMAGE_ASPECT_DEPTH_BIT;

	if (this->m_image_view == nullptr)
		this->m_image_view = vk_create_image_view(device, this->m_image, to_vulkan_pixelformat(this->format()), ror::static_cast_safe<uint32_t>(this->mips().size()), aspect_flags);

	if (needs_upload)
	{
		assert(this->data());
		assert(this->size() == this->width() * this->height() * this->bytes_per_pixel());

		properties = rhi::to_vulkan_resource_option(rhi::ResourceStorageOption::managed);        // TODO: Confirm this from vulkan tutorial
		vk_create_buffer_with_memory(device, staging_buffer, std::max(1ull, this->size()), usage, mode, queue_family_indices, staging_memory, memory_properties.memoryProperties, properties);

		auto texture_data = vk_map_memory(device, staging_memory);
		memcpy(texture_data, this->data(), this->size());
		vk_unmap_memory(device, staging_memory);

		std::vector<VkImage>  texture_images{this->m_image};
		std::vector<VkBuffer> source_textures{staging_buffer};

		VkCommandPool command_pool = vk_create_command_pools(device, a_device.platform_transfer_queue_index(), 0);

		vk_transition_image_layout(device, command_pool, transfer_queue, this->m_image, ror::static_cast_safe<uint32_t>(this->mips().size()), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &transfer_queue_mutex);
		vk_copy_staging_buffer_to_image(device, transfer_queue, command_pool, staging_buffer, this->m_image, copy_regions, &transfer_queue_mutex);
		vk_transition_image_layout(device, command_pool, transfer_queue, this->m_image, ror::static_cast_safe<uint32_t>(this->mips().size()), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, final_layout, &transfer_queue_mutex);

		// Cleanup staging buffers
		vk_destroy_buffer(device, staging_buffer);
		vk_destroy_memory(device, staging_memory);
		vk_destroy_command_pools(device, command_pool);
	}

	this->ready(true);
}

void TextureSamplerVulkan::upload(rhi::Device &a_device)
{
	auto device   = a_device.platform_device();
	auto mip_mode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	if (this->mip_mode() != rhi::TextureMipFilter::not_mipmapped)
		mip_mode = to_vulkan_texture_mip_filter(this->mip_mode());

	this->m_sampler = vk_create_image_sampler(device,
	                                          to_vulkan_texture_address_mode(this->wrap_s()),
	                                          to_vulkan_texture_address_mode(this->wrap_t()),
	                                          to_vulkan_texture_address_mode(this->wrap_u()),
	                                          to_vulkan_texture_filter(this->min_filter()),
	                                          to_vulkan_texture_filter(this->mag_filter()),
	                                          mip_mode,
	                                          to_vulkan_texture_border(this->border_color()));
}

define_translation_unit_vtable(TextureImageVulkan)
{}

define_translation_unit_vtable(TextureSamplerVulkan)
{}

define_translation_unit_vtable(TextureVulkan)
{}

}        // namespace rhi
