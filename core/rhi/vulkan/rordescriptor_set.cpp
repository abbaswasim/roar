// Roar Source Code
// Wasim Abbas
// http://www.waZim.com
// Copyright (c) 2024
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

#include "rhi/vulkan/rordescriptor_pool.hpp"
#include "rhi/vulkan/rordescriptor_set.hpp"
#include "rhi/vulkan/rorvulkan_common.hpp"
#include "rhi/vulkan/rorvulkan_utils.hpp"

namespace rhi
{

void DescriptorSet::push_binding(uint32_t a_binding, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags)
{
	VkDescriptorSetLayoutBinding descriptor_set_layout_binding = vk_create_descriptor_set_layout_binding(a_binding, a_type, a_stage_flags);

	this->m_bindings.push_back(descriptor_set_layout_binding);
}

void DescriptorSet::push_binding(uint32_t a_binding, VkDescriptorType a_type, VkDescriptorImageInfo *a_image_info, VkDescriptorBufferInfo *a_buffer_info)
{
	VkWriteDescriptorSet write_descriptor_set = vk_create_write_descriptor_set(nullptr, a_binding, a_type, a_image_info, a_buffer_info);

	this->m_writes.push_back(write_descriptor_set);
}

void DescriptorSet::push_binding(uint32_t a_binding, VkDescriptorImageInfo *a_image_info, VkDescriptorBufferInfo *a_buffer_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags)
{
	this->push_binding(a_binding, a_type, a_stage_flags);
	this->push_binding(a_binding, a_type, a_image_info, a_buffer_info);
}

void DescriptorSet::push_image(uint32_t a_binding, VkDescriptorImageInfo *a_image_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags)
{
	this->push_binding(a_binding, a_image_info, nullptr, a_type, a_stage_flags);
}

void DescriptorSet::push_buffer(uint32_t a_binding, VkDescriptorBufferInfo *a_buffer_info, VkDescriptorType a_type, VkShaderStageFlags a_stage_flags)
{
	this->push_binding(a_binding, nullptr, a_buffer_info, a_type, a_stage_flags);
}

VkDescriptorSetLayout DescriptorSet::allocate(const VkDevice a_device, DescriptorSetLayoutCache &a_layout_cache, DescriptorPool &a_pool)
{
	auto descriptor_layout = a_layout_cache.make_layout(a_device, this->m_bindings);
	this->m_handle         = a_pool.allocate(a_device, descriptor_layout);

	this->update_writes();        // NOTE: This might or might not do much depending on which push_binding overload is used and hence would need to be called later separately

	return descriptor_layout;
}

void DescriptorSet::reset_writes()
{
	this->m_writes.clear();
}

void DescriptorSet::update_writes()
{
	for (VkWriteDescriptorSet &write : this->m_writes)
		write.dstSet = this->m_handle;
}

void DescriptorSet::update(const VkDevice a_device)
{
	assert(this->m_writes.size() && "No writes defined yet can't update");
	vkUpdateDescriptorSets(a_device, static_cast<uint32_t>(this->m_writes.size()), this->m_writes.data(), 0, nullptr);
}

size_t DescriptorSetCache::emplace(DescriptorSet a_set)
{
	std::lock_guard<std::mutex> lock{this->m_mutex};

	auto index{this->m_descriptors.size()};
	this->m_descriptors.emplace_back(std::move(a_set));

	return index;
}

DescriptorSet &DescriptorSetCache::at(size_t a_index)
{
	assert(a_index < this->m_descriptors.size() && "Descriptor index out of bounds");

	std::lock_guard<std::mutex> lock{this->m_mutex};
	return this->m_descriptors[a_index];
}

// void DescriptorSet::free(const VkDevice a_device)
// {
// 	vkFreeDescriptorSets(a_device, this->m_pool, 1, &this->m_handle);
// }

// The machinery's bindless descriptors layout sounds interesting
/*
struct tm_vulkan_bindless_setup_t
{
    uint32_t storage_buffers;                //  = 512 * 1024,
    uint32_t samplers;                       //  = 4 * 1024,
    uint32_t sampled_images;                 //  = 512 * 1024,
    uint32_t storage_images;                 //  = 64 * 1024,
    uint32_t acceleration_structures;        //  = 32 * 1024
};

static const tm_vulkan_bindless_setup_t default_bindless_setup = {
    .storage_buffers         = 512 * 1024,
    .samplers                = 4 * 1024,
    .sampled_images          = 512 * 1024,
    .storage_images          = 64 * 1024,
    .acceleration_structures = 32 * 1024};

const VkDescriptorSetLayoutBinding bindless_layout[] = {
    {.binding         = 0,
     .descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
     .descriptorCount = default_bindless_setup.storage_buffers,
     .stageFlags      = VK_SHADER_STAGE_ALL},
    {.binding         = 1,
     .descriptorType  = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
     .descriptorCount = default_bindless_setup.sampled_images,
     .stageFlags      = VK_SHADER_STAGE_ALL},
    {.binding         = 2,
     .descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
     .descriptorCount = default_bindless_setup.storage_images,
     .stageFlags      = VK_SHADER_STAGE_ALL},
    {.binding         = 3,
     .descriptorType  = VK_DESCRIPTOR_TYPE_SAMPLER,
     .descriptorCount = default_bindless_setup.samplers,
     .stageFlags      = VK_SHADER_STAGE_ALL},
    {.binding         = 4,
     .descriptorType  = VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR,
     .descriptorCount = default_bindless_setup.acceleration_structures,
     .stageFlags      = VK_SHADER_STAGE_ALL},
};
*/

}        // namespace rhi
